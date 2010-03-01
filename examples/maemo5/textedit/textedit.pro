TEMPLATE = app
SOURCES = main.cpp
HEADERS = texteditautoresizer.h

# install
target.path = $$[QT_INSTALL_EXAMPLES]/maemo5/textedit
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS textedit.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/maemo5/textedit
INSTALLS += target sources
