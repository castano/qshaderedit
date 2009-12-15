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

#include "qglview.h"
#include "effect.h"
#include "messagepanel.h"
#include "texmanager.h"
#include "scene.h"
#include "glutils.h"

#include <QtCore/QUrl>
#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>


SceneView::SceneView(QWidget * parent, QGLWidget * shareWidget) : QGLWidget(parent, shareWidget),
	m_effect(NULL), 
	m_scene(NULL), 
	m_wireframe(false), 
    m_ortho(false)
{
	setAutoBufferSwap(false);
}


SceneView::~SceneView()
{
}


QSize SceneView::sizeHint() const
{
	return QSize(200, 200);
}
QSize SceneView::minimumSizeHint() const
{
	return QSize(100, 100);
}


void SceneView::setEffect(Effect * effect)
{
	m_effect = effect;
	if( m_effect != NULL ) {
		makeCurrent();
	}
}

void SceneView::setScene(Scene * scene)
{
	if( m_scene != NULL ) {
		delete m_scene;
	}
	m_scene = scene;
	resetTransform();
	emit updateGL();
}


/* @@ Move to system info dialog.
void SceneView::init(MessagePanel * output)
{
	Q_ASSERT(output != NULL);
	
	makeCurrent();
	setAutoBufferSwap(false);
	
	const char * vendor = (const char *)glGetString(GL_VENDOR);
	output->info(QString("OpenGL vendor: ").append(vendor));
	
	const char * renderer = (const char *)glGetString(GL_RENDERER);
	output->info(QString("OpenGL renderer: ").append(renderer));
		
	const char * version = (const char *)glGetString(GL_VERSION);
	output->info(QString("OpenGL version: ").append(version));
	
	output->info(QString("OpenGL extensions:"));
	
	if( GLEW_ARB_vertex_shader ) output->info("- ARB_vertex_shader FOUND");
	else output->error("- ARB_vertex_shader NOT FOUND");
	
	if( GLEW_ARB_fragment_shader ) output->info("- ARB_fragment_shader FOUND");
	else output->error("- ARB_fragment_shader NOT FOUND");
	
	if( GLEW_ARB_shader_objects ) output->info("- ARB_shader_objects FOUND");
	else output->error("- ARB_shader_objects NOT FOUND");
	
	if( GLEW_ARB_shading_language_100 ) output->info("- ARB_shading_language_100 FOUND");
	else output->error("- ARB_shading_language_100 NOT FOUND");
	
	if( GLEW_VERSION_2_0 ) {
		const char * glsl_version = (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
		output->info(QString("GLSL version: ").append(glsl_version));
	}
}
*/

void SceneView::initializeGL()
{
        //glClearColor (0.0, 0.0, 0.0, 0.0);

        glClearColor(0.217,0.216, 0.215, 1.0);

	glEnable(GL_DEPTH_TEST);
	
        m_alpha = -138.421f;
        m_beta = 24.2105f;
        m_x = -0.29f;
        m_y = 0.0f;
        m_z = 5.0f;

	m_scene = SceneFactory::defaultScene();
	
	/*
	// Set special settings for mesa.
	const char * vendor = (const char *)glGetString(GL_VENDOR);
	
	if( qstrcmp(vendor, "Brian Paul") == 0 ) {
		glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST );
	}
	*/
}

void SceneView::resetGL()
{
	delete m_scene;
}


void SceneView::resizeGL(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	updateMatrices();
}

void SceneView::paintGL()
{
	if( !isVisible() )
	{
		// @@ updateGL should not be called when the window is hidden!
		return;
	}
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	if( m_scene != NULL )
	{
		if (m_effect != NULL && !m_effect->isBuilding() && m_effect->isValid())
		{
			// Setup ligh parameters @@ Move this to scene->setup() or begin()
            float light_vector[4] = {1.2f/sqrt(3.08f), 1.0f/sqrt(3.08f), 0.8f/sqrt(3.08f), 0.0f};
            glLightfv( GL_LIGHT0, GL_POSITION, light_vector );

			if (m_wireframe) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			
			m_effect->begin();
			
			for(int i = 0; i < m_effect->getPassNum(); i++)
			{
				m_effect->beginPass(i);
				
				m_scene->draw(m_effect);
				
				m_effect->endPass();
			}
			
			m_effect->end();
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			m_scene->draw(NULL);
		}

	}

    /*qglColor(Qt::white);

    QFont f("Sans",10,10,false);
    qglColor(Qt::white);
    QString str = QString("%1 RX: %2, RY: %3").arg(tr("Rotation")).arg(m_alpha).arg(m_beta);
    str += QString("   %1: X=%2, Y=%3, Z=%4").arg(tr("Position")).arg(m_x).arg(m_y).arg(m_z);

    renderText(0,15,str,f);*/


    swapBuffers();
	
    //qDebug("paint!");
}


void SceneView::updateMatrices()
{
	makeCurrent();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	float aspect = float(width())/float(height());
	
	if( m_ortho ) {
		glOrtho(-aspect,aspect, -1,1, -30,30);
		glScalef(m_z/5, m_z/5, m_z/5);
	}
	else {
		perspective(30, aspect, 0.3, 50);
	}
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// World transform:
    gluLookAt(m_x, m_y, m_z, m_x, m_y, m_z-1, 0, 1, 0);
    glRotatef(m_beta, 1, 0, 0);
	glRotatef(m_alpha, 0, 1, 0);
	
	// Object transform:
    m_scene->transform();
}


void SceneView::mousePressEvent(QMouseEvent *event)
{
	m_pos = event->pos();
	m_button = event->button();
}

void SceneView::mouseMoveEvent(QMouseEvent *event)
{
	QPoint pos = event->pos();

	if(m_button == Qt::LeftButton && event->modifiers() == Qt::NoModifier) {
		m_alpha += (240.0f * (pos - m_pos).x()) / height();
		m_beta += (240.0f * (pos - m_pos).y()) / height();
		if(m_beta < -90) m_beta = -90;
		else if(m_beta > 90) m_beta = 90;
	}
	else if(m_button == Qt::RightButton ||
		(m_button == Qt::LeftButton && event->modifiers() == Qt::ControlModifier)) 
	{
		m_x -= (0.5 * m_z * (pos - m_pos).x()) / height();
		m_y += (0.5 * m_z * (pos - m_pos).y()) / height();
	}
	else if(m_button == Qt::MidButton ||
		(m_button == Qt::LeftButton && event->modifiers() == Qt::ShiftModifier)) 
	{
		m_z -= (m_z * (pos - m_pos).y()) / height();
		if( m_z < 0.00001 ) m_z = 0.00001;
	}

	m_pos = pos;

	updateMatrices();
	emit updateGL();
}

void SceneView::mouseReleaseEvent(QMouseEvent *event)
{
	QPoint pos = event->pos();
	m_button = Qt::NoButton;
}

void SceneView::wheelEvent(QWheelEvent *e)
{
	m_z += (m_z * e->delta()/120.0)/20.0;
	updateMatrices();
	emit updateGL();
}

void SceneView::resetTransform()
{
    m_alpha = -138.421f;
    m_beta = 24.2105f;
    m_x = -0.29f;
    m_y = 0.0f;
    m_z = 5.0f;
	updateMatrices();	
}

bool SceneView::isWireframe() const
{
	return m_wireframe;
}

void SceneView::setWireframe(bool b)
{
	m_wireframe = b;
	emit updateGL();
}

bool SceneView::isOrtho() const
{
	return m_ortho;
}

void SceneView::setOrtho(bool b)
{
	m_ortho = b;
	updateMatrices();
	emit updateGL();
}

/*************************Peter Komar code, august 2009 ************************/
QImage SceneView::takeSnapshot()
{
    emit updateGL();

    //QPixmap pix = renderPixmap();

    QImage imageToSave = /*pix.toImage();//*/grabFrameBuffer(false);

    return imageToSave;
}

/********************************************************************/
