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
