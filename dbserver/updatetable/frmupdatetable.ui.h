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
#include <qstringlist.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qmessagebox.h>

void frmupdatetable::initSettings(QStringList fileColList, QStringList filedtList, QStringList filelnList,QStringList tabColList, QStringList tabdtList, QStringList tablnList)
{
    QStringList columnList;
    
    columnList.append("Selection");
    columnList.append("Column name[FILE]");
    columnList.append("Data type[FILE]");
    columnList.append("Length[FILE]");
    columnList.append("Column name[TABLE]");
    columnList.append("Data type[TABLE]");
    columnList.append("Length[TABLE]");
    
    numRows = (int)fileColList.count() - 1;
    numCols = 7;
    table1->setNumRows(numRows);
    table1->setNumCols(numCols);
    table1->setColumnLabels(columnList);
    
    filColList = fileColList;
    fildtList = filedtList;
    fillnList = filelnList;
    tbColList = tabColList;
    tabdttypeList = tabdtList;
    tablntypeList = tablnList;
    
    cmbinpfkey->insertStringList(fileColList);
    cmbtabkey->insertStringList(tabColList);
    cmbtabkey_activated(0);
    //cmbinpfkey_activated(0);
    //cmbinpfkey->setEnabled(false);
    cmbtabkey->setEnabled(false);
}

void frmupdatetable::cmbinpfkey_activated(int id)
{
    populateTable(id);
}

void frmupdatetable::cmbtabkey_activated(int id)
{
    subTbColList.clear();
    for(QStringList::iterator it = tbColList.begin(); it != tbColList.end(); ++it)
    {
	if(*it != cmbtabkey->text(id))
	    subTbColList.append(*it);
    }
    populateTable(cmbinpfkey->currentItem());
}

void frmupdatetable::populateTable(int id)
{
    removeRows();
    table1->setNumRows(numRows);
    //~~Populate file mapping table[START]
    QStringList::iterator it1 = filColList.begin();
    QStringList::iterator it2 = fildtList.begin();
    QStringList::iterator it3 = fillnList.begin();
    
    for(int i = 0; i < numRows; )
    {
	if(cmbinpfkey->text(id) != *it1)
	{
	    QCheckBox *cbox = new QCheckBox(table1);
	    table1->setCellWidget(i,0,cbox);
	    table1->setText(i,1,*it1);
	    table1->setText(i,2,*it2);
	    table1->setText(i,3,*it3);
	    QComboBox *cmb = new QComboBox(table1);
	    connect(cmb,SIGNAL(activated(int )),this,SLOT(updatetableCell45(int )));
	    cmb->insertStringList(subTbColList,0);
	    table1->setCellWidget(i,4,cmb);
	    //updatetableCell45(0);
	    ++i;
	}
	++it1;
	++it2;
	++it3;
    }
    
    for(int i = 0; i < 6; ++i)
    {
	table1->adjustColumn(i);
	table1->setColumnReadOnly(i,true);
    }
    //~~Populate file mapping table[STOP]
}

void frmupdatetable::removeRows()
{
    for(int i = numRows - 1; i >= 0; --i)
	table1->removeRow(i);
}

void frmupdatetable::updatetableCell45(int id)
{
    QStringList::iterator it1 = tabdttypeList.begin();
    ++it1;
    QStringList::iterator it2 = tablntypeList.begin();
    ++it2;
    
    for(int i = 0; i < id; ++i)
    {
	++it1;
	++it2;
    }
    table1->setText(curR,5,*it1);
    table1->setText(curR,6,*it2);   
}

void frmupdatetable::table1_pressed(int row, int col, int button, const QPoint &p)
{
    curR = row;
}

void frmupdatetable::cmdConfirm_clicked()
{
    if(inputValidationPass())
    {
	sourceColList.clear();
	targetColList.clear();
	for(int i = 0; i < numRows; ++i)
	{
	    QCheckBox *cbox = (QCheckBox *)table1->cellWidget(i,0);
	    if(cbox->isChecked())
	    {
		sourceColList.append(table1->text(i,1));
		QComboBox *c = (QComboBox *)table1->cellWidget(i,4);
		targetColList.append(c->currentText());
	    }
	}
	statFlag = true;
	accept();
    }
}

bool frmupdatetable::inputValidationPass()
{
    if(emptyTargetInput() && uniqueTargetColumns() && dataTypeANDLengthMatching())
	return true;
    else
	return false;
}

bool frmupdatetable::emptyTargetInput()
{
    int rowId = -1;
    int colId = -1;
    bool retStat = true;
    
    for(int i = 0; i < numRows; ++i)
    {
	QCheckBox *cbox = (QCheckBox *)table1->cellWidget(i,0);
	if(cbox->isChecked())
	{
	    if(table1->text(i,5) == "")
	    {
		rowId = i;
		colId = 5;
		break;
	    }
	    else if(table1->text(i,6) == "")
	    {
		rowId = i;
		colId = 6;
		break;
	    }
	}
    }
    
    if(colId != -1)
    {
	QMessageBox::information(this,tr("Value missing!"),tr("Value missing in row: ") + QString::number(rowId) + tr(" col: ") + QString::number(colId) + tr(" in the table\nYou first click on the cell under 5th or 6th column in which you want to place value\nThen choose the column name from the dropdown box in the 4th column"));
	retStat = false;
    }
    return retStat;
}

bool frmupdatetable::uniqueTargetColumns()
{
    QString colStr;
    QStringList colList;
    int j = 0;
    bool retStat = true;
    
    for(int i = 0; i < numRows; ++i)
    {
	QCheckBox *cbox = (QCheckBox *)table1->cellWidget(i,0);
	if(cbox->isChecked())
	{
	    QComboBox *c = (QComboBox *)table1->cellWidget(i,4);
	    colList.append(c->currentText());
	}
    }
    
    colList.sort();
    QStringList::iterator it = colList.begin();
    colStr = *it; ++it;
    for(; it != colList.end() && j < numRows - 1; ++it, ++j)
    {
	if(*it == colStr)
	{
	    retStat = false;
	    QMessageBox::information(this,tr("Wrong field selection!"),tr("In the 4th column of the table same column name[") + *it + tr("]\nhas been selected more than once"));
	    break;
	}
	colStr = *it;
    }
    
    return retStat;
}

bool frmupdatetable::dataTypeANDLengthMatching()
{
    int retStat = true;
    bool ok = false;
    
    for(int i = 0; i < numRows; ++i)
    {
	QCheckBox *cbox = (QCheckBox *)table1->cellWidget(i,0);
	if(cbox->isChecked())
	{
	    if(table1->text(i,2) != table1->text(i,5))
	    {
		QMessageBox::information(this,tr("Data type mismatch!"),tr("Data type mismatch in ") + QString::number(i + 1) + tr("th row\n") + table1->text(i,2) + tr(" <---->") + table1->text(i,5));
		retStat = false;
		break;
	    }
	    else if((table1->text(i,3)).toInt(&ok) > (table1->text(i,6)).toInt(&ok))
	    {
		QMessageBox::information(this,tr("Length mismatch!"),tr("Length mismatch in ") + QString::number(i + 1) + tr("th row\n") + table1->text(i,3) + tr(" <---->") + table1->text(i,6) + tr("\nFile column length must be less than or equals to the table column length"));
		retStat = false;
		break;
	    }
	}
    }
    return retStat;
}

void frmupdatetable::cmdCancel_clicked()
{
    statFlag = false;
    accept();
}
