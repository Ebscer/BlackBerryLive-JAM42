# Auto-generated by IDE. All changes by user will be lost!
# Created at 5/10/13 1:08 PM

BASEDIR = $$_PRO_FILE_PWD_

INCLUDEPATH +=  \
    $$BASEDIR/src

SOURCES +=  \
    $$BASEDIR/src/BBLiveGame.cpp \
    $$BASEDIR/src/PaymentServiceControl.cpp \
    $$BASEDIR/src/ScoreLoopThread.cpp \
    $$BASEDIR/src/main.cpp

HEADERS +=  \
    $$BASEDIR/src/BBLiveGame.hpp \
    $$BASEDIR/src/PaymentServiceControl.hpp \
    $$BASEDIR/src/ScoreLoopThread.hpp

CONFIG += precompile_header
PRECOMPILED_HEADER = $$BASEDIR/precompiled.h

lupdate_inclusion {
    SOURCES += \
        $$BASEDIR/../assets/*.qml
}

TRANSLATIONS = \
    $${TARGET}.ts

