#ifndef GLUTILS_H
#define GLUTILS_H

#include <GL/glew.h>

#include <math.h>

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
	GLThread(QGLWidget * shareWidget) : m_pbuffer(QSize(1,1), shareWidget->format(), shareWidget)
	{
	}
	
	void makeCurrent()
	{
		m_pbuffer.makeCurrent();
	}
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
