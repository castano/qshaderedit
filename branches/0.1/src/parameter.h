#ifndef PARAMETER_H
#define PARAMETER_H

#include <QtCore/QVariant>

QColor variantToColor(const QVariant & v);
QVariant colorToVariant(const QColor & c, int count);


class Parameter
{
public:
	
	enum Widget
	{
		Widget_Default,
		Widget_Color,
		//Widget_Texture,
	};
	
	Parameter();
	Parameter(const QString & name);
	virtual ~Parameter() { }
	
	const QString & name() const { return m_name; }
	const QString & description() const { return m_description; }
	
	void setDescription(const QString& desc) { m_description = desc; }
	
	QVariant value() const { return m_value; }
	virtual void setValue(const QVariant& value);
	
	int type() const { return m_value.userType(); }
	
	virtual QString displayValue() const;	
	virtual QVariant decoration() const;

	virtual bool isEditable() const;
	virtual Widget widget() const { return m_widget; }
	
	bool hasRange() const;
	QVariant minValue() const { return m_minValue; } 
	QVariant maxValue() const { return m_maxValue; }
	void setRange(const QVariant& min, const QVariant& max);
	void clearRange();

	virtual int rows() const = 0;
	virtual int columns() const = 0;
	
	int componentCount() const;
	virtual bool componentsAreEditable() const;
	virtual QString componentName(int idx) const;	
	virtual QVariant componentValue(int idx) const;
	virtual QString componentDisplayValue(int idx) const;
	virtual void setComponentValue(int idx, const QVariant & value);
	
	virtual int componentType() const; 	// the same for all components
	virtual QVariant componentMinValue() const;
	virtual QVariant componentMaxValue() const;
	
protected:
	void setName(const QString& name) { m_name = name; }
	void setWidget(Widget w) { /*Q_ASSERT(m_value.type() == QVariant::List);*/ m_widget = w; }
	
private:
	QString m_name;
	QString m_description;
	
	QVariant m_value;
	QVariant m_minValue, m_maxValue;
	
	Widget m_widget;
};

#endif // PARAMETER_H

