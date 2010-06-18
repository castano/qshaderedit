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

#ifndef SCENEPANEL_H
#define SCENEPANEL_H

#include <QtGui/QDockWidget>

class QTimer;
class QGLWidget;
class QMenu;

class Effect;
class SceneView;

class ScenePanel : public QDockWidget
{
	Q_OBJECT
public:

	ScenePanel(const QString & title, QWidget * parent = 0, QGLWidget * shareWidget = 0, Qt::WFlags flags = 0);
	~ScenePanel();

	void setEffect(Effect * effect);
	
	QMenu * menu();
	
	void setViewUpdatesEnabled(bool enable);
	
	void startAnimation();
	void stopAnimation();
	
	
public slots:
	
	void refresh();
	void selectScene();	
	
private:

	SceneView * m_view;
	QTimer * m_animationTimer;
	
	QMenu * m_sceneMenu;
	QMenu * m_renderMenu;
	QAction * m_wireframeAction;
	QAction * m_orthoAction;
	
};



#endif // SCENEPANEL_H
