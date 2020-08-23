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
#include <qlineedit.h>
#include <qpushbutton.h>
#include <iostream.h>
#include <qsqldatabase.h>
#include <qsqlcursor.h>
#include <qvariant.h>
#include <qmessagebox.h>

void fnwizard::init()
{
    txtnewcolname->setText("");
    txtEdit->clear();
    txtSyntax->clear();
    tabCursor = 0;
    
    fnTypeList.append("All");
    fnTypeList.append("Logical");
    fnTypeList.append("Mathematical");
    cmbFunctionType->insertStringList(fnTypeList);
    
    logicalList.append("IF");
    
    mathList.append("+");
    mathList.append("-");
    mathList.append("*");
    mathList.append("/");
    mathList.append("ABS");
    mathList.append("CEIL");
    mathList.append("FLOOR");
    mathList.append("SIN");
    mathList.append("COS");
    mathList.append("TAN");
    mathList.append("DEGREES");
    mathList.append("EXP");
    mathList.append("LN");
    mathList.append("LOG");
    mathList.append("LOG10");
    mathList.append("RADIANS");
    mathList.append("SQRT");
    mathList.append("ROUND");
    
    statList.clear();
    cmbFunctionType_activated(0);
}

void fnwizard::initSettings(QString wDir,QSqlDatabase *db,QString op_tabName)
{
    workDir = wDir;
    sqlDB = db;
    tableName = op_tabName;
    loadOperation_Table(0);
    preparefieldLists();
}

void fnwizard::loadOperation_Table(int mode)
{
    freeOperation_Table(mode);
    tabCursor = new QSqlCursor(tableName,true,sqlDB);
    tabCursor->setMode(mode);
}

void fnwizard::preparefieldLists()
{
    int numFields = tabCursor->count();
    QVariant v;
    
    colList.clear();
    targetCol.clear();
    
    tabCursor->select();
    tabCursor->next();
    
    for(register int itr = 0; itr < numFields; ++itr)
    {
	if(tabCursor->value(itr).toString() != "0")
	    colList.append(tabCursor->fieldName(itr));
	else
	    targetCol.append(tabCursor->fieldName(itr));
    }
    cmbColList->insertStringList(colList);
}

void fnwizard::freeOperation_Table(int mode)
{
    if(tabCursor)
    {
	tabCursor->clear();
	delete tabCursor;
	tabCursor = 0;
    }
}

bool fnwizard::checkColNameSyntax(QString colName)
{
    bool ok = true;
    int length = colName.length();
    QChar ch;
	    
    for(register int i = 0; i < length; ++i)
    {
	ch = colName.at(i);
	if(!((ch >= 'A' && ch <= 'Z') || (ch == '_') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9')))
	{
	    ok = false;
	    break;
	}
    }
    return ok;
}

void fnwizard::cmdOK_clicked()
{
    if(txtnewcolname->text() != "")
    {
	if(checkColNameSyntax(txtnewcolname->text()))
	{
	    int colCount = cmbColList->count();
	    QString colStr = "",query = "";
	    
	    for(int i = 0; i < colCount; ++i)
		colStr += cmbColList->text(i) + ",";
	    colStr += txtEdit->text() + " as " + txtnewcolname->text();
	    query = "create table " + tableName + "_temp as SELECT " + colStr + " FROM " + tableName;
	    fscanVal = -1;
	    fscanVal = system("mysql " + sqlDB->databaseName() + " -e \"" + query + "\"");
	    if(fscanVal == 0)
	    {
		fscanVal = -1;
		fscanVal = system("mysql " + sqlDB->databaseName() + " -e \"drop table " + tableName + "\"");
		if(fscanVal == 0)
		{
		    fscanVal = -1;
		    fscanVal = system("mysql " + sqlDB->databaseName() + " -e \"rename table " + tableName + "_temp to " + tableName + "\"");
		    if(fscanVal == 0)
			close();
		    else
			QMessageBox::information(this,"Table rename error","Can not rename table\nInform developer");
		}
		else
		    QMessageBox::information(this,"Table drop error","Can not drop table\nInform developer");
	    }
	    else
		QMessageBox::information(this,"Execution error","Query execution error\nInform developer");
	}
	else
	    QMessageBox::information(this,tr("Input error"),tr("Invalid column name: ") + txtnewcolname->text() + tr("\nColumn name can contains only charecter from 'A' to 'Z' case insensitive, \n'0' to '9' and underscore"));
    }
    else
	QMessageBox::information(this,"New column name","You must give a new column name");
}

void fnwizard::cmdCancel_clicked()
{
    close();
}


void fnwizard::cmbFunctionType_activated( int id)
{
    QStringList fnList;
    switch(id)
    {
    case 0:
	cmbFunctions->clear();
	fnList.clear();
	fnList += logicalList;
	fnList += mathList;
	fnList +=statList;
	cmbFunctions->insertStringList(fnList);
	fnList.clear();
	break;
    case 1:
	cmbFunctions->clear();
	fnList.clear();
	fnList += logicalList;
	cmbFunctions->insertStringList(fnList);
	fnList.clear();
	break;
    case 2:
	cmbFunctions->clear();
	fnList.clear();
	fnList += mathList;
	cmbFunctions->insertStringList(fnList);
	fnList.clear();
	break;
    case 3:
	cmbFunctions->clear();
	fnList.clear();
	fnList += statList;
	cmbFunctions->insertStringList(fnList);
	fnList.clear();
    }
}

void fnwizard::cmbFunctions_activated( int id)
{
    bool flag = false;
    int lines = txtEdit->lines();
    cout << lines << endl;
    QString str = txtEdit->text(lines-1);
    QString str2;
    txtEdit->clear();
    str2 = str + cmbFunctions->text(id);
    if(cmbFunctions->text(id) != "+" && cmbFunctions->text(id) != "-" && cmbFunctions->text(id) != "*" && cmbFunctions->text(id) != "/")
    {
	str2 += "()";
	flag = true;
    }
    str2 = str2.stripWhiteSpace();
    txtEdit->append(str2);
    if(flag == true)
	txtEdit->setCursorPosition(txtEdit->paragraphs(),txtEdit->paragraphLength(txtEdit->paragraphs()) - 1);
    else
	txtEdit->setCursorPosition(txtEdit->paragraphs(),txtEdit->paragraphLength(txtEdit->paragraphs()));
    //~~~~~~~~~~~~~~~~~~
    QString fnStr;
    
    fnStr = cmbFunctions->text(id);
    if(fnStr == "IF")
	txtSyntax->append("IF(Condition,is true value,else value)\n");
    else if(fnStr == "ABS")
	txtSyntax->append("ABS(Column1)\n");
    else if(fnStr == "CEIL")
	txtSyntax->append("CEIL(Column1)\n");
    else if(fnStr == "FLOOR")
	txtSyntax->append("FLOOR(Column1)\n");
    else if(fnStr == "SIN")
	txtSyntax->append("SIN(Column1)\n");
    else if(fnStr == "COS")
	txtSyntax->append("COS(Column1)\n");
    else if(fnStr == "TAN")
	txtSyntax->append("TAN(Column1)\n");
    else if(fnStr == "DEGREES")
	txtSyntax->append("DEGREES(Column1)\n");
    else if(fnStr == "EXP")
	txtSyntax->append("EXP(Column1)\n");
    else if(fnStr == "LN")
	txtSyntax->append("LN(Column1)\n");
    else if(fnStr == "LOG")
	txtSyntax->append("LOG(Column1)\n");
    else if(fnStr == "LOG10")
	txtSyntax->append("LOG10(Column1)\n");
    else if(fnStr == "RADIANS")
	txtSyntax->append("RADIANS(Column1)\n");
    else if(fnStr == "SQRT")
	txtSyntax->append("SQRT(Column1)\n");
    else if(fnStr == "ROUND")
	txtSyntax->append("ROUND(Column1)\n");
    //~~~~~~~~~~~~~~~~~~
}

void fnwizard::cmbColList_activated( int id)
{
    int para, index;
    
    txtEdit->getCursorPosition(&para,&index);
    txtEdit->insertAt(cmbColList->text(id),para,index);
}
