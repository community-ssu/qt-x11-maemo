TEMPLATE = app
SOURCES = main.cpp fingerswipegesture.cpp

QT += maemo5

# install
target.path = $$[QT_INSTALL_EXAMPLES]/maemo5/gesture
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS gesture.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/maemo5/gesture
INSTALLS += target sources
