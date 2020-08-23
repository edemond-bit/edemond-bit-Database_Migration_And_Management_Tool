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
#include <qstringlist.h>
#include <qsqldatabase.h>
#include <qsqlcursor.h>
#include <qsqlfield.h>
#include <qsqlindex.h>
#include <qprogressdialog.h>
#include <iostream.h>
#include <qmessagebox.h>

void frmrelationestablishment::init()
{
    QStringList colList;
    sqlDB = 0;
    table1->setNumCols(3);
    colList.append("\'Foreign key\' Table");
    colList.append("\'References\' Table");
    colList.append("Status");
    table1->setColumnLabels(colList);
    cmdrelation->setEnabled(false);
}

void frmrelationestablishment::initSettings(QString wDir)
{
    QStringList dbNameList;
    QString dbName = "";
		     
    workDir = wDir;
    tabIconPath = workDir + tr("/images/datafile.png");
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
	cmbdataset->insertStringList(dbNameList,0);	
	cmbdataset_activated(cmbdataset->currentText());
    }
    setUnsetchkComplete();
}

void frmrelationestablishment::setUnsetchkComplete()
{
    if(isRelationExist())
	chkComplete->setChecked(true);
    else
	chkComplete->setChecked(false);
}

bool frmrelationestablishment::isRelationExist()
{
    bool retStat = false,ok = false;
    
    QSqlCursor curr(tr("db_relations"),true,sqlDB);
    if(curr.select())
    {
	if(curr.next())
	{
	    if((curr.value(0)).toInt(&ok) == 1)
		retStat = true;
	}
    }
    return retStat;
}

void frmrelationestablishment::cmbdataset_activated(const QString &dbName)
{
    int numTabs = 0;
    
    removePreviousStatus();
    if(sqlDB != 0 && sqlDB->isOpen())
    {
	sqlDB->close();
	sqlDB = 0;
    }
    
    if(createConnections(dbName))
    {
	QStringList tableList, tList = sqlDB->tables();
	for(QStringList::iterator it = tList.begin(); it != tList.end(); ++it)
	{
	    if(*it != tr("reserveWords") && *it != tr("standardtypes") && *it != tr("pk_fk_relations") && *it != tr("db_relations"))
	    {
		tableList.append(*it);
		++numTabs;
	    }
	}
	cmbprimarytable->clear();
	cmbprimarytable->insertStringList(tableList,0);
	cmbtargettable->clear();
	cmbtargettable->insertStringList(tableList,0);
	table1->setNumRows(calculateNumRows(numTabs));
	load_existed_relation();
	setUnsetchkComplete();
    }
}

void frmrelationestablishment::removePreviousStatus()
{
    int numRows = table1->numRows();
    for(register int i = 0; i < numRows; ++i)
	table1->removeRow(i);
    row = col = 0;
    cmbprimarytableval->clear();
    cmbtargettableval->clear();
}

int frmrelationestablishment::calculateNumRows(int &numTabs)
{
    return fact(numTabs)/(2*fact(numTabs - 2));
}

int frmrelationestablishment::fact(int n)
{
    if(n <= 1)
	return 1;
    else
	return n*fact(n - 1);
}

bool frmrelationestablishment::createConnections(QString dbName)
{
    //Create the QMYSQL3 Database Connection QPSQL7
    if(QSqlDatabase::isDriverAvailable("QMYSQL3"))
    {
	sqlDB = QSqlDatabase::addDatabase( "QMYSQL3" );
	if(!sqlDB) 
	{
	    qWarning( "Failed to connect to driver" );
	    return false;
	}
	else
	{	
	    sqlDB->setDatabaseName(dbName);
	    sqlDB->setUserName( "root" ); // QMYSQL3
	    sqlDB->setPassword( "" ); // QMYSQL3
	    sqlDB->setHostName( "localhost" );
	    if(!sqlDB->open()) 
	    {
		qWarning("Failed to open population Genetics database: " + sqlDB->lastError().driverText());
		qWarning( sqlDB->lastError().databaseText());
		return false;
	    }
	    else
		return true;
	}
    }
    else
	return false;
}

void frmrelationestablishment::load_existed_relation()
{
    QSqlCursor cur(tr("pk_fk_relations"),true,sqlDB);
    if(cur.select())
    {
	while(cur.next())
	{
	    table1->setText(row,col,(cur.value(col+1)).toString());
	    table1->setText(row,col+1,(cur.value(col+2)).toString());
	    table1->setText(row,col+2,tr("exist"));
	    ++row;
	}
    }
}

void frmrelationestablishment::cmbprimarytable_activated(const QString &tabName)
{
    cmbprimarytableval->clear();
    if(cmbtargettable->currentText() != tabName)
    {
	if(checkForSameSet(tabName))
	{
	    QStringList colLabels;
	    QSqlCursor cur(tabName,true,sqlDB);
	    for(register int i = 0; i < (int)cur.count(); ++i)
		colLabels.append(cur.fieldName(i));
	    cmbprimarytableval->insertStringList(colLabels,0);
	    cmdrelation->setEnabled(true);
	}
	else
	    QMessageBox::information(this,tr("Exist!"),"Primary Key --> Foreign Key relationship already exist!");
    }	
}

void frmrelationestablishment::cmbtargettable_activated( const QString &tabName)
{
    QStringList colLabels;
    cmbtargettableval->clear();
    QSqlCursor cur(tabName,true,sqlDB);
    for(register int i = 0; i < (int)cur.count(); ++i)
	colLabels.append(cur.fieldName(i));
    cmbtargettableval->insertStringList(colLabels,0);
}

bool frmrelationestablishment::checkForSameSet(QString tabName)
{
    bool status = true;
    
    for(register int i = 0; i <= row; i++)
    {
	if((table1->text(i,col) == cmbtargettable->currentText() && table1->text(i,col + 1) == tabName) || (table1->text(i,col) == tabName && table1->text(i,col + 1) == cmbtargettable->currentText()))
	{
	    status = false;
	    break;
	}
    }
    return status;
}

void frmrelationestablishment::cmdrelation_clicked()
{
    int status = -1;
    if(check_feasibilityOfRelation())
    {
	QString query = "ALTER TABLE " + cmbtargettable->currentText() + " ADD CONSTRAINT FOREIGN KEY(" + cmbtargettableval->currentText() + ") REFERENCES " + cmbprimarytable->currentText() + "(" + cmbprimarytableval->currentText() + " ON DELETE CASCADE);";
	status = system("mysql " + sqlDB->databaseName() + " -e \"" + query + "\"");
	if(status > -1)
	{
	    //Relation establishment is successful
	    status = -1;
	    query = "INSERT INTO pk_fk_relations VALUES(\'" + QString::number(row + 1) + "\',\'" + cmbtargettable->currentText() + "\',\'" + cmbprimarytable->currentText() + "\');";
	    status = system("mysql " + sqlDB->databaseName() + " -e \"" + query + "\"");
	    if(status > -1)
	    {
		table1->setText(row,col,cmbtargettable->currentText());
		table1->setText(row,col+1,cmbprimarytable->currentText());
		table1->setText(row++,col+2,tr("new"));
		cmdrelation->setEnabled(false);
	    }
	}
	else
	    QMessageBox::information(this,tr("Relation establishment error!"),tr("Relation between given fields can't be established."));
    }
}

bool frmrelationestablishment::check_feasibilityOfRelation()
{
    bool returnVal = false;
    QProgressDialog *progressDlg = 0;
    
    //Open two table using cursor
    QSqlCursor curP(cmbprimarytable->currentText(),true,sqlDB);
    QSqlCursor curF(cmbtargettable->currentText(),true,sqlDB);
    //Check weather field datatype are same
    QSqlField *fldP = curP.field(cmbprimarytableval->currentText());
    QSqlField *fldF = curF.field(cmbtargettableval->currentText());
    if(fldP->type() == fldF->type())
    {
	//field's types are identical
	QSqlIndex indexP = curP.primaryIndex();
	QSqlIndex indexF = curF.primaryIndex();
	if((indexP.name() == tr("PRIMARY")))
	{
	    //Primary table's field is "Pk field" and Target table's field is "Fk field"
	    if(!QMessageBox::question(this, tr("checking required?"),tr("Field types are identical\nvalue checking required\nIt may takes long time depending on number of records"),tr("Yes"),tr("No"),QString::null,0,1))
	    {
		bool status = false;
		int progressStepSize = 0,totalProgress = 0,baseRows = 0,progress = 0;
		curF.select();
		//~~~~~~~~~~ Just for progressStepSize calculation[START]
		totalProgress = curF.size();
		progressStepSize = totalProgress/100;
		progressStepSize = (totalProgress%100 == 0 ? progressStepSize : progressStepSize + 1);
		//~~~~~~~~~~ Just for progressStepSize calculation[END]
		progressDialogSettings(&progressDlg,totalProgress/progressStepSize,tr("Value checking in progress"),"Progress label");
		curF.select();
		while(curF.next())
		{
		    status = false;
		    curP.select(cmbprimarytableval->currentText() + "=" + (curF.value(cmbtargettableval->currentText())).toString());
		    if(curP.next())
		    {
			//cout << (curF.value(cmbtargettableval->currentText())).toString() << endl;
			if(baseRows%progressStepSize == 0)
			{
			    progressDlg->setProgress(++progress);
			    progressDlg->setLabelText(QString::number(baseRows) + " of " + QString::number(totalProgress) + " completed...");
			    progressDlg->show();
			}
			status = true;
		    }
		    if(status == false)
			break;
		    ++baseRows;
		}
		progressDlg->close();
		if(status == false) //indicate there is an fk value which is not in pk field
		{
		    QMessageBox::information(this,tr("Relation establishment error!"),tr("There is value in \'Foreign Key\' field which is not in the field, \'Referenced\'\nSo it is not possible to establish relation between given fields."));
		}
		else //ok relation can be established
		    returnVal = true;
	    }
	    else
		returnVal = false;
	}
	else
	    QMessageBox::information(this,tr("Relation establishment error!"),tr("The field \'Referenced\' must be \'Primary key\' field\nSo it is not possible to establish relation between given fields."));
    }
    else
	QMessageBox::information(this,tr("Relation establishment error!"),tr("\'Foreign key\' field's data type doesn't match with that of the field, \'Referenced\'\nSo it is not possible to establish relation between given fields."));
    return returnVal;
}

void frmrelationestablishment::chkComplete_toggled(bool state)
{
    QString query = "";
    int status = -1;
    
    if(state == true)
    {
	query = "UPDATE db_relations SET status = 1;";
	status = system("mysql " + sqlDB->databaseName() + " -e \"" + query + "\"");
    }
    else
    {
	query = "UPDATE db_relations SET status = '0';";
	status = system("mysql " + sqlDB->databaseName() + " -e \"" + query + "\"");
    }
}

void frmrelationestablishment::progressDialogSettings(QProgressDialog **progressDlg,int totalSteps, QString title, QString label)
{
    QLabel *lblProgress = 0;
    *progressDlg = new QProgressDialog(0,tr(title),true,WDestructiveClose);
    (*progressDlg)->setTotalSteps(totalSteps);
    lblProgress = new QLabel(*progressDlg,label,WDestructiveClose);
    (*progressDlg)->setLabel(lblProgress);
}

void frmrelationestablishment::closeEvent(QCloseEvent *e)
{
    if(sqlDB->isOpen())
    {
	sqlDB->close();
	sqlDB = 0;
    }
    e->accept();
}


void frmrelationestablishment::table1_pressed( int row, int col, int button, const QPoint &mouse)
{
    int fscanVal = -1;
    
    if(button == Qt::RightButton)
    {
	if(!QMessageBox::question(this,tr("Remove?"),tr("Remove row"),tr("&Yes"), tr("&No"),QString::null,0,1))
	{
	    fscanVal = system("mysql " + sqlDB->databaseName() + " -e \"" + "delete from pk_fk_relations where fktable = \'" + table1->text(row,0) + "\' and targettable = \'" + table1->text(row,1) + "\';" + "\"");
	    table1->removeRow(row);
	}
    }
}
