
#include "qshaderedit.h"
#include "qglview.h"
#include "effect.h"
#include "messagepanel.h"
#include "parameterpanel.h"
#include "editor.h"
#include "newdialog.h"
#include "scene.h"

#include <QtCore/QFile>
#include <QtCore/QTimer>
#include <QtCore/QSettings>
#include <QtCore/QUrl>
#include <QtCore/QtDebug>	//
#include <QtGui/QApplication>
#include <QtGui/QMenu>
#include <QtGui/QToolBar>
#include <QtGui/QAction>
#include <QtGui/QMenuBar>
#include <QtGui/QTextEdit>
#include <QtGui/QTabWidget>
#include <QtGui/QStatusBar>
#include <QtGui/QDockWidget>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QCloseEvent>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>

namespace {
#ifdef Q_WS_MAC
	static const QString s_resourcePath = ":/images/mac";
#else
	static const QString s_resourcePath = ":/images/win";
#endif
}

/// Ctor.
QShaderEdit::QShaderEdit(const QString& filename) :
	m_editor(NULL),
	m_fileToolBar(NULL),
	m_techniqueToolBar(NULL),
	m_techniqueCombo(NULL),
	m_sceneViewDock(NULL),
	m_paramViewDock(NULL),
	m_logViewDock(NULL),
	m_sceneView(NULL),
	m_positionLabel(NULL),
	m_openAction(NULL),
	m_saveAction(NULL),
	m_recentFileSeparator(NULL),
	m_clearRecentAction(NULL),
	m_timer(NULL),
	m_animationTimer(NULL),
	m_file(NULL),
	m_effectFactory(NULL),
	m_effect(NULL),
	m_modified(false),
	m_autoCompile(true)
{
	// @@ Should we use this attrib?
	//setAttribute(Qt::WA_DeleteOnClose);
	
	// Create central widget.
	m_editor = new Editor(this);
	setCentralWidget(m_editor);
	m_editor->setFocus();
	connect(m_editor, SIGNAL(textChanged()), this, SLOT(shaderTextChanged()));
	connect(m_editor, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged()));

	createActions();
	createToolbars();
	createDockWindows();
	createMenus();
	createStatusbar();

    loadSettings();

	// Start timers.
	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(keyTimeout()));

	m_animationTimer = new QTimer(this);
	connect(m_animationTimer, SIGNAL(timeout()), m_sceneView, SLOT(updateGL()));

	show();

	// Make sure the main window is shown before the new file dialog.
	QApplication::processEvents();

	if (filename.isEmpty() || !load(filename)) {
		newFile(true);
	}
}

QShaderEdit::~QShaderEdit()
{
	closeEffect();
}

QSize QShaderEdit::sizeHint() const
{
	return QSize(600, 400);
}

void QShaderEdit::createActions()
{
	m_newAction = new QAction(QIcon(s_resourcePath + "/filenew.png"), tr("&New"), this);
	m_newAction->setShortcut(tr("Ctrl+N"));
	m_newAction->setStatusTip(tr("Create a new effect"));
	connect(m_newAction, SIGNAL(triggered()), this, SLOT(newFile()));

	m_openAction = new QAction(QIcon(s_resourcePath + "/fileopen.png"), tr("&Open"), this);
	m_openAction->setShortcut(tr("Ctrl+O"));
	m_openAction->setStatusTip(tr("Open an existing effect"));
	connect(m_openAction, SIGNAL(triggered()), this, SLOT(open()));

	m_saveAction = new QAction(QIcon(s_resourcePath + "/filesave.png"), tr("&Save"), this);
	m_saveAction->setEnabled(false);
	m_saveAction->setShortcut(tr("Ctrl+S"));
	m_saveAction->setStatusTip(tr("Save the effect"));
	connect(m_saveAction, SIGNAL(triggered()), this, SLOT(save()));

	m_saveAsAction = new QAction(tr("Save &As..."), this);
	m_saveAsAction->setStatusTip(tr("Save the effect under a new name"));
	m_saveAsAction->setEnabled(false);
	connect(m_saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

	for (int i = 0; i < MaxRecentFiles; ++i) {
		m_recentFileActions[i] = new QAction(this);
		m_recentFileActions[i]->setVisible(false);
		connect(m_recentFileActions[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
	}
	
	m_clearRecentAction = new QAction(tr("&Clear Recent"), this);
	m_clearRecentAction->setEnabled(false);
	connect(m_clearRecentAction, SIGNAL(triggered()), this, SLOT(clearRecentFiles()));

	m_findAction = new QAction(tr("&Find"), this);
	m_findAction->setEnabled(false);
	m_findAction->setShortcut(tr("Ctrl+F"));
	connect(m_findAction, SIGNAL(triggered()), m_editor, SLOT(findDialog()));
	
	m_findNextAction = new QAction(tr("&Find Next"), this);
	m_findNextAction->setEnabled(false);
#ifdef Q_WS_MAC
	m_findNextAction->setShortcut(tr("Ctrl+G"));
#else
	m_findNextAction->setShortcut(tr("F3"));
#endif
	connect(m_findNextAction, SIGNAL(triggered()), m_editor, SLOT(findNext()));
	
	m_findPreviousAction = new QAction(tr("&Find Previous"), this);
	m_findPreviousAction->setEnabled(false);
#ifdef Q_WS_MAC
	m_findPreviousAction->setShortcut(tr("Shift+Ctrl+G"));
#else
	m_findPreviousAction->setShortcut(tr("Shift+F3"));
#endif
	connect(m_findPreviousAction, SIGNAL(triggered()), m_editor, SLOT(findPrevious()));
	
	m_gotoAction = new QAction(tr("&Goto"), this);
	m_gotoAction->setEnabled(false);
#ifdef Q_WS_MAC
	m_gotoAction->setShortcut(tr("Ctrl+L"));
#else
	m_gotoAction->setShortcut(tr("Ctrl+G"));
#endif
	connect(m_gotoAction, SIGNAL(triggered()), m_editor, SLOT(gotoDialog()));

	
	// Add hidden actions.
	QAction * action = NULL;

	action = new QAction(tr("&Next Tab"), this);
	action->setShortcut(tr("Alt+Right"));
	connect(action, SIGNAL(triggered()), m_editor, SLOT(nextTab()));
	this->addAction(action);
	
	action = new QAction(tr("&Previous Tab"), this);
	action->setShortcut(tr("Alt+Left"));
	connect(action, SIGNAL(triggered()), m_editor, SLOT(previousTab()));
	this->addAction(action);
	
	m_compileAction = new QAction(tr("&Compile"), this);
	m_compileAction->setCheckable(true);
	m_compileAction->setChecked(true);
	m_compileAction->setShortcut(tr("F7"));
	connect(m_compileAction, SIGNAL(toggled(bool)), this, SLOT(compileChecked(bool)));
	this->addAction(m_compileAction);
}

void QShaderEdit::createMenus()
{
	QAction * action = NULL;

	QMenu * fileMenu = menuBar()->addMenu(tr("&File"));

	fileMenu->addAction(m_newAction);
	fileMenu->addAction(m_openAction);
	
	QMenu * recentFileMenu = fileMenu->addMenu(tr("Open Recent"));
	for(int i = 0; i < MaxRecentFiles; i++) {
		recentFileMenu->addAction(m_recentFileActions[i]);
	}
	m_recentFileSeparator = recentFileMenu->addSeparator();
	recentFileMenu->addAction(m_clearRecentAction);
	updateRecentFileActions();
	
	fileMenu->addAction(m_saveAction);
	fileMenu->addAction(m_saveAsAction);

//	fileMenu->addSeparator();

//	fileMenu->addAction(m_compile);
	
	fileMenu->addSeparator();
	
	action = new QAction(tr("E&xit"), this);
	action->setShortcut(tr("Ctrl+Q"));
	action->setStatusTip(tr("Exit the application"));
	connect(action, SIGNAL(triggered()), this, SLOT(close()));
	fileMenu->addAction(action);

	QMenu * editMenu = menuBar()->addMenu(tr("&Edit"));

	action = new QAction(QIcon(s_resourcePath + "/editundo.png"), tr("&Undo"), this);
	action->setShortcut(tr("Ctrl+Z"));
	action->setEnabled(false);
	connect(action, SIGNAL(triggered()), m_editor, SLOT(undo()));
	connect(m_editor, SIGNAL(undoAvailable(bool)), action, SLOT(setEnabled(bool)));
	editMenu->addAction(action);

	action = new QAction(QIcon(s_resourcePath + "/editredo.png"), tr("&Redo"), this);
	action->setShortcut(tr("Ctrl+Shift+Z"));
	action->setEnabled(false);
	connect(action, SIGNAL(triggered()), m_editor, SLOT(redo()));
	connect(m_editor, SIGNAL(redoAvailable(bool)), action, SLOT(setEnabled(bool)));
	editMenu->addAction(action);

	editMenu->addSeparator();

	action = new QAction(QIcon(s_resourcePath + "/editcut.png"), tr("C&ut"), this);
	action->setShortcut(tr("Ctrl+X"));
	action->setEnabled(false);
	connect(action, SIGNAL(triggered()), m_editor, SLOT(cut()));
	connect(m_editor, SIGNAL(copyAvailable(bool)), action, SLOT(setEnabled(bool)));
	editMenu->addAction(action);

	action = new QAction(QIcon(s_resourcePath + "/editcopy.png"), tr("&Copy"), this);
	action->setShortcut(tr("Ctrl+C"));
	action->setEnabled(false);
	connect(action, SIGNAL(triggered()), m_editor, SLOT(copy()));
	connect(m_editor, SIGNAL(copyAvailable(bool)), action, SLOT(setEnabled(bool)));
	editMenu->addAction(action);

	action = new QAction(QIcon(s_resourcePath + "/editpaste.png"), tr("&Paste"), this);
	action->setShortcut(tr("Ctrl+V"));
	action->setEnabled(false);
	connect(action, SIGNAL(triggered()), m_editor, SLOT(paste()));
	connect(m_editor, SIGNAL(pasteAvailable(bool)), action, SLOT(setEnabled(bool)));
	editMenu->addAction(action);

	editMenu->addSeparator();
	
	editMenu->addAction(m_findAction);
	editMenu->addAction(m_findNextAction);
	editMenu->addAction(m_findPreviousAction);
	editMenu->addAction(m_gotoAction);
	
	QMenu * viewMenu = menuBar()->addMenu(tr("&View"));
	
	// Should this be under the "Panels" menu? 
	viewMenu->addAction(m_sceneViewDock->toggleViewAction());
	viewMenu->addAction(m_paramViewDock->toggleViewAction());
	viewMenu->addAction(m_logViewDock->toggleViewAction());
	
	viewMenu->addSeparator();
	
	// On KDE this should be under the "Preferences" menu.
	QMenu * toolbarMenu = viewMenu->addMenu(tr("&Toolbars"));
	toolbarMenu->addAction(m_fileToolBar->toggleViewAction());
	toolbarMenu->addAction(m_techniqueToolBar->toggleViewAction());

	if( m_sceneView != NULL ) {	
		QMenu * sceneMenu = menuBar()->addMenu(tr("&Scene"));
		QMenu * sceneSelectionMenu = sceneMenu->addMenu(tr("&Select"));
	
		// Use scene plugins to create menus.
		const int count = SceneFactory::factoryList().count();
		for(int i = 0; i < count; i++) {
			const SceneFactory * factory = SceneFactory::factoryList().at(i);
			Q_ASSERT(factory != NULL);
			
			action = new QAction(QString("&%1 %2").arg(i+1).arg(factory->name()), this);
			action->setData(factory->name());
			connect(action, SIGNAL(triggered()), this, SLOT(selectScene()));
			sceneSelectionMenu->addAction(action);
		}
	
		m_renderMenu = sceneMenu->addMenu(tr("Render Options"));
		m_sceneView->populateMenu(m_renderMenu);
	}
	
	QMenu * helpMenu = menuBar()->addMenu(tr("&Help"));

	action = new QAction(tr("&About"), this);
	action->setStatusTip(tr("Show the application's about box"));
	connect(action, SIGNAL(triggered()), this, SLOT(about()));
	helpMenu->addAction(action);

	action = new QAction(tr("About &Qt"), this);
	action->setStatusTip(tr("Show the Qt library's about box"));
	connect(action, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	helpMenu->addAction(action);
	
	
}

void QShaderEdit::createToolbars()
{
	m_fileToolBar = new QToolBar(tr("File Toolbar"), this);
	m_fileToolBar->setObjectName("FileToolBar");
	this->addToolBar(m_fileToolBar);

	m_fileToolBar->addAction(m_newAction);
	m_fileToolBar->addAction(m_openAction);
	m_fileToolBar->addAction(m_saveAction);

	m_techniqueToolBar = new QToolBar(tr("Technique Toolbar"), this);
	m_techniqueToolBar->setObjectName("TechniqueToolBar");
	this->addToolBar(m_techniqueToolBar);

	m_techniqueCombo = new QComboBox();
	m_techniqueCombo->setEditable(false);
	m_techniqueCombo->setInsertPolicy(QComboBox::InsertAtBottom);
	m_techniqueCombo->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	m_techniqueCombo->setEnabled(false);
	connect(m_techniqueCombo, SIGNAL(activated(int)), this, SLOT(techniqueChanged(int)));

	QLabel * techniqueLabel = new QLabel(tr("Technique: "), m_techniqueToolBar);
	techniqueLabel->setBuddy(m_techniqueCombo);
	m_techniqueToolBar->addWidget(techniqueLabel);
	m_techniqueToolBar->addWidget(m_techniqueCombo);
}

void QShaderEdit::createStatusbar()
{
	statusBar()->showMessage(tr("Ready"));
	m_positionLabel = new QLabel(statusBar());
	statusBar()->addPermanentWidget(m_positionLabel);
}

void QShaderEdit::createDockWindows()
{
	m_logViewDock = new MessagePanel(tr("Messages"), this);
	m_logViewDock->setObjectName("MessagesDock");
	m_logViewDock->setAllowedAreas(Qt::BottomDockWidgetArea);
	m_logViewDock->setVisible(false);
	addDockWidget(Qt::BottomDockWidgetArea, m_logViewDock);
	connect(m_logViewDock, SIGNAL(messageClicked(int, int, int)), m_editor, SLOT(gotoLine(int, int, int)));

	m_sceneViewDock = new QDockWidget(tr("Scene"), this);
	m_sceneViewDock->setObjectName("SceneDock");
	m_sceneViewDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	if( QGLFormat::hasOpenGL() ) {
		QGLFormat format;
		//format.setRgba(true);
		//format.setAlpha(false);
		format.setDepth(true);
		//format.setStencil(false);	// not for now...
		format.setDoubleBuffer(true);
		m_sceneView = new QGLView(format, m_sceneViewDock);
		
		if( !m_sceneView->init(m_logViewDock) ) {
			QMessageBox::critical(this, tr("Error"), tr("OpenGL initialization failed."));
			m_logViewDock->setVisible(true);
		}
		m_sceneViewDock->setWidget(m_sceneView);
	}
	addDockWidget(Qt::RightDockWidgetArea, m_sceneViewDock);

	m_paramViewDock = new ParameterPanel(tr("Parameters"), this);
	m_paramViewDock->setObjectName("ParameterDock");
	m_paramViewDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	addDockWidget(Qt::RightDockWidgetArea, m_paramViewDock);
	connect(m_paramViewDock, SIGNAL(parameterChanged()), this, SLOT(setModified()));
}

bool QShaderEdit::closeEffect()
{
	if (m_effect == NULL) {
		Q_ASSERT(m_file == NULL);
		// No effect open.
		return true;
	}
	
	if( m_effectFactory != NULL && m_modified ) {
		QString fileName;
		if( m_file != NULL ) {
			fileName = strippedName(*m_file);
		}
		else {
			QString extension = m_effectFactory->extension();
			fileName = tr("untitled") + "." + extension;
		}

		while(true) {
			int answer = QMessageBox::question(this, tr("Save modified files"), tr("Do you want to save '%1' before closing?").arg(fileName), QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
			if( answer == QMessageBox::Yes ) {
				if( save() ) {
					break;
				}
			}
			else if( answer == QMessageBox::Cancel ) {
				return false;
			}
			else {
				break;
			}
		}
	}

	// Delete effect.
	m_sceneView->resetEffect();

	delete m_effect;
	m_effect = NULL;

	m_paramViewDock->clear();

	updateEditor();
	updateTechniques();

	// Delete effect file.
	delete m_file;
	m_file = NULL;
	
	return true;
}

void QShaderEdit::updateWindowTitle()
{
	QString title;

	if( m_file == NULL ) {
		title = tr("Untitled");
	}
	else {
		title = strippedName(*m_file);
	}

	if( m_modified ) {
		title.append(" ");
		title.append(tr("[modified]"));
	}

	title.append(" - ");

	title.append("QShaderEditor"); //QApplication::instance()->applicationName();


	this->setWindowTitle(title);
}

void QShaderEdit::updateActions()
{
	if(m_effectFactory == NULL) {
		return;
	}

	//m_saveAction->setVisible(m_file != NULL);
	m_saveAction->setEnabled(m_modified);
	m_saveAsAction->setEnabled(true);
	
	m_findAction->setEnabled(true);
	m_findNextAction->setEnabled(true);
	m_findPreviousAction->setEnabled(true);
	m_gotoAction->setEnabled(true);

	QString fileName;
	if( m_file == NULL ) {
		QString extension = m_effectFactory->extension();
		fileName = tr("untitled") + "." + extension;
	}
	else {
		fileName = strippedName(*m_file);
	}

	m_saveAction->setText(tr("Save %1").arg(fileName));
}

void QShaderEdit::updateEditor()
{
	if( m_effect == NULL ) {
		// Remove all the tabs from the editor.
		while(m_editor->count() > 0) {
			m_editor->removeTab(0);
		}
	}
	else {
		// Open all the inputs in tabs.
		int inputNum = m_effect->getInputNum();
		for(int i = 0; i < inputNum; i++) {
			QTextEdit * textEdit = m_editor->addEditor(m_effect->getInputName(i));

			Highlighter * hl = new Highlighter(textEdit->document());
			hl->setRules(m_effect->factory()->highlightingRules());
			hl->setMultiLineCommentStart(m_effect->factory()->multiLineCommentStart());
			hl->setMultiLineCommentEnd(m_effect->factory()->multiLineCommentEnd());

			textEdit->setPlainText(m_effect->getInput(i));
			textEdit->document()->setModified(false);
		}
	}

	// @@ This is not enough, the highligher modifies the document and triggers the textChanged event!
	// Stop the timer, that was triggered by setPlainText.
	m_timer->stop();

	m_modified = false;
}

void QShaderEdit::updateTechniques()
{
	QString lastText = m_techniqueCombo->currentText();
	
	int count = 0;
	if( m_effect != NULL ) {
		count = m_effect->getTechniqueNum();
	}
	
	if( count == 0 ) {
		m_techniqueCombo->setEnabled(false);
	}
	else {
		m_techniqueCombo->setEnabled(true);
		m_techniqueCombo->clear();
		
		for(int i = 0; i < count; i++) {
			m_techniqueCombo->addItem(m_effect->getTechniqueName(i));
		}
		m_techniqueCombo->setEnabled(count > 1);
		
		int idx = m_techniqueCombo->findText(lastText);
		if( idx != -1 ) {
			m_techniqueCombo->setCurrentIndex(idx);
			m_effect->selectTechnique(idx);
		}
	}
}

bool QShaderEdit::isModified()
{
	Q_ASSERT(m_effect != NULL);
	Q_ASSERT(m_editor != NULL);

	bool modified = false;

	const int num = m_editor->count();
	for(int i = 0; i < num; i++) {
		QTextEdit * editor = qobject_cast<QTextEdit *>(m_editor->widget(i));

		if( editor != NULL && editor->document()->isModified() ) {
			modified = true;
		}
	}

	return modified;
}

void QShaderEdit::setModified(bool modified)
{
	Q_ASSERT(m_effect != NULL);
	Q_ASSERT(m_editor != NULL);

	const int num = m_editor->count();
	for(int i = 0; i < num; i++) {
		QTextEdit * editor = qobject_cast<QTextEdit *>(m_editor->widget(i));

		if( editor != NULL  ) {
			editor->document()->setModified(modified);
		}
	}

	m_modified = modified;
}

void QShaderEdit::techniqueChanged(int index)
{
	if( index >= 0 ) {
		Q_ASSERT(m_effect != NULL);
		m_effect->selectTechnique(index);
		m_sceneView->updateGL();
	}
}

void QShaderEdit::cursorPositionChanged()
{
	int line = m_editor->line();
	int column = m_editor->column();
	m_positionLabel->setText(QString(tr(" Line: %1  Col: %2")).arg(line).arg(column));
}

void QShaderEdit::selectScene()
{
	QAction * action = qobject_cast<QAction *>(sender());
	if( action != NULL ) {
		const SceneFactory * factory = SceneFactory::findFactory(action->data().toString());
		Q_ASSERT(factory != NULL);
		m_sceneView->setScene(factory->createScene());
		
		m_renderMenu->clear();
		m_sceneView->populateMenu(m_renderMenu);
	}
}


void QShaderEdit::closeEvent(QCloseEvent * event)
{
	Q_ASSERT(event != NULL);
	
	if (closeEffect()) {
		event->accept();
		saveSettings();
	}
	else {
		event->ignore();
	}
}

void QShaderEdit::keyPressEvent(QKeyEvent * event)
{
	if( event->key() == Qt::Key_Escape ) {
		m_logViewDock->close();
		if( m_editor->currentWidget() ) {
			m_editor->currentWidget()->setFocus();
		}
		event->accept();
	}
	else {
		event->ignore();
	}
}

void QShaderEdit::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
	
	// @@ Make sure the file is a supported effect.
    event->acceptProposedAction();
	
	if( urls.size() ) {
        load(urls[0].toLocalFile());
	}
}


void QShaderEdit::newEffect(const EffectFactory * effectFactory)
{
	Q_ASSERT(effectFactory != NULL);
	Q_ASSERT(m_sceneView != NULL);

	if (!closeEffect())
		return;

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

	m_effectFactory = effectFactory;
	m_effect = m_effectFactory->createEffect(m_sceneView);
	Q_ASSERT(m_effect != NULL);

	updateEditor();
	updateWindowTitle();
	updateActions();
	updateTechniques();

	// Init scene view.
	m_sceneView->setEffect(m_effect);
	build(true);

	QApplication::restoreOverrideCursor();
}

void QShaderEdit::newFile(bool startup)
{
	// Count effect file types.
	const QList<const EffectFactory *> & effectFactoryList = EffectFactory::factoryList();

	int count = 0;
	foreach(const EffectFactory * ef, effectFactoryList) {
		if(ef->isSupported()) {
			++count;
		}
	}

	const EffectFactory * effectFactory = NULL;
	if(count == 0) {
		// Display error.
		QMessageBox::critical(this, tr("Error"), tr("No effect files supported"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
	}
	else if(count == 1) {
		// Use the first supported effect type.
		foreach(const EffectFactory * ef, effectFactoryList) {
			if(ef->isSupported()) {
				effectFactory = ef;
				break;
			}
		}
	}
	else {
		// Let the user choose the effect type.
		NewDialog newDialog(this, startup);

		int result = newDialog.exec();
		if( result == QDialog::Accepted ) {
			QString selection = newDialog.shaderType();

			foreach(const EffectFactory * ef, effectFactoryList) {
				if(ef->isSupported() && ef->name() == selection) {
					effectFactory = ef;
					break;
				}
			}
		}
		else if (result == NewDialog::OpenEffect) {
			open();
			return;
		}
	}

	if( effectFactory != NULL ) {
		newEffect(effectFactory);
	}
}

void QShaderEdit::open()
{
	// Find supported effect types.
	QStringList effectTypes;
	QStringList effectExtensions;
	foreach(const EffectFactory * factory, EffectFactory::factoryList()) {
		if( factory->isSupported() ) {
			QString effectType = QString("%1 (*.%2)").arg(factory->namePlural()).arg(factory->extension());
			effectTypes.append(effectType);

			QString effectExtension = factory->extension();
			effectExtensions.append("*." + effectExtension);
		}
	}

	if(effectTypes.isEmpty()) {
		QMessageBox::critical(this, tr("Error"), tr("No effect files supported"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
		return;
	}

	//QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), tr("."), effectTypes.join(";"));
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
            m_lastEffect, QString(tr("Effect Files (%1)")).arg(effectExtensions.join(" ")));

	if( !fileName.isEmpty() ) {
        m_lastEffect = fileName;
        load( fileName );
	}
}

void QShaderEdit::openRecentFile()
{
	QAction * action = qobject_cast<QAction *>(sender());
	if (action) {
		load(action->data().toString());
	}
}

void QShaderEdit::clearRecentFiles()
{
	// @@ TBD
 	QSettings settings( "Castano Inc", "QShaderEdit" );
	settings.setValue("recentFileList", QStringList());

	/*foreach (QWidget *widget, QApplication::topLevelWidgets()) {
		MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
		if (mainWin)
			mainWin->updateRecentFileActions();
	}*/
	
	// Only a single main window.
	updateRecentFileActions();	
}

bool QShaderEdit::load( const QString& fileName )
{
	Q_ASSERT(m_sceneView != NULL);
		
	if (!closeEffect())
		return false;

	m_file = new QFile(fileName);
	if (!m_file->open(QIODevice::ReadOnly)) {
		delete m_file;
		m_file = NULL;
		return false;
	}

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

	int idx = fileName.lastIndexOf('.');
	QString fileExtension = fileName.mid(idx+1);

	m_effectFactory = EffectFactory::factoryForExtension(fileExtension);
	if( m_effectFactory ) {
		Q_ASSERT(m_effectFactory->isSupported());
		m_effect = m_effectFactory->createEffect(m_sceneView);
		m_effect->load(m_file);
	}

	m_file->close();

	updateEditor();
	updateWindowTitle();
	updateActions();
	updateTechniques();
	setCurrentFile(fileName);

	// Init scene view.
	if( m_effect ) {
		m_sceneView->setEffect(m_effect);
		build(false);
	}
	else
	{
		m_sceneView->resetEffect();
	}

	QApplication::restoreOverrideCursor();
	return true;
}

bool QShaderEdit::save()
{
	if( m_file == NULL ) {
		// Choose file dialog.
		QString fileExtension = m_effectFactory->extension();
		QString effectType = QString("%1 (*.%2)").arg(m_effectFactory->namePlural()).arg(fileExtension);
		QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), tr("untitled") + "." + fileExtension, effectType);

		if( fileName.isEmpty() ) {
			return false;
		}

		m_file = new QFile(fileName);
	}

	m_file->open(QIODevice::WriteOnly);
	updateEffectInputs();
	m_effect->save(m_file);
	m_file->close();

	setModified(false);

	updateWindowTitle();
	updateActions();

	return true;
}

void QShaderEdit::saveAs()
{
	Q_ASSERT(m_effectFactory != NULL);

	QString fileExtension = m_effectFactory->extension();
	QString effectType = QString("%1 (*.%2)").arg(m_effectFactory->namePlural()).arg(fileExtension);

	QString fileName;
	if( m_file != NULL ) {
		fileName = strippedName(*m_file);
	}
	else {
		fileName = tr("untitled") + "." + fileExtension;
	}

	// Choose file dialog.
	fileName = QFileDialog::getSaveFileName(this, tr("Save File"), fileName, effectType);

	if( fileName.isEmpty() ) {
		return;
	}

	m_file = new QFile(fileName);

	m_file->open(QIODevice::WriteOnly);
	updateEffectInputs();
	m_effect->save(m_file);
	m_file->close();

	setModified(false);

	updateWindowTitle();
	updateActions();
	setCurrentFile(fileName);
}

void QShaderEdit::setCurrentFile(const QString &fileName)
{
 	QSettings settings( "Castano Inc", "QShaderEdit" );
	QStringList files = settings.value("recentFileList").toStringList();
	files.removeAll(fileName);
	files.prepend(fileName);
	while (files.size() > MaxRecentFiles)
		files.removeLast();

	settings.setValue("recentFileList", files);

	/*foreach (QWidget *widget, QApplication::topLevelWidgets()) {
		MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
		if (mainWin)
			mainWin->updateRecentFileActions();
	}*/
	
	// Only a single main window.
	updateRecentFileActions();
}

void QShaderEdit::updateRecentFileActions()
{
	QSettings settings( "Castano Inc", "QShaderEdit" );
	QStringList files = settings.value("recentFileList").toStringList();

	int numRecentFiles = qMin(files.count(), (int)MaxRecentFiles);

	for (int i = 0; i < numRecentFiles; ++i) {
		
		QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
		m_recentFileActions[i]->setText(text);
		m_recentFileActions[i]->setData(files[i]);
		m_recentFileActions[i]->setVisible(true);
		
		// Disable entry if file type not supported...
		int idx = files[i].lastIndexOf('.');
		QString fileExtension = files[i].mid(idx+1);

		const EffectFactory * factory = EffectFactory::factoryForExtension(fileExtension);
		if (factory == NULL || !factory->isSupported()) {
			m_recentFileActions[i]->setEnabled(false);
		}
	}

	for (int i = numRecentFiles; i < MaxRecentFiles; ++i) {
		m_recentFileActions[i]->setVisible(false);
	}
	
	m_recentFileSeparator->setVisible(numRecentFiles > 0);
	m_clearRecentAction->setEnabled(numRecentFiles > 0);
}

void QShaderEdit::about()
{
	QMessageBox::about(this, tr("About QShaderEdit"),
			tr("<b>QShaderEdit</b> is a simple shader editor"));
}

void QShaderEdit::setAutoCompile(bool enable)
{
	m_autoCompile = enable;
	if( m_autoCompile ) {
		m_timer->stop();
	}
}

void QShaderEdit::shaderTextChanged()
{
	bool modified = isModified();

	// if modified changed.
	if( m_modified != modified ) {
		m_modified = modified;
		updateWindowTitle();
		updateActions();
	}

	if( m_autoCompile )
	{
		// Compile after 1.5 seconds of inactivity.
		m_timer->start(1500);
	}
}

void QShaderEdit::keyTimeout()
{
	// Delay recompilation while editor is active or while compiling.
	if(m_paramViewDock->isEditorActive() || m_effect->isBuilding()) {
		m_timer->start(1500);
		return;
	}
	
	// Stop the timer.
	m_timer->stop();

	// @@ Set status bar message!
	statusBar()->showMessage(tr("Compiling..."));

//	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	
	updateEffectInputs();

	// Compile the effect.
	build(false);

//	QApplication::restoreOverrideCursor();
}

void QShaderEdit::compileChecked(bool checked)
{
	setAutoCompile(checked);
	
	if(checked == true)
	{
		// If checked, trigger compilation.
		keyTimeout();
	}
}

void QShaderEdit::build(bool silent)
{
	Q_ASSERT(m_effect != NULL);

	m_paramViewDock->clear();

	connect(m_effect, SIGNAL(built()), this, SLOT(built()));	
	
	if( silent ) {
		m_effect->build(false);
	}
	else {
		// clear messages.
		m_logViewDock->clear();
		
		// Connect effect signals.
		connect(m_effect, SIGNAL(infoMessage(QString)), m_logViewDock, SLOT(info(QString)));
		connect(m_effect, SIGNAL(errorMessage(QString)), m_logViewDock, SLOT(error(QString)));
		connect(m_effect, SIGNAL(buildMessage(QString,int,OutputParser*)), m_logViewDock, SLOT(log(QString,int,OutputParser*)));
		
		// Stop animation while building.
		if( m_effect->isAnimated() ) {
			m_animationTimer->stop();
		}
		
		// Start builder thread.
		m_effect->build(true);
	}
}

void QShaderEdit::built()
{
	qDebug() << "Built!";
	
	disconnect(m_effect, SIGNAL(built()), this, SLOT(built()));	
	disconnect(m_effect, SIGNAL(infoMessage(QString)), m_logViewDock, SLOT(info(QString)));
	disconnect(m_effect, SIGNAL(errorMessage(QString)), m_logViewDock, SLOT(error(QString)));
	disconnect(m_effect, SIGNAL(buildMessage(QString,int,OutputParser*)), m_logViewDock, SLOT(log(QString,int,OutputParser*)));
	
	if( m_effect->isValid() ) {
		statusBar()->showMessage(tr("Compilation succeed."), 2000);
		m_logViewDock->info(tr("Compilation succeed."));
	}
	else {
		statusBar()->showMessage(tr("Compilation failed."), 2000);
		m_logViewDock->setVisible(true);
		m_logViewDock->error(tr("Compilation failed."));
	}
	
	updateTechniques();
	m_paramViewDock->setEffect(m_effect);

	if( m_effect->isAnimated() ) {
		m_animationTimer->start(30);
	}
	else {
		m_animationTimer->stop();
	}

	m_sceneView->updateGL();
}

void QShaderEdit::setModified()
{
	m_modified = true;
	updateActions();
	updateWindowTitle();
	m_sceneView->updateGL();
}

void QShaderEdit::loadSettings()
{
	QSettings pref( "Castano Inc", "QShaderEdit" );

	pref.beginGroup("MainWindow");

	resize(pref.value("size", QSize(640,480)).toSize());
	bool maximize = pref.value("maximized", false).toBool();

	QByteArray state = pref.value("state").toByteArray();
	if (!state.isEmpty()) {
		restoreState(state);
	}

	pref.endGroup();

	m_autoCompile = pref.value("autoCompile", true).toBool();
	m_lastEffect = pref.value("lastEffect", ".").toString();
	SceneFactory::setLastFile(pref.value("lastScene", ".").toString());
	ParameterPanel::setLastPath(pref.value("lastParameterPath", ".").toString());

	if (maximize) {
		setWindowState(windowState() | Qt::WindowMaximized);
	}
}

void QShaderEdit::saveSettings()
{
	QSettings pref( "Castano Inc", "QShaderEdit" );

	pref.beginGroup("MainWindow");
	pref.setValue("maximized", isMaximized());
#if defined(Q_WS_WIN)
	pref.setValue("size", normalGeometry().size());
#else
	pref.setValue("size", size());
#endif
	pref.setValue("state", saveState());
	pref.endGroup();

	pref.setValue("autoCompile", m_autoCompile);
	pref.setValue("lastEffect", m_lastEffect);
	pref.setValue("lastScene", SceneFactory::lastFile());
	pref.setValue("lastParameterPath", ParameterPanel::lastPath());
}

void QShaderEdit::updateEffectInputs()
{
	Q_ASSERT(m_effect);
	int inputNum = m_effect->getInputNum();
	for(int i = 0; i < inputNum; i++) {
		QTextEdit * textEdit = qobject_cast<QTextEdit *>(m_editor->widget(i));
		if( textEdit != NULL ) {
			m_effect->setInput(i, textEdit->toPlainText().toLatin1());
		}
	}
}

// static
QString QShaderEdit::strippedName(const QString & fileName)
{
	return QFileInfo(fileName).fileName();
}

// static
QString QShaderEdit::strippedName(const QFile & file)
{
	return QFileInfo(file).fileName();
}
