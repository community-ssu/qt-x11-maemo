# Qt network kernel module

INCLUDEPATH += $$PWD

HEADERS += maemo/qmaemointernetconnectivity.h \ 
           maemo/qmaemointernetconnectivity_p.h \
           maemo/qgconfbackend_p.h \
           maemo/gconfsymbols_p.h

SOURCES += maemo/qmaemointernetconnectivity.cpp \
           maemo/qgconfbackend.cpp \
           maemo/gconfsymbols.cpp

CONFIG += link_pkgconfig
PKGCONFIG_PRIVATE += gconf-2.0
