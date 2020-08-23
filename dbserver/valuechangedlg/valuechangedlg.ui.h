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
#include <stdio.h>
#include <qfile.h>
#include <iostream.h>

void valuechangedlg::init()
{
    FILE *gpIn;
    table1->setColumnWidth(0,50);
    table1->setColumnWidth(1,50);
    rowCount = 0;
    QStringList valueList;
    QString rowScan = "";
    
    //~~~~~~~~~~~~~~~~~~~Line count [START]~~~~~~~~~~~~~~~~~~~~~
    system("wc -l .valuetochange.txt > numLines.txt");
    gpIn = fopen("numLines.txt","r");
    fscanf(gpIn,"%d",&rowCount);
    fclose(gpIn);
    gpIn = NULL;
    system("rm -f numLines.txt");
    //~~~~~~~~~~~~~~~~~~~Line count [STOP]~~~~~~~~~~~~~~~~~~~~~
    table1->setNumRows(rowCount);
    rowCount = 0;
    QFile fpRead(".valuetochange.txt");
    if(fpRead.open(IO_ReadOnly | IO_Truncate))
    {
	QTextStream fpIn(&fpRead);
	fpIn.setEncoding(QTextStream::UnicodeUTF8);
	while(!fpIn.atEnd())
	{
	    rowScan = fpIn.readLine();
	    valueList = QStringList::split("\t",rowScan);
	    QStringList::Iterator it = valueList.begin();
	    table1->setText(rowCount,0,*it);
	    ++it;
	    table1->setText(rowCount,1,*it);
	    ++rowCount;
	}
	fpRead.close();
    }
}

void valuechangedlg::txtReplace_textChanged(const QString &text)
{
    QString str = text.right(1);
    if(str == "\t")
    {       
	str = text;
	str.remove(str.length()-1,1);
	txtReplace->setText(str);
    }
}

void valuechangedlg::txtWith_textChanged(const QString &text)
{
    QString str = text.right(1);
    if(str == "\t")
    {       
	str = text;
	str.remove(str.length()-1,1);
	txtWith->setText(str);
    }
}

void valuechangedlg::cmdAdd_clicked()
{
    if(table1->numRows() < rowCount + 1)
	table1->setNumRows(rowCount + 1);
    table1->setText(rowCount,0,txtReplace->text());
    table1->setText(rowCount,1,txtWith->text());
    ++rowCount;
    txtReplace->setText("");
    txtWith->setText("");
}

void valuechangedlg::cmdRemove_clicked()
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

void valuechangedlg::cmdOK_clicked()
{
    QFile fpWrite(".valuetochange.txt");
    if(fpWrite.open(IO_WriteOnly | IO_Truncate))
    {
	QTextStream fpOut(&fpWrite);
	fpOut.setEncoding(QTextStream::UnicodeUTF8);
	for(int i = 0; i < rowCount; ++i)
	    fpOut << table1->text(i,0) << "\t" << table1->text(i,1) << endl;
	fpWrite.close();
	accept();
    }
}
