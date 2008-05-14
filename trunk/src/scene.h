/*
    QShaderEdit - Simple multiplatform shader editor
    Copyright (C) 2007 Ignacio Castaño <castano@gmail.com>
    Copyright (C) 2007 Lars Uebernickel <larsuebernickel@gmx.de>

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

#ifndef SCENE_H
#define SCENE_H

#include <QtCore/QObject>
#include <QtGui/QIcon>

class QMenu;

// @@ I'm not sure where to expose the scene selection. Here are 
// a few options:
// - combo box on glview toolbar.
// - combo box on a new main window toolbar.
// - submenu in the scene menu.
// - extra dialog.

// @@ It would be nice if scenes could add menu options to
// the Scene menu. That would allow you to turn lights on and off,
// change background color, etc.

// Poor man's scene
class Scene 
{
public:
	virtual ~Scene() {}
	
	// pass effect to work around ATI driver bug (Effect::beginMaterialGroup())
	virtual void draw(class Effect* effect) const = 0;
	
	virtual void transform() const = 0;
	virtual void setupMenu(QMenu * menu) const = 0;
};


class SceneFactory : public QObject
{
public:
	virtual QString name() const = 0;
	virtual QString description() const = 0;
	virtual QIcon icon() const = 0;
	virtual Scene * createScene() const = 0;
	
	static const SceneFactory * findFactory(const QString & name);
	static const QList<const SceneFactory *> & factoryList();
	static void addFactory(const SceneFactory * factory);
	static void removeFactory(const SceneFactory * factory);
	static Scene * defaultScene();
	
	static const QString & lastFile();
	static void setLastFile(const QString & lastFile);
};


#define REGISTER_SCENE_FACTORY(Factory) \
	namespace { \
		static Factory factory##Factory; \
		struct Factory##Registrar { \
			Factory##Registrar() { SceneFactory::addFactory(&factory##Factory); } \
			~Factory##Registrar() { SceneFactory::removeFactory(&factory##Factory); } \
		}; \
		static Factory##Registrar registrar##Factory; \
	}


#endif // SCENE_H
