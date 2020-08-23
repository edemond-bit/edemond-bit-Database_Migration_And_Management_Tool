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
#include <qstringlist.h>
#include <iostream.h>

int frmDecisionDlg::retVal = -2;

void frmDecisionDlg::initSetting(QString keyValue,QStringList fieldList)
{
    int numRec,Counter;
    QString str;
    bool oldField,newField;
    
    retVal = -2;
    numRec = fieldList.count();
    Counter = 1;
    
    lEdit->append("Record for the rs no: \"" + keyValue + "\"\n");
    lEdit->append("Old Fields: \n[");
    for(QStringList::Iterator it = fieldList.begin(); it != fieldList.end(); ++it,++Counter)
    {
	if(Counter < numRec)
	{
	    if(Counter < numRec - 1)
		str += *it + "\n";
	    else
		str += *it;
	}
	else
	{
	    if(str.isEmpty())
		oldField = false;
	    else
		oldField = true;
	    lEdit->append(str + "]\n");
	    lEdit->append("New Fields: \n[");
	    if(*it)
		newField = false;
	    else
		newField = true;
	    lEdit->append(*it + "]\n");
	}
    }
    lEdit->append("INSTRUCTION: 1) UPDATE(if you want to update old rows with matching \'rs\' and insert new rows)\n");
    lEdit->append("                        2) INSERT(if you want to insert only new rows)\n");
}

void frmDecisionDlg::cmdOK_clicked()
{
    if(rdInsert->isChecked())
	retVal = (chkApply->isChecked() ? 2 : 1);
    else if(rdUpdate->isChecked())
	retVal = (chkApply->isChecked() ? 4 : 3);
    close();
}

void frmDecisionDlg::cmdCancel_clicked()
{
    retVal = 0;
    close();
}

void frmDecisionDlg::cmdAbort_clicked()
{	
    retVal = -1;
    close();
}
