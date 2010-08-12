CONFIG -= qt

# force linking to do nothing
QMAKE_LINK = echo

desktop.path = /usr/share/applications/hildon
desktop.files += QMLViewer-experimental.desktop

icon.path = /usr/share/icons/hicolor/64x64/apps
icon.files += QMLViewer-experimental.png

INSTALLS = desktop icon
