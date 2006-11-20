
#include "kshaderedit.h"

#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kurl.h>

static const char description[] = I18N_NOOP("A simple shader editor");

static const char version[] = "%{VERSION}";

static KCmdLineOptions options[] =
{
	{ "+[URL]", I18N_NOOP( "Document to open" ), 0 },
    KCmdLineLastOption
};


int main(int argc, char **argv)
{
	KAboutData about("kshaderedit", I18N_NOOP("KShaderEdit"), version, description,
		KAboutData::License_GPL, "(C) 2006 Ignacio Castano", 0, 0, "castano-stuff-qshaderedit@lists.sourceforge.net");
	about.addAuthor("Ignacio Castano", 0, "castanyo@yahoo.es");
	about.addAuthor("Lars Uebernickel", 0, "larsuebernickel@gmx.de");
	about.addCredit("Karl Robillard", 0, "krobillard@san.rr.com");

	KCmdLineArgs::init(argc, argv, &about);
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
