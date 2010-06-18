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

#ifndef QGLVIEW_H
#define QGLVIEW_H

// Include GLEW before anything else.
#include <GL/glew.h>

#include <QtOpenGL/QGLWidget>


class QRectF;
class QWheelEvent;
class QMouseEvent;
class Effect;
class MessagePanel;
class Scene;

class SceneView : public QGLWidget
{
	Q_OBJECT
public:
	SceneView(QWidget * parent, QGLWidget * shareWidget);
	virtual ~SceneView();
	
	virtual QSize sizeHint() const;
	virtual QSize minimumSizeHint() const;

	void setEffect(Effect * effect);
	
	void setScene(Scene * scene);

	bool isWireframe() const;
	bool isOrtho() const;
	
public slots:
	
	void setWireframe(bool b);	
	void setOrtho(bool b);
	

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();	
	
	void resetGL();
	
	void updateMatrices();
	
	// Mouse events
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);	
	virtual void wheelEvent(QWheelEvent *e);

	void resetTransform();
	
private:
	
	float m_alpha;
	float m_beta;
	float m_x, m_y, m_z;
	Qt::MouseButton m_button;
	
	QPoint m_pos;
	
	Effect * m_effect;
	
	Scene * m_scene;
	
	bool m_wireframe;
	bool m_ortho;
};

#endif // QGLVIEW_H
