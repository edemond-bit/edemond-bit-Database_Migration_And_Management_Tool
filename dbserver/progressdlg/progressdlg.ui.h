/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/
#include <qlayout.h>
#include <qtimer.h>

void progressDlg::init()
{
    isDone = false;
    childProgress->hide();
    lblChild->hide();
    startTimer(10);
}

void progressDlg::initSettings(bool k1, bool k2)
{
    isparentP = k1;
    ischildP = k2;
    if(!ischildP)
    {
	QRect g = frame4->geometry();
	frame4->setGeometry(g.x(),g.y(),g.width(),g.height()/2);
	QRect g1 = buttonGroup1->geometry();
	buttonGroup1->setGeometry(g1.x(),g1.y() - g.height()/2,g1.width(),g1.height());
	QRect g2 = geometry();
	setGeometry(g2.x(),g2.y(),g2.width(),g2.height() - g.height()/2);
    }
}

void progressDlg::cmdCancel_clicked()
{
    if(isDone)
	close();
}

void progressDlg::timerEvent(QTimerEvent *e)
{
    if(isparentP)
    {
	parentProgress->show();
	lblParent->show();
    }
    
    if(ischildP)
    {
	childProgress->show();
	lblChild->show();
    }
    
    cmdCancel->show();
}
