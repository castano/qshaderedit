	
#include "glutils.h"


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


