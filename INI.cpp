/*  INI C++
*
*  Copyright (c) bartekdvd, 2014
*
*  This software is provided "as-is", without any express or implied warranty.
*  In no event will the authors be held liable for any damages arising from
*  the use of this software.
*
*  Permission is granted to anyone to use this software for any purpose,
*  including commercial applications, and to alter it and redistribute it
*  freely, subject to the following restrictions:
*
*  1.  The origin of this software must not be misrepresented; you must not
*      claim that you wrote the original software. If you use this software in
*      a product, an acknowledgment in the product documentation would be
*      appreciated but is not required.
*  2.  Altered source versions must be plainly marked as such, and must not be
*      misrepresented as being the original software.
*  3.  This notice may not be removed or altered from any source distribution.
*/

#include "INI.h"

INI::INI(char* szFile)
{
	int array_size = strlen(szFile) + 1;
	this->szFile = new char[array_size];
	memcpy(this->szFile, szFile, array_size);
}

INI::~INI()
{
	this->close();
	delete [] this->szFile;
}

bool INI::open()
{
	this->fileRead.open(this->szFile);
	return this->fileRead.is_open();
}

void INI::close()
{
	if (this->fileRead.is_open())
	{
		this->fileRead.clear();
		this->fileRead.close();
	}
}

bool INI::GetValue(char* szKey, char* szValue, int maxcount)
{
	if (!this->fileRead.is_open() || !szKey || !szValue)
	{
		if (maxcount > 0 && szValue)
			szValue[0] = 0;
		return false;
	}

	unsigned long long seek_pos = this->fileRead.tellg();
	char buff[1024];
	bool success = false;
	
	while (!success && this->fileRead.good())
	{
		this->fileRead.getline(buff, sizeof(buff));
		int i = 0;
		while (buff[i] == ' ' || buff[i] == '\t') i++;
		int startpos = i;
		while (buff[i] && buff[i] != ';' && buff[i] != '=' && buff[i] != ' ' && buff[i] != '\n' && buff[i] != '\r' && buff[i] != '\t') i++;
		char tmp = buff[i];
		buff[i] = 0;

		if (_stricmp(buff + startpos, szKey) == 0)
		{
			buff[i] = tmp;
			
			if (tmp != '=' && tmp)
			{
				i++;
				while (buff[i] != '=' && buff[i] && buff[i] != ';') i++;
			}

			if (buff[i] == '=')
			{
				i++;
				while (buff[i] == ' ' || buff[i] == '\t')
					i++;
				
				int j = 0;
				maxcount--;
				while (buff[i] != '\r' && buff[i] != '\n' && buff[i] != ';' && buff[i] && j < maxcount)
				{
					szValue[j] = buff[i];
					j++;
					i++;
				}
				szValue[j] = 0;
				success = true;
			}
		}
	}

	this->fileRead.clear();
	this->fileRead.seekg(seek_pos);
	return success;
}

bool INI::GoToSection(char* szSection)
{
	if (!this->fileRead.is_open() || !szSection)
		return false;

	this->fileRead.clear();
	this->fileRead.seekg(0);

	char buff[1024];
	bool success = false;

	char section[256];
	{
		int i = 0;
		int j = 0;
		while (szSection[i] && j < (sizeof(section) - 1))
		{
			if (szSection[i] != '[' && szSection[i] != ']')
			{
				section[j] = szSection[i];
				j++;
			}
			i++;
		}
		section[j] = 0;
	}

	while (!success && this->fileRead.good())
	{
		this->fileRead.getline(buff, sizeof(buff));
		int i = 0;
		while (buff[i] == ' ' || buff[i] == '\t') i++;
		if (buff[i] == '[')
		{
			i++;
			while (buff[i] == ' ' || buff[i] == '\t') i++;
			
			int j = 0;
			char sectiontmp[256];
			while (buff[i] && buff[i] != ';' && buff[i] != ' ' && buff[i] != '\n' && buff[i] != '\r' && buff[i] != '\t' && buff[i] != ']')
			{
				sectiontmp[j] = buff[i];
				j++;
				i++;
			}
			sectiontmp[j] = 0;

			while (buff[i] == ' ' || buff[i] == '\t') i++;
			
			if (buff[i] == ']')
			{
				buff[i] = 0;
				if (_stricmp(sectiontmp, section) == 0)
					success = true;
			}
		}
	}

	if (!success)
	{
		this->fileRead.clear();
		this->fileRead.seekg(0);
	}

	return success;
}

int INI::GetIntegerValue(char* szKey)
{
	char szValue[32];
	this->GetValue(szKey, szValue, sizeof(szValue));
	return atoi(szValue);
}

float INI::GetFloatValue(char* szKey)
{
	char szValue[32];
	this->GetValue(szKey, szValue, sizeof(szValue));
	return (float)atof(szValue);
}

bool INI::SetValue(char* szKey, char* szValue)
{
	if (!szKey || !szValue)
		return false;

	bool bOpened = true;

	if (!this->fileRead.is_open())
	{
		bOpened = false;
		
		this->open();
		
		if (!this->fileRead.is_open())
			return false;
	}


	std::ofstream fileWrite;
	std::ifstream fileTmpRead;
	std::ofstream fileTmpWrite;

	int iTmpSize = strlen(this->szFile) + 32;
	char* szTmp = new char[iTmpSize];
	szTmp[0] = 0;

	strcat_s(szTmp, iTmpSize, this->szFile);
	strcat_s(szTmp, iTmpSize, ".i.n.i.tmp");

	fileTmpWrite.open(szTmp);

	if (!fileTmpWrite.is_open())
	{
		if (!bOpened)
			this->close();

		delete [] szTmp;
		return false;
	}

	unsigned long long seek_pos = this->fileRead.tellg();
	this->fileRead.clear();
	this->fileRead.seekg(0);

	char buff[4096];
	bool success = false;
	
	while (this->fileRead.good())
	{
		this->fileRead.getline(buff, sizeof(buff));
		
		int i = 0;
		while (buff[i] == ' ' || buff[i] == '\t') i++;
		int startpos = i;
		while (buff[i] && buff[i] != ';' && buff[i] != '=' && buff[i] != ' ' && buff[i] != '\n' && buff[i] != '\r' && buff[i] != '\t') i++;
		char tmp = buff[i];
		buff[i] = 0;

		if (!success && _stricmp(buff + startpos, szKey) == 0)
		{
			buff[i] = tmp;
			
			if (tmp != '=' && tmp)
			{
				i++;
				while (buff[i] != '=' && buff[i] && buff[i] != ';') i++;
			}

			if (buff[i] == '=')
			{
				i++;
				while (buff[i] == ' ' || buff[i] == '\t')
					i++;

				int n = sprintf_s(buff, sizeof(buff), "%s = %s", szKey, szValue);
				fileTmpWrite.write(buff, n);
				fileTmpWrite << std::endl;
				success = true;
			}
		}
		else
		{
			buff[i] = tmp;
			int len = strlen(buff);
			if (len > 0)
				fileTmpWrite.write(buff, len);
			
			if (this->fileRead.good())
				fileTmpWrite << std::endl;
		}
	}

	fileTmpWrite.close();

	if (success)
	{
		fileTmpRead.open(szTmp, std::ios_base::binary);
		
		if (fileTmpRead.is_open())
		{
			this->close();
			fileWrite.open(this->szFile, std::ios_base::binary);
			
			if (fileWrite.is_open())
			{
				while (fileTmpRead.good())
				{
					fileTmpRead.read(buff, sizeof(buff));
					fileWrite.write(buff, fileTmpRead.gcount());
				}

				fileWrite.close();
			}

			fileTmpRead.close();
			
			if (bOpened)
				this->open();
		}
	}

	if (bOpened)
	{
		this->fileRead.clear();
		this->fileRead.seekg(seek_pos);
	}
	else if (this->fileRead.is_open())
		this->close();

	remove(szTmp);
	delete [] szTmp;
	
	return success;
}

bool INI::SetIntegerValue(char* szKey, int iValue)
{
	char szValue[64];
	sprintf_s(szValue, "%d", iValue);
	return this->SetValue(szKey, szValue);
}

bool INI::SetFloatValue(char* szKey, float fValue)
{
	char szValue[64];
	sprintf_s(szValue, "%f", fValue);
	return this->SetValue(szKey, szValue);
}