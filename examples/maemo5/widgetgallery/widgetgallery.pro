TEMPLATE = app

SOURCES += main.cpp \
           common.cpp \
           buttons.cpp \
           itemviews.cpp \
           input.cpp \
           container.cpp \
           dialogs.cpp \
           display.cpp \
           others.cpp \
           qml.cpp

QT += declarative

maemo5:SOURCES += maemo5.cpp
           
maemo5:QT += maemo5

contains(QT_CONFIG, webkit) {
    QT += webkit multimedia network opengl
} else {
    DEFINES += QT_NO_WEBKIT
}

SNIPPETS += snippets/textinput.qml snippets/textedit.qml

RESOURCES += widgetgallery.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/maemo5/widgetgallery
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS $$SNIPPETS widgetgallery.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/maemo5/widgetgallery
INSTALLS += target sources
