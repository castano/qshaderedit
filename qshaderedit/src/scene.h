#ifndef SCENE_H
#define SCENE_H

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
	virtual void draw() = 0;
	virtual void transform() = 0;
};


// @@ TODO: Add scene factory class.

Scene * createTeapot();
Scene * createQuad();

#endif // SCENE_H
