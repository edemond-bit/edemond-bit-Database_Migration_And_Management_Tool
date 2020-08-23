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
#include <iostream.h>
#include <qcheckbox.h>

void frmeditheader::init()
{
    headList.clear();
}

void frmeditheader::initSettings()
{
    int counter = 0;
    
    exitStatus = false;
    numHeader = headList.count();
    if(numHeader > 0)
    {
	table1->setNumCols(numHeader);
	table1->setColumnLabels(headList);
	for(QStringList::iterator it = headList.begin(); it != headList.end(); ++it, ++counter)
	{
	    QCheckBox *chkBox = new QCheckBox(table1);
	    if(counter > 0)
		chkBox->setChecked(true);
	    else
		chkBox->setChecked(false);
	    table1->setCellWidget(0,counter,(QCheckBox *)chkBox);
	}
    }
}

void frmeditheader::cmdOK_clicked()
{
    headList.clear();
    for(int i = 0; i < numHeader; ++i)
    {
	QCheckBox *chkBox = (QCheckBox *)table1->cellWidget(0,i);
	if(chkBox->isChecked())
	    headList.append(QString::number(i+1));
    }
    exitStatus = true;
    accept();
}

void frmeditheader::cmdCancel_clicked()
{
    exitStatus = false;
    accept();
}
