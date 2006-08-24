
#include "parameterdelegate.h"
#include "parameter.h"
#include "parametermodel.h"
#include "texmanager.h"

#include <QtGui/QHeaderView>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QLineEdit>
#include <QtGui/QHBoxLayout>
#include <QtGui/QToolButton>
#include <QtGui/QFileDialog>
#include <QtGui/QPainter>
#include <QtGui/QKeyEvent>
#include <QtGui/QLabel>
#include <QtGui/QColorDialog>
#include <QtGui/QComboBox>

#include "ui_parameterpropertiesdialog.h"

namespace {
	static QString s_lastPath = ".";
}


QSize ParameterDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
	// make more room for editor widgets
	return QItemDelegate::sizeHint(option, index) + QSize(0, 6);
}

QWidget * ParameterDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	Q_UNUSED(option);
	const ParameterModel * model = qobject_cast<const ParameterModel *>(index.model());
	Q_ASSERT(model != NULL);
	Parameter* param = model->parameter(index);
	
	if (model->isComponent(index)) {
		if (param->componentType() == QVariant::Double) {
			return createScalarEditor(parent, param->componentMinValue(), param->componentMaxValue());
		}
		else if (param->componentType() == QVariant::Bool) {
			return createBooleanEditor(parent);
		}
		else {
			return QItemDelegate::createEditor(parent, option, index);
		}
	}
	else {
		ParameterEditor* editor = new ParameterEditor(param, parent);
		editor->installEventFilter(const_cast<ParameterDelegate*>(this));
		connect(editor, SIGNAL(valueChanged()), this, SLOT(editorValueChanged()));
		return editor;
	}
}

void ParameterDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
	const ParameterModel * model = qobject_cast<const ParameterModel *>(index.model());
	Q_ASSERT(model != NULL);
	Parameter* param = model->parameter(index);
	
	if (model->isComponent(index)) {
		if (param->componentType() == QVariant::Double) {
			DoubleNumInput * input = static_cast<DoubleNumInput*>(editor);
			input->setValue(param->componentValue(index.row()).toDouble());
		}
		else if (param->componentType() == QVariant::Bool) {
			BooleanInput * input = static_cast<BooleanInput*>(editor);
			input->setValue(param->componentValue(index.row()).toDouble());
		}
		else {
			QItemDelegate::setEditorData(editor, index);
		}
	}
	else {
		ParameterEditor* paramEditor = static_cast<ParameterEditor*>(editor);
		paramEditor->updateValue();
	}
}

void ParameterDelegate::setModelData(QWidget * editor, QAbstractItemModel * abstractModel, const QModelIndex & index) const
{
	ParameterModel * model = qobject_cast<ParameterModel *>(abstractModel);
	Q_ASSERT(model != NULL);
	Parameter* param = model->parameter(index);
	
	if (model->isComponent(index)) {
		if (param->componentType() == QVariant::Double) {
			DoubleNumInput * input = static_cast<DoubleNumInput*>(editor);
			model->setData(index, input->value());
		}
		else if (param->componentType() == QVariant::Bool) {
			BooleanInput * input = static_cast<BooleanInput*>(editor);
			model->setData(index, input->value());
		}
		else {
			QItemDelegate::setModelData(editor, model, index);
		}
	}
	else {
		ParameterEditor* paramEditor = static_cast<ParameterEditor*>(editor);
		model->setData(index, paramEditor->value());
	}		
}

void ParameterDelegate::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	Q_UNUSED(index);
	editor->setGeometry(option.rect);
}

bool ParameterDelegate::eventFilter(QObject* object, QEvent* event)
{
	QWidget* editor = qobject_cast<QWidget*>(object);
	if (!editor)
		return false;

	if (event->type() == QEvent::KeyPress) {
		QKeyEvent *keyEvent = (QKeyEvent*)event;
		switch (keyEvent->key()) {
			case Qt::Key_Up:
				emit commitData(editor);
				emit closeEditor(editor, QAbstractItemDelegate::EditPreviousItem);
				return true;

			case Qt::Key_Down:
				emit commitData(editor);
				emit closeEditor(editor, QAbstractItemDelegate::EditNextItem);
				return true;

			case Qt::Key_Enter:
			case Qt::Key_Return:
				emit commitData(editor);
				emit closeEditor(editor, QAbstractItemDelegate::EditNextItem);
				return true;
		}
	} 

	return false; //QItemDelegate::eventFilter(object, event);
}

void ParameterDelegate::paint(QPainter * painter, const QStyleOptionViewItem & o, const QModelIndex & index) const
{
	const ParameterModel * model = qobject_cast<const ParameterModel *>(index.model());
	Q_ASSERT(model != NULL);
	const Parameter* param = model->parameter(index);
	
	QStyleOptionViewItem option = o;
	
	// dont't draw selection behind editor widgets
	if (index.column() == 1 && param->isEditable()) {
		option.state &= ~QStyle::State_Selected;
		option.state &= ~QStyle::State_HasFocus;
	}

	QItemDelegate::paint(painter, option, index);
}

void ParameterDelegate::editorValueChanged()
{
	if (sender() && sender()->isWidgetType())
		emit commitData(static_cast<QWidget *>(sender()));
}

QWidget* ParameterDelegate::createScalarEditor(QWidget* parent, const QVariant& minValue, const QVariant& maxValue) const
{
	DoubleNumInput * editor = new DoubleNumInput(parent);
	
	if (minValue.isValid() && maxValue.isValid()) {
		double min = minValue.toDouble();
		double max = maxValue.toDouble();
		editor->setRange(min, max);
		editor->setSingleStep((max - min) / 20.0);
		editor->setPageStep((max - min) / 10.0);
	}	
	editor->installEventFilter(const_cast<ParameterDelegate*>(this));
	connect(editor, SIGNAL(valueChanged(double)), this, SLOT(editorValueChanged()));
	return editor;
}

QWidget* ParameterDelegate::createBooleanEditor(QWidget* parent) const
{
	BooleanInput * editor = new BooleanInput(parent);
	editor->installEventFilter(const_cast<ParameterDelegate*>(this));
	connect(editor, SIGNAL(valueChanged(bool)), this, SLOT(editorValueChanged()));
	return editor;
}

bool ParameterDelegate::isEditorActive() const
{
	return false;
}

const QString & ParameterDelegate::lastPath()
{
	return s_lastPath;
}

void ParameterDelegate::setLastPath(const QString & lastPath)
{
	s_lastPath = lastPath;
}



//
// ParameterEditor
//

ParameterEditor::ParameterEditor(Parameter* param, QWidget* parent):
		QWidget(parent), m_param(param)
{
	Q_ASSERT(m_param);
	
	setAutoFillBackground(true);
	
	if (m_param->type() == QVariant::Double) {
		DoubleNumInput * input = new DoubleNumInput(this);
		connect(input, SIGNAL(valueChanged(double)), this, SIGNAL(valueChanged()));
		if (param->minValue().isValid() && param->maxValue().isValid()) {
			double min = param->minValue().toDouble();
			double max = param->maxValue().toDouble();
			input->setRange(min, max);
			input->setSingleStep((max - min) / 20.0);
			input->setPageStep((max - min) / 10.0);
		}
		m_editor = input;	
	}
	else if (m_param->type() == QVariant::Bool) {
		BooleanInput * boolEditor = new BooleanInput(this);
		connect(boolEditor, SIGNAL(valueChanged(bool)), this, SIGNAL(valueChanged()));
		m_editor = boolEditor;
	}
	else if (m_param->type() == QVariant::Color) {
		ColorEditor * colorEditor = new ColorEditor(this);
		connect(colorEditor, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
		m_editor = colorEditor;
	}
	else if (m_param->type() == qMetaTypeId<GLTexture>()) {
		FileEditor * fileEditor = new FileEditor(this);
		connect(fileEditor, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
		m_editor = fileEditor;
	}
	else {
		QLabel * label = new QLabel(this);
		m_editor = label;
	}
	
	setFocusProxy(m_editor);
		
	QToolButton* settingsButton = new QToolButton(this);
	settingsButton->setIcon(QIcon(":images/toolbutton.png"));
	connect(settingsButton, SIGNAL(clicked()), this, SLOT(openParameterSettings()));
	
	QHBoxLayout * layout = new QHBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);
	layout->addWidget(m_editor);		
	layout->addWidget(settingsButton);
	setLayout(layout);
}

void ParameterEditor::updateValue()
{	
	if (m_param->type() == QVariant::Double) {
		DoubleNumInput * editor = static_cast<DoubleNumInput*>(m_editor);
		editor->setValue(m_param->value().toDouble());
	}
	else if (m_param->type() == QVariant::Bool) {
		BooleanInput * editor = static_cast<BooleanInput*>(m_editor);
		editor->setValue(m_param->value().toBool());
	}
	else if (m_param->type() == QVariant::Color) {
		ColorEditor * editor = static_cast<ColorEditor*>(m_editor);
		editor->setColor(m_param->value().value<QColor>()); 
	}
	else if (m_param->type() == qMetaTypeId<GLTexture>()) {
		FileEditor * editor = static_cast<FileEditor*>(m_editor);
		editor->setText(m_param->value().value<GLTexture>().name());
	}
	else {
		QLabel * label = static_cast<QLabel*>(m_editor);
		label->setText(m_param->displayValue());
	}
}

QVariant ParameterEditor::value() const
{
	if (m_param->type() == QVariant::Double) {
		DoubleNumInput* editor = static_cast<DoubleNumInput*>(m_editor);
		return editor->value();
	}
	else if (m_param->type() == QVariant::Bool) {
		BooleanInput * editor = static_cast<BooleanInput*>(m_editor);
		return editor->value();
	}
	else if (m_param->type() == QVariant::Color) {
		ColorEditor * editor = static_cast<ColorEditor*>(m_editor);
		return editor->color();		
	}
	else if (m_param->type() == qMetaTypeId<GLTexture>()) {
		FileEditor * editor = static_cast<FileEditor*>(m_editor);
		return editor->text();
	}
	else {
		return QVariant();
	}
}

void ParameterEditor::openParameterSettings()
{
	QDialog * dialog = new QDialog(this);	
	Ui::ParamPropertiesDialog ui;
	ui.setupUi(dialog);
	
	ui.nameEdit->setText(m_param->name());
	ui.descriptionEdit->setText(m_param->description());
	if (m_param->hasRange()) {
		ui.limitRangeCheckBox->setChecked(true);
		ui.minSpinBox->setValue(m_param->minValue().toDouble());
		ui.maxSpinBox->setValue(m_param->maxValue().toDouble());		
	}
	
	if (dialog->exec() == QDialog::Accepted) {
		m_param->setDescription(ui.descriptionEdit->text());
		if (ui.limitRangeCheckBox->isChecked())
			m_param->setRange(ui.minSpinBox->value(), ui.maxSpinBox->value());
		else
			m_param->clearRange();
	}
	
	delete dialog;
	
	updateMetaData();
}

void ParameterEditor::updateMetaData()
{
	if (m_param->type() == QVariant::Double) {
		DoubleNumInput * editor = static_cast<DoubleNumInput*>(m_editor);
		if (m_param->hasRange())
			editor->setRange(m_param->minValue().toDouble(), m_param->maxValue().toDouble());
		else
			editor->clearRange();
	}
}


//
// FilePicker Widget.
//

FileEditor::FileEditor(QWidget * parent /*= 0*/) : QWidget(parent)
{
	setAutoFillBackground(true);

	QHBoxLayout * m_layout = new QHBoxLayout(this);
	m_layout->setMargin(0);
	m_layout->setSpacing(0);

	m_lineEdit = new QLineEdit(this);
	m_lineEdit->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding));
	m_lineEdit->setFrame(false);
	m_layout->addWidget(m_lineEdit);
	
	QToolButton * button = new QToolButton(this);
	button->setToolButtonStyle(Qt::ToolButtonTextOnly);
	button->setText("...");
	button->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding));
	m_layout->addWidget(button);
	connect(button, SIGNAL(clicked()), this, SLOT(openFileDialog()));
	
	setFocusProxy(m_lineEdit);
}

void FileEditor::openFileDialog()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Choose file", s_lastPath, "Images (*.png *.jpg)");
	if( !fileName.isEmpty() ) {
		m_lineEdit->setText(fileName);
		s_lastPath = fileName;
	}
	emit valueChanged();
	emit done(this);
}

QString FileEditor::text() const
{
	return m_lineEdit->text();
}

void FileEditor::setText(const QString & str)
{
	m_lineEdit->setText(str);
}


//
// Color Editor
//

ColorEditor::ColorEditor(QColor color, QWidget* parent /*= 0*/):
		QWidget(parent), m_color(color)
{
	init();
}

ColorEditor::ColorEditor(QWidget* parent /*= 0*/): QWidget(parent)
{
	init();
}

void ColorEditor::init()
{
	m_colorLabel = new QLabel(this);
	m_colorLabel->setAutoFillBackground(true);
	updateLabel();

	QToolButton * button = new QToolButton(this);
	button->setToolButtonStyle(Qt::ToolButtonTextOnly);
	button->setIcon(QIcon(":images/colorpicker.png"));
	connect(button, SIGNAL(clicked()), this, SLOT(openColorPicker()));
	
	QHBoxLayout * layout = new QHBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);
	layout->addWidget(button);
	layout->addWidget(m_colorLabel);
	
//	setFocusProxy(button);
}

QColor ColorEditor::color() const
{
	return m_color;
}

void ColorEditor::setColor(QColor color)
{
	m_color = color;
	updateLabel();
}

void ColorEditor::updateLabel()
{
	QString text = "[";
	text += QString().setNum(m_color.redF(), 'g', 2) + ", ";
	text += QString().setNum(m_color.greenF(), 'g', 2) + ", ";
	text += QString().setNum(m_color.blueF(), 'g', 2) + ", ";
	text += QString().setNum(m_color.alphaF(), 'g', 2);
	text += "]";

	m_colorLabel->setText(text);
}

void ColorEditor::openColorPicker()
{
	QColor color = QColorDialog::getColor(m_color);
	if (color.isValid()) {
		m_color = color;
		updateLabel();
	}
	emit valueChanged();
	emit done(this);
}


//
// DoubleNumInput
//

DoubleNumInput::DoubleNumInput(QWidget * parent) : QWidget(parent), m_slider(NULL)
{
	setAutoFillBackground(true);

	m_spinBox = new QDoubleSpinBox(this);
	m_spinBox->setRange(-10000.0, 10000.0);
	m_spinBox->setDecimals(2);
	m_spinBox->setSingleStep(0.1);
// 	m_spinBox->setFocusProxy(this);
	connect(m_spinBox, SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChanged(double)));
	
	QHBoxLayout * layout = new QHBoxLayout(this);
	layout->setMargin(2);
	layout->addWidget(m_spinBox);
}

double DoubleNumInput::value() const
{
	return m_spinBox->value();
}

void DoubleNumInput::setSingleStep(double step)
{
	m_spinBox->setSingleStep(step);
	if (m_slider) {
		m_slider->setRange( qRound(m_spinBox->minimum() / m_spinBox->singleStep()),
			qRound(m_spinBox->maximum() / m_spinBox->singleStep()));
	}
}

void DoubleNumInput::setPageStep(double step)
{
	if (m_slider)
		m_slider->setPageStep(qRound(step / m_spinBox->singleStep()));
}

void DoubleNumInput::setDecimals(int decimals)
{
	m_spinBox->setDecimals(decimals);
}

void DoubleNumInput::setRange(double min, double max)
{
	if (!m_slider) {
		m_slider = new QSlider(Qt::Horizontal, this);
		m_slider->setFocusProxy(this);
		connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
		
		delete this->layout();
		
		QHBoxLayout * layout = new QHBoxLayout(this);
		layout->setMargin(2);
		layout->addWidget(m_slider);
		layout->addWidget(m_spinBox);
	}
	
	m_spinBox->setRange(min, max);
	m_slider->setRange(qRound(min / m_spinBox->singleStep()), qRound(max / m_spinBox->singleStep()));
	m_slider->setValue(qRound(m_spinBox->value() / m_spinBox->singleStep()));
}

void DoubleNumInput::clearRange()
{
	if (m_slider) {
		delete m_slider;
		m_slider = NULL;
	}
	
	m_spinBox->setRange(-10000.0, 10000.0);
}

void DoubleNumInput::setValue(double value)
{
	m_spinBox->setValue(value);
	if (m_slider)
		m_slider->setValue(qRound(value / m_spinBox->singleStep()));
}

void DoubleNumInput::keyPressEvent(QKeyEvent* event)
{
	if (m_slider)
		m_slider->event(event);
}

void DoubleNumInput::sliderValueChanged(int value)
{
	double dvalue = (double)value * m_spinBox->singleStep();
	m_spinBox->setValue(dvalue);
}

void DoubleNumInput::spinBoxValueChanged(double value)
{
	int ivalue = qRound(value / m_spinBox->singleStep());
	if (m_slider)
		m_slider->setValue(ivalue);
	emit valueChanged(value);
}


BooleanInput::BooleanInput(QWidget * parent /*= 0*/) : QWidget(parent), m_comboBox(NULL)
{
	setAutoFillBackground(true);
	
	m_comboBox = new QComboBox(this);
	m_comboBox->addItem("false", false);
	m_comboBox->addItem("true", true);
	connect(m_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxValueChanged(int)));
	
	QHBoxLayout * layout = new QHBoxLayout(this);
	layout->setMargin(1);
	layout->addWidget(m_comboBox);
	
}

bool BooleanInput::value() const
{
	return m_comboBox->currentIndex() != 0;
}

void BooleanInput::setValue(bool value)
{
	m_comboBox->setCurrentIndex(value);
}

void BooleanInput::keyPressEvent(QKeyEvent* event)
{
	if (m_comboBox)
		m_comboBox->event(event);
}

void BooleanInput::comboBoxValueChanged(int value)
{
	emit valueChanged(value != 0);
}
	
