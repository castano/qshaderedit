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

#ifndef GLUTILS_H
#define GLUTILS_H

#include <GL/glew.h>

#include <math.h>

#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtOpenGL/QGLContext>
#include <QtOpenGL/QGLWidget>


/// Report OpenGL errors.
void ReportGLErrors();


class GLWidget : public QGLWidget
{
	GLWidget(QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0) : QGLWidget(parent, shareWidget, f)
	{
	}
	
	void lock() { return m_mutex.lock(); }
	bool tryLock() { return m_mutex.tryLock(); }
	void unlock() { return m_mutex.unlock(); }
	
protected:
	
	mutable QMutex m_mutex;
	
};


/// A thread that wraps a dummy GL context. 
class GLThread : public QThread
{
	QGLWidget * m_glWidget;
	
public:
	GLThread(QGLWidget * shareWidget)
	{
		m_glWidget = new QGLWidget(NULL, shareWidget);
	}
	~GLThread()
	{
		delete m_glWidget;
	}
	
	void makeCurrent();
	void doneCurrent();
};



inline float toDegrees(float radians) { return radians * (180.0f / M_PI); }
inline float toRadians(float degrees) { return degrees * (M_PI / 180.0f); }

inline void perspective(float fov, float aspect, float zNear, float zFar)
{
	if (aspect < 1) {
		// fov applies to the smaller dimension.
		float real_fov = toDegrees(2 * atan(tan(toRadians(fov/2))/aspect));
		gluPerspective(real_fov, aspect, zNear, zFar);
	}
	else {
		gluPerspective(fov, aspect, zNear, zFar);
	}
}




#endif // GLUTILS_H
