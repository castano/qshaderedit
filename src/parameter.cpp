
#include "parameter.h"
#include "texmanager.h"

#include <QtCore/QFileInfo>
#include <QtGui/QColor>
#include <QtGui/QPixmap>


QColor variantToColor(const QVariant & v)
{
	QColor color(127, 127, 127, 255);
	if (v.canConvert(QVariant::List)) {
		QVariantList list = v.toList();
		int count = list.count();
		switch(count) {
			case 4: color.setAlphaF(list.at(3).toDouble());
			case 3: color.setBlueF(list.at(2).toDouble());
			case 2: color.setGreenF(list.at(1).toDouble());
			case 1: color.setRedF(list.at(0).toDouble());
		}
	}
	return color;
}

QVariant colorToVariant(const QColor & c, int count)
{
	QVariantList list;
	if(count > 0) list << c.redF();
	if(count > 1) list << c.greenF();
	if(count > 2) list << c.blueF();
	if(count > 3) list << c.alphaF();
	return list;
}

// Assign variant preserving type.
static void assignVariant(QVariant & to, const QVariant & from)
{
	QVariant::Type type = to.type();
	if (from.canConvert(type)) {
		to = from;
		to.convert(type);
	}
}




Parameter::Parameter() : m_widget(Widget_Default)
{
}

Parameter::Parameter(const QString & name) : m_name(name), m_widget(Widget_Default)
{
}

void Parameter::setValue(const QVariant& value)
{
	if (!value.isValid())
		return;

	// copy list contents, preserving size.
	if (m_value.type() == QVariant::List && value.type() == QVariant::List)
	{
		QVariantList thisList = m_value.toList();
		QVariantList thatList = value.toList();
		
		if(thisList.count() != thatList.count())
		{
			// @@ This works fine for vectors, but matrices need to be handled better.
			int count = qMin(thisList.count(), thatList.count());
			for(int i = 0; i < count; i++) {
				assignVariant(thisList[i], thatList.at(i));
			}
			m_value = thisList;
		}
		else
		{
			m_value = value;
		}
	}
	else if (m_value.userType() == qMetaTypeId<GLTexture>())
	{
		// convert strings to GLTexture
		if (value.type() == QVariant::String)
		{
			GLTexture tex = m_value.value<GLTexture>();
			if (tex.name() != value.toString())
			{
				m_value.setValue(GLTexture::open(value.toString()));
			}
		}
		else if(value.userType() == qMetaTypeId<GLTexture>())
		{
			m_value = value;
		}
	}
	else if (m_value.isValid())
	{
		// only take new value if it can be converted to the new one.
		assignVariant(m_value, value);
	}
	else
	{
		m_value = value;
	}
}

QString Parameter::displayValue() const
{
	if (m_value.type() == QVariant::Double) {
		return QString::number(m_value.toDouble(), 'g', 3);
	}

	if (m_value.userType() == qMetaTypeId<GLTexture>()) {
		return QFileInfo(m_value.value<GLTexture>().name()).fileName();
	}
	
	if (m_value.canConvert(QVariant::String)) {
		return m_value.toString();
	}
	
	if (m_value.canConvert(QVariant::StringList)) {
		if (columns() > 1) {
			// Matrices
			return "[...]";
		}
		else {
			// Vectors
			QString str = "[";
			
			QVariantList list = m_value.toList();
			const int count = list.count();
			for(int i = 0; i < count; i++) {
				//str += componentDisplayValue(i)
				if (list.at(i).type() == QVariant::Double) {
					str += QString::number(list.at(i).toDouble(), 'g', 3);
				}
				else {
					str += list.at(i).toString();
				}
				if( i != count-1 ) {
					str += ", ";
				}
			}
			return str + "]";
		}
	}
	
	return "####";
}


QVariant Parameter::decoration() const
{
	if (m_value.type() == QVariant::List && widget() == Widget_Color) {
		QPixmap pm(12, 12);
		pm.fill(variantToColor(m_value));
		return pm;
	}

	if (m_value.userType() == qMetaTypeId<GLTexture>()) {
		return m_value.value<GLTexture>().icon();
	}
	
	return QVariant();
}

// Is this used?
bool Parameter::isEditable() const
{
	return m_value.type() == QVariant::String ||
			m_value.type() == QVariant::Bool ||
			m_value.type() == QVariant::Int ||
			m_value.type() == QVariant::Double;
}

bool Parameter::hasRange() const
{
	return m_minValue.isValid() && m_maxValue.isValid();
}

void Parameter::setRange(const QVariant& min, const QVariant& max)
{
	m_minValue = min;
	m_maxValue = max;
}

void Parameter::clearRange()
{
	m_minValue = m_maxValue = QVariant();
}

int Parameter::componentCount() const
{
	return rows() * columns();
}
	
bool Parameter::componentsAreEditable() const
{
	return true;
}	
	
QString Parameter::componentName(int idx) const
{
	Q_ASSERT(idx < componentCount());
	
	if (m_widget == Widget_Color) {
		Q_ASSERT(idx < 4);
		return QString("rgba"[idx]);
	}
	
	if (rows() == 1 || columns() == 1) {
		Q_ASSERT(idx < 4);
		return QString("xyzw"[idx]);
	}
	
	return QString("(%1,%2)").arg(idx/4).arg(idx%4);
}	

QVariant Parameter::componentValue(int idx) const
{
	Q_ASSERT(idx < componentCount());
	
	if (m_value.userType() == QVariant::List) {
		return m_value.toList().at(idx);
	}
	return QVariant();
}

QString Parameter::componentDisplayValue(int idx) const
{
	Q_ASSERT(idx < componentCount());
	
	QVariant val = componentValue(idx);
	
	if (val.type() == QVariant::Double) {
		return QString::number(val.toDouble(), 'g', 3);
	}
	if (val.canConvert(QVariant::String)) {
		return componentValue(idx).toString();
	}
	
	return "####";
}

int Parameter::componentType() const
{
	if (m_value.type() == QVariant::List) {
		if (m_value.toList().size() > 0) {
			return m_value.toList().at(0).userType();
		}
	}
	return QVariant::Invalid;	
}

QVariant Parameter::componentMinValue() const
{
	if (m_widget == Widget_Color) {
		return 0.0;
	}
	return QVariant();   
}

QVariant Parameter::componentMaxValue() const
{
	if (m_widget == Widget_Color) {
		return 1.0;
	}
	return QVariant();   
}

void Parameter::setComponentValue(int idx, const QVariant & value)
{
	Q_ASSERT(idx < componentCount());
	Q_ASSERT(m_value.userType() == QVariant::List);
	
	QList<QVariant> list = m_value.toList();
	list.replace(idx, value);
	m_value = list;
}
