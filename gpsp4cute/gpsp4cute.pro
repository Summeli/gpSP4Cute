TEMPLATE = app
TARGET = gpsp

QT += core \
    gui \
    opengl

HEADERS += filewidget.h \
    dpadwidget.h \
    QRemoteControlKeys.h \
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
    rightbuttonwidget.h \
    AntAudio.h
    
SOURCES += cuteErrorDialog.cpp \
    filewidget.cpp \
    dpadwidget.cpp \
    QRemoteControlKeys.cpp \
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
    rightbuttonwidget.cpp \
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

symbian:LIBS += -lgpsp4symbian.lib \
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

 symbian:TARGET.UID3 \
    = \
    0xE0D278F3
symbian:TARGET.EPOCHEAPSIZE = 0x200000 \
    0x1000000
    
symbian:TARGET.EPOCSTACKKSIZE = 0x10000
symbian: TARGET.CAPABILITY = UserEnvironment
symbian:ICON = gfx/gpsp.svg
symbian:INCLUDEPATH += ../gpsp/inc/
symbian:MACRO = SYMBIAN_ENABLE_SPLIT_HEADERS

OTHER_FILES += summelistyle.qss

#pkg files
gpspFiles.sources = sis\\game_config.txt
gpspFiles.path = !:\\private\\E0D278F3\\game_config.txt
DEPLOYMENT += gpspFiles;


packageheader = "$${LITERAL_HASH}{\"gpsp\"}, (0xE0D278F3), 0, 5, 0 "
default_deployment.pkg_prerules = packageheader
