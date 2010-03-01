TEMPLATE = app
SOURCES = main.cpp

QT += maemo5 dbus

# install
target.path = $$[QT_INSTALL_EXAMPLES]/maemo5/rotation
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS rotation.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/maemo5/rotation
INSTALLS += target sources
