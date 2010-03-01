TEMPLATE = app
HEADERS = window.h
SOURCES = main.cpp

QT += maemo5

# install
target.path = $$[QT_INSTALL_EXAMPLES]/maemo5/kineticscroller
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS kineticscroller.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/maemo5/kineticscroller
INSTALLS += target sources
