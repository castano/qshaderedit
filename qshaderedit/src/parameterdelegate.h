 
#ifndef DELEGATE_H
#define DELEGATE_H

#include <QtGui/QItemDelegate>

class Parameter;


class ParameterDelegate: public QItemDelegate
{
	Q_OBJECT
public:
	ParameterDelegate(QObject *parent = 0) :
		QItemDelegate(parent) {}
	
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index ) const;
	
	QWidget * createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
	void setEditorData ( QWidget * editor, const QModelIndex & index ) const;
	void setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;
	void updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const;
	
	bool eventFilter(QObject* object, QEvent* event);
	
	void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;

	bool isEditorActive() const;
		
	static const QString & lastPath();
	static void setLastPath(const QString & lastPath);

private slots:
	void editorValueChanged();

private:
	QWidget* createScalarEditor(QWidget* parent, const QVariant& min, const QVariant& max) const;
};



class QTableView;
class Effect;
class QDoubleSpinBox;
class QLabel;


class ParameterEditor: public QWidget
{
	Q_OBJECT
	
public:
	ParameterEditor(Parameter* param, QWidget* parent = 0);
	
	void updateValue();   // from param
	QVariant value() const;
	
signals:
	void valueChanged();
	
private slots:
	void openParameterSettings();
	
private:
	void updateMetaData();
	
private:
	Parameter* m_param;
	QWidget* m_editor;
};


class FileEditor : public QWidget
{
	Q_OBJECT
public:
	FileEditor(QWidget * parent = 0);

	QString text() const;
	void setText(const QString & str);

	QLineEdit * lineEditor() { return m_lineEdit; }

private slots:
	void openFileDialog();

signals:
	void valueChanged();
	void done(QWidget *);

private:
	QLineEdit * m_lineEdit;
};


class ColorEditor: public QWidget
{
	Q_OBJECT
			
public:
	ColorEditor(QColor color = Qt::black, QWidget* parent = 0);
	ColorEditor(QWidget* parent = 0);

	QColor color() const;
	void setColor(QColor color);

signals:
	void valueChanged();
	void done(QWidget* widget);

private slots:
	void openColorPicker();

private:
	void init();
	void updateLabel();

private:
	QColor m_color;
	QLabel* m_colorLabel;
};


class DoubleNumInput : public QWidget
{
	Q_OBJECT
public:
	DoubleNumInput(QWidget * parent = 0);

	double value() const;

	void setSingleStep(double step);
	void setPageStep(double step);
	void setDecimals(int decimals);
	void setRange(double min, double max);
	void clearRange();

public slots:
	void setValue(double value);

signals:
	void valueChanged(double value);

protected:
	void keyPressEvent(QKeyEvent* event);

private slots:
	void spinBoxValueChanged(double value);
	void sliderValueChanged(int value);
	
private:
	QDoubleSpinBox * m_spinBox;
	QSlider * m_slider;
};

#endif
