
#include "parameterdelegate.h"
#include "parameter.h"
#include "parametermodel.h"

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
#include <QtGui/QSpinBox>

#include "ui_parameterpropertiesdialog.h"


namespace {
	static QString s_lastPath = ".";
	static QIcon s_toolButtonIcon;
	static QIcon s_colorPickerIcon;
}


ParameterDelegate::ParameterDelegate(QObject *parent /*= 0*/) : QItemDelegate(parent), m_editorActive(false)
{
	s_toolButtonIcon = QIcon(":images/toolbutton.png");
	s_colorPickerIcon = QIcon(":images/colorpicker.png");
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
		connect(editor, SIGNAL(activated()), this, SLOT(editorOpened()));
		connect(editor, SIGNAL(done()), this, SLOT(editorClosed()));
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
			input->setValue(param->componentValue(index.row()).toBool());
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
				//emit commitData(editor);
				emit closeEditor(editor, QAbstractItemDelegate::EditPreviousItem);
				return true;

			case Qt::Key_Down:
				//emit commitData(editor);
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
	if (sender() && sender()->isWidgetType()) {
		emit commitData(static_cast<QWidget *>(sender()));
	}
}

QWidget* ParameterDelegate::createScalarEditor(QWidget* parent, const QVariant& minValue, const QVariant& maxValue) const
{
	DoubleNumInput * editor = new DoubleNumInput(parent);
	
	if (minValue.isValid() && maxValue.isValid()) {
		double min = minValue.toDouble();
		double max = maxValue.toDouble();
		editor->setRange(min, max);
		editor->setSingleStep((max - min) / 500.0);
		editor->setPageStep((max - min) / 20.0);
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
	return m_editorActive;
}

void ParameterDelegate::editorOpened()
{
	m_editorActive = true;
}

void ParameterDelegate::editorClosed()
{
	m_editorActive = false;
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
			input->setSingleStep((max - min) / 500.0);
			input->setPageStep((max - min) / 20.0);
		}
		m_editor = input;	
	}
	else if (m_param->type() == QVariant::Bool) {
		BooleanInput * boolEditor = new BooleanInput(this);
		connect(boolEditor, SIGNAL(valueChanged(bool)), this, SIGNAL(valueChanged()));
		m_editor = boolEditor;
	}
	else if (m_param->type() == QVariant::Int) {
		IntegerInput * intEditor = new IntegerInput(this);
		connect(intEditor, SIGNAL(valueChanged(int)), this, SIGNAL(valueChanged()));
		if (param->minValue().isValid() && param->maxValue().isValid()) {
			int min = param->minValue().toInt();
			int max = param->maxValue().toInt();
			intEditor->setRange(min, max);
		}
		m_editor = intEditor;
	}
	else if (m_param->type() == QVariant::List && m_param->widget() == Parameter::Widget_Color) {
		ColorEditor * colorEditor = new ColorEditor(this);
		connect(colorEditor, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
		connect(colorEditor, SIGNAL(activated()), this, SIGNAL(activated()));
		connect(colorEditor, SIGNAL(done(QWidget*)), this, SIGNAL(done()));
		m_editor = colorEditor;
	}
	else if (m_param->type() == qMetaTypeId<GLTexture>()) {
		FileEditor * fileEditor = new FileEditor(this);
		connect(fileEditor, SIGNAL(valueChanged()), this, SIGNAL(valueChanged()));
		connect(fileEditor, SIGNAL(activated()), this, SIGNAL(activated()));
		connect(fileEditor, SIGNAL(done(QWidget*)), this, SIGNAL(done()));
		m_editor = fileEditor;
	}
	else {
		QLabel * label = new QLabel(this);
		m_editor = label;
	}
	
	//setFocusProxy(m_editor);
	
	QToolButton* settingsButton = new QToolButton(this);
	settingsButton->setIcon(s_toolButtonIcon);
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
	else if (m_param->type() == QVariant::Int) {
		IntegerInput * editor = static_cast<IntegerInput*>(m_editor);
		editor->setValue(m_param->value().toInt());
	}
	else if (m_param->type() == QVariant::List && m_param->widget() == Parameter::Widget_Color) {
		ColorEditor * editor = static_cast<ColorEditor*>(m_editor);
		editor->setValue(m_param->value().toList());
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
	else if (m_param->type() == QVariant::Int) {
		IntegerInput * editor = static_cast<IntegerInput*>(m_editor);
		return editor->value();
	}
	else if (m_param->type() == QVariant::List && m_param->widget() == Parameter::Widget_Color) {
		ColorEditor * editor = static_cast<ColorEditor*>(m_editor);
		return editor->value();
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
	emit activated();
	
	if (m_param->type() == QVariant::Double) {
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
	}
	else if (m_param->type() == qMetaTypeId<GLTexture>()) {
		TexturePropertiesDialog dialog(this);

		GLTexture tex = m_param->value().value<GLTexture>();
		dialog.setWrapMode(tex.wrapS(), tex.wrapT());
		dialog.setMinFilter(tex.minifyingFilter());
		dialog.setMagFilter(tex.magnificationFilter());
		dialog.descriptionEdit->setText(m_param->description());
		dialog.previewLabel->setPixmap(QPixmap::fromImage(tex.image()));

		if (dialog.exec()) {
			tex.setWrapMode(dialog.wrapS(), dialog.wrapT());
			tex.setFilteringMode(dialog.minFilter(), dialog.magFilter());

			m_param->setValue(qVariantFromValue(tex));
			m_param->setDescription(dialog.descriptionEdit->text());
		}
	}
	updateMetaData();
	emit done();
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
	else if (m_param->type() == QVariant::Int) {
		IntegerInput * editor = static_cast<IntegerInput*>(m_editor);
		if (m_param->hasRange())
			editor->setRange(m_param->minValue().toInt(), m_param->maxValue().toInt());
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

	m_lineEdit = new QLineEdit(this);
	m_lineEdit->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding));
	m_lineEdit->setFrame(false);
	connect(m_lineEdit, SIGNAL(editingFinished()), this, SLOT(textChanged()));
	
	QToolButton * button = new QToolButton(this);
	button->setToolButtonStyle(Qt::ToolButtonTextOnly);
	button->setText("...");
	button->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding));
	connect(button, SIGNAL(clicked()), this, SLOT(openFileDialog()));
	
	QHBoxLayout * m_layout = new QHBoxLayout(this);
	m_layout->setMargin(0);
	m_layout->setSpacing(0);
	m_layout->addWidget(button);
	m_layout->addWidget(m_lineEdit);
	
//	setFocusProxy(m_lineEdit);
}

void FileEditor::openFileDialog()
{
	emit activated();
	QString fileName = QFileDialog::getOpenFileName(this, "Choose file", s_lastPath, "Images (*.png *.jpg *.ppm)");
	if( !fileName.isEmpty() && fileName != m_lineEdit->text() ) {
		m_lineEdit->setText(fileName);
		s_lastPath = fileName;
		emit valueChanged();
	}
	emit done(this);
}

void FileEditor::textChanged()
{
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
	button->setIcon(s_colorPickerIcon);
	connect(button, SIGNAL(clicked()), this, SLOT(openColorPicker()));
	
	QHBoxLayout * layout = new QHBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);
	layout->addWidget(button);
	layout->addWidget(m_colorLabel);
	
//	setFocusProxy(button);
}

const QVariant & ColorEditor::value() const
{
	return m_value;
}

void ColorEditor::setValue(const QVariant & value)
{
	m_value = value;
	m_color = variantToColor(value);
	updateLabel();
}


void ColorEditor::updateLabel()
{
	//QString text = "[" + m_value.toStringList().join(", ") + "]";
	
	// @@ This code is already in Parameter::displayValue()
	QVariantList list = m_value.toList();
	
	QString text;
	foreach(QVariant value, list) {
		if (text.isEmpty()) text = "[" + QString::number(value.toDouble(), 'g', 3);
		else text += ", " + QString::number(value.toDouble(), 'g', 3);
	}
	text += "]";
	
	m_colorLabel->setText(text);
}

void ColorEditor::openColorPicker()
{
	emit activated();
	QColor color;
	if (m_value.toList().count() == 4) {
		color = QColor(QColorDialog::getRgba(m_color.rgba()));
	}
	else {
		color = QColorDialog::getColor(m_color);
	}

	if (color.isValid() && m_color != color) {
		m_color = color;
		m_value = colorToVariant(color, m_value.toList().count());
		updateLabel();
		emit valueChanged();
	}
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
	m_spinBox->setDecimals(3);
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
		double min = m_spinBox->minimum();
		double max = m_spinBox->maximum();
		m_slider->setRange( qRound(min / step), qRound(max / step));
		//m_slider->setSingleStep( ... );
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
	if (m_slider) {
		m_slider->setValue(ivalue);
	}
	emit valueChanged(value);
}


// Boolean parameter editor.
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

void BooleanInput::comboBoxValueChanged(int value)
{
	emit valueChanged(value != 0);
}


// Integer parameter editor.
IntegerInput::IntegerInput(QWidget * parent /*= 0*/) : QWidget(parent), m_spinBox(NULL)
{
	setAutoFillBackground(true);
	
	m_spinBox = new QSpinBox(this);
	m_spinBox->setRange(-65535, 65535);
	connect(m_spinBox, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
	
	QHBoxLayout * layout = new QHBoxLayout(this);
	layout->setMargin(1);
	layout->addWidget(m_spinBox);
}

int IntegerInput::value() const
{
	return m_spinBox->value();
}

void IntegerInput::setRange(int min, int max)
{
	m_spinBox->setRange(min, max);
}

void IntegerInput::clearRange()
{
	m_spinBox->setRange(-65535, 65535);
}

void IntegerInput::setValue(int value)
{
	m_spinBox->setValue(value);
}

void IntegerInput::spinBoxValueChanged(int value)
{
	emit valueChanged(value);
}


// Texture properties.
TexturePropertiesDialog::TexturePropertiesDialog(QWidget * parent) : QDialog(parent)
{
	setupUi(this);
}

int wrapModeIndex(GLint mode)
{
	switch (mode) {
		default:
		case GL_REPEAT:
			return 0;
		case GL_CLAMP:
			return 1;
		case GL_CLAMP_TO_BORDER:
			return 2;
		case GL_CLAMP_TO_EDGE:
			return 3;
	}
}

GLint wrapModeFromIndex(int index)
{
	switch (index) {
		default:
		case 0:
			return GL_REPEAT;
		case 1:
			return GL_CLAMP;
		case 2:
			return GL_CLAMP_TO_BORDER;
		case 3:
			return GL_CLAMP_TO_EDGE;
	}
}

void TexturePropertiesDialog::setWrapMode(GLint s, GLint t)
{
	wrapModeSComboBox->setCurrentIndex(wrapModeIndex(s));
	wrapModeTComboBox->setCurrentIndex(wrapModeIndex(t));
}

void TexturePropertiesDialog::setMinFilter(GLint filter)
{
	switch (filter) {
		case GL_NEAREST:
			minFilterComboBox->setCurrentIndex(0);
			break;
		case GL_LINEAR:
			minFilterComboBox->setCurrentIndex(1);
			break;
		case GL_NEAREST_MIPMAP_NEAREST:
			minFilterComboBox->setCurrentIndex(2);
			break;
		case GL_LINEAR_MIPMAP_NEAREST:
			minFilterComboBox->setCurrentIndex(3);
			break;
		case GL_NEAREST_MIPMAP_LINEAR:
			minFilterComboBox->setCurrentIndex(4);
			break;
		case GL_LINEAR_MIPMAP_LINEAR:
			minFilterComboBox->setCurrentIndex(5);
			break;
	}
}

void TexturePropertiesDialog::setMagFilter(GLint filter)
{
	switch (filter) {
		case GL_NEAREST:
			magFilterComboBox->setCurrentIndex(0);
			break;
		case GL_LINEAR:
			magFilterComboBox->setCurrentIndex(1);
			break;
	}
}

GLint TexturePropertiesDialog::wrapS() const
{
	return wrapModeFromIndex(wrapModeSComboBox->currentIndex());
}

GLint TexturePropertiesDialog::wrapT() const
{
	return wrapModeFromIndex(wrapModeTComboBox->currentIndex());
}

GLint TexturePropertiesDialog::minFilter() const
{
	switch (minFilterComboBox->currentIndex()) {
		default:
		case 0:
			return GL_NEAREST;
		case 1:
			return GL_LINEAR;
		case 2:
			return GL_NEAREST_MIPMAP_NEAREST;
		case 3:
			return GL_LINEAR_MIPMAP_NEAREST;
		case 4:
			return GL_NEAREST_MIPMAP_LINEAR;
		case 5:
			return GL_LINEAR_MIPMAP_LINEAR;
	}
}

GLint TexturePropertiesDialog::magFilter() const
{
	switch (magFilterComboBox->currentIndex()) {
		default:
		case 0:
			return GL_NEAREST;
		case 1:
			return GL_LINEAR;
	}
}
