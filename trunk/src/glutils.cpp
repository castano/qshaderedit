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

#if QT_WS_X11
#endif

#include "glutils.h"

//#include <QtGui/QX11Info>

//extern "C"
//{
//#include <X11/Xlib-xcb.h>
//#include <xcb/xcbxlib.h>
//}


// Report OpenGL errors.
void ReportGLErrors() 
{
	int error = glGetError();

	if( error != GL_NO_ERROR ) {
		switch( error ) {
			case GL_INVALID_ENUM:
				qDebug( "*** ReportGLErrors: Invalid enum.\n" );
				break;
			case GL_INVALID_VALUE:
				qDebug( "*** ReportGLErrors: Invalid value.\n" );
				break;
			case GL_INVALID_OPERATION:
				qDebug( "*** ReportGLErrors: Invalid operation.\n" );
				break;
			case GL_STACK_OVERFLOW:
				qDebug( "*** ReportGLErrors: Stack overflow.\n" );
				break;
			case GL_STACK_UNDERFLOW:
				qDebug( "*** ReportGLErrors: Stack underflow.\n" );
				break;
			case GL_OUT_OF_MEMORY:
				qDebug( "*** ReportGLErrors: Out of memory.\n" );
				break;
			default:
				qDebug( "*** ReportGLErrors: Unknown error!\n" );
		}
	}
}

/*
class XLock
{
	Display * const dpy;
	
public:
	XLock() : dpy(QX11Info::display())
	{
		XLockDisplay(dpy);
		//xcb_connection_t * c = XGetXCBConnection(dpy);
		//xcb_xlib_lock(c);
	}
	~XLock()
	{
		//xcb_xlib_unlock(c);
		XUnlockDisplay(dpy);
	}
};
*/

void GLThread::makeCurrent()
{
	//XLock lock;
	//XLockDisplay(QX11Info::display());
	m_glWidget->makeCurrent();
}

void GLThread::doneCurrent()
{
	//XLock lock;
	m_glWidget->doneCurrent();
	//XUnlockDisplay(QX11Info::display());
}

