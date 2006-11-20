#ifndef KSHADEREDIT_H
#define KSHADEREDIT_H

#include <kmainwindow.h>
#include <kurl.h>

class QTabWidget;
class QGLView;
class MessagePanel;
class ParameterPanel;
class Editor;
class QTimer;
class QFile;
class QComboBox;
class QLabel;
class QToolBar;
class KUrl;
class KRecentFilesAction;
struct Effect;
struct EffectFactory;

class KShaderEdit : public KMainWindow
{
	Q_OBJECT
public:
	KShaderEdit(const KUrl & url = KUrl());
	~KShaderEdit();

	virtual QSize sizeHint() const;

public slots:
	void newFile(bool startup=false);
	
protected slots:
	bool load( const KUrl & url );
	
	void open();
//	void openRecentFile();
//	void clearRecentFiles();
	bool save();
	void saveAs();
	
	void about();
	
	void setAutoCompile(bool enable);
	void shaderTextChanged();
	void keyTimeout();
	void compileChecked(bool checked);
	
	void build(bool silent);
	void built();
	
	void setModified();

	void techniqueChanged(int index);
	void cursorPositionChanged();
	void selectScene();
	
protected:
	
	void newEffect(const EffectFactory * effectFactory);
	
	void createActions();
	void createMenus();
	void createToolbars();
	void createStatusbar();
	void createDockWindows();
	
	bool closeEffect();
	
	void updateWindowTitle();
	void updateActions();
	void updateEditor();	// @@ This should be closeEditor/initEditor
	void updateTechniques();
	
	bool isModified();
	void setModified(bool modified);

	void setCurrentFile(const QString &fileName);
//	void updateRecentFileActions();
	
	// Events
	virtual void closeEvent(QCloseEvent * event);
	virtual void keyPressEvent(QKeyEvent * event);
	
	
private:

    void loadSettings();
    void saveSettings();

	void updateEffectInputs();

	static QString strippedName(const QString & fileName);
	static QString strippedName(const QFile & file);


	// Central widget.
	Editor * m_editor;
	
	// Toolbars.
	QToolBar * m_fileToolBar;
	QToolBar * m_techniqueToolBar;
	QComboBox * m_techniqueCombo;
	
	// Panels.
	QDockWidget * m_sceneViewDock;
	ParameterPanel * m_paramViewDock;
	MessagePanel * m_logViewDock;
	QGLView * m_sceneView;

	// Status widgets.
	QLabel * m_positionLabel;

	// Menus.
	QMenu * m_renderMenu;
	
	// Actions.
	KAction * m_newAction;
	KAction * m_openAction;
	KAction * m_saveAction;
	KAction * m_saveAsAction;

//	enum { MaxRecentFiles = 5 };
//	KAction * m_recentFileActions[MaxRecentFiles];	

//	QAction * m_recentFileSeparator;
//	KAction * m_clearRecentAction;
	KRecentFilesAction * m_recentFiles;
	
	KAction * m_compileAction;
	
	KAction * m_findAction;
	KAction * m_findNextAction;
	KAction * m_findPreviousAction;
	KAction * m_gotoAction;
	
	// Timers.
	QTimer * m_timer;	// compilation timer.
	QTimer * m_animationTimer;
	
	// State.	
	QFile * m_file;
	const EffectFactory * m_effectFactory;
	Effect * m_effect;
	bool m_modified;
	
	// settings.
	bool m_autoCompile;
	QString m_lastEffect;
};


#endif // KSHADEREDIT_H



