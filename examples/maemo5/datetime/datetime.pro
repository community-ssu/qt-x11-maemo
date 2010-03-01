TEMPLATE = app
SOURCES = main.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/maemo5/datetime
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS datetime.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/maemo5/datetime
INSTALLS += target sources
