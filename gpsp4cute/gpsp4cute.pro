TEMPLATE = app
TARGET = gpsp
#CONFIG += mobility
#MOBILITY += sensors \
#			multimedia
QT += core \
    gui 
HEADERS += filewidget.h \
    dpadwidget.h \
	QRemoteControlKeys.h \
	aboutdialog.h \
    controlsettings.h \
    videosettings.h \
    audiosettings.h \
    gpsp.loc \
    keyconfigdialog.h \
    viewcontroller.h \
    emusettings.h \
    gpspadaptation.h \
    QBlitterWidget.h \
    gpspSettings.h \
    gpsp4Qt.h \
    cuteErrorDialog.h \
    smalloptionswidget.h \
    rightbuttonwidget.h \
    smallgpsplogo.h \
    audio.h
    
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
    cuteDebug.cpp \
    gpspadaptation.cpp \
    QBlitterWidget.cpp \
    main.cpp \
    gpsp4Qt.cpp \
    bitmapblit.cpp \
    smalloptionswidget.cpp \
    rightbuttonwidget.cpp \
    smallgpsplogo.cpp \
    audio.cpp \
    sound_symbian.cpp
    
FORMS += cuteErrorDialog.ui \
    filewidget.ui \
    dpadwidget.ui \
    aboutdialog.ui \
    controlsettings.ui \
    videosettings.ui \
    keyconfigdialog.ui \
    audiosettings.ui \
    emusettings.ui \
    smalloptionswidget.ui \
    rightbuttonwidget.ui \
    smallgpsplogo.ui

RESOURCES += gpspresources.qrc

symbian:LIBS += -lgpsp4symbian.lib \
    -lscdv \
    -lws32 \
    -lcone \
    -lbitgdi \
    -lmediaclientaudiostream \
    -leikcore \
    -lavkon \
    -lremconcoreapi \
    -lremconinterfacebase \
    -lecom \
    -lhal \
    -lmmfcontrollerframework #TODO: remove when audio can be removed..
 symbian:TARGET.UID3 \
    = \
    0xE0D278F3
symbian:TARGET.EPOCHEAPSIZE = 0x200000 \
    0x800000
    
symbian:TARGET.EPOCSTACKKSIZE = 0x10000
symbian:TARGET.CAPABILITY += SwEvent
symbian:ICON = gfx/gpsp.svg
symbian:INCLUDEPATH += ../inc/

OTHER_FILES += summelistyle.qss

packageheader = "$${LITERAL_HASH}{\"gpsp\"}, (0xE0D278F3), 0, 5, 0 "
default_deployment.pkg_prerules = packageheader