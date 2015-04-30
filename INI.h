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

#ifndef _INI_H
#define _INI_H

#include <iostream>
#include <fstream>

class INI
{
public:
	INI(char* szFile);
	~INI();
	bool open();
	void close();
	bool GetValue(char* szKey, char* szValue, int maxcount);
	bool GoToSection(char* szSection);

	int GetIntegerValue(char* szKey);
	float GetFloatValue(char* szKey);

	bool SetValue(char* szKey, char* szValue);
	bool SetIntegerValue(char* szKey, int iValue);
	bool SetFloatValue(char* szKey, float fValue);
private:
	std::ifstream fileRead;
	char* szFile;
};

#endif