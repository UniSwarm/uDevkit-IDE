# RtIDE

[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0)
[![Build Status](https://travis-ci.org/Robotips/rtide.svg?branch=master)](https://travis-ci.org/Robotips/rtide)
[![Build status](https://ci.appveyor.com/api/projects/status/rdd4wup94uuo9ykv?svg=true)](https://ci.appveyor.com/project/sebcaux/rtide)

A simple IDE for [RtProg](https://github.com/Robotips/rtprog). This IDE want to be very lightweight and fast to load without a huge project management system.

Still in development.

## Features

* files project viewer with git integration
* code highlighting (based on [edbee](https://github.com/edbee/edbee-lib))
* dark theme to preserve developer eyes :wink: (based on [QDarkStyleSheet](https://github.com/ColinDuquesnoy/QDarkStyleSheet))

## Build

```
mkdir build
cd build
qmake ../src
make -j
```

Binaries will be placed in bin/ directory.
