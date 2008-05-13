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
