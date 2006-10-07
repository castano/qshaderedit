 
#include "parametermodel.h"
#include "parameter.h"
#include "effect.h"


QModelIndex ParameterModel::index(int row, int column, const QModelIndex & parent) const
{
	if( !parent.isValid() ) {
		return createIndex(row, column, -1);
	}
	else {
		return createIndex(row, column, parent.row());
	}
	return QModelIndex();
}

QModelIndex ParameterModel::parent(const QModelIndex & index) const
{
	if( !index.isValid() || isParameter(index) ) {
		return QModelIndex();
	}
	else {
		return createIndex(index.internalId(), 0, -1);
	}
}

int ParameterModel::rowCount(const QModelIndex & parent) const
{
	if( m_effect != NULL ) {
		if( !parent.isValid() ) {
			return m_effect->parameterCount();
		}
		else if( isParameter(parent) ) {
			return parameter(parent)->componentCount();
		}
	}
	return 0;
}

int ParameterModel::columnCount(const QModelIndex & parent) const
{
	Q_UNUSED(parent);
	return 2;
}

QVariant ParameterModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) {
		return QVariant();
	}

	if( isComponent(index) ) {
		// Parameter component.
		if (index.column() == 0) {			
			if (role == Qt::DisplayRole)
				return parameter(index)->componentName(index.row());
		}
		else if (index.column() == 1) {
			if (role == Qt::DisplayRole)
				return parameter(index)->componentDisplayValue(index.row());
			if  (role == Qt::EditRole)
				return parameter(index)->componentValue(index.row());
		}
	}
	else {
		// Root parameter.
		if (index.column() == 0) {
			if (role == Qt::DisplayRole || role == Qt::EditRole) {
				return parameter(index)->name();
			}
			if (role == Qt::ToolTipRole) { // only show tooltip when over col 0, otherwise it gets annoying
				return parameter(index)->description();
			}
		}
		else if (index.column() == 1) {
			if (role == Qt::DisplayRole) {
				return parameter(index)->displayValue();
			}
			else if (role == Qt::EditRole) {
				return parameter(index)->value();
			}
			else if (role == Qt::DecorationRole) {
				return parameter(index)->decoration();
			}
		}
	}
	return QVariant();
}

QVariant ParameterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole) {
		if (orientation == Qt::Horizontal) {
			if( section == 0 ) return tr("Name");
			else if( section == 1 ) return tr("Value");
		}
	}

	return QVariant();
}

QModelIndex ParameterModel::buddy(const QModelIndex & index) const
{
	if (index.column() == 0) {
		return createIndex(index.row(), 1, (qint32)index.internalId());
	}
	return index;
}

Qt::ItemFlags ParameterModel::flags(const QModelIndex &index) const
{
	Q_ASSERT(index.isValid());
	if (index.column() == 1) {
		return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; 
	}
	return QAbstractItemModel::flags(index);
}

bool ParameterModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid() || index.column() != 1 || role != Qt::EditRole)
		return false;

	Parameter* param = parameter(index);
	if( isComponent(index) ) {
		// Parameter component.
		if( param->componentValue(index.row()) != value ) {
			param->setComponentValue(index.row(), value);
			emit dataChanged(index, index);
			QModelIndex parentIndex = createIndex(index.internalId(), 1, -1);
			emit dataChanged(parentIndex, parentIndex);
		}
		return true;
	}
	else {
		// Root parameter.
		if( param->value() != value ) {
			param->setValue(value);
			emit dataChanged(index, index);
		}
		return true;
	}
	return false;
}

void ParameterModel::clear()
{
	m_effect = NULL;
	reset();
}

void ParameterModel::setEffect(Effect * effect)
{
	Q_ASSERT(effect != NULL);
	m_effect = effect;
	reset();
}

Parameter* ParameterModel::parameter(const QModelIndex& index) const
{
	if (isParameter(index)) {
		Q_ASSERT(index.row() < m_effect->parameterCount());	
		return m_effect->parameterAt(index.row());
	}
	else {
		Q_ASSERT(index.internalId() < m_effect->parameterCount()); 
		return m_effect->parameterAt(index.internalId());
	}
}

// static
bool ParameterModel::isParameter(const QModelIndex &index)
{
	return index.internalId() == -1;
}

// static
bool ParameterModel::isComponent(const QModelIndex &index)
{
	return index.internalId() != -1;
}
