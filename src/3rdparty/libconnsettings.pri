INCLUDEPATH += $$PWD/libconnsettings/src

QMAKE_CFLAGS += $$QT_CFLAGS_ICD

message($$QMAKE_CFLAGS)

HEADERS += \
    $$PWD/libconnsettings/src/conn_settings.h

SOURCES += \
    $$PWD/libconnsettings/src/conn_settings.c


