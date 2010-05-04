QT      +=  webkit network multimedia opengl
HEADERS =   mainwindow.h
SOURCES =   main.cpp \
            mainwindow.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/maemo5/graphicswebview
sources.files = $$SOURCES $$HEADERS *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/maemo5/graphicswebview
INSTALLS += target sources
