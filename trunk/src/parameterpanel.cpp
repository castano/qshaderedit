
#include "parameterpanel.h"
#include "effect.h"
#include "parametermodel.h"
#include "parameterdelegate.h"

#include <QtGui/QHeaderView>


ParameterPanel::ParameterPanel(const QString & title, QWidget * parent /*= 0*/, Qt::WFlags flags /*= 0*/) :
		QDockWidget(title, parent, flags), m_model(NULL), m_delegate(NULL), m_view(NULL)
{
	initWidget();
}

ParameterPanel::ParameterPanel(QWidget * parent /*= 0*/, Qt::WFlags flags /*= 0*/) :
		QDockWidget(parent, flags), m_model(NULL), m_delegate(NULL), m_view(NULL)
{
	initWidget();
}

ParameterPanel::~ParameterPanel()
{
	delete m_model;
	m_model = NULL;
	delete m_delegate;
	m_delegate = NULL;
	delete m_view;
	m_view = NULL;
}

QSize ParameterPanel::sizeHint() const
{
	return QSize(200, 200);
}

bool ParameterPanel::isEditorActive() const
{
	return m_delegate->isEditorActive();
}

void ParameterPanel::initWidget()
{
	m_model = new ParameterModel(this);
	connect(m_model, SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SIGNAL(parameterChanged()));

	m_delegate = new ParameterDelegate(this);

	m_view = new QTreeView(this);
	m_view->setModel(m_model);
	m_view->setItemDelegate(m_delegate);
	m_view->header()->setStretchLastSection(true);
	m_view->header()->setClickable(false);
	m_view->setAlternatingRowColors(true);
	m_view->setEditTriggers(QAbstractItemView::AllEditTriggers);
	m_view->setTextElideMode(Qt::ElideMiddle);

	//	m_view->setIndentation(0);	// @@ This would be nice if it didn't affect the roots.

	setWidget(m_view);
}

void ParameterPanel::setEffect(Effect * effect)
{
	if (effect != NULL)
	{
		m_model->setEffect(effect);
		m_view->resizeColumnToContents(0);
	}
	else
	{
		m_model->clear();
	}
}

/*static*/ const QString & ParameterPanel::lastPath()
{
	return ParameterDelegate::lastPath();
}

/*static*/ void ParameterPanel::setLastPath(const QString & lastPath)
{
	ParameterDelegate::setLastPath(lastPath);
}

