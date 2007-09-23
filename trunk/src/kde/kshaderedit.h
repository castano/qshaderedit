#ifndef KSHADEREDIT_H
#define KSHADEREDIT_H

#include <KXmlGuiWindow>
#include <KUrl>

#include <KConfig>

class QTabWidget;
class QGLView;
class QTimer;
class QFile;
class QComboBox;
class QLabel;
class QToolBar;
class KUrl;
class QAction;
class KRecentFilesAction;
class KConfig;

class MessagePanel;
class ParameterPanel;
class Editor;

struct Effect;
struct EffectFactory;

class Document;


class KShaderEdit : public KXmlGuiWindow
{
	Q_OBJECT
public:
	KShaderEdit(const KUrl & url = KUrl());
	~KShaderEdit();

	virtual QSize sizeHint() const;

public slots:
	void newFile(bool startup=false);
	
protected slots:
	bool load(const QString & fileName);
	bool load(const KUrl & url);
	
	void open();
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

	void optionsPreferences();
	
protected:
	
	void newEffect(const EffectFactory * effectFactory);
	
	void createActions();
	void createMenus();
	void createToolbars();
	void createStatusbar();
	void createDockWindows();
	
	void closeEffect();
	
	void updateWindowTitle();
	void updateActions();
	void updateEditor();	// @@ This should be closeEditor/initEditor
	void updateTechniques();
	
	bool isModified();
	void setModified(bool modified);

	void setCurrentFile(const QString &fileName);
	
	// Events
	virtual void keyPressEvent(QKeyEvent * event);
	virtual void dropEvent(QDropEvent * event);
	
	virtual bool queryClose();
	virtual void saveGlobalProperties(KConfig * config);
	
	void readConfig();
	void readConfig(KSharedConfigPtr config);
	
	void writeConfig();
	void writeConfig(KSharedConfigPtr config);
	
private:

	
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
	QAction * m_newAction;
	QAction * m_openAction;
	QAction * m_saveAction;
	QAction * m_saveAsAction;

	KRecentFilesAction * m_recentFiles;
	
	QAction * m_compileAction;
	
	QAction * m_findAction;
	QAction * m_findNextAction;
	QAction * m_findPreviousAction;
	QAction * m_gotoAction;
	
	// Timers.
	QTimer * m_timer;	// compilation timer.
	QTimer * m_animationTimer;
	
	// Document.
	QFile * m_file;
	const EffectFactory * m_effectFactory;
	Effect * m_effect;
	bool m_modified;
	
	// settings.
	bool m_autoCompile;
	QString m_lastEffect;
};


#endif // KSHADEREDIT_H



