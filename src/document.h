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

#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QtCore/QString>
#include <QtCore/QFileSystemWatcher>

class Effect;
class EffectFactory;

class QFile;
class QTimer;
class QWidget;
class QGLWidget;


class Document : public QObject
{
	Q_OBJECT
public:
	
	Document(QGLWidget * glWidget, QWidget * parent = 0);
	~Document();
	
	QFile * file() { return m_file; };
	Effect * effect() { return m_effect; }
	
	bool isModified() const;
	void setModified(bool m);
	
	QString fileName() const;
	QString title() const;
	
	bool canLoadFile(const QString & fileName) const;	
	bool loadFile(const QString & fileName);
	
	// @@ Move to settings.
	static QString lastEffect();
	static void setLastEffect(const QString & effectPath);
	
public slots:
	
	// Actions.
	void reset(bool startup = false);
	void open();
	bool save();
	void saveAs();
	bool close();
	
	void build(bool threaded = true);
	
	void onParameterChanged();
	
protected:
	
	void newEffect(const EffectFactory * effectFactory);
	bool saveEffect();
	void closeEffect();
	
	
signals:
	
	// Emit this when we need the editors to commit their changes.
	void synchronizeEditors();
	
	void fileNameChanged(QString fileName);
	void titleChanged(QString fileName);
	void modifiedChanged(bool modified);
	
	void effectCreated();
	void effectDeleted();
	void effectBuilding();
	void effectBuilt(bool);
	
private:	
	
	QGLWidget * m_glWidget;
	QWidget * m_owner;
	
	const EffectFactory * m_effectFactory;		
	
	QFile * m_file;
	Effect * m_effect;
	
	bool m_modified;
	
	QFileSystemWatcher m_watch;	
	
	// @@ Move to settings.
	static QString s_lastEffect;
};



#endif // DOCUMENT_H
