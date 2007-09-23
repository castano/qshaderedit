#ifndef SCENEPANEL_H
#define SCENEPANEL_H

#include <QtGui/QDockWidget>

class QTimer;
class QGLWidget;
class QMenu;

class Effect;
class SceneView;

class ScenePanel : public QDockWidget
{
	Q_OBJECT
public:

	ScenePanel(const QString & title, QWidget * parent = 0, QGLWidget * shareWidget = 0, Qt::WFlags flags = 0);
	~ScenePanel();

	void setEffect(Effect * effect);
	
	QMenu * menu();
	
	void setViewUpdatesEnabled(bool enable);
	
	void startAnimation();
	void stopAnimation();
	
	
public slots:
	
	void refresh();
	void selectScene();	
	
private:

	SceneView * m_view;
	QTimer * m_animationTimer;
	
	QMenu * m_sceneMenu;
	QMenu * m_renderMenu;
	QAction * m_wireframeAction;
	QAction * m_orthoAction;
	
};



#endif // SCENEPANEL_H
