/*
    QShaderEdit - Simple multiplatform shader editor
    Copyright (C) 2007 Ignacio Castaño <castano@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "document.h"
#include "effect.h"
#include "newdialog.h"

#include <QtCore/QFile>
#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>

namespace
{
	static QString strippedName(const QString & fileName)
	{
		return QFileInfo(fileName).fileName();
	}
	
	static QString strippedName(const QFile & file)
	{
		return QFileInfo(file).fileName();
	}
}

// static 
QString Document::lastEffect()
{
	return s_lastEffect;
}

// static 
void Document::setLastEffect(const QString & effectPath)
{
	s_lastEffect = effectPath;
}

// static
QString Document::s_lastEffect;



Document::Document(QGLWidget * glWidget, QWidget * parent/*= 0*/) : QObject(parent)
{
	Q_ASSERT(glWidget != NULL);
	
	m_glWidget = glWidget;
	m_owner = parent;
	
	m_effectFactory = NULL;
	m_file = NULL;
	m_effect = NULL;
	m_modified = false;
}

Document::~Document()
{
	closeEffect();
}


bool Document::isModified() const
{
	return m_modified;
}

void Document::setModified(bool m)
{
	// If file not saved, modified is always true.
	//if (m_modified != m)	// Sometimes want to force an update by invoking the signal.
	{
		m_modified = m;
		emit modifiedChanged(m_modified);
	}
}


/// Return the current document file name.
QString Document::fileName() const
{
	QString fileName;
	
	if (m_file != NULL)
	{
		fileName = strippedName(*m_file);
	}
	else
	{
		QString fileExtension = m_effectFactory->extension();
		fileName = tr("untitled") + "." + fileExtension;
	}
	
	return fileName;
}

/// Return document's title.
QString Document::title() const
{
	QString title;
	
	if (m_file == NULL)
	{
		title = tr("Untitled");
	}
	else
	{
		title = strippedName(*m_file);
	}

	if (m_modified)
	{
		title.append(" ");
		title.append(tr("[modified]"));
	}
	
	return title;
}


bool Document::canLoadFile(const QString & fileName) const
{
	int idx = fileName.lastIndexOf('.');
	QString fileExtension = fileName.mid(idx+1);

	const EffectFactory * effectFactory = EffectFactory::factoryForExtension(fileExtension);
	
	return (effectFactory != NULL) && m_effectFactory->isSupported();
}

bool Document::loadFile(const QString & fileName)
{
	if (!close())
	{
		// User cancelled the operation.
		return false;
	}
	
	delete m_file;
	m_file = new QFile(fileName);
	if (!m_file->open(QIODevice::ReadOnly))
	{
		delete m_file;
		m_file = NULL;
		return false;
	}

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

	m_modified = false;

	int idx = fileName.lastIndexOf('.');
	QString fileExtension = fileName.mid(idx+1);

	m_effectFactory = EffectFactory::factoryForExtension(fileExtension);
	if (m_effectFactory != NULL)
	{
		Q_ASSERT(m_effectFactory->isSupported());
		m_effect = m_effectFactory->createEffect(m_glWidget);
		Q_ASSERT(m_effect != NULL);
		
		connect(m_effect, SIGNAL(built(bool)), this, SIGNAL(effectBuilt(bool)));
		
		m_effect->load(m_file);
		
		emit effectCreated();
		
		build(false);
	}
	else
	{
		// @@ Can this happen?
	}

	m_file->close();
	
	emit titleChanged(title());
	
	s_lastEffect = fileName;	// @@ Move this somewhere else!
	emit fileNameChanged(fileName);
	
	QApplication::restoreOverrideCursor();
	return true;
}


void Document::reset(bool startup)
{
	// Count effect file types.
	const QList<const EffectFactory *> & effectFactoryList = EffectFactory::factoryList();

	int count = 0;
	foreach(const EffectFactory * ef, effectFactoryList)
	{
		if(ef->isSupported()) {
			++count;
		}
	}

	const EffectFactory * effectFactory = NULL;
	if (count == 0)
	{
		// Display error.
		QMessageBox::critical(m_owner, tr("Error"), tr("No effect files supported"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
	}
	else if(count == 1)
	{
		// Use the first supported effect type.
		foreach(const EffectFactory * ef, effectFactoryList) {
			if(ef->isSupported()) {
				effectFactory = ef;
				break;
			}
		}
	}
	else
	{
		// Let the user choose the effect type.
		NewDialog newDialog(m_owner, startup);
		
		int result = newDialog.exec();
		if (result == QDialog::Accepted)
		{
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

	if (effectFactory != NULL)
	{
		newEffect(effectFactory);
	}
}

void Document::open()
{
	// Find supported effect types.
	QStringList effectTypes;
	QStringList effectExtensions;
	foreach (const EffectFactory * factory, EffectFactory::factoryList())
	{
		if (factory->isSupported())
		{
			QString effectType = QString("%1 (*.%2)").arg(factory->namePlural()).arg(factory->extension());
			effectTypes.append(effectType);
			
			QString effectExtension = factory->extension();
			effectExtensions.append("*." + effectExtension);
		}
	}

	if (effectTypes.isEmpty())
	{
		QMessageBox::critical(m_owner, tr("Error"), tr("No effect files supported"), QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
		return;
	}

	//QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), tr("."), effectTypes.join(";"));
	QString fileName = QFileDialog::getOpenFileName(m_owner, tr("Open File"),
            s_lastEffect, QString(tr("Effect Files (%1)")).arg(effectExtensions.join(" ")));

	if (!fileName.isEmpty())
	{
        loadFile(fileName);
	}
}


bool Document::save()
{
	if (m_file == NULL)
	{
		// Choose file dialog.
		QString fileExtension = m_effectFactory->extension();
		QString effectType = QString("%1 (*.%2)").arg(m_effectFactory->namePlural()).arg(fileExtension);
		QString fileName = QFileDialog::getSaveFileName(m_owner, tr("Save File"), tr("untitled") + "." + fileExtension, effectType);
		
		if( fileName.isEmpty() ) {
			return false;
		}
		
		m_file = new QFile(fileName);
	}

	// @@ Check for errors.
	return saveEffect();
}

void Document::saveAs()
{
	Q_ASSERT(m_effectFactory != NULL);

	QString fileExtension = m_effectFactory->extension();
	QString effectType = QString("%1 (*.%2)").arg(m_effectFactory->namePlural()).arg(fileExtension);

	// Choose file dialog.
	QString fileName = QFileDialog::getSaveFileName(m_owner, tr("Save File"), this->fileName(), effectType);

	if( fileName.isEmpty() ) {
		return;
	}

	delete m_file;
	m_file = new QFile(fileName);

	// @@ Check for errors.
	saveEffect();
	
	// @@ Emit only when file name actually changed.
	emit fileNameChanged(fileName);
}

bool Document::close()
{
	if (m_effect == NULL)
	{
		Q_ASSERT(m_file == NULL);
		// No effect open.
		return true;
	}

	if (m_effectFactory != NULL && isModified())
	{
		QString fileName;
		if (m_file != NULL)
		{
			fileName = strippedName(*m_file);
		}
		else
		{
			QString extension = m_effectFactory->extension();
			fileName = tr("untitled") + "." + extension;
		}
		
		while (true)
		{
			int answer = QMessageBox::question(m_owner, tr("Save modified files"), tr("Do you want to save '%1' before closing?").arg(fileName), 
				QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
			
			if (answer == QMessageBox::Yes)
			{
				if( save() ) {
					break;
				}
			}
			else if (answer == QMessageBox::Cancel)
			{
				return false;
			}
			else
			{
				break;
			}
		}
	}
	
	emit effectDeleted();
	
	closeEffect();	
	
	return true;
}


void Document::build(bool threaded /*= true*/)
{
	Q_ASSERT(m_effect != NULL);
	
	emit synchronizeEditors();
	emit effectBuilding();
	
	m_effect->build(threaded);
}


void Document::onParameterChanged()
{
	Q_ASSERT(m_effectFactory != NULL);
	
	if (m_effectFactory->savesParameters() && !isModified())
	{
		setModified(true);
	}
}


void Document::newEffect(const EffectFactory * effectFactory)
{
	Q_ASSERT(effectFactory != NULL);

	if (!close())
	{
		// User cancelled the operation.
		return;
	}

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

	m_effectFactory = effectFactory;
	m_effect = m_effectFactory->createEffect(m_glWidget);
	Q_ASSERT(m_effect != NULL);

	connect(m_effect, SIGNAL(built(bool)), this, SIGNAL(effectBuilt(bool)));
	
	m_modified = false;
	
	emit effectCreated();
	emit titleChanged(title());
	
	build(false);

	QApplication::restoreOverrideCursor();
}


bool Document::saveEffect()
{
	m_file->open(QIODevice::WriteOnly);
	
	// Synchronize before save.
	emit synchronizeEditors();
	m_effect->save(m_file);
	
	m_file->close();

	setModified(false);

	emit titleChanged(title());
	
	//updateActions();				// qshaderedit listens to modifiedChanged
}


void Document::closeEffect()
{
	// Delete effect.
	delete m_effect;
	m_effect = NULL;
	
	delete m_file;
	m_file = NULL;
}




