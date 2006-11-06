
SET(SRCS
	effect.h
	effect.cpp
	glsleffect.cpp
	asmeffect.cpp
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
	gotodialog.cpp
	glutils.h
	glutils.cpp
	imageplugin.h
	imageplugin.cpp)

IF(FOUND_CG)
	SET(SRCS ${SRCS} cgfxeffect.cpp)
ENDIF(FOUND_CG)

SET(QT_SRCS ${SRCS}
	main.cpp
	qshaderedit.h
	qshaderedit.cpp)

SET(MOC_SRCS
	editor.h
	highlighter.h
	messagepanel.h
	parameterpanel.h
	parameterdelegate.h
	qglview.h
	newdialog.h
	finddialog.h
	gotodialog.h
	effect.h)

SET(QT_MOC_SRCS qshaderedit.h)

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
QT4_WRAP_CPP(QT_MOCS ${QT_MOC_SRCS})

ADD_EXECUTABLE(qshaderedit MACOSX_BUNDLE ${QT_SRCS} ${MOCS} ${QT_MOCS} ${UICS} ${RCCS})
TARGET_LINK_LIBRARIES(qshaderedit ${LIBS})
INSTALL(TARGETS qshaderedit DESTINATION bin)

# KDE4 target
FIND_PACKAGE(KDE4)

IF(KDE4_FOUND)

	INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

	SET(KDE_SRCS ${SRCS}
		kde/main.cpp
		kde/kshaderedit.h
		kde/kshaderedit.cpp)

	SET(KDE_MOC_SRCS kde/kshaderedit.h)

	QT4_WRAP_CPP(KDE_MOCS ${KDE_MOC_SRCS})

	#KDE4_AUTOMOC(${KDE_SRCS})
	#kde4_add_ui_files(kapp4_SRCS)
	#kde4_add_kcfg_files(kapp4_SRCS kde/kshaderedit.kcfgc)
	KDE4_ADD_EXECUTABLE(kshaderedit ${KDE_SRCS} ${MOCS} ${KDE_MOCS} ${UICS} ${RCCS})

	TARGET_LINK_LIBRARIES(kshaderedit ${KDE4_KDEUI_LIBS} ${LIBS})

	INSTALL(TARGETS kshaderedit DESTINATION ${BIN_INSTALL_DIR})

	INSTALL(FILES kde/kshaderedit.desktop  DESTINATION  ${XDG_APPS_DIR})
	INSTALL(FILES kde/kshaderedit.kcfg  DESTINATION  ${KCFG_INSTALL_DIR})
	#install(FILES kde/kapp4ui.rc  DESTINATION  ${DATA_INSTALL_DIR}/kapp4)

ENDIF(KDE4_FOUND)

