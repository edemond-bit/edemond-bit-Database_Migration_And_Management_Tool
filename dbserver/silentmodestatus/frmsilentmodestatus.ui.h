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
#include <qlabel.h>
#include <qimage.h>
#include <qpixmap.h>

void frmsilentmodestatus::initSettings(int &numLines, int &silentModePivot,int *importedRecsList)
{
    QString fileName;
    QStringList columnList;
    
    columnList.append(tr("Data File"));
    columnList.append(tr("Status"));
    columnList.append(tr("Records"));
    
    waitPath = workDir + tr("/images/wait.png");
    tickPath = workDir + tr("/images/tick.png");
    
    table1->setNumRows(numLines);
    table1->setNumCols(3);
    table1->setColumnLabels(columnList);
    
    QFile fpRead("datasource.txt");
    if(fpRead.open(IO_ReadOnly | IO_Truncate))
    {
	QTextStream fpIn(&fpRead);
	fpIn.setEncoding(QTextStream::UnicodeUTF8);
	for(register int i = 0; i < numLines; ++i)
	{
	    fileName = fpIn.readLine();
	    table1->setText(i,0,fileName.section('/',-1));
	    QLabel *lbl = new QLabel(table1,tr("label"),WDestructiveClose);
	    if(i <= silentModePivot)
	    {
		lbl->setPixmap(QPixmap(QImage(tickPath)));
		table1->setText(i,2,QString::number(importedRecsList[i]));
	    }
	    else
		lbl->setPixmap(QPixmap(QImage(waitPath)));
	    lbl->adjustSize();
	    table1->setCellWidget(i,1,lbl);
	}
	table1->adjustColumn(0);
	table1->adjustColumn(1);
	table1->adjustColumn(2);
	fpRead.close();
    }
}

void frmsilentmodestatus::updateStatus(int &index, int *importedRecsList)
{   
    QLabel *lbl = (QLabel *)table1->cellWidget(index,1);
    lbl->setPixmap(QPixmap(QImage(tickPath)));
    lbl->adjustSize();
    table1->setText(index,2,QString::number(importedRecsList[index]));
    table1->adjustColumn(1);
    table1->adjustColumn(2);
    table1->ensureVisible(index,0);
}

void frmsilentmodestatus::cmdStop_clicked()
{
    close();
}
