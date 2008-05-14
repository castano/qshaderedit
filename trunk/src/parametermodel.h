/*
    QShaderEdit - Simple multiplatform shader editor
    Copyright (C) 2007 Lars Uebernickel <larsuebernickel@gmx.de>
    Copyright (C) 2007 Ignacio Castaño <castano@gmail.com>

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

#ifndef PARAMETERMODEL_H
#define PARAMETERMODEL_H

#include <QtCore/QAbstractItemModel>

class Effect;
class Parameter;


class ParameterModel: public QAbstractItemModel
{
	Q_OBJECT
public:
	ParameterModel(QObject *parent = 0) :
		QAbstractItemModel(parent), m_effect(NULL) {}

	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount( const QModelIndex & parent = QModelIndex() ) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

	virtual QModelIndex buddy(const QModelIndex & index) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	
	void clear();
	void setEffect(Effect * effect);
	Parameter* parameter(const QModelIndex& index) const;
	
	// Helper methods.
	bool isEditable(const QModelIndex &index) const;
	static bool isParameter(const QModelIndex &index);
	static bool isComponent(const QModelIndex &index);
	
private:
	Effect* m_effect;
};

#endif // PARAMETERMODEL_H
