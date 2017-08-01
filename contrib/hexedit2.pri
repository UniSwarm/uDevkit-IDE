
INCLUDEPATH += $$PWD/hexedit2/src

SOURCES += \
        $$PWD/hexedit2/src/chunks.cpp \
        $$PWD/hexedit2/src/commands.cpp \
        $$PWD/hexedit2/src/qhexedit.cpp

HEADERS += \
        $$PWD/hexedit2/src/chunks.h \
        $$PWD/hexedit2/src/commands.h \
        $$PWD/hexedit2/src/qhexedit.h

DEFINES += QHEXEDIT_EXPORTS
