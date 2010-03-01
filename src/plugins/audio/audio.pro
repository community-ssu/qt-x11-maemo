TEMPLATE = subdirs
SUBDIRS =

contains(QT_CONFIG, audio-backend) {
    symbian {
        SUBDIRS += symbian
    }
}


maemo5:SUBDIRS += n900audio
