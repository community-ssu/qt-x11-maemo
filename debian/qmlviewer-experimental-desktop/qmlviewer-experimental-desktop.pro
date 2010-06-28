CONFIG -= qt

# force linking to do nothing
QMAKE_LINK = echo

desktop.path = /usr/share/applications/hildon
desktop.files += QMLViewer-experimental.desktop
INSTALLS = desktop
