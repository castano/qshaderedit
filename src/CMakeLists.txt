
SET(SRCS
	effect.h
	effect.cpp
	glsleffect.cpp
	asmeffect.cpp
	cgfxeffect.cpp
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
	scenepanel.h
	scenepanel.cpp
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
	imageplugin.cpp
	cgexplicit.h
	cgexplicit.cpp
	document.h
	document.cpp
	titlebar.h
	titlebar.cpp 
	nofocusstyle.h
	nofocusstyle.cpp
	managerpanel.h	
	managerpanel.cpp
	configdlg.h	
	configdlg.cpp
	completer.h
	completer.cpp
)

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
	parametermodel.h
	scenepanel.h
	qglview.h
	newdialog.h
	finddialog.h
	gotodialog.h
	effect.h
	document.h
	titlebar.h
	managerpanel.h
	configdlg.h
	completer.h
)

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

IF(UNIX)
  ADD_EXECUTABLE(qshaderedit.bin ${QT_SRCS} ${MOCS} ${QT_MOCS} ${UICS} ${RCCS})
  #set_target_properties(qshaderedit.bin PROPERTIES LINK_FLAGS_RELEASE)
  TARGET_LINK_LIBRARIES(qshaderedit.bin ${LIBS})

  INSTALL(TARGETS qshaderedit.bin DESTINATION bin)
  INSTALL(FILES ${Data_Dir}/linux/qshaderedit DESTINATION bin 
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
  INSTALL(FILES ${Data_Dir}/COPYING DESTINATION /usr/share/qshaderedit)
  INSTALL(DIRECTORY ${Data_Dir}/qshaderedit/ DESTINATION share/qshaderedit FILES_MATCHING PATTERN "*.*")
  INSTALL(FILES ${Data_Dir}/linux/qshaderedit.desktop DESTINATION share/applications)
ELSE (UNIX)
  ADD_EXECUTABLE(qshaderedit ${QT_SRCS} ${MOCS} ${QT_MOCS} ${UICS} ${RCCS})
  TARGET_LINK_LIBRARIES(qshaderedit optimized ${LIBS})

  INSTALL(TARGETS qshaderedit DESTINATION ${Data_Dir}/../QShaderEdit CONFIGURATIONS Release)
  INSTALL(FILES ${Data_Dir}/COPYING DESTINATION ${Data_Dir}/../QShaderEdit)
  INSTALL(DIRECTORY ${Data_Dir}/qshaderedit/ DESTINATION ${Data_Dir}/../QShaderEdit FILES_MATCHING PATTERN "*.*")
ENDIF(UNIX)

