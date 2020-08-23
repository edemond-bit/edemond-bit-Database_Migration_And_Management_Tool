#include <qapplication.h>
//#include <qsplashscreen.h>
#include"frmgeno.h"

int main(int argc,char *argv[])
{
	QApplication app(argc,argv,true);
	app.setName("dbserver");
	//QSplashScreen *splash = new QSplashScreen(QPixmap::fromMimeSource("DNA.png"));
	//splash->message(QObject::tr("Setting up the main window..."),Qt::AlignRight | Qt::AlignTop, Qt::white);
	frmgeno *mainApp = new frmgeno;
	//splash->message(QObject::tr("Loading Modules..."),Qt::AlignRight | Qt::AlignTop, Qt::white);
	app.setMainWidget(mainApp);
	//splash->message(QObject::tr("Establishing Connection..."),Qt::AlignRight | Qt::AlignTop, Qt::white);
	mainApp->appQ = &app;
	mainApp->show();
	//splash->finish(mainApp);
	app.connect( &app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
	//delete splash;
	//Qt::WindowMaximized;
	return app.exec();
}
