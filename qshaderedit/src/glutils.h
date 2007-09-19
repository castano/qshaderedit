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
	
	void makeCurrent()
	{
		m_glWidget->makeCurrent();
	}
	void doneCurrent()
	{
		m_glWidget->doneCurrent();
	}
};


/*
/// A thread that wraps a dummy GL context. 
class GLThread : public QThread
{
	QGLContext m_context;
	
public:
	GLThread(const QGLContext * shareContext) : m_context(shareContext->format())
	{
		m_context.create(shareContext);
	}
	
	void makeCurrent()
	{
		if (m_context.isValid())
		{
			m_context.makeCurrent();
		}
	}
};
*/


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
