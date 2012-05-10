TEMPLATE = app
SOURCES = main.cpp

LIBS += -lX11

# install
target.path = $$[QT_INSTALL_EXAMPLES]/maemo5/zoom
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS zoom.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/maemo5/zoom
INSTALLS += target sources
