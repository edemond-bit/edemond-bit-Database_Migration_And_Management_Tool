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
#include <qevent.h>
#include <qtable.h>
#include <qcheckbox.h>
#include <qfile.h>
#include <qmessagebox.h>

void definetabstructure::init()
{
    QStringList colList;
    QStringList dttypeList;
    
    colList.clear();
    colList.append("Column name");
    colList.append("Data type");
    colList.append("Length");
    
    table1->setNumCols(3);
    table1->setColumnLabels(colList);
    table1->setNumRows(2);
    
    dttypeList.append(" ");
    dttypeList.append("VARCHAR");
    dttypeList.append("INT");
    dttypeList.append("FLOAT");
    cmbdttype->insertStringList(dttypeList,0);
    colPanel->setEnabled(false);
}

void definetabstructure::loadDatabaseList(QString wDir)
{
    QStringList dbNameList;
    QString dbName = "";
		     
    workDir = wDir;
    QFile pathIn(workDir + tr("/.dbCreated.txt"));
    if(pathIn.open(IO_ReadOnly | IO_Truncate))
    {
	QTextStream fpIn(&pathIn);
	while(!fpIn.atEnd())
	{
	    fpIn >> dbName;
	    dbNameList.append(dbName);
	}
	pathIn.close();
	cmbdatabase->insertStringList(dbNameList,0);
	initialRow();
    }
}

void definetabstructure::initialRow()
{
    table1->setText(0,0,"AutoIndex");
    table1->setText(0,1,"INT");
    table1->setText(0,2,"10");
    table1->setRowReadOnly(0,true);
}

void definetabstructure::table1_clicked( int row, int col, int button, const QPoint &p)
{
    int numRows = table1->numRows();
    
    if(numRows == row + 1 && col == 0) //If last row's first column is clicked
    {
	table1->setNumRows(numRows + 1);
	currentRow = row;
	colPanel->setEnabled(true);
	refreshColumnDetails();
    }
}

void definetabstructure::refreshColumnDetails()
{
    txtcolname->setText("");
    cmbdttype->setCurrentItem(0);
    txtlength->setText("");
}

void definetabstructure::txtcolname_textChanged(const QString &text)
{
    table1->setText(currentRow,0,text);
}

void definetabstructure::cmbdttype_activated(int id)
{
    if(cmbdttype->text(id) != " ")
    {
	table1->setText(currentRow,1,cmbdttype->text(id));
	if(cmbdttype->text(id) == "FLOAT")
	{
	    lblHints1->setText("Total digits,Digits after point");
	    lblHints2->setText("M,D with condition: (M - D) > 0");
	}
	else
	{
	    lblHints1->setText("");
	    lblHints2->setText("");
	}
    }
}

void definetabstructure::txtlength_textChanged(const QString &text)
{
    bool ok = false, ok1 = false;
    QString value = text, value1;
    
    if(cmbdttype->currentItem() == 3 && text.contains(",") >= 1)
    {
	if(text.contains(",") > 1)
	{
	    value = text;
	    value.truncate(value.length() - 1);
	    txtlength->setText(value);
	}
	else
	{
	    value = text.section(",",0,0);
	    if(value == "")
		txtlength->setText("");
	    else
	    {
		int length = value.toInt(&ok,10);
		if(!ok)
		    txtlength->setText("");
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
			    txtlength->setText(value);
			}
		    }
		}
	    }
	}
	table1->setText(currentRow,2,txtlength->text());
    }
    else
    {
	int length = value.toInt(&ok,10);
	if(!ok)
	{
	    value.truncate(text.length() - 1);
	    txtlength->setText(value);
	}
	table1->setText(currentRow,2,txtlength->text());
    }
}

void definetabstructure::cmdCreate_clicked()
{
    QString tabName = txttablename->text();
        
    if(tabName != "")
    {
	if(checkTbNameSyntax(tabName))
	{
	    if(validateTableDefinition())
	    {
		nowCreateTheTable();
	    }
	}
	else
	    QMessageBox::information(this,tr("Table name"),tr("Incompitable table name: ") + txttablename->text() + tr("\nName can contains only charecter from 'A' to 'Z' case insensitive, \n'0' to '9' and underscore"));
    }
    else
	QMessageBox::information(this,tr("Table name missing"),tr("No table name given!"));
}

bool definetabstructure::validateTableDefinition()
{
    bool retStat = false;
    QString colName = "";

    for(int i = 0; i < table1->numRows() - 1; ++i)
    {
	colName = table1->text(i,0);
	if(colName != "")
	{
	    if(checkTbNameSyntax(colName))
		retStat = true;
	    else
	    {
		retStat = false;
		QMessageBox::information(this,tr("Column name"),tr("Incompitable column name: ") + colName + tr("\nName can contains only charecter from 'A' to 'Z' case insensitive, \n'0' to '9' and underscore"));
		break;
	    }
	}
	else
	    QMessageBox::information(this,tr("Column name missing"),tr("No column name given!"));
    }

    if(table1->numRows() == 1)
    {
	QMessageBox::information(this,tr("Column name missing"),tr("No column name given!"));
	retStat = false;
    }
    return retStat;
}

void definetabstructure::cmdClose_clicked()
{
    close();
}

bool definetabstructure::checkTbNameSyntax(QString &tbName)
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

void definetabstructure::nowCreateTheTable()
{
    QString dbName = "";
    
    dbName = cmbdatabase->currentText();
    prepareANDExecuteTheQuery(dbName);
}

void definetabstructure::prepareANDExecuteTheQuery(QString dbName)
{
    QString qCommand = "";
    QString query = "CREATE TABLE ";
    int scanVal = 0;
    
    query += txttablename->text() + "(";
    for(int i = 0; i < table1->numRows() - 1; ++i)
    {
	if(i == 0)
	{
	    query += table1->text(i,0) + " " + table1->text(i,1) + "(" + table1->text(i,2) + ") AUTO_INCREMENT, ";
	}
	else if(i < table1->numRows() - 2)
	    query += table1->text(i,0) + " " + table1->text(i,1) + "(" + table1->text(i,2) + "), ";
	else
	    query += table1->text(i,0) + " " + table1->text(i,1) + "(" + table1->text(i,2) + "),";
    }
    query += " PRIMARY KEY (" + table1->text(0,0) + "))ENGINE=MyISAM";
    cout << query << endl;
    qCommand = "mysql " + dbName + " -e \"" + query + "\"";
    scanVal = system(qCommand);
    cout << "scanVal: " << scanVal << endl;
    if(scanVal > -1)
	QMessageBox::information(this,tr("Table creation status"),tr("Table created successfully!"));
    else
	QMessageBox::information(this,tr("Table creation status"),tr("Table creation unsuccessful\n"));
}

void definetabstructure::keyPressEvent(QKeyEvent * e)
{
    if(Qt::Key_Delete == e->key())
    {
	if(table1->isRowSelected(table1->currentRow()))
	    table1->removeRow(table1->currentRow());
    }
}
