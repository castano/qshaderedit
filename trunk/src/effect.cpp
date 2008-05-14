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

#include <QtOpenGL/QGLWidget>

#include "effect.h"

namespace {
	static QList<const EffectFactory *> * s_factoryList = NULL;
}

/// Get an effect factory for the given file extension.
const EffectFactory * EffectFactory::factoryForExtension(const QString & ext)
{
	foreach(const EffectFactory * factory, *s_factoryList) {
		Q_ASSERT(factory != NULL);
		if( factory->extension() == ext ) {
			return factory;
		}
	}
	return NULL;
}

/// Get the list of effect factories.
const QList<const EffectFactory *> & EffectFactory::factoryList()
{
	static QList<const EffectFactory *> empty;
	if(s_factoryList == NULL) {
		return empty;
	}
	else {
		return *s_factoryList;
	}
}

/// Add a the given factory.
void EffectFactory::addFactory(const EffectFactory * factory)
{
	Q_ASSERT(factory != NULL);
	if( s_factoryList == NULL ) {
		s_factoryList = new QList<const EffectFactory *>();
	}
	s_factoryList->append(factory);
}

/// Remove the given factory.
void EffectFactory::removeFactory(const EffectFactory * factory)
{
	Q_ASSERT(factory != NULL);
	
	const int index = s_factoryList->lastIndexOf(factory);
	Q_ASSERT(index != -1);
	s_factoryList->removeAt(index);
	
	if( s_factoryList->isEmpty() ) {
		delete s_factoryList;
		s_factoryList = NULL;
	}
}


void Effect::makeCurrent()
{
	m_widget->makeCurrent();
}

void Effect::doneCurrent()
{
	m_widget->makeCurrent();
}

