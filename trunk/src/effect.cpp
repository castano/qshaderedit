
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

