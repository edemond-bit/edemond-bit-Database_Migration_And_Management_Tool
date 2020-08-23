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
#include <qapplication.h>
#include <iostream.h>
#include <qsqldatabase.h>
#include <qsqlselectcursor.h>
#include <qdatatable.h>
#include <qheader.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qinputdialog.h>
#include <qlabel.h>
#include <qprogressdialog.h>
#include <qmessagebox.h>

void frmmerge::init()
{
    lstcollist1a->clear();
    lstcollist1b->clear();
    lstcollist2->clear();
    lstcollist3->clear();
    numMRows = 0;
}

void frmmerge::initSettings(QString wDir,QSqlDatabase *sqlD,QStringList tabList,QString dbName)
{
    sqlDB = sqlD;
    workDir = wDir;
    cmbtable->insertStringList(tabList);
    cmbtable1->insertStringList(tabList);
    cmbtable2->insertStringList(tabList);
}

void frmmerge::cmbtable_activated( const QString &currTab)
{
    int index = 0;
    QSqlCursor *viewCur = new QSqlCursor(currTab,true,sqlDB);	
    viewCur->setMode(0);
    QDataTable *tabView = new QDataTable(viewCur,true,0,"datatable");
    tabView->refresh();
    numMRows = (tabView->numRows() > numMRows ? tabView->numRows() : numMRows);
    QHeader *head = tabView->horizontalHeader();
    int headCount = head->count();
    
    lstcollist1a->clear();
    lstcollist1b->clear();
    
    //Populate field list boxes[START]
    for(int i = 0; i < headCount; ++i)
    {
	if(index == 0)
	    lstcollist1a->insertItem(currTab + "." + head->label(i),index++);
	else
	    lstcollist1b->insertItem(currTab + "." + head->label(i),index++);
    }
    //Populate field list boxes[STOP]
    tabView->close();
    viewCur->clear();
    delete tabView;
    tabView = 0;
    delete viewCur;
    viewCur = 0;
}

void frmmerge::cmdAdd_clicked()
{
    QStringList fldList;
    
    unsigned int numItems = lstcollist2->count();
    for(register int itr = 0; itr < (int)numItems; ++itr)
	fldList.append(lstcollist2->text(itr));
    
    if(lstcollist1a->isSelected(0))
	fldList.prepend(lstcollist1a->text(0));
    
    numItems = lstcollist1b->count();
    for(register int itr = 0; itr < (int)numItems; ++itr)
    {
	if(lstcollist1b->isSelected(itr))
	    fldList.append(lstcollist1b->text(itr));
    }
    
    lstcollist2->clear();
    lstcollist2->insertStringList(fldList,0);
    fldList.clear();
    lstcollist1a->clearSelection();
    lstcollist1b->clearSelection();
    lstcollist2->clearSelection();
}

void frmmerge::cmdRemove_clicked()
{
    unsigned int numItems = lstcollist2->count();
    for(register int itr = (int)numItems - 1; itr >= 0 ; --itr)
    {
	if(lstcollist2->isSelected(itr))
	    lstcollist2->removeItem(itr);
    }
    lstcollist2->clearSelection();
}

void frmmerge::cmdMore_clicked()
{
    QStringList fldList;
    
    unsigned int numItems = lstcollist3->count();
    for(register int itr = 0; itr < (int)numItems; ++itr)
	fldList.append(lstcollist3->text(itr));
    fldList.append(cmbtable1->currentText() + "." + cmbfield1->currentText() + " = " + cmbtable2->currentText() + "." + cmbfield2->currentText());
    lstcollist3->clear();
    lstcollist3->insertStringList(fldList,0);
    fldList.clear();
    lstcollist3->clearSelection();
}

void frmmerge::cmdDel_clicked()
{
    unsigned int numItems = lstcollist3->count();
    for(register int itr = (int)numItems - 1; itr >= 0 ; --itr)
    {
	if(lstcollist3->isSelected(itr))
	    lstcollist3->removeItem(itr);
    }
    lstcollist3->clearSelection();
}

void frmmerge::cmbtable1_activated(const QString &currTab)
{
    QStringList colList;
    QSqlCursor *viewCur = new QSqlCursor(currTab,true,sqlDB);	
    viewCur->setMode(0);
    QDataTable *tabView = new QDataTable(viewCur,true,0,"datatable");
    tabView->refresh();
    QHeader *head = tabView->horizontalHeader();
    int headCount = head->count();
    
    cmbfield1->clear();
    //Populate field list boxes[START]
    for(int i = 0; i < headCount; ++i)
	colList.append(head->label(i));
    cmbfield1->insertStringList(colList);
    //Populate field list boxes[STOP]
    tabView->close();
    viewCur->clear();
    delete tabView;
    tabView = 0;
    delete viewCur;
    viewCur = 0;
}

void frmmerge::cmbtable2_activated(const QString &currTab)
{
    QStringList colList;
    QSqlCursor *viewCur = new QSqlCursor(currTab,true,sqlDB);	
    viewCur->setMode(0);
    QDataTable *tabView = new QDataTable(viewCur,true,0,"datatable");
    tabView->refresh();
    QHeader *head = tabView->horizontalHeader();
    int headCount = head->count();
    
    cmbfield2->clear();
    //Populate field list boxes[START]
    for(int i = 0; i < headCount; ++i)
	colList.append(head->label(i));
    cmbfield2->insertStringList(colList);
    //Populate field list boxes[STOP]
    tabView->close();
    viewCur->clear();
    delete tabView;
    tabView = 0;
    delete viewCur;
    viewCur = 0;
}

void frmmerge::prepareQuery(QString &tbName, QString &query)
{
    QString subQuery1 = "";
    QString subQuery2 = "";
    QString subQuery3 = "";
    QString tableName = "";
    
    //Create field list[START]
    int numItems = (int)lstcollist2->count();
    for(register int itr = 0; itr < (int)numItems; ++itr)
    {
	if(itr < numItems - 1)
	    subQuery1 += lstcollist2->text(itr) + ",";
	else
	    subQuery1 += lstcollist2->text(itr);
	//creating a list of table names[START]
	tableName = (lstcollist2->text(itr)).section('.',0,0);
	if(subQuery2.contains(tableName) == 0) 
	{
	    if(subQuery2.isEmpty())
		subQuery2 = tableName;
	    else
		subQuery2 += "," + tableName;
	}
	//creating a list of table names[STOP]
    }
    //Create field list[STOP]
    //Create matching column's list[START]
    numItems = lstcollist3->count();
    for(register int itr = 0; itr < (int)numItems; ++itr)
    {
	if(itr < numItems - 1)
	    subQuery3 = lstcollist3->text(itr) + " AND ";
	else
	    subQuery3 += lstcollist3->text(itr);
    }
    //Create matching column's list[STOP]
    query = tr("create table ") + tbName + tr(" as select ") + subQuery1 + tr(" from ") + subQuery2;
    query += tr(" where ") + subQuery3 + ";";
}

bool frmmerge::checkTbNameSyntax(QString &tbName)
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

void frmmerge::progressDialogSettings(QProgressDialog **progressDlg, int totalSteps, QString title, QString label)
{
    (*progressDlg) = new QProgressDialog(0,tr(title),false,WDestructiveClose);
    (*progressDlg)->setCaption(title);
    (*progressDlg)->setTotalSteps(totalSteps);
    lblProgress = new QLabel(*progressDlg,label,WDestructiveClose);
    (*progressDlg)->setLabel(lblProgress);
    (*progressDlg)->setAutoReset(true);
}

void frmmerge::cmbOK_clicked()
{
    bool ok;
    FILE *fpIn;
    QString query = "";
    QProgressDialog *progressDlg = 0;
    int progressStepSize = 1, progress = 0,Loop = 1;
    int fscanVal1 = -1;
    
    QString tbName = QInputDialog::getText("Table merge","Enter table name:",QLineEdit::Normal,QString::null,&ok,this);
    if(ok && !tbName.isEmpty()) 
    {
	if(checkTbNameSyntax(tbName))
	{
	    prepareQuery(tbName,query);
	    //~~Progress dialog settings[START]
	    progressDialogSettings(&progressDlg,numMRows/progressStepSize,tr("Table merge"),tr("Table merge operation in progress"));
	    //~~[rogress dialog settings[STOP]
	    fscanVal = system("mysql " + sqlDB->databaseName() + " -e \"" + query + "\"; > mergestat.txt&");
	    for(int i = 0;;++i) //Creating a busy loop
	    {
		if((fpIn = fopen("mergestat.txt","r")) != NULL)
		{
		    fclose(fpIn);
		    fpIn = NULL;
		    fscanVal1 = system("rm -f mergestat.txt");
		    progressDlg->close();
		    break;
		}
		else
		{
		    if(progress >= numMRows/progressStepSize)
		    {
			progress = 0;
			Loop++;
		    }
		    progressDlg->setProgress(++progress);
		    progressDlg->setLabelText("This is an approximate progress status...\nLoop No: " + QString::number(Loop));
		    progressDlg->show();
		    qApp->processEvents();
		}
	    }
	    
	    if(fscanVal == -1)
		QMessageBox::information(this,tr("Execution failure!"),tr("Failed to execute the query!\nInform the developer."));
	    else
		QMessageBox::information(this,tr("Successful execution"),tr("Merged data is stored in the table: ") + tbName);
	}
    }
    close();
}

void frmmerge::cmdCancel_clicked()
{
    close();
}
