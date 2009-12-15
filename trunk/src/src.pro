TEMPLATE = app
CONFIG += warn_on \
    thread \
    qt \
    release
unix {
TARGET = bin/qshaderedit.bin
} else {
 TARGET = bin/qshaderedit
}

OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
RCC_DIR = build
RESOURCES += qshaderedit.qrc
SOURCES += main.cpp \
    messagepanel.cpp \
    newdialog.cpp \
    outputparser.cpp \
    parameter.cpp \
    parameterdelegate.cpp \
    parametermodel.cpp \
    parameterpanel.cpp \
    qglview.cpp \
    qshaderedit.cpp \
    scene.cpp \
    scenepanel.cpp \
    teapot.cpp \
    texmanager.cpp \
    asmeffect.cpp \
    cgexplicit.cpp \
    cgfxeffect.cpp \
    document.cpp \
    editor.cpp \
    effect.cpp \
    finddialog.cpp \
    glsleffect.cpp \
    glutils.cpp \
    gotodialog.cpp \
    highlighter.cpp \
    imageplugin.cpp \
    titlebar.cpp \
    nofocusstyle.cpp \
    managerpanel.cpp \
    configdlg.cpp \
    completer.cpp
QT += xml \
    opengl
HEADERS += cgexplicit.h \
    document.h \
    editor.h \
    effect.h \
    finddialog.h \
    glutils.h \
    gotodialog.h \
    highlighter.h \
    imageplugin.h \
    messagepanel.h \
    newdialog.h \
    outputparser.h \
    parameterdelegate.h \
    parameter.h \
    parametermodel.h \
    parameterpanel.h \
    qglview.h \
    qshaderedit.h \
    scene.h \
    scenepanel.h \
    texmanager.h \
    titlebar.h \
    nofocusstyle.h \
    managerpanel.h \
    configdlg.h \
    completer.h
FORMS += finddialog.ui \
    gotodialog.ui \
    newdialog.ui \
    parameterpropertiesdialog.ui \
    texturepropertiesdialog.ui
LIBS += -lGLEW
unix{
  target.path = /usr/bin
  data.path = /usr/share/qshaderedit
  data.files = ../qshaderedit/* \
               ../COPYING
  desktop.path = /usr/share/applications
  desktop.files = ../linux/qshaderedit.desktop
  script.path = /usr/bin
  script.files = ../linux/qshaderedit

INSTALLS += target \
    data \
    desktop \
    script
}
macx{
  target.path = ../../QShaderEdit
  data.path = ../../QShaderEdit
  data.files = ../qshaderedit/*

INSTALLS += target \
    data
}
win32{
  target.path = ../../QShaderEdit
  data.path = ../../QShaderEdit
  data.files = ../qshaderedit/*

INSTALLS += target \
    data
}

  
