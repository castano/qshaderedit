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
