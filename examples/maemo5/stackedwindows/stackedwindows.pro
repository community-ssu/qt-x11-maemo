TEMPLATE = app
SOURCES = main.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/maemo5/stackedwindows
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS stackedwindows.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/maemo5/stackedwindows
INSTALLS += target sources
