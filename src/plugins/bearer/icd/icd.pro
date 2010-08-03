TARGET = qicdbearer
include(../../qpluginbase.pri)

QT += network dbus

QMAKE_CXXFLAGS *= $$QT_CFLAGS_ICD
LIBS += $$QT_LIBS_ICD

HEADERS += qicdengine.h \
           qnetworksession_impl.h

SOURCES += main.cpp \
           qicdengine.cpp \
           qnetworksession_impl.cpp

#DEFINES += BEARER_MANAGEMENT_DEBUG

include(../../../3rdparty/libconninet.pri)
include(../../../3rdparty/libgq.pri)
include(../../../3rdparty/libconnsettings.pri)

QTDIR_build:DESTDIR = $$QT_BUILD_TREE/plugins/bearer
target.path += $$[QT_INSTALL_PLUGINS]/bearer
INSTALLS += target
