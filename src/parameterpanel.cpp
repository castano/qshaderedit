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

