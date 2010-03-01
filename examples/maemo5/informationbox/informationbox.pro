TEMPLATE = app
SOURCES = main.cpp
HEADERS = examplewidget.h

QT += maemo5

# install
target.path = $$[QT_INSTALL_EXAMPLES]/maemo5/informationbox
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS informationbox.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/maemo5/informationbox
INSTALLS += target sources
