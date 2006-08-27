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
class QMenu;
class QAction;

class QGLView : public QGLWidget
{
	Q_OBJECT
public:
	QGLView(const QGLFormat & format, QWidget *parent );
	virtual ~QGLView();
	
	virtual QSize sizeHint() const;
	virtual QSize minimumSizeHint() const;

	bool init(MessagePanel * output);
	void setEffect(Effect * effect);
	void resetEffect();
	
	void setScene(Scene * scene);
	void populateMenu(QMenu * menu);

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
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);	
	virtual void wheelEvent(QWheelEvent *e);

	void resetTransform();
	
protected slots:
	
	void settingsChanged();
	
private:
	
	float m_alpha;
	float m_beta;
	float m_x, m_y, m_z;
	Qt::MouseButton m_button;
	
	QPoint m_pos;
	
	Effect * m_effect;
	
	Scene * m_scene;
	
	QAction * m_wireframeAction;
	QAction * m_orthoAction;
	
};

#endif // QGLVIEW_H
