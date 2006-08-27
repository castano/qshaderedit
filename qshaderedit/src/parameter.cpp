
#include "parameter.h"
#include "texmanager.h"

#include <QtGui/QColor>
#include <QtGui/QPixmap>

void Parameter::setValue(const QVariant& value)
{
	if (!value.isValid())
		return;

	// converts float lists of 3 or 4 elements to QColors 
	if (value.type() == QVariant::List && m_name.contains("color", Qt::CaseInsensitive)) {
		QVariantList list = value.toList();
		if (list.size() == 3) {
			m_value = QColor::fromRgbF(list.at(0).toDouble(), list.at(1).toDouble(), list.at(2).toDouble());
		}
		else if (list.size() == 4) {
			m_value = QColor::fromRgbF(list.at(0).toDouble(), list.at(1).toDouble(), list.at(2).toDouble(), list.at(3).toDouble());
		}
		else
			m_value = value;
	}
	
	// convert strings to GLTexture
	else if (m_value.userType() == qMetaTypeId<GLTexture>() && value.type() == QVariant::String) {
		m_value = qVariantFromValue(GLTexture::open(value.toString()));
	}

	// only take new value if it has the same type as the old one
	else if (!m_value.isValid() || m_value.userType() == value.userType())
		m_value = value;
}

QString Parameter::displayValue() const
{
	if (m_value.userType() == QVariant::Color) {
		QColor color = m_value.value<QColor>();
		return QString("[%1, %2, %3, %4]").
				arg(color.redF(), 0, 'g', 3).
				arg(color.greenF(), 0, 'g', 3).
				arg(color.blueF(), 0, 'g', 3).
				arg(color.alphaF(), 0, 'g', 3);
	}
	
	if (m_value.type() == QVariant::Double) {
		return QString::number(m_value.toDouble(), 'g', 4);
	}

	if (m_value.userType() == qMetaTypeId<GLTexture>()) {
		return m_value.value<GLTexture>().name();
	}
	
	if (m_value.canConvert(QVariant::String)) {
		return m_value.toString();
	}
	
	if (componentCount() > 0 && m_value.canConvert(QVariant::StringList)) {
		if (componentCount() > 4)
			return "[...]";
		else
			return "[" + m_value.toStringList().join(", ") + "]";
	}
	
	return "####";
}

QVariant Parameter::decoration() const
{
	if (m_value.userType() == QVariant::Color) {
		QPixmap pm(12, 12);
		pm.fill(m_value.value<QColor>());
		return pm;
	}

	if (m_value.userType() == qMetaTypeId<GLTexture>()) {
		return m_value.value<GLTexture>().icon();
	}
	
	return QVariant();
}

bool Parameter::isEditable() const
{
	return m_value.type() == QVariant::String ||
			m_value.type() == QVariant::Color ||
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
	if (m_value.userType() == QVariant::Color)
		return 4;
	else if (m_value.userType() == QVariant::List)
		return m_value.toList().size();	
	return 0;
}
	
bool Parameter::componentsAreEditable() const
{
	return true;
}	
	
QString Parameter::componentName(int idx) const
{
	Q_ASSERT(idx < componentCount());
	
	if (m_value.userType() == QVariant::Color)
		return QString("rgba"[idx]);
	
	if (componentCount() <= 4)
		return QString("xyzw"[idx]);
	
	return QString("[%1]").arg(idx);
}	

QVariant Parameter::componentValue(int idx) const
{
	Q_ASSERT(idx < componentCount());
	
	if (m_value.userType() == QVariant::Color) {
		switch (idx) {
			case 0: return m_value.value<QColor>().redF();
			case 1: return m_value.value<QColor>().greenF();
			case 2: return m_value.value<QColor>().blueF();
			case 3: return m_value.value<QColor>().alphaF();					
		}
	}
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
		return QString::number(val.toDouble(), 'g', 2);
	}
	if (val.canConvert(QVariant::String))
		return componentValue(idx).toString();
	
	return "####";
}

int Parameter::componentType() const
{
	if (m_value.type() == QVariant::Color) {
		return QVariant::Double;
	}
	else if (m_value.type() == QVariant::List) {
		if (m_value.toList().size() > 0)
			return m_value.toList().at(0).userType();
	}
	return QVariant::Invalid;	
}

QVariant Parameter::componentMinValue() const
{
	if (m_value.userType() == QVariant::Color) {
		return 0.0;
	}
	return QVariant();   
}

QVariant Parameter::componentMaxValue() const
{
	if (m_value.userType() == QVariant::Color) {
		return 1.0;
	}
	return QVariant();   
}

void Parameter::setComponentValue(int idx, QVariant value)
{
	Q_ASSERT(idx < componentCount());
	
	if (m_value.userType() == QVariant::Color) {
		double fval = value.toDouble();
		QColor color = m_value.value<QColor>();
		switch (idx) {
			case 0: color.setRedF(fval); break;
			case 1: color.setGreenF(fval); break;
			case 2: color.setBlueF(fval); break;
			case 3: color.setAlphaF(fval); break;					
		}
		m_value = color;
	}
	else if (m_value.userType() == QVariant::List) {
		QList<QVariant> list = m_value.toList();
		list.replace(idx, value);
		m_value = list;
	}
}
