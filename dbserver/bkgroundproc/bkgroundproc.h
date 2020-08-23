#ifndef BKGROUNDPROC_H
#define BKGROUNDPROC_H
//#include <qapplication.h>
//#include <qworkspace.h>
#include <qobject.h>
#include <qthread.h>
#include <qstring.h>
#include <qlabel.h>
#include <iostream.h>
#include <qtimer.h>
//#include <qprogressbar.h>
//#include <qprogressdialog.h>
//#include <qlineedit.h>
//#include <qeventloop.h>
//#include "./progressdlg/progressdlg.h"

class bkgroundproc;
class timerClass : public QObject
{
    Q_OBJECT
private:
    bkgroundproc *proc;
    QTimer *t;
private slots:
    void haltStateTask();
public:
    timerClass(bkgroundproc *pr)
    {
	proc = pr;
	t = new QTimer();
	t->start(100);
	connect(t,SIGNAL(timeout()),this,SLOT(haltStateTask()));
    }
};

class bkgroundproc : public QThread
{
private:
    QString query;
    int fscanVal;
public:
    bkgroundproc(QString str)
    {
	query = str;
	//timerClass tob(this);
    }
    
    ~bkgroundproc()
    {
	query = "";
    }

private:
    void backgroundProcess();
    virtual void run()
    {
	backgroundProcess();
    }
};

#endif //BKGROUNDPROC_H
