TEMPLATE = app

HEADERS = window.h
SOURCES = main.cpp window.cpp
RESOURCES = widgets.qrc

QT += dbus maemo5

# install
target.path = $$[QT_INSTALL_EXAMPLES]/maemo5/widgets
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS widgets.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/maemo5/widgets
INSTALLS += target sources
