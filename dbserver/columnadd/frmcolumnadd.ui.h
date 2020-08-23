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
#include <qmessagebox.h>

void frmcolumnadd::init()
{
    QStringList dttypeList;
    
    cmbcolpos->setEnabled(false);
    dttypeList.append(" ");
    dttypeList.append("VARCHAR");
    dttypeList.append("INT");
    dttypeList.append("FLOAT");
    cmbdttype->insertStringList(dttypeList,0);
}

void frmcolumnadd::initSettings(QStringList colList)
{
    cmbcolpos->insertStringList(colList);
}

void frmcolumnadd::txtLength_textChanged(const QString &text)
{
    bool ok = false, ok1 = false;
    QString value = text, value1;
    
    if(cmbdttype->currentItem() == 3 && text.contains(",") >= 1)
    {
	if(text.contains(",") > 1)
	{
	    value = text;
	    value.truncate(value.length() - 1);
	    txtLength->setText(value);
	}
	else
	{
	    value = text.section(",",0,0);
	    if(value == "")
		txtLength->setText("");
	    else
	    {
		int length = value.toInt(&ok,10);
		if(!ok)
		    txtLength->setText("");
		else
		{
		    value1 = text.section(",",-1);
		    if(value1 != "")
		    {
			int length1 = value1.toInt(&ok1,10);
			if(!ok1 || length - length1 <= 0) //if ok then float length value given in correct format(M,D), now check whether M-D > 0
			{
			    value = text;
			    value.truncate(value.length() - value1.length());
			    txtLength->setText(value);
			}
		    }
		}
	    }
	}
    }
    else
    {
	int length = value.toInt(&ok,10);
	if(!ok)
	{
	    value.truncate(text.length() - 1);
	    txtLength->setText(value);
	}
    }
}

void frmcolumnadd::rdFirst_toggled(bool state)
{
    if(state)
	position = 0;
	
}

void frmcolumnadd::rdAfter_toggled(bool state)
{
    if(state)
    {
	position = 1;
	cmbcolpos->setEnabled(true);
    }
    else
	cmbcolpos->setEnabled(false);
}

void frmcolumnadd::cmdOK_clicked()
{
    colName = txtcolname->text();
    
    if(checkColNameSyntax(colName))
    {
	if(validationOK())
	{
	    if(rdAfter->isChecked())
		selCol = cmbcolpos->currentText();
	    else
		selCol = "";
	    dttype = cmbdttype->currentText();
	    Length = txtLength->text();
	    accept();
	}
    }
    else
	QMessageBox::information(this,tr("Input error"),tr("Invalid column name: ") + colName + tr("\nColumn name can contains only charecter from 'A' to 'Z' case insensitive, \n'0' to '9' and underscore"));
}

void frmcolumnadd::cmdCancel_clicked()
{
    position = -1;
    accept();
}

bool frmcolumnadd::validationOK()
{
    bool retStat = false;
    bool ok = false;
    int item = cmbdttype->currentItem();
    
    if(item == 3)
    {
	QString str = txtLength->text();
	if(str.contains(',') > 0)
	{
	    QString str1 = str.section(',',-1);
	    QString str2 = str.section(',',0,0);
	    int val1 = str1.toInt(&ok);
	    int val2 = str2.toInt(&ok);
	    if(val2 < val1)
	    {
		QMessageBox::information(this,tr("Invalid input!"),tr("Length value given for 'FLOAT' data type is wrong.\nLength format : (total length, length after decimal point)\n Here total length >= length after decimal point\n example : (14,10)"),QMessageBox::Ok);
		retStat = false;
	    }
	    else
		retStat = true;
	}
	else
	{
	    QMessageBox::information(this,tr("Invalid input!"),tr("Length value given for 'FLOAT' data type is wrong.\nLength format : (total length, length after decimal point)\n Here total length >= length after decimal point\n example : (14,10)"),QMessageBox::Ok);
	    retStat = false;
	}
    }
    else if(item == 1 || item == 2)
    {
	int value2 = (txtLength->text()).toInt(&ok,10);
	if(!ok)
	{
	    if(item == 1)
		QMessageBox::information(this,tr("Invalid input!"),tr("Length value given for 'VARCHAR' data type is wrong.\nIt should be just an integer value"),QMessageBox::Ok);
	    else if(item == 2)
		QMessageBox::information(this,tr("Invalid input!"),tr("Length value given for 'INT' data type is wrong.\nIt should be just an integer value"),QMessageBox::Ok);
	    retStat = false;
	}
	else
	    retStat = true;
    }
    
    //~~~Check duplicate column name[START]
    for(int i = 0; i < cmbcolpos->count(); ++i)
    {
	if(cmbcolpos->text(i) == txtcolname->text())
	{
	    QMessageBox::information(this,tr("Duplicate column"),tr("Column with the same name already exist!"),QMessageBox::Ok);
	    retStat = false;
	    break;
	}
    }
    //~~~Check duplicate column name[STOP]
    return retStat;
}

bool frmcolumnadd::checkColNameSyntax(QString &tbName)
{
    bool ok = true;
    int length = tbName.length();
    QChar ch;
	    
    for(register int i = 0; i < length; ++i)
    {
	ch = tbName.at(i);
	if(!((ch >= 'A' && ch <= 'Z') || (ch == '_') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9')))
	{
	    ok = false;
	    break;
	}
    }
    return ok;
}
