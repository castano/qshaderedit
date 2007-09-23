
#include "kshaderedit.h"

#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kurl.h>


int main(int argc, char **argv)
{
	KAboutData about("kshaderedit",
		0,
		ki18n("KShaderEdit"),
		"1.0",
		ki18n("A simple shader editor"),
		KAboutData::License_GPL,
		ki18n("(C) 2006 Ignacio Castano"),
		KLocalizedString(),
		0,	// homepage!
		"castano-stuff-qshaderedit@lists.sourceforge.net");

	about.addAuthor(ki18n("Ignacio Castano"), KLocalizedString(), 0, "castanyo@yahoo.es");
	about.addAuthor(ki18n("Lars Uebernickel"), KLocalizedString(), 0, "larsuebernickel@gmx.de");
	about.addCredit(ki18n("Karl Robillard"), KLocalizedString(), 0, "krobillard@san.rr.com");

	KCmdLineArgs::init(argc, argv, &about);

	KCmdLineOptions options;
	options.add("+[URL]", ki18n("Document to open"));

	KCmdLineArgs::addCmdLineOptions(options);

	KApplication app;

    /*// see if we are starting with session management
    if (app.isSessionRestored())
    {
        //RESTORE(KApp4);
    }
    else
    {
        // no session.. just start up normally
    }*/

	//QApplication app(argc, argv);
	// no session.. just start up normally
	KCmdLineArgs * args = KCmdLineArgs::parsedArgs();
	
	KUrl url;
	if (args->count() > 0) {
		url = qApp->arguments().at(0);
	}
	args->clear();
	
	KShaderEdit * shaderEdit = new KShaderEdit(url);
	
    return app.exec();
}
