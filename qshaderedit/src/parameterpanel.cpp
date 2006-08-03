#include "parameterpanel.h"
#include "effect.h"

#include <QtGui/QHeaderView>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QLineEdit>
#include <QtGui/QHBoxLayout>
#include <QtGui/QToolButton>
#include <QtGui/QFileDialog>
#include <QtGui/QApplication>
#include <QtGui/QPainter>
#include <QtGui/QKeyEvent>

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
	setFocusProxy(m_lineEdit);

	QToolButton * button = new QToolButton(this);
	button->setToolButtonStyle(Qt::ToolButtonTextOnly);
	button->setText("...");
	button->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding));
	m_layout->addWidget(button);
	connect(button, SIGNAL(clicked()), this, SLOT(openFileDialog()));
}

void FileEditor::openFileDialog()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Choose file", "", "Images (*.png *.jpg)");
	if( !fileName.isEmpty() ) {
		m_lineEdit->setText(fileName);
	}
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
// DoubleNumInput
//

DoubleNumInput::DoubleNumInput(QWidget * parent) : QWidget(parent)
{
	setAutoFillBackground(true);

	m_spinBox = new QDoubleSpinBox(this);
	m_spinBox->setRange(0.0, 1.0);
	m_spinBox->setDecimals(2);
	m_spinBox->setSingleStep(0.1);
	connect(m_spinBox, SIGNAL(valueChanged(double)), this, SLOT(spinBoxValueChanged(double)));

	m_slider = new QSlider(Qt::Horizontal, this);
	m_slider->setRange(0, 10);
	connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));

	QHBoxLayout * layout = new QHBoxLayout(this);
	layout->setMargin(2);
	layout->addWidget(m_slider);
	layout->addWidget(m_spinBox);

	setFocusProxy(m_slider);
}

double DoubleNumInput::value() const
{
	return m_spinBox->value();
}

void DoubleNumInput::setSingleStep(double step)
{
	m_spinBox->setSingleStep(step);
	m_slider->setRange(
		qRound(m_spinBox->minimum() / m_spinBox->singleStep()),
		qRound(m_spinBox->maximum() / m_spinBox->singleStep()));
}

void DoubleNumInput::setPageStep(double step)
{
	m_slider->setPageStep(qRound(step / m_spinBox->singleStep()));
}

void DoubleNumInput::setDecimals(int decimals)
{
	m_spinBox->setDecimals(decimals);
}

void DoubleNumInput::setRange(double min, double max)
{
	m_spinBox->setRange(min, max);
	m_slider->setRange(qRound(min / m_spinBox->singleStep()), qRound(max / m_spinBox->singleStep()));
}

void DoubleNumInput::setValue(double value)
{
	m_spinBox->setValue(value);
	m_slider->setValue(qRound(value / m_spinBox->singleStep()));
}

void DoubleNumInput::sliderValueChanged(int value)
{
	double dvalue = (double)value * m_spinBox->singleStep();
	m_spinBox->setValue(dvalue);
}

void DoubleNumInput::spinBoxValueChanged(double value)
{
	int ivalue = qRound(value / m_spinBox->singleStep());
	m_slider->setValue(ivalue);
	emit valueChanged(value);
}


//
// ParameterDelegate
//

QWidget * ParameterDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	Q_UNUSED(option);
	Q_UNUSED(index);

	const ParameterTableModel * model = qobject_cast<const ParameterTableModel *>(index.model());
	Q_ASSERT(model != NULL);

	if( model->useNumericEditor(index)  ) {
		QVariant value = model->data(index, Qt::EditRole);

		if( value.type() == QVariant::Double ) {
			QDoubleSpinBox * editor = new QDoubleSpinBox(parent);
			editor->setRange(-1000, 1000);
			editor->setSingleStep( 0.1 );
			editor->setDecimals( 3 );
			editor->installEventFilter(const_cast<ParameterDelegate*>(this));
			connect(editor, SIGNAL(valueChanged(double)), this, SLOT(editorValueChanged()));
			return editor;
		}
	}
	else if( model->useColorEditor(index) ) {
		QVariant value = model->data(index, Qt::EditRole);
		DoubleNumInput * editor = new DoubleNumInput(parent);
		editor->setSingleStep(0.05);
		editor->setPageStep(0.1);
		editor->installEventFilter(const_cast<ParameterDelegate*>(this));
		connect(editor, SIGNAL(valueChanged(double)), this, SLOT(editorValueChanged()));
		return editor;
	}
	else if( model->useFileEditor(index) ) {
		QVariant value = model->data(index, Qt::EditRole);
		FileEditor * editor = new FileEditor(parent);
		editor->installEventFilter(const_cast<ParameterDelegate*>(this));
		connect(editor, SIGNAL(done(QWidget*)), this, SIGNAL(commitData(QWidget*)));
		connect(editor, SIGNAL(done(QWidget*)), this, SIGNAL(closeEditor(QWidget*)));
		return editor;
	}

	// default
	return QItemDelegate::createEditor(parent, option, index);
}

void ParameterDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
	const ParameterTableModel * model = qobject_cast<const ParameterTableModel *>(index.model());
	Q_ASSERT(model != NULL);

	if( model->useNumericEditor(index) ) {
		QVariant value = model->data(index, Qt::EditRole);

		if( value.type() == QVariant::Double ) {
			double value = model->data(index, Qt::DisplayRole).toDouble();
			QDoubleSpinBox * spinBox = static_cast<QDoubleSpinBox *>(editor);
			spinBox->setValue(value);
			return;
		}
	}
	else if( model->useColorEditor(index) ) {
		double value = model->data(index, Qt::EditRole).toDouble();
		DoubleNumInput * input = static_cast<DoubleNumInput*>(editor);
		input->setValue(value);
	}
	else if( model->useFileEditor(index) ) {
		QVariant value = model->data(index, Qt::EditRole);

		FileEditor * fileEditor = static_cast<FileEditor *>(editor);
		fileEditor->setText(value.toString());
		return;
	}

	// default
	QItemDelegate::setEditorData(editor, index);
}

void ParameterDelegate::setModelData(QWidget * editor, QAbstractItemModel * abstractModel, const QModelIndex & index) const
{
	ParameterTableModel * model = qobject_cast<ParameterTableModel *>(abstractModel);
	Q_ASSERT(model != NULL);

	if( model->useNumericEditor(index) ) {
		QVariant value = model->data(index, Qt::EditRole);

		if( value.type() == QVariant::Double ) {
			QDoubleSpinBox * spinBox = static_cast<QDoubleSpinBox *>(editor);
			spinBox->interpretText();
			model->setData(index, spinBox->value());
			return;
		}
	}
	else if( model->useColorEditor(index) ) {
		DoubleNumInput * input = static_cast<DoubleNumInput*>(editor);
		model->setData(index, input->value());
		return;
	}
	else if( model->useFileEditor(index) ) {
		FileEditor * fileEditor = static_cast<FileEditor *>(editor);
		model->setData(index, fileEditor->text());
		return;
	}

	// default
	QItemDelegate::setModelData(editor, model, index);
}

void ParameterDelegate::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	Q_UNUSED(index);
	editor->setGeometry(option.rect);
}

QSize ParameterDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	return QItemDelegate::sizeHint(option, index) + QSize(4, 4);
}

bool ParameterDelegate::eventFilter(QObject* object, QEvent* event)
{
	switch (event->type()) {
		case QEvent::KeyPress:
		case QEvent::KeyRelease:
			QKeyEvent *keyEvent = (QKeyEvent*)event;
			// ignore key up and down events to allow for keyboard navigation in the view
			if (keyEvent->key() == Qt::Key_Up || keyEvent->key() == Qt::Key_Down) {
				return true;
			}
			break;
		default:
			break;
	}

	return QItemDelegate::eventFilter(object, event);
}

void ParameterDelegate::editorValueChanged()
{
	if (sender() && sender()->isWidgetType())
		emit commitData(static_cast<QWidget *>(sender()));
}


//
// ParameterTableModel
//

QModelIndex ParameterTableModel::index(int row, int column, const QModelIndex & parent) const
{
	if( !parent.isValid() ) {
		return createIndex(row, column, -1);
	}
	else {
		return createIndex(row, column, parent.row());
	}
	return QModelIndex();
}

QModelIndex ParameterTableModel::parent(const QModelIndex & index) const
{
	Q_UNUSED(index);
	if( !index.isValid() || isParameter(index) ) {
		return QModelIndex();
	}
	else {
		return createIndex(parameter(index), 0, -1);
	}
}

int ParameterTableModel::rowCount(const QModelIndex & parent) const
{
	if( m_effect != NULL ) {
		if( !parent.isValid() ) {
			return m_effect->getParameterNum();
		}
		else if( isParameter(parent) ) {
			// Return parameter components.
			QVariant value = m_effect->getParameterValue(parent.row());
			if( value.canConvert(QVariant::List) ) {
				QVariantList list = value.toList();
				return list.count();
			}
		}
	}
	return 0;
}

int ParameterTableModel::columnCount(const QModelIndex & parent) const
{
	Q_UNUSED(parent);
	return 2;
}

QVariant ParameterTableModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) {
		return QVariant();
	}

	if( isComponent(index) ) {
		// Parameter component.
		if (index.column() == 0) {
			if (role == Qt::DisplayRole || role == Qt::EditRole) {
				Effect::EditorType editorType = m_effect->getParameterEditor(parameter(index));
				if( editorType == Effect::EditorType_Vector ) {
					Q_ASSERT(component(index) < 4);
					return QString("xyzw"[component(index)]);
				}
				else if( editorType == Effect::EditorType_Color ) {
					Q_ASSERT(component(index) < 4);
					return QString("rgba"[component(index)]);
				}
				else if( editorType == Effect::EditorType_Matrix ) {
					const int rows = m_effect->getParameterColumns(parameter(index));
					const int idx = component(index);
					int x = idx / rows;
					int y = idx % rows;
					return "[" + QString::number(x) + ", " + QString::number(y) + "]";
				}
			}
		}
		else if (index.column() == 1) {
			if (role == Qt::DisplayRole || role == Qt::EditRole) {
				QVariantList value = m_effect->getParameterValue(parameter(index)).toList();
				Q_ASSERT( component(index) < value.count() );
				return value.at(component(index));
			}
		}
	}
	else {
		// Root parameter.
		if (index.column() == 0) {
			if (role == Qt::DisplayRole || role == Qt::EditRole) {
				return m_effect->getParameterName(parameter(index));
			}
		}
		else if (index.column() == 1) {
			if (role == Qt::DisplayRole) {
				Effect::EditorType editorType = m_effect->getParameterEditor(parameter(index));
				if( editorType == Effect::EditorType_Matrix ) {
					return "[...]";
				}
				else {
					QVariant value = m_effect->getParameterValue(parameter(index));
					if( value.canConvert(QVariant::String) ) {
						return value;
					}
					else if( value.canConvert(QVariant::StringList) ) {
						return "[" + value.toStringList().join(", ") + "]";
					}
					else {
						/*if(value.isValid()) {
							printf("!! %s\n", value.typeName());
						}
						else {
							printf("!! invalid value returned by getParameterEditor %d\n", index);
						}*/
						printf("!!");
					}
				}
			}
			else if (role == Qt::EditRole) {
				return m_effect->getParameterValue(parameter(index));
			}
			else if (role == Qt::DecorationRole) {
				Effect::EditorType editorType = m_effect->getParameterEditor(parameter(index));
				if (editorType == Effect::EditorType_Color) {
					QVariantList value = m_effect->getParameterValue(parameter(index)).toList();
					QColor color;
					color.setRgbF(value.at(0).toDouble(), value.at(1).toDouble(), value.at(2).toDouble());
					QPixmap pm(12, 12);
					pm.fill(color);
					return pm;
				}
			}
		}
	}

	if (role == Qt::EditRole) {
		return QVariant();
	}

	return QVariant();
}

QVariant ParameterTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole) {
		if (orientation == Qt::Horizontal) {
			if( section == 0 ) return tr("Name");
			else if( section == 1 ) return tr("Value");
		}
	}

	return QVariant();
}

QModelIndex ParameterTableModel::buddy(const QModelIndex & index) const
{
	if (index.column() == 0) {
		return createIndex(index.row(), 1, index.internalId());
	}
	return index;
}

Qt::ItemFlags ParameterTableModel::flags(const QModelIndex &index) const
{
	Q_ASSERT(index.isValid());
	Qt::ItemFlags f = QAbstractItemModel::flags(index);
	if (index.column() == 1 && isEditable(index)) {
		return f | Qt::ItemIsEditable;
	}
	return f;
}

bool ParameterTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	Q_UNUSED(index);
	Q_UNUSED(value);
	Q_UNUSED(role);

	if (!index.isValid() || index.column() != 1)
	{
		return false;
	}
	if (value.isNull())
	{
		return false;
	}

	if (role == Qt::EditRole) {
		if( isComponent(index) ) {
			// Parameter component.
			QVariantList list = m_effect->getParameterValue(parameter(index)).toList();
			Q_ASSERT( component(index) < list.count() );
			list.replace(component(index), value);
			m_effect->setParameterValue(parameter(index), list);
			emit dataChanged(index, index);
			QModelIndex pindex = createIndex(parameter(index), 1, -1);
			emit dataChanged(pindex, pindex);
			return true;
		}
		else {
			// Root parameter.
			m_effect->setParameterValue(parameter(index), value);
			emit dataChanged(index, index);
			return true;
		}
	}

	return false;
}


void ParameterTableModel::clear()
{
	m_effect = NULL;
	reset();
}

void ParameterTableModel::setEffect(Effect * effect)
{
	Q_ASSERT(effect != NULL);
	m_effect = effect;
	reset();
}


bool ParameterTableModel::isEditable(const QModelIndex &index) const
{
	if( isParameter(index) ) {
		QVariant value = m_effect->getParameterValue(parameter(index));
		return value.isValid() && !value.canConvert(QVariant::List);
	}
	else {
		// components are always editable.
		return true;
	}
}

// static
bool ParameterTableModel::isParameter(const QModelIndex &index)
{
	return index.internalId() == -1;
}

// static
bool ParameterTableModel::isComponent(const QModelIndex &index)
{
	return index.internalId() != -1;
}

// static
int ParameterTableModel::parameter(const QModelIndex &index)
{
	if( isParameter(index) ) {
		return index.row();
	}
	else {
		return index.internalId();
	}
}

// static
int ParameterTableModel::component(const QModelIndex &index)
{
	Q_ASSERT(isComponent(index));
	return index.row();
}

bool ParameterTableModel::useColorEditor(const QModelIndex &index) const
{
	Q_ASSERT(m_effect != NULL);
	if (!index.isValid())
		return false;
	Effect::EditorType editor = m_effect->getParameterEditor(parameter(index));
	return editor == Effect::EditorType_Color;
}

bool ParameterTableModel::useNumericEditor(const QModelIndex &index) const
{
	Q_ASSERT(m_effect != NULL);
	if (!index.isValid())
		return false;
	Effect::EditorType editor = m_effect->getParameterEditor(parameter(index));
	return editor == Effect::EditorType_Scalar ||
		editor == Effect::EditorType_Vector ||
		editor == Effect::EditorType_Matrix;
}

bool ParameterTableModel::useFileEditor(const QModelIndex &index) const
{
	Q_ASSERT(m_effect != NULL);
	if (!index.isValid())
		return false;
	Effect::EditorType editor = m_effect->getParameterEditor(parameter(index));
	return editor == Effect::EditorType_File;
	//return false;
}



//
// ParameterPanel
//

ParameterPanel::ParameterPanel(const QString & title, QWidget * parent /*= 0*/, Qt::WFlags flags /*= 0*/) :
		QDockWidget(title, parent, flags), m_model(NULL), m_delegate(NULL), m_table(NULL)
{
	initWidget();
}

ParameterPanel::ParameterPanel(QWidget * parent /*= 0*/, Qt::WFlags flags /*= 0*/) :
		QDockWidget(parent, flags), m_model(NULL), m_delegate(NULL), m_table(NULL)
{
	initWidget();
}


QSize ParameterPanel::sizeHint() const
{
	return QSize(200, 200);
}

void ParameterPanel::initWidget()
{
	m_model = new ParameterTableModel(this);
	connect(m_model, SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SIGNAL(parameterChanged()));

	m_delegate = new ParameterDelegate(this);

	m_table = new QTreeView(this);
	m_table->setModel(m_model);
	m_table->setItemDelegate(m_delegate);
	m_table->header()->setStretchLastSection(true);
	m_table->header()->setClickable(false);
	m_table->setAlternatingRowColors(true);
	m_table->setEditTriggers(QAbstractItemView::AllEditTriggers);

	//	m_table->setIndentation(0);	// @@ This would be nice if it didn't affect the roots.

	setWidget(m_table);
}

void ParameterPanel::clear()
{
	m_model->clear();
}

void ParameterPanel::setEffect(Effect * effect)
{
	m_model->setEffect(effect);
	m_table->resizeColumnToContents(0);
}


