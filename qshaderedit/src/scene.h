#ifndef SCENE_H
#define SCENE_H

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
	virtual void draw() const = 0;
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
