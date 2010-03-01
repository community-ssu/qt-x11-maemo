TEMPLATE = lib
SOURCES = plugin.c
TARGET = qtpanelplugin

CONFIG += plugin link_pkgconfig
PKGCONFIG += gtk+-2.0 libosso

# dummy 'install'
target.path = $$[QT_INSTALL_EXAMPLES]/maemo5/controlpanel/qtpanelplugin
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS qtpanelplugin.pro qtpanelplugin.desktop
sources.path = $$[QT_INSTALL_EXAMPLES]/maemo5/controlpanel/qtpanelplugin
INSTALLS += target sources

# for a real install, you would want to use something like this:
# # install
# desktop.path = $$system(pkg-config hildon-control-panel --variable=plugindesktopentrydir)
# desktop.files = qtpanelplugin.desktop
# target.path = $$system(pkg-config hildon-control-panel --variable=pluginlibdir)
# INSTALLS += target desktop
