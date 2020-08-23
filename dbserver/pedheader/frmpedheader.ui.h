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
#include <qstring.h>
#include <iostream.h>
#include <qsqldatabase.h>
#include <qsqlselectcursor.h>

void frmpedheader::init()
{
    statFlag = false;
}

void frmpedheader::initSettings(QSqlDatabase *db)
{
    int counter = 0;
    
    sqlDB = db;
    for(QStringList::Iterator it = tableList.begin(); it != tableList.end(); ++it)
    {
	if(*it != tr("reserveWords") && *it != tr("standardtypes") && *it != tr("pk_fk_relations") && *it != tr("db_relations") && ((*it).contains("_view")) == 0)
	    cmbTables->insertItem(*it,counter++);
    }
}

void frmpedheader::cmdOK_clicked()
{
    refTable = cmbTables->currentText();
    refField = cmbFieldList->currentText();
    statFlag = true;
    accept();
}

void frmpedheader::cmdCancel_clicked()
{
    statFlag = false;
    accept();
}

void frmpedheader::cmbTables_activated( const QString &tableName)
{
    QStringList fieldList;
    QSqlSelectCursor* cur = new QSqlSelectCursor(tr("select * from ") + tableName);
    int numFields = (int)cur->count();
    
    for(int i = 0; i < numFields; ++i)
	fieldList.append(cur->fieldName(i));
    delete cur;
    cur = 0;
    cmbFieldList->clear();
    cmbFieldList->insertStringList(fieldList);
}
