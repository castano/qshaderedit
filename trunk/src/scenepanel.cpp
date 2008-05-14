/*
    QShaderEdit - Simple multiplatform shader editor
    Copyright (C) 2007 Ignacio Castaño <castano@gmail.com>

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

#include "scenepanel.h"

#include <QtCore/QTimer>
#include <QtGui/QMenu>
#include <QtGui/QAction>

#include "qglview.h"
#include "scene.h"


ScenePanel::ScenePanel(const QString & title, QWidget * parent /*= 0*/, QGLWidget * shareWidget /*= 0*/, Qt::WFlags flags /*= 0*/) :
	QDockWidget(title, parent, flags), m_view(NULL)
{
	m_view = new SceneView(this, shareWidget);
	setWidget(m_view);
	
	m_animationTimer = new QTimer(this);
	connect(m_animationTimer, SIGNAL(timeout()), this, SLOT(refresh()));
	

	m_sceneMenu = new QMenu(tr("&Scene"), this);
	
	QMenu * sceneSelectionMenu = m_sceneMenu->addMenu(tr("&Select"));
	
	// Use scene plugins to create menus.
	const int count = SceneFactory::factoryList().count();
	for(int i = 0; i < count; i++) {
		const SceneFactory * factory = SceneFactory::factoryList().at(i);
		Q_ASSERT(factory != NULL);
		
		QAction * action = new QAction(QString("&%1 %2").arg(i+1).arg(factory->name()), this);
		action->setData(factory->name());
		connect(action, SIGNAL(triggered()), this, SLOT(selectScene()));
		sceneSelectionMenu->addAction(action);
	}
	
	m_renderMenu = m_sceneMenu->addMenu(tr("Render Options"));
	
	m_wireframeAction = new QAction(tr("Wireframe"), this);
	m_wireframeAction->setCheckable(true);
	m_wireframeAction->setChecked(false);
	connect(m_wireframeAction, SIGNAL(toggled(bool)), m_view, SLOT(setWireframe(bool)));
	
	m_orthoAction = new QAction(tr("Ortho"), this);
	m_orthoAction->setCheckable(true);
	m_orthoAction->setChecked(false);
	connect(m_orthoAction, SIGNAL(toggled(bool)), m_view, SLOT(setOrtho(bool)));
	
	m_renderMenu->addAction(m_wireframeAction);
	m_renderMenu->addAction(m_orthoAction);
}

ScenePanel::~ScenePanel()
{
}

void ScenePanel::setEffect(Effect * effect)
{
	Q_ASSERT(m_view);
	m_view->setEffect(effect);
}

QMenu * ScenePanel::menu()
{
	return m_sceneMenu;
}

void ScenePanel::setViewUpdatesEnabled(bool enable)
{
	m_view->setUpdatesEnabled(enable);
}

void ScenePanel::startAnimation()
{
	m_animationTimer->start(30);
}

void ScenePanel::stopAnimation()
{
	m_animationTimer->stop();
}

void ScenePanel::refresh()
{
	m_view->updateGL();
}

void ScenePanel::selectScene()
{
	QAction * action = qobject_cast<QAction *>(sender());
	if( action != NULL )
	{
		const SceneFactory * factory = SceneFactory::findFactory(action->data().toString());
		Q_ASSERT(factory != NULL);
		m_view->setScene(factory->createScene());
	}
}

