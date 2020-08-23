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
#include <qfile.h>

void frmcrypticsettings::init()
{
    QString rowScan = "";
    
    rowCount = 0;
    QFile fpRead(".crypticsettings.txt");
    if(fpRead.open(IO_ReadOnly | IO_Truncate))
    {
	QTextStream fpIn(&fpRead);
	fpIn.setEncoding(QTextStream::UnicodeUTF8);
	if(!fpIn.atEnd())
	{
	    rowScan = fpIn.readLine();
	    txtthreshold->setText(rowScan);
	    while(!fpIn.atEnd())
	    {
		rowScan = fpIn.readLine();
		table1->setText(rowCount,0,rowScan);
		++rowCount;
	    }
	}
	fpRead.close();
    }
    table1->adjustColumn(0);
}

void frmcrypticsettings::txtthreshold_textChanged(const QString &text)
{
    bool ok = false;
    QString str = text;
    
    double val = text.toDouble(&ok);
    if(!ok)
    {
	str.truncate((int)str.length() - 1);
	txtthreshold->setText(str);
    }
	
}

void frmcrypticsettings::cmdadd_clicked()
{
    if(table1->numRows() < rowCount + 1)
	table1->setNumRows(rowCount + 1);
    table1->setText(rowCount,0,txtundefinedgenotype->text());
    ++rowCount;
    txtundefinedgenotype->setText("");
}

void frmcrypticsettings::cmdremove_clicked()
{
    for(int i = 0; i < rowCount; ++i)
    {
	if(table1->isRowSelected(i))
	{
	    table1->removeRow(i);
	    --rowCount;
	}
    }
}

void frmcrypticsettings::cmdclose_clicked()
{
    QFile fpWrite(".crypticsettings.txt");
    if(fpWrite.open(IO_WriteOnly | IO_Truncate))
    {
	QTextStream fpOut(&fpWrite);
	fpOut.setEncoding(QTextStream::UnicodeUTF8);
	fpOut << txtthreshold->text() << endl;
	for(int i = 0; i < rowCount; ++i)
	    fpOut << table1->text(i,0) << endl;
	fpWrite.close();
	close();
    }
}
