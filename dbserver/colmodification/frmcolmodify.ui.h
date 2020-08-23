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

void frmcolmodify::init()
{
    QStringList colList;
    
    colList.append("Name[old]");
    colList.append("Name[Change]");
    colList.append("Data type");
    colList.append("Length[old]");
    colList.append("Length[Change]");
    
    table1->setNumCols(5);
    table1->setColumnLabels(colList);
}

void frmcolmodify::initSettings(QStringList &colName, QStringList &dttype, QStringList &length)
{
    rows = colName.count();
    QStringList::iterator it1 = colName.begin();
    QStringList::iterator it2 = dttype.begin();
    QStringList::iterator it3 = length.begin();
    table1->setNumRows(rows);
    for(int i = 0; i < rows; ++i)
    {
	table1->setText(i,0,*it1);
	table1->setText(i,1,*it1);
	table1->setText(i,2,*it2);
	table1->setText(i,3,*it3);
	table1->setText(i,4,*it3);
	it1++;
	it2++;
	it3++;
    }

    table1->setColumnReadOnly(0,true);
    table1->setColumnReadOnly(2,true);
    table1->setColumnReadOnly(3,true);
}

void frmcolmodify::cmdOK_clicked()
{
    if(validationOK())
    {
	QString name;
	for(int i = 0; i < rows; ++i)
	{
	    name = table1->text(i,1);
	    colNameList.append(name.stripWhiteSpace());
	    lengthList.append(table1->text(i,4));
	}
	retStat = true;
	accept();
    }
}

void frmcolmodify::cmdCancel_clicked()
{
    retStat = false;
    accept();
}

void frmcolmodify::table1_currentChanged(int row, int col)
{
    if(col == 4)
    {
	bool ok = false;
	int length = (table1->text(row,col)).toInt(&ok);
	if(!ok)
	{
	    QString Text = table1->text(row,col);
	    Text.truncate(length - 1);
	    table1->setText(row,col,Text);
	}
    }
}

bool frmcolmodify::validationOK()
{
    QString Text;
    int oldLength, newLength;
    bool ok, retStat;
    
    for(int i = 0; i < rows; ++i)
    {
	Text = table1->text(i,1);
	if(checkColNameSyntax(Text))
	{
	    if((table1->text(i,2)).upper() == "FLOAT")
	    {
		int oldLength1, newLength1;
		
		Text = (table1->text(i,3)).section(',',0,0); //Fetching M part of float length value[OLD]
		oldLength = Text.toInt(&ok);
		Text = (table1->text(i,3)).section(',',-1); //Fetching D part of float length value[OLD]
		oldLength1 = Text.toInt(&ok);
		
		Text = (table1->text(i,4)).section(',',0,0); //Fetching M part of float length value[NEW]
		newLength = Text.toInt(&ok);
		if(ok)
		{
		    Text = (table1->text(i,4)).section(',',-1); //Fetching D part of float length value[NEW]
		    newLength1 = Text.toInt(&ok);
		    if(ok)
		    {
			if(newLength >= oldLength)
			{
			    if(newLength1 >= oldLength1)
				retStat = true;
			    else
			    {
				QMessageBox::information(this,tr("Input error"),tr("Error in row: ") + QString::number(i + 1) + tr(" ,column: ") + QString::number(4) + tr("\nInvalid length value...\nCorrect format: M,D where M - D > 0 and M,D are integer\nHere, M(user given) >= M(old)"),QMessageBox::Ok);
				retStat = false;
			    }
			}
			else
			{
			    QMessageBox::information(this,tr("Input error"),tr("Error in row: ") + QString::number(i + 1) + tr(" ,column: ") + QString::number(4) + tr("\nInvalid length value...\nCorrect format: M,D where M - D > 0 and M,D are integer\nHere, D(user given) >= D(old)"),QMessageBox::Ok);
			    retStat = false;
			}
		    }
		    else
		    {
			QMessageBox::information(this,tr("Input error"),tr("Error in row: ") + QString::number(i + 1) + tr(" ,column: ") + QString::number(4) + tr("\nInvalid length value...\nCorrect format: M,D where M - D > 0 and M,D are integer"),QMessageBox::Ok);
			retStat = false;
		    }
		}
		else
		{
		    QMessageBox::information(this,tr("Input error"),tr("Error in row: ") + QString::number(i + 1) + tr(" ,column: ") + QString::number(4) + tr("\nInvalid length value...\nCorrect format: M,D where M - D > 0 and M,D are integer"),QMessageBox::Ok);
		    retStat = false;
		}
	    }
	    else
	    {
		oldLength = (table1->text(i,3)).toInt(&ok);
		newLength = (table1->text(i,4)).toInt(&ok);
		if(ok)
		{
		    if(newLength >= oldLength)
			retStat = true;
		    else
		    {
			QMessageBox::information(this,tr("Input error"),tr("Error in row: ") + QString::number(i + 1) + tr(" ,column: ") + QString::number(4) + tr("\nNew length value must be greater than old length value"),QMessageBox::Ok);
			retStat = false;
		    }
		}
		else
		{
		    QMessageBox::information(this,tr("Input error"),tr("Error in row: ") + QString::number(i + 1) + tr(" ,column: ") + QString::number(4) + tr("\nInvalid length value...\nCorrect format: D where D is an integer > 0"),QMessageBox::Ok);
		    retStat = false;
		}
	    }
	}
	else
	{
	    QMessageBox::information(this,tr("Input error"),tr("Invalid column name: ") + Text + tr("\nColumn name can contains only charecter from 'A' to 'Z' case insensitive, \n'0' to '9' and underscore"));
	    retStat = false;
	}
    }
}

bool frmcolmodify::checkColNameSyntax(QString &colName)
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
