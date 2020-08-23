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
#include <qfile.h>
#include <qsqldatabase.h>
#include <qmessagebox.h>

void frmfieldedit::initSettings()
{
    QStringList lblCol;
    QString fieldName = "";
    int row = 0;
    
    table1->setNumRows((int)rejectedFieldList.count());
    table1->setNumCols(1);
    lblCol.append("Field Name");
    table1->setColumnLabels(lblCol);
    
    for(QStringList::Iterator it = rejectedFieldList.begin(); it != rejectedFieldList.end(); ++it)
	table1->setText(row++,0,*it);
    table1->adjustColumn(0);
    dlgStatus = false;
}

void frmfieldedit::table1_valueChanged(int row,int col)
{
    table1->updateCell(row,col);
}

void frmfieldedit::cmdOK_clicked()
{
    if(checkForEdit())
    {
	postEditCheck();
	if(postErrList.count())
	{
	    int count = 1;
	    QString msg = "Following edited field names conflict with reserve words again! \n";
	    for(QStringList::Iterator it = postErrList.begin(); it != postErrList.end(); ++it,++count)
		msg += QString::number(count) + ") " + *it + "\n";
	    msg += "Kindly change these field names.";
	    QMessageBox::warning(this,tr("Post edit error!"),tr(msg),tr("Close"),0,0);
	}
	else
	{
	    int numRows = table1->numRows();
	    rejectedFieldList.clear();
	    for(int i = 0; i < numRows; ++i)
		rejectedFieldList.append(table1->text(i,0));
	    dlgStatus = true;
	    sqlDB = NULL;
	    postErrList.clear();
	    accept();
	}
    }
    else
	QMessageBox::warning(this,tr("Edit Problem?"),tr("Do you have edited all field names?\nfield names conflict with reserve words\nIf yes then\n1) Click once at the top left corner of the table.\n2) Now click on 'OK' button"),tr("Close"),0,0);
}

bool frmfieldedit::checkForEdit()
{
    int tabIndex = 0;
    bool chFlag = true;
    
    for(QStringList::Iterator it = rejectedFieldList.begin(); it != rejectedFieldList.end(); ++it,++tabIndex)
    {
	if(*it == table1->text(tabIndex,0))
	{
	    chFlag = false;
	    break;
	}
    }
    return chFlag;
}

void frmfieldedit::postEditCheck()
{
    QString query;
    int numRows = table1->numRows();
    
    postErrList.clear();
    for(int i = 0; i < numRows; ++i)
    {
	//~~~~~~Check weather any field name is RESERVE WORD or not~~~~~~
	query = "select * from reserveWords where res_words='" + table1->text(i,0).upper() + "';";
	QSqlQuery sq = sqlDB->exec(query);
	if(sq.numRowsAffected())
	    postErrList.append(table1->text(i,0));
    }
}

void frmfieldedit::cmdCancel_clicked()
{
    dlgStatus = false;
    accept();
}
