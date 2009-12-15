//
// C++ Implementation: managerpanel
//
// Description: 
//
// august 2009
// Author: Peter Komar <marcusk@i.ua>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "managerpanel.h"
#include "titlebar.h"
#include "qshaderedit.h"
#include <QtGui/QTabWidget>
#include <QtGui/QListWidget>
#include <QtGui/QToolBox>
#include <QtCore/QSize>
#include <QtCore/QDir>


ManagerPanel::ManagerPanel(const QString & title, QWidget * parent, Qt::WindowFlags flags )
	: QDockWidget(title, parent, flags)
{
    setTitleBarWidget(new TitleBar(this));
    init_panel();
}


ManagerPanel::~ManagerPanel()
{
}


void ManagerPanel::init_panel()
{
	toolB = new QToolBox(this);
	

       bar = new QTabWidget(toolB);
       list_glsl = new QListWidget;
       list_cgfx  = new QListWidget;
       list_arb = new QListWidget;

       bar->addTab(list_glsl, tr("GLSL shaders"));
       bar->addTab(list_cgfx, tr("CgFX effects"));
       bar->addTab(list_arb, tr("ARB programs"));
       bar->setTabPosition(QTabWidget::West);

       list_glsl->setViewMode(QListWidget::IconMode);
       list_glsl->setWrapping(true);
       list_glsl->setResizeMode(QListWidget::Adjust);
       list_glsl->setUniformItemSizes(true);
       list_glsl->setIconSize(QSize(100,100));

       list_cgfx->setViewMode(QListWidget::IconMode);
       list_cgfx->setWrapping(true);
       list_cgfx->setResizeMode(QListWidget::Adjust);
       //list_cgfx->setUniformItemSizes(true);
       list_cgfx->setIconSize(QSize(100,100));

       list_arb->setViewMode(QListWidget::IconMode);
       list_arb->setWrapping(true);
       list_arb->setResizeMode(QListWidget::Adjust);
       list_arb->setUniformItemSizes(true);
       list_arb->setIconSize(QSize(100,100));

        connect(list_glsl,SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(slot_click_bshader(QListWidgetItem*)));
        connect(list_cgfx,SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(slot_click_bshader(QListWidgetItem*)));
        connect(list_arb,SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(slot_click_bshader(QListWidgetItem*)));

	
	
        toolB->addItem(bar, tr("Library Shaders"));
	setWidget(toolB);
        setVisible(TRUE);
}

void ManagerPanel::loadShaders()
{
    list_glsl->clear();
    list_cgfx->clear();
    list_arb->clear();

     QString path_shaders = "library";
     QShaderEdit::find_data_dir(path_shaders);
     QDir dir(path_shaders);
     dir.setFilter(QDir::Files | QDir::NoSymLinks);
     dir.setSorting(QDir::Size | QDir::Reversed);
     dir.setNameFilters(QStringList()<< "*.glsl" << "*.cgfx" << "*.asm");

     QFileInfoList list = dir.entryInfoList();

     QStringList slist_glsl, slist_cgfx, slist_arb;

     for (int i = 0; i < list.size(); ++i) {
         QFileInfo fileInfo = list.at(i);

         if(fileInfo.suffix() == "glsl")
             slist_glsl.append(fileInfo.baseName());
         else if(fileInfo.suffix() == "cgfx")
                 slist_cgfx.append(fileInfo.baseName());
         else if(fileInfo.suffix() == "asm")
                 slist_arb.append(fileInfo.baseName());
     }

     QListWidgetItem *pi=0;

     foreach(QString str, slist_glsl)
     {
         pi = new QListWidgetItem(str, list_glsl);
         pi->setIcon(QPixmap(path_shaders+"/"+str+"_glsl.png"));
         pi->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
     }

     foreach(QString str, slist_cgfx)
     {
         pi = new QListWidgetItem(str, list_cgfx);
         pi->setIcon(QPixmap(path_shaders+"/"+str+"_cgfx.png"));
         pi->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
     }

     foreach(QString str, slist_arb)
     {
         pi = new QListWidgetItem(str, list_arb);
         pi->setIcon(QPixmap(path_shaders+"/"+str+"_asm.png"));
         pi->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
     }
}


void ManagerPanel::slot_click_bshader(QListWidgetItem * item)
{
    QString shader;
    QString path_shaders = "library";
    QShaderEdit::find_data_dir(path_shaders);
    switch(bar->currentIndex())
    {
        case 0: shader = path_shaders+"/"+item->text()+".glsl"; break;
        case 1: shader = path_shaders+"/"+item->text()+".cgfx"; break;
        case 2: shader = path_shaders+"/"+item->text()+".asm"; break;
    }

   emit emit_shader_name(shader);
}
