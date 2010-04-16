QT      +=  webkit network multimedia
HEADERS =   mainwindow.h
SOURCES =   main.cpp \
            mainwindow.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/maemo5/maemobrowser
sources.files = $$SOURCES $$HEADERS *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/maemo5/maemobrowser
INSTALLS += target sources
