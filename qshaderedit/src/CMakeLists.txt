
SET(SRCS
	effect.h
	effect.cpp
	glfxeffect.cpp
	glsleffect.cpp
	asmeffect.cpp
	main.cpp
	editor.h
	editor.cpp
	highlighter.h
	highlighter.cpp
	messagepanel.h
	messagepanel.cpp
	parameterpanel.h
	parameterpanel.cpp
	parameter.h
	parameter.cpp
	parametermodel.h
	parametermodel.cpp
	parameterdelegate.h
	parameterdelegate.cpp
	qglview.h
	qglview.cpp
	qshaderedit.h
	qshaderedit.cpp
	teapot.cpp
	newdialog.h
	newdialog.cpp
	texmanager.h
	texmanager.cpp
	outputparser.h
	outputparser.cpp
	scene.h
	scene.cpp
	finddialog.h
	finddialog.cpp
	gotodialog.h
	gotodialog.cpp)

IF(FOUND_CG)
	SET(SRCS ${SRCS} cgeffect.cpp cgfxeffect.cpp)
ENDIF(FOUND_CG)


SET(MOC_SRCS
	editor.h
	highlighter.h
	messagepanel.h
	parameterpanel.h
	parameterdelegate.h
	qglview.h
	qshaderedit.h
	newdialog.h
	finddialog.h
	gotodialog.h
	effect.h)

SET(UIC_SRCS 
	newdialog.ui 
	parameterpropertiesdialog.ui
	texturepropertiesdialog.ui
	finddialog.ui
	gotodialog.ui)

SET(RCC_SRCS
	qshaderedit.qrc)


#CONFIGURE_FILE(${QShaderEdit_SOURCE_DIR}/config.h.in ${QShaderEdit_SOURCE_DIR}/config.h)

QT4_WRAP_UI(UICS ${UIC_SRCS})
QT4_ADD_RESOURCES(RCCS ${RCC_SRCS})
QT4_WRAP_CPP(MOCS ${MOC_SRCS})

ADD_EXECUTABLE(qshaderedit MACOSX_BUNDLE ${SRCS} ${MOCS} ${UICS} ${RCCS})
TARGET_LINK_LIBRARIES(qshaderedit ${LIBS})
INSTALL_TARGETS(/bin qshaderedit)
