# uDevkit-IDE

[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0)
[![Build Status](https://travis-ci.org/UniSwarm/uDevkit-IDE.svg?branch=master)](https://travis-ci.org/UniSwarm/uDevkit-IDE)
[![Build status](https://ci.appveyor.com/api/projects/status/7ts34vord4jnooub?svg=true)](https://ci.appveyor.com/project/sebcaux/udevkit-ide)

A simple IDE for [RtProg](https://github.com/UniSwarm/rtprog) written in Qt5. This IDE want to be very lightweight and fast to load without a huge project management system.

Still in development.

![Screenshot](screenshot_rtide.png)

## Features

* files project viewer with git integration
* code editor with highlighting and git integration (based on [edbee](https://github.com/edbee/edbee-lib))
* hexadecimal editor (based on [qhexedit2](https://github.com/Simsys/qhexedit2))
* dark theme to preserve developer eyes :wink: (based on [QDarkStyleSheet](https://github.com/ColinDuquesnoy/QDarkStyleSheet))
* Makefile parser to extract external sources and rules
* compile log with link to file
* image viewer
* HTML live preview

## dependencies
### Qt > 5.2

Qt module used :

- core
- gui
- widgets
- webenginewidgets (Qt > 5.6) or webkitwidgets (Qt < 5.6) or a lite support for web

## Build

```
mkdir build
cd build
qmake ../src
make -j
```

Binaries will be placed in `bin/` directory.

## Binaries downloads

For convenience, a pre-built version is available at :

https://ci.appveyor.com/api/projects/sebcaux/udevkit-IDE/artifacts/udk-ide-win64-v0-nodll.zip

Without Qt DLL (very lite) :

https://ci.appveyor.com/api/projects/sebcaux/udevkit-IDE/artifacts/udk-ide-win64-v0.zip
