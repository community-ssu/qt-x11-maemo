TEMPLATE = app
SOURCES = main.cpp
TARGET = qtpanel

# dummy 'install'
target.path = $$[QT_INSTALL_EXAMPLES]/maemo5/controlpanel/qtpanel
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS qtpanel.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/maemo5/controlpanel/qtpanel
INSTALLS += target sources

# for a real install, you would want to use something like this:
# # install
# target.path = /usr/bin
# INSTALLS += target
