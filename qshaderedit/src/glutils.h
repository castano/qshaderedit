#ifndef GLUTILS_H
#define GLUTILS_H

#include <GL/glew.h>

#include <QtCore/QThread>
#include <QtOpenGL/QGLContext>
#include <QtOpenGL/QGLPixelBuffer>


/// Report OpenGL errors.
void ReportGLErrors();


/// A thread that wraps a dummy GL context. 
class GLThread : public QThread
{
	QGLPixelBuffer m_pbuffer;
	
public:
	GLThread() : m_pbuffer(QSize(1,1), QGLContext::currentContext()->format())
	{
	}
	
	void makeCurrent()
	{
		m_pbuffer.makeCurrent();
	}
};
	

#endif // GLUTILS_H
