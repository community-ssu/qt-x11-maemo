TEMPLATE = app
SOURCES = main.cpp
HEADERS = examplewidget.h fullscreenexitbutton.h

QT += dbus

# install
target.path = $$[QT_INSTALL_EXAMPLES]/maemo5/windowstates
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS windowstates.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/maemo5/windowstates
INSTALLS += target sources
