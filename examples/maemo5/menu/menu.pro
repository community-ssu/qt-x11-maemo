TEMPLATE = app
SOURCES = main.cpp

QT += maemo5

# install
target.path = $$[QT_INSTALL_EXAMPLES]/maemo5/menu
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS menu.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/maemo5/menu
INSTALLS += target sources
