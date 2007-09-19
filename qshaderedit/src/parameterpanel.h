#ifndef PARAMETERPANEL_H
#define PARAMETERPANEL_H

#include <QtGui/QDockWidget>
#include <QtGui/QTreeView>

class Effect;
class ParameterModel;
class ParameterDelegate;


class ParameterPanel : public QDockWidget
{
	Q_OBJECT
public:

	ParameterPanel(const QString & title, QWidget * parent = 0, Qt::WFlags flags = 0);
	ParameterPanel(QWidget * parent = 0, Qt::WFlags flags = 0);
	~ParameterPanel();

	virtual QSize sizeHint() const;

	bool isEditorActive() const;
	
	static const QString & lastPath();
	static void setLastPath(const QString & lastPath);
	
signals:
	void parameterChanged();

public slots:
	void setEffect(Effect * effect);

private:
	void initWidget();

private:

	ParameterModel * m_model;
	ParameterDelegate * m_delegate;
	QTreeView * m_view;

	static QString s_lastTexture; 
};



#endif // PARAMETERPANEL_H
