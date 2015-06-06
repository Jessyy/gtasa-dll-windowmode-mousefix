[Fixes for GTA: San Andreas][github]
================
[![Build status - Windows][build_status_win]][build_win]

- It gives you opportunity to run GTA: San Andreas in windowed mode.
- This library provides you High Definition mouse fix for GTA: San Andreas. If you were experiencing problem with your mouse playing GTA:SA on Windows 7/8/8.1 this library is for you. It repleaces bugged DirectInput with High Definition RAWINPUT: http://msdn.microsoft.com/en-us/library/windows/desktop/ee418864(v=vs.85).aspx

Installation
------------
Just copy dinput8.dll and fixes.ini to GTA San Andreas installation directory.

Configuration is performed through the ini file. These are default settings:
```
[VIDEO]
windowed=false

[INPUT]
hd_mouse=true
```

Additional info
---------------
After applying my library you may experience mouse speedup. That's because HD input gets more information per second. You can change mouse sensitivity in GTA options through main menu.

Credits
-------
- bartekdvd for creating this project

[github]: https://github.com/Jessyy/gtasa-dll-windowmode-mousefix
[build_status_win]: https://ci.appveyor.com/api/projects/status/9i4i191lx2w86wk7/branch/master?svg=true
[build_win]: https://ci.appveyor.com/project/Jessyy/gtasa-dll-windowmode-mousefix/branch/master