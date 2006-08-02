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
	QShaderEdit();
	~QShaderEdit();

	virtual QSize sizeHint() const;

public slots:
	void newFile();

protected slots:
    void load( const QString& fileName );

	void open();
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
	
	// Events
	virtual void closeEvent(QCloseEvent * event);
	virtual void keyPressEvent(QKeyEvent * event);
	
private:

    void loadSettings();
    void saveSettings();

	Editor * m_editor;
	
	QToolBar * m_fileToolBar;
	QToolBar * m_techniqueToolBar;
	
	QDockWidget * m_sceneViewDock;
	ParameterPanel * m_paramViewDock;
	MessagePanel * m_logViewDock;
	QLabel * m_positionLabel;
	
	QGLView * m_sceneView;

	QAction * m_newAction;
	QAction * m_openAction;
	QAction * m_saveAction;
	QAction * m_saveAsAction;
	
	QComboBox * m_techniqueCombo;
	
	QTimer * m_timer;	// compilation timer.
	QTimer * m_animationTimer;
	
	QFile * m_file;
	const EffectFactory * m_effectFactory;
	Effect * m_effect;
	bool m_modified;
	
	// settings.
	bool m_autoCompile;
	QString m_openDir;
};


#endif //QSHADEREDIT_H



