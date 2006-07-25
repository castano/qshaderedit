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
class TexManager;

class QGLView : public QGLWidget
{
	Q_OBJECT
public:
	QGLView(const QGLFormat & format, QWidget *parent );
	virtual QSize sizeHint() const;
	virtual QSize minimumSizeHint() const;

	bool init(MessagePanel * output);
	void setEffect(Effect * effect);
	void resetEffect();
	
signals:

    void fileDropped( const QString& );

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();	
	
	void resetGL();
	
	void updateMatrices();
	
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

	// Mouse events
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);	
	void wheelEvent(QWheelEvent *e);


private:
	
	float m_alpha;
	float m_beta;
	float m_x, m_y, m_z;
	Qt::MouseButton m_button;
	
	QPoint m_pos;
	
	Effect * m_effect;
	
	GLuint m_dlist;	
	
	TexManager * m_textureManager;
};

#endif // QGLVIEW_H
