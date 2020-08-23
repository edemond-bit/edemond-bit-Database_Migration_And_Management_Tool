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
#include <qmessagebox.h>

void createheader::initSettings()
{
    headerNameList.clear();
    exitStatus = false;
    table2->setNumRows(1);
    table2->setNumCols(6);
    table2->setText(0,0,tr("FID"));
    table2->setText(0,1,tr("INDID"));
    table2->setText(0,2,tr("FATHERID"));
    table2->setText(0,3,tr("MOTHERID"));
    table2->setText(0,4,tr("SEX"));
    table2->setText(0,5,tr("AFFECSTAT"));
    table2->setUpdatesEnabled(true);
}

void createheader::cmdOK_clicked()
{
    bool isEmpty = false;
    table2->update();
    for(int i = 0; i < 6; ++i)
    {
	if(table2->text(0,i) == "")
	{
	    isEmpty = true;
	    break;
	}
    }
    
    if(isEmpty)
	QMessageBox::information(this,tr("Give all header name"),tr("Kindly give all header name"));
    else
    {
	for(int i = 0; i < 6; ++i)
	    headerNameList.append(table2->text(0,i));
	exitStatus = true;
	accept();
    }
}

void createheader::cmdCancel_clicked()
{
    headerNameList.clear();
    exitStatus = false;
    accept();
}
