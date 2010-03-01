TARGET  = n900audio
include(../../qpluginbase.pri)

DESTDIR = $$QT_BUILD_TREE/plugins/audio
target.path = $$[QT_INSTALL_PLUGINS]/audio
INSTALLS += target

LIBS += -lpulse-simple -lasound

HEADERS += n900audio.h
SOURCES += main.cpp \
           n900audio.cpp
