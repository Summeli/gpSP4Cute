TEMPLATE = app
TARGET = gpsp4cute

QT += core \
    gui \
    opengl

HEADERS += filewidget.h \
    dpadwidget.h \
    aboutdialog.h \
    controlsettings.h \
    videosettings.h \
    audiosettings.h \
    keyconfigdialog.h \
    viewcontroller.h \
    emusettings.h \
    gpspadaptation.h \
    gpspSettings.h \
    gpsp4Qt.h \
    cuteErrorDialog.h \
    rightbuttonwidget.h 
    
SOURCES += cuteErrorDialog.cpp \
    filewidget.cpp \
    dpadwidget.cpp \
    aboutdialog.cpp \
    controlsettings.cpp \
    videosettings.cpp \
    audiosettings.cpp \
    keyconfigdialog.cpp \
    viewcontroller.cpp \
    emusettings.cpp \
    gpspadaptation.cpp \
    main.cpp \
    gpsp4Qt.cpp \
    rightbuttonwidget.cpp 
    
FORMS += cuteErrorDialog.ui \
    filewidget.ui \
    aboutdialog.ui \
    controlsettings.ui \
    videosettings.ui \
    keyconfigdialog.ui \
    audiosettings.ui \
    emusettings.ui

INCLUDEPATH += ../gpsp/inc/

symbian: {

HEADERS += QRemoteControlKeys.h \
           AntAudio.h

SOURCES += QRemoteControlKeys.cpp \
           AntAudio.cpp
           
FORMS += cuteErrorDialog.ui \
    filewidget.ui \
    aboutdialog.ui \
    controlsettings.ui \
    videosettings.ui \
    keyconfigdialog.ui \
    audiosettings.ui \
    emusettings.ui
    
RESOURCES += gpspresources.qrc   
 
LIBS += -lgpsp4symbian.lib \
    -lscdv \
    -lws32 \
    -lcone \
    -lmediaclientaudiostream \
    -leikcore \
    -lavkon \
    -lremconcoreapi \
    -lremconinterfacebase \
    -lecom \
    -lhal \
    -llibEGL  \
    -llibGLESv2 \
    -lmmfcontrollerframework #TODO: remove when audio can be removed..

TARGET.UID3 = 0xE0D278F3
TARGET.EPOCHEAPSIZE = 0x200000 \
    0x1000000
    
TARGET.EPOCSTACKKSIZE = 0x10000
TARGET.CAPABILITY = UserEnvironment
ICON = gfx/gpsp.svg

OTHER_FILES += summelistyle.qss

#pkg files
gpspFiles.sources = sis\\game_config.txt
gpspFiles.path = !:\\private\\E0D278F3\\game_config.txt
DEPLOYMENT += gpspFiles;


packageheader = "$${LITERAL_HASH}{\"gpsp\"}, (0xE0D278F3), 0, 7, 0 "
default_deployment.pkg_prerules = packageheader
}

unix:!symbian:!maemo5 {
FORMS += ui_meego/cuteErrorDialog.ui \
    ui_meego/filewidget.ui \
    ui_meego/aboutdialog.ui \
    ui_meego/controlsettings.ui \
    ui_meego/videosettings.ui \
    ui_meego/keyconfigdialog.ui \
    ui_meego/audiosettings.ui \
    ui_meego/emusettings.ui
    
HEADERS += meegoAudio.h  meegoAdaptation.h
SOURCES += meegoAudio.cpp meegoAdaptation.cpp

RESOURCES += meegoresources.qrc

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog

LIBS += ../gpsp/gpsp4meego.a
LIBS += -lpulse-simple

include(deployment.pri)
qtcAddDeployment()
}

