TEMPLATE = app

# Input
SOURCES += main.cpp
HEADERS += examplewidget.h qwebviewselectionsuppressor.h

QT += webkit multimedia opengl network

# install
target.path = $$[QT_INSTALL_EXAMPLES]/maemo5/webview
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS webview.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/maemo5/webview
INSTALLS += target sources
