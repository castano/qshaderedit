//
// C++ Interface: managerpanel
//
// Description: 
//
//august 2009
// Author: Peter Komar <marcusk@i.ua>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MANAGERPANEL_H
#define MANAGERPANEL_H

#include <QtGui/QDockWidget>

class QToolBox;
class QListWidget;
class QPushButton;
class QTreeView;
class QTabWidget;
class QMenu;
class QListWidgetItem;

/**
	@author Peter Komar <marcusk@i.ua>
*/
class ManagerPanel : public QDockWidget
{
Q_OBJECT
public:
	ManagerPanel(const QString & title, QWidget * parent = 0, Qt::WindowFlags flags = 0 );

    ~ManagerPanel();

    void loadShaders();

private:
	void init_panel();
        bool get_text_shader(const QString& fileName, QString& vertex, QString& fragment);
	//void render_view_shaders();
		
	QToolBox *toolB;
        QListWidget *list_glsl;
        QListWidget *list_cgfx;
        QListWidget *list_arb;
        QTabWidget *bar;
        //QMenu *context_menu;
	
Q_SIGNALS:
        void emit_shader_name(const QString& Name_Lib);
	
private Q_SLOTS:
        void slot_click_bshader(QListWidgetItem * item);

};

#endif
