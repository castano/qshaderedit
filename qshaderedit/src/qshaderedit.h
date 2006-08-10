#ifndef QSHADEREDIT_H
#define QSHADEREDIT_H

#include <QtGui/QMainWindow>

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
struct Effect;
struct EffectFactory;

class QShaderEdit : public QMainWindow
{
	Q_OBJECT
public:
	QShaderEdit(const QString& filename = QString());
	~QShaderEdit();

	virtual QSize sizeHint() const;

public slots:
	void newFile(bool startup=false);
	
protected slots:
	bool load( const QString& fileName );
	
	void open();
	void openRecentFile();
	void clearRecentFiles();
	bool save();
	void saveAs();
	
	void about();
	
	void setAutoCompile(bool enable);
	void shaderTextChanged();
	void keyTimeout();

	void build(bool silent);
	
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
	void updateRecentFileActions();
	
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

	// Actions.
	QAction * m_newAction;
	QAction * m_openAction;
	QAction * m_saveAction;
	QAction * m_saveAsAction;

	enum { MaxRecentFiles = 5 };
	QAction * m_recentFileActions[MaxRecentFiles];	

	QAction * m_recentFileSeparator;
	QAction * m_clearRecentAction;
	
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


#endif //QSHADEREDIT_H



