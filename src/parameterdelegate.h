 
#ifndef DELEGATE_H
#define DELEGATE_H

#include <QtGui/QItemDelegate>
#include "ui_texturepropertiesdialog.h"
#include "texmanager.h"

class Parameter;


class ParameterDelegate: public QItemDelegate
{
	Q_OBJECT
public:
	ParameterDelegate(QObject *parent = 0);
	
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
	void editorOpened();
	void editorClosed();

private:
	QWidget* createScalarEditor(QWidget* parent, const QVariant& min, const QVariant& max) const;
	QWidget* createBooleanEditor(QWidget* parent) const;

	mutable bool m_editorActive;
};



class QTableView;
class Effect;
class QDoubleSpinBox;
class QLabel;
class QComboBox;
class QSpinBox;

class ParameterEditor: public QWidget
{
	Q_OBJECT
public:
	ParameterEditor(Parameter* param, QWidget* parent = 0);
	
	void updateValue();   // from param
	QVariant value() const;
	
signals:
	void valueChanged();
	void activated();
	void done();
	
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
	void textChanged();

signals:
	void valueChanged();
	void activated();
	void done(QWidget *);

private:
	QLineEdit * m_lineEdit;
};


class ColorEditor: public QWidget
{
	Q_OBJECT
public:
	ColorEditor(QWidget* parent = 0);

	const QVariant & value() const;
	void setValue(const QVariant & color);
	
signals:
	void valueChanged();
	void done(QWidget* widget);
	void activated();

private slots:
	void openColorPicker();

private:
	void init();
	void updateLabel();

private:
	QColor m_color;
	QVariant m_value;
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


/// Boolean parameter editor.
class BooleanInput : public QWidget
{
	Q_OBJECT
public:
	BooleanInput(QWidget * parent = 0);

	bool value() const;

public slots:
	void setValue(bool value);

signals:
	void valueChanged(bool value);

private slots:
	void comboBoxValueChanged(int value);
	
private:
	QComboBox * m_comboBox;
};


/// Integer parameter editor.
class IntegerInput : public QWidget
{
	Q_OBJECT
public:
	IntegerInput(QWidget * parent = 0);

	int value() const;

	void setRange(int min, int max);
	void clearRange();
	
public slots:
	void setValue(int value);

signals:
	void valueChanged(int value);

private slots:
	void spinBoxValueChanged(int value);
	
private:
	QSpinBox * m_spinBox;
};


/// Texture properties.
class TexturePropertiesDialog : public QDialog, public Ui::TexturePropertiesDialog
{
	Q_OBJECT
public:
	TexturePropertiesDialog(QWidget * parent = 0);

	void setWrapMode(GLint s, GLint t);
	void setMinFilter(GLint filter);
	void setMagFilter(GLint filter);

	GLint wrapS() const;
	GLint wrapT() const;
	GLint minFilter() const;
	GLint magFilter() const;  
	
};

#endif
