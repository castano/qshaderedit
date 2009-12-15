/*
    QShaderEdit - Simple multiplatform shader editor
    Copyright (C) 2007 Ignacio Castaño <castano@gmail.com>
    Copyright (C) 2007 Lars Uebernickel <larsuebernickel@gmx.de>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef QSHADEREDIT_H
#define QSHADEREDIT_H

#include <QtGui/QMainWindow>
#include <QtGui/QLabel>
#include <QtGui/QPixmap>
#include <QtGui/QColor>

class QTimer;
class QComboBox;
class QLabel;
class QToolBar;

class QGLWidget;

class ManagerPanel;

class MessagePanel;
class ParameterPanel;
class ScenePanel;
class Editor;
class Document;
struct Effect;
struct EffectFactory;

class QShaderEdit : public QMainWindow
{
	Q_OBJECT
public:
	QShaderEdit(const QString& filename = QString());
	~QShaderEdit();

	virtual QSize sizeHint() const;
        /********************** code makus **************************************************/
        static void find_data_dir(QString& path); //return path to data: styles, lybrary
        /***********************************************************************************/

protected slots:
	
	void about();
	
	void openRecentFile();
	void clearRecentFiles();

	void onCursorPositionChanged();	
	void onShaderTextChanged();
	void onModifiedChanged(bool modified);
	
	void onActivityTimeout();
	
	void onTitleChanged(QString title);
	void onFileNameChanged(QString fileName);
	void onEffectCreated();
	void onEffectDeleted();
	void onEffectBuilding();
	void onEffectBuilt(bool succeed);
	void onParameterChanged();
	void onTechniqueChanged(int index);

        /*****************Peter Komar code, august 2009 ********************/

        void slot_styles();
        void slotRenderPreview();
        void slotConfigureEditor();

        /**************************************************/
	
	void updateEffectInputs();	
	
protected:
	
	void initGL();
	
	void createDocument();
	
	void createEditor();
	
	void createActions();
	void createMenus();
	void createToolbars();
	void createStatusbar();
	void createDockWindows();

        /******************Peter Komar code, august 2009 ****************************/
        //creates preview shaders in manager
        void takeEffectPreview(const QString& name);
        void CreatePixmap(const QString& name);
        /**********************************************************/

	
	void updateActions();
	void updateTechniques();
	void updateWindowTitle(QString title);
	void updateRecentFileActions();
	void addRecentFile(QString filename);
	

	// Events
	virtual void closeEvent(QCloseEvent * event);
	virtual void keyPressEvent(QKeyEvent * event);
	virtual void dragEnterEvent(QDragEnterEvent *event);
	virtual void dropEvent(QDropEvent * event);
	
    void loadSettings();
    void saveSettings();
	
	
	/*
	void techniqueChanged(int index);
	void cursorPositionChanged();
	void selectScene();
	*/
	
	/*
	QAction * m_compileAction;
	*/
	
	/*
	// settings.
	bool m_autoCompile;
	QString m_lastEffect;
	*/
	
	QGLWidget * m_glWidget;
	Document * m_document;	
	
	// Central widget.
	Editor * m_editor;
	
	// Toolbars.
	QToolBar * m_fileToolBar;
	QToolBar * m_techniqueToolBar;
	QComboBox * m_techniqueCombo;
	
	// Status bar.
	QLabel * m_statusLabel;
	
	// Panels.
	MessagePanel * m_messagePanel;
	ParameterPanel * m_parameterPanel;
	ScenePanel * m_scenePanel;
	ManagerPanel * m_managerPanel;
	
	// Actions.
	QAction * m_newAction;
	QAction * m_openAction;
	QAction * m_saveAction;
	QAction * m_saveAsAction;

	enum { MaxRecentFiles = 5 };
	QAction * m_recentFileActions[MaxRecentFiles];	

	QAction * m_recentFileSeparator;
	QAction * m_clearRecentAction;
	
	QAction * m_compileAction;
	
	QAction * m_findAction;
	QAction * m_findNextAction;
	QAction * m_findPreviousAction;
	QAction * m_gotoAction;

        QList<QAction*> stylesActions;


	// Timer.
	QTimer * m_activityTimer;
};

/**********************Peter Komar code, august 2009 *********************/

class QAbout : public QLabel
{
        Q_OBJECT
public:
    QAbout(const QPixmap& pixmap, const QString& version, const QString& text, const QColor& color=Qt::white,QWidget * parent = 0);

protected:
    void init_data();
    void mousePressEvent( QMouseEvent * event );

    QPixmap m_pixmap;
    QString m_version;
    QString m_text;
    QColor m_color;
};
/*******************************************************/

#endif //QSHADEREDIT_H



