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
//******************************* Version 1.0 ***********************************/
#define MAXVIEWS 1
#define MAXTABLES 50

#include <qapplication.h>
#include <qsessionmanager.h>
#include <qclipboard.h>
#include <qworkspace.h>
#include <qdockwindow.h>
#include <qdockarea.h>
#include <qtoolbar.h>
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <qdatetime.h>
#include <qevent.h>
#include <qvbox.h>
#include <qsizepolicy.h>
#include <qobject.h>
#include <qheader.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qinputdialog.h>
#include <qcombobox.h>
#include <qtoolbutton.h>
#include <qmessagebox.h>
//`````````For Dtabase```````````
#include <qsqldatabase.h>
#include <qsqldriver.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qiconset.h>
#include <qheader.h>
#include <qstringlist.h>
#include <qsqlquery.h>
#include <qstring.h>
#include <qsqlcursor.h>
#include <qsqlfield.h> 
#include <qsqlselectcursor.h>
#include <qsqlrecord.h>
#include <qdatatable.h>
#include <qvariant.h>
#include <qmemarray.h>
#include <qobjectlist.h>
#include <qptrlist.h>
#include <qprogressdialog.h>
#include <qstatusbar.h>
#include "./inputdlg/frminput.h"
#include "./fieldselndlg/dlgfieldseln.h"
#include "./dataanalyzer/frmdataanalyzer.h"
#include "./tabrelation/frmrelationestablishment.h"
#include "./summarydata/summarydata.h"
#include "./tablecreate/frmtablecreate.h"
#include "./fieldnameedit/frmfieldedit.h"
#include "./silentmodestatus/frmsilentmodestatus.h"
#include "./valuechangedlg/valuechangedlg.h"
#include "./threadrun/concurrentrun.h"
#include "./fnwizard/fnwizard.h"
#include "./createheader/createheader.h"
#include "./parallelcopy/parallelcopy.h"
#include "./bkgroundproc/bkgroundproc.h"
#include "./progressdlg/progressdlg.h"
#include "./definetabstructure/definetabstructure.h"
#include "./columnadd/frmcolumnadd.h"
#include "./colmodification/frmcolmodify.h"
#include "./updatetable/frmupdatetable.h"
#include "./findreplace/frmfindreplace.h"
#include "./merge/frmmerge.h"
#include "./datarearrange/frmdatarearrange.h"
#include <qcstring.h>
#include <qvaluelist.h>
#include <qtabwidget.h>

using namespace std;

void frmgeno::init()
{
    QString query;
    
    setUpdatesEnabled(true);
   
    MAXCOLS = 2000;
    MAXTHREAD = 10;
    MAXCPTHREAD = 10;
    
    clearGarbageFiles();
    workDirSetting();    
    menuBarSettings();
    workSpaceSetting();
    databaseViewSetting();
        
    sqlDB = 0;
    activeDatabase = "";
    dataanalyzer = 0;
    tabRel = 0;
    isSingleClick = isDoubleClick = false;
    singleClickItem = doubleClickItem = "";
    //currentRowIndex = currentColIndex = 0;
    
    for(register int i = 0; i < MAXVIEWS; ++i)
    {
	openFlag[i] = false;
	viewCur[i] = 0;
	tabView[i] = 0;
    }

    for(int i = 0; i < MAXTABLES; ++i)
    {
	tabArr[i] = 0;
	tabArrFlag[i] = false;
    }
    tabArrIndex = 0;
    rowO = -1;
    colO = -1;
    rowN = -1;
    colN = -1;
    numRows = -1;
    numCols = -1;
    abortStatus = false; //Responsible to carry abort status
    
    createToolBar_Db();
    createToolBar_Ds();
    createToolBar_Tn();
   
    if(checkIfnotExistCreateUser())
    {
	if(notSufficientMemory())
	{
	    startDatabaseService();
	    startTimer(100);
	}
	else
	{
	    QMessageBox::information(this,tr("Insufficient memory!"),tr("Minimum 512 Mb RAM is required to run this application\nPlease ensure this requirement before running this application"));
	    exit(0);
	}
    }
    else
	exit(0);
}

bool frmgeno::clearGarbageFiles()
{
    fscanVal = system("rm -f .meminfo.txt > /dev/null");
    fscanVal = system("rm -f .tempFile.txt > /dev/null");
    fscanVal = system("rm -f .mysqlsexist.txt > /dev/null");
    fscanVal = system("rm -f .fieldInfo.txt > /dev/null");
    fscanVal = system("rm -f datasource.txt > /dev/null");
    fscanVal = system("rm -f numLines.txt > /dev/null");
    fscanVal = system("rm -f .statusout.txt > /dev/null");
    fscanVal = system("rm -f .trackITR.txt > /dev/null");
    fscanVal = system("rm -f .charsetPlink.txt > /dev/null");
    fscanVal = system("rm -f .charsetPlink2.txt > /dev/null");
    fscanVal = system("rm -f .correctField.txt > /dev/null");
    fscanVal = system("rm -f .crypticsettings.txt > /dev/null");
    fscanVal = system("rm -f tempPlink_Lines.txt > /dev/null");
    fscanVal = system("rm -f .fieldInfo.txt > /dev/null");
    fscanVal = system("rm -f .tempFields.txt > /dev/null");
    fscanVal = system("rm -f .applyChangesToAll.txt > /dev/null");
}

bool frmgeno::notSufficientMemory()
{
    char Mem[12],kB[3];
    int amSize = 0;
    FILE *fp;
    
    //Memory available in the system[START]
    fscanVal = system("cat /proc/meminfo > .meminfo.txt");
    if((fp = fopen(".meminfo.txt","r")) != NULL)
    {
	fscanVal = fscanf(fp,"%s%d%s\n",Mem,&amSize,kB);
	fclose(fp);
	fp = NULL;
    }
    //Memory available in the system[STOP]
    if(amSize >= 512)
	return true;
    else
	return false;
}

void frmgeno::menuBarSettings()
{
    menuGeno = menuBar();
    //Database menu bar[Start]
    databaseGeno = new QPopupMenu(menuGeno);
    
    //``````````````````````````Database Service Start-Stop part````````
    dbServiceStartPath = workDir + "/images/startService.png";
    serviceStartActionDb = new QAction(QIconSet(QPixmap(QImage(dbServiceStartPath))),tr("Star&t service"),tr("SHIFT+T"),this,tr("Start Service"));
    connect(serviceStartActionDb,SIGNAL(activated()),this,SLOT(startDatabaseService()));
    serviceStartActionDb->addTo(databaseGeno);
    
    dbServiceStopPath = workDir + "/images/stopService.png";
    serviceStopActionDb = new QAction(QIconSet(QPixmap(QImage(dbServiceStopPath))),tr("Sto&p service"),tr("SHIFT+P"),this,tr("Stop Service"));
    connect(serviceStopActionDb,SIGNAL(activated()),this,SLOT(stopDatabaseService()));
    serviceStopActionDb->addTo(databaseGeno);
    //``````````````````````````````````````Database Creation Part~~~~~
    dbCreatePath = workDir + "/images/addDb.png";
    createActionDb = new QAction(QIconSet(QPixmap(QImage(dbCreatePath))),tr("Create &database"),tr("SHIFT+D"),this,tr("Create Action"));
    connect(createActionDb,SIGNAL(activated()),this,SLOT(createDatabase()));
    createActionDb->addTo(databaseGeno);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    connectPath = workDir + "/images/refresh.png";
    connectActionDb = new QAction(QIconSet(QPixmap(QImage(connectPath))),tr("&Refresh database connection"),tr("SHIFT+R"),this,tr("Connect Action"));
    connect(connectActionDb,SIGNAL(activated()),this,SLOT(connectionToDatabase()));
    connectActionDb->addTo(databaseGeno);
    
    closePath = workDir + "/images/closeDb.png";
    closeActionDb = new QAction(QIconSet(QPixmap(QImage(closePath))),tr("C&lose Database Connetion"),tr("SHIFT+L"),this,tr("Close Action"));
    connect(closeActionDb,SIGNAL(activated()),this,SLOT(closeAllDatabases()));
    closeActionDb->addTo(databaseGeno);
    closeActionDb->setVisible(false);  
    
    menuGeno->insertItem("&Database",databaseGeno);
    //Database menu bar[Stop]
    //Data Source menu bar setting[Start]
    datasourceGeno = new QPopupMenu(menuGeno);
    
    createPath = workDir + "/images/importData.png";
    createActionGeno = new QAction(QIconSet(QPixmap(QImage(createPath))),tr("&Data Input"),tr("SHIFT+D"),this,tr("Data Input"));
    connect(createActionGeno,SIGNAL(activated()),this,SLOT(tableCreate()));
    createActionGeno->addTo(datasourceGeno);
    
    menuGeno->insertItem("Data so&urce",datasourceGeno);
    //Data Source menu bar setting[Stop]
    //New Table menu Bar Setting[START]
    newGeno = new QPopupMenu(menuGeno);
    
    newPath = workDir + "/images/new.png";
    newActionGeno = new QAction(QIconSet(QPixmap(QImage(newPath))),tr("&New"),tr("SHIFT+N"),this,tr("Edit"));
    connect(newActionGeno,SIGNAL(activated()),this,SLOT(createNewTable()));
    newActionGeno->addTo(newGeno);
    
    menuGeno->insertItem("Ne&w table",newGeno);
    //New Table menu Bar Setting[STOP]
    //Edit menu Bar[START]
    editGeno = new QPopupMenu(menuGeno);
    
    /*cutPath = workDir + "/images/cut.png";
    cutActionGeno = new QAction(QIconSet(QPixmap(QImage(cutPath))),tr("Cu&t"),tr("SHIFT+T"),this,tr("Edit"));
    connect(cutActionGeno,SIGNAL(activated()),this,SLOT(cutTableData()));
    cutActionGeno->addTo(editGeno);
    
    //copyHPath = workDir + "/images/copy.png";
    //copyHActionGeno = new QAction(QIconSet(QPixmap(QImage(copyHPath))),tr("Copy &Header"),tr("SHIFT+H"),this,tr("Edit"));
    //connect(copyHActionGeno,SIGNAL(activated()),this,SLOT(copyHeaderTableData()));
    //copyHActionGeno->addTo(editGeno);
    
    copyPath = workDir + "/images/copy.png";
    copyActionGeno = new QAction(QIconSet(QPixmap(QImage(copyPath))),tr("&Copy"),tr("SHIFT+C"),this,tr("Edit"));
    connect(copyActionGeno,SIGNAL(activated()),this,SLOT(copyTableData()));
    copyActionGeno->addTo(editGeno);
    
    pastePath = workDir + "/images/paste.png";
    pasteActionGeno = new QAction(QIconSet(QPixmap(QImage(pastePath))),tr("&Paste"),tr("SHIFT+P"),this,tr("Edit"));
    connect(pasteActionGeno,SIGNAL(activated()),this,SLOT(pasteTableData()));
    pasteActionGeno->addTo(editGeno);
    
    menuGeno->insertItem("&Edit",editGeno);*/
    //Edit menu bar[STOP]
    //Data definition language setting[START]
    ddlGeno = new QPopupMenu(menuGeno);
    //Columns menu bar setting[Start]
    columnGeno = new QPopupMenu(ddlGeno);
    
    //Table renaming[START]
    ddlGeno->insertItem("Table &rename",this,SLOT(renameOpenTable()),tr("SHIFT+R"),1,1);
    //Table renaming[STOP]
    
    colAddPath = workDir + "/images/colAdd.png";
    colAddActionGeno = new QAction(QIconSet(QPixmap(QImage(colAddPath))),tr("&Add"),tr("SHIFT+A"),this,tr("Column"));
    connect(colAddActionGeno,SIGNAL(activated()),this,SLOT(addColumn()));
    colAddActionGeno->addTo(columnGeno);
    
    colModPath = workDir + "/images/colModify.png";
    colModActionGeno = new QAction(QIconSet(QPixmap(QImage(colModPath))),tr("&Modify"),tr("SHIFT+M"),this,tr("Column"));
    connect(colModActionGeno,SIGNAL(activated()),this,SLOT(modifyColumn()));
    colModActionGeno->addTo(columnGeno);
    
    colDelPath = workDir + "/images/colDel.png";
    colDelActionGeno = new QAction(QIconSet(QPixmap(QImage(colDelPath))),tr("&Drop"),tr("SHIFT+D"),this,tr("Column"));
    connect(colDelActionGeno,SIGNAL(activated()),this,SLOT(dropColumn()));
    colDelActionGeno->addTo(columnGeno);
    
    ddlGeno->insertItem(tr("&Column"),columnGeno);
    //Columns menu bar setting[Stop]
    menuGeno->insertItem(tr("&Data definition"),ddlGeno);
    //Data definition language setting[STOP]
    
    //Data manipulation language setting[START]
    dmlGeno = new QPopupMenu(menuGeno);
    dmlGeno->insertItem(tr("&Insert"),this,SLOT(insertDataInTheExistingTable()),tr("SHIFT+I"),0,0);
    dmlGeno->insertItem(tr("&Update"),this,SLOT(updateDataInTheExistingTable()),tr("SHIFT+U"),1,1);
    dmlGeno->insertItem(tr("&Delete"),this,SLOT(deleteDataInTheExistingTable()),tr("SHIFT+D"),2,2);
    menuGeno->insertItem(tr("&Data manipulation"),dmlGeno);
    //Data manipulation language setting[STOP]
    
    toolsourceGeno = new QPopupMenu(menuGeno);
    
    tabrelatePath = workDir + "/images/Tabrelate.png";
    tabrelateActionGeno = new QAction(QIconSet(QPixmap(QImage(tabrelatePath))),tr("&Relate tables"),tr("SHIFT+R"),this,tr("Relate Table"));
    connect(tabrelateActionGeno,SIGNAL(activated()),this,SLOT(relateTable()));
    tabrelateActionGeno->addTo(toolsourceGeno);
    tabrelateActionGeno->setVisible(true);
    
    datarearrangePath = workDir + "/images/dataRearrange.png";
    datarearrangeActionGeno = new QAction(QIconSet(QPixmap(QImage(datarearrangePath))),tr("&Data re-arrangement"),tr("SHIFT+D"),this,tr("Data re-arrangement"));
    connect(datarearrangeActionGeno,SIGNAL(activated()),this,SLOT(dataRearrange()));
    datarearrangeActionGeno->addTo(toolsourceGeno);
    datarearrangeActionGeno->setVisible(true);
            
    dataAnalyzerPath = workDir + "/images/dataEdit.png";
    dataAnalyzerActionGeno = new QAction(QIconSet(QPixmap(QImage(dataAnalyzerPath))),tr("Data &analyzer"),tr("SHIFT+A"),this,tr("Data Analyzer"));
    connect(dataAnalyzerActionGeno,SIGNAL(activated()),this,SLOT(dataAnalyzer()));
    dataAnalyzerActionGeno->addTo(toolsourceGeno);
    dataAnalyzerActionGeno->setVisible(false);
    
    mergePath = workDir + tr("/images/merge.png");
    mergeActionGeno = new QAction(QIconSet(QPixmap(QImage(mergePath))),tr("&Merge tables"),tr("SHIFT+M"),this,tr("mergetable"));
    connect(mergeActionGeno,SIGNAL(activated()),this,SLOT(mergeTableWizard()));
    mergeActionGeno->addTo(toolsourceGeno);
    mergeActionGeno->setVisible(false);
    
    fnwizardPath = workDir + tr("/images/fnwizard.png");
    fnwizardActionGeno = new QAction(QIconSet(QPixmap(QImage(fnwizardPath))),tr("&Function wizard"),tr("SHIFT+F"),this,tr("fnwizard"));
    connect(fnwizardActionGeno,SIGNAL(activated()),this,SLOT(functionwizard()));
    fnwizardActionGeno->addTo(toolsourceGeno);
    fnwizardActionGeno->setVisible(false);
    
    menuGeno->insertItem(tr("&Tools"),toolsourceGeno);
    //Tools menu bar setting[Stop]
    //Settings menu bar [START]
    settingsGeno = new QPopupMenu(menuGeno);
    settingsGeno->insertItem(tr("Maximum &columns"),this,SLOT(maxcolumnsSetting()),tr("SHIFT+C"),0,0);
    settingsGeno->insertItem(tr("Maximum threads(&Transpose)"),this,SLOT(maxthreadsTSetting()),tr("SHIFT+T"),1,1);
    //createActionGeno->addTo(datasourceGeno);
    menuGeno->insertItem("&Settings",settingsGeno);
    //Settings menu bar [STOP]
    //Help menu bar [SATRT]
    helpGeno = new QPopupMenu(menuGeno);
    helpGeno->insertItem(tr("&About"),this,SLOT(helpAbout()),tr("SHIFT+A"));
    menuGeno->insertItem("&Help",helpGeno);
    //Help menu bar [STOP]
}

void frmgeno::helpAbout()
{
    QMessageBox::information(this,tr("About this application"),tr("\tVersion 1.0\nApplication development platform:\nOS: Fedora Core 5\nLanguage: Qt Designer Version 3.3.5\nDatabase: Mysql Version 5.0"));
}
//Initial mandatory Settings
void frmgeno::centralContainerSetting()
{
    container = new QTabWidget(wGeno,"abc",WDestructiveClose);    
    container->show();
}

void frmgeno::workSpaceSetting()
{
    vb = new QVBox(this);
    vb->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    wGeno = new QWorkspace(vb);
    wGeno->setScrollBarsEnabled(true);
    setCentralWidget(vb);
    vb->show();
}

void frmgeno::workDirSetting()
{
    QString tmpDir;
   
    //Keep track of Current Directory full path
    fscanVal = system("pwd > .tempFile.txt");
    QFile fpRead(".tempFile.txt");
    if(fpRead.open(IO_ReadOnly | IO_Truncate))
    {
	QTextStream fpIn(&fpRead);
	fpIn.setEncoding(QTextStream::UnicodeUTF8);
	workDir = fpIn.readLine();
	fpRead.close();
	fscanVal = system("rm .tempFile.txt");
    }
    //Keep track of Current Directory full path[END]
}	

void frmgeno::maxcolumnsSetting()
{
    bool ok = false;
    int val = QInputDialog::getInteger(tr("Maximum Columns"),tr("Enter Value[max 2000]"),MAXCOLS,1,2000,1,&ok,wGeno,0);
    if(ok)
	MAXCOLS = val;
}

void frmgeno::maxthreadsTSetting()
{
    bool ok = false;
    int val = QInputDialog::getInteger(tr("Maximum number of threads for transpose operation"),tr("Enter Value[max 40]"),10,1,40,1,&ok,wGeno,0);
    if(ok)
	MAXCPTHREAD = val;
}

void frmgeno::createToolBar_Db()
{
    dBar = new QToolBar(this,tr("Database Connection"));
    serviceStartActionDb->addTo(dBar);
    serviceStopActionDb->addTo(dBar);
    dBar->addSeparator();
    createActionDb->addTo(dBar);
    dBar->addSeparator();
    connectActionDb->addTo(dBar);
    closeActionDb->addTo(dBar);    
}

void frmgeno::createToolBar_Ds()
{
    sBar = new QToolBar(this,tr("Data Source"));
    createActionGeno->addTo(sBar);
}

void frmgeno::createToolBar_Tn()
{
    tBar = new QToolBar(this,tr("Tools"));
    tabrelateActionGeno->addTo(tBar);
    dataAnalyzerActionGeno->addTo(tBar);
    mergeActionGeno->addTo(tBar);
    fnwizardActionGeno->addTo(tBar);
}

bool frmgeno::checkIfnotExistCreateUser()
{
    bool flag = false;
    int status = -2;
    QString str = "";
    
    status = system("find /etc/rc.d/init.d -name \"mysqld\" -print > .mysqlsexist.txt");
    QFile fpRead(".mysqlsexist.txt");
    if(fpRead.open(IO_ReadOnly | IO_Truncate))
    {
	QTextStream fpIn(&fpRead);
	fpIn.setEncoding(QTextStream::UnicodeUTF8);
	fpIn >> str;
	if(str.contains("mysqld"))
	{
	    status = system("mysql -e \"select user from mysql.user where user = 'root';\" > \\dev\\null");
	    if(status == -1)
	    {
		status = system("mysql -e \"create user root identified by '';\" > \\dev\\null");
		if(status != -1)
		    cout << "User Created\n";
	    }
	    else
		cout << "User Exist!\n";
	    flag = true;
	}
	else
	{
	    QMessageBox::information(this,tr("mysqld service status"),tr("No mysqld service exist in directory /etc/rc.d/init.d/ !\n Install mysql in your system properly then run this application\n Right now this application will be terminated"));
	    flag = false;
	}
	fpRead.close();
	status = system("rm -f .mysqlsexist.txt");
    }
    return flag;
}

void frmgeno::startDatabaseService()
{
    fscanVal = system(tr("/sbin/service mysqld start"));
    connectionToDatabase();
}

void frmgeno::stopDatabaseService()
{
    closeAllDatabases();
    fscanVal = system(tr("/sbin/service mysqld stop"));
    connectionToDatabase();
}

void frmgeno::createDatabase()
{
    bool ok;
    QString query,read_dbName;
    int status;
    
    QString dbName = QInputDialog::getText("Create database","Enter database name:",QLineEdit::Normal,QString::null,&ok,this);
    if(ok && !dbName.isEmpty()) 
    {
	if(checkDbNameSyntax(dbName))
	{
	    QFile fpread(".dbCreated.txt");
	    if(fpread.open(IO_ReadOnly | IO_Truncate))
	    {
		QTextStream fpred(&fpread);
		fpred.setEncoding(QTextStream::UnicodeUTF8);
		while(!fpred.atEnd())
		{
		    fpred >> read_dbName;
		    if(read_dbName == dbName)
		    {
			ok = false;
			break;
		    }
		}
		fpread.close();
	    }

	    if(ok)
	    {
		query = "mysql -e \"create database " + dbName + ";\"";
		status = system(query);
		if(status == -1)
		    QMessageBox::information(this,tr("Database creation status"),tr("Failed to create database: ") + dbName + "\n");
		else
		{
		    QMessageBox::information(this,tr("Database creation status"),tr("New database ") + dbName + tr(" created successfully"));
		    activeDatabase = dbName;
		    QFile fpAppend(".dbCreated.txt");
		    if(fpAppend.open(IO_WriteOnly | IO_Append))
		    {
			QTextStream fpApp(&fpAppend);
			fpApp.setEncoding(QTextStream::UnicodeUTF8);
			fpApp << dbName << endl;
			fpAppend.close();
			connectionToDatabase();
			defaultTablesCreation();
		    }
		}
	    }
	    else
		QMessageBox::information(this,tr("Database creation status"),tr("Failed to create database: ") + dbName + tr("\nDatabase with same name already exist!"));
	}
	else
	    QMessageBox::information(this,tr("Database creation status"),tr("Failed to create database: ") + dbName + tr("\nProblem with given name! \nDatabase name can contains only charecter from 'A' to 'Z' case insensitive, \n'0' to '9' and underscore"));
    }
}

bool frmgeno::checkDbNameSyntax(QString &dbName)
{
    bool ok = true;
    int length = dbName.length();
    QChar ch;
	    
    for(register int i = 0; i < length; ++i)
    {
	ch = dbName.at(i);
	if(!((ch >= 'A' && ch <= 'Z') || (ch == '_') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9')))
	{
	    ok = false;
	    break;
	}
    }
    return ok;
}

void frmgeno::databaseViewSetting()
{
    lv = new QListView(wGeno,"Database View",0);
    lv->setGeometry(lv->x(),lv->y(),235,150);
    lv->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    lv->setCaption("Database Viewer");
    //databaseView_geometrySetting();
    connect(lv,SIGNAL(clicked (QListViewItem * , const QPoint & , int )),this,SLOT(singleClickOn_ListViewItem(QListViewItem * , const QPoint &, int )));
    connect(lv,SIGNAL(doubleClicked(QListViewItem *, const QPoint &, int )),this,SLOT(doubleClickOn_ListViewItem(QListViewItem *, const QPoint &, int )));
    connect(lv,SIGNAL(mouseButtonClicked(int ,QListViewItem *, const QPoint &, int )),this,SLOT(clickOn_ListViewItem(int ,QListViewItem *, const QPoint &, int )));
    connect(lv,SIGNAL(expanded(QListViewItem *)),this,SLOT(onListItemExpansion(QListViewItem *)));
    connect(lv,SIGNAL(collapsed(QListViewItem *)),this,SLOT(onListItemCollapse(QListViewItem *)));
    lv->show();
}

//~~~~~~~~~~~~~~~~~~~~~~~~defaultTablesCreation [START]~~~~~~~~~~~~~~~~~~~~~~
void frmgeno::defaultTablesCreation()
{
    standardDataTypes_TableCreation();
    reserveWords_TableCreation();
    relationStat_TableCreation();
    dbRelation_TableCreation();
}

void frmgeno::standardDataTypes_TableCreation()
{
    QStringList tabList;
    bool okStat;
    tabList = sqlDB->tables();
    
    if(sqlDB->isOpen())
    {
	okStat = false;
	for(QStringList::iterator it = tabList.begin(); it != tabList.end(); ++it)
	{
	    if((*it).upper() == tr("standardtypes").upper())
	    {
		okStat = true;
		break;
	    }
	}

	if(okStat == false)
	{
	    QString query = "";
	    query = "create table standardtypes(fieldtype varchar(50), datatype varchar(25), value varchar(50) , constraint pk_fieldtype primary key(fieldtype));";
	    sqlDB->transaction();
	    QSqlQuery sq = sqlDB->exec(query);
	    if(sq.isActive())
	    {
		query = "insert into standardtypes values(\"SEX\",\"ENUM\",\"'Female','Male'\");";
		sqlDB->exec(query);
		query = "insert into standardtypes values(\"GENDER\",\"ENUM\",\"'Female','Male'\");";
		sqlDB->exec(query);
		query = "insert into standardtypes values(\"RELIGION\",\"ENUM\",\"'Muslim','Hindu'\");";
		sqlDB->exec(query);
	    }
	    sqlDB->commit();
	}
    }
}

void frmgeno::reserveWords_TableCreation()
{
    QStringList tabList;
    bool okStat;
    tabList = sqlDB->tables();
    
    if(sqlDB->isOpen())
    {
	okStat = false;
	for(QStringList::iterator it = tabList.begin(); it != tabList.end(); ++it)
	{
	    if((*it).upper() == tr("reserveWords").upper())
	    {
		okStat = true;
		break;
	    }
	}

	if(okStat == false)
	{
	    QString query;
	    query = "create table reserveWords(res_words varchar(50), constraint pk_res_words primary key(res_words));";
	    sqlDB->transaction();
	    QSqlQuery sq = sqlDB->exec(query);
	    if(sq.isActive())
	    {
		query = "mysqlimport --local --silent --ignore-lines=1 -u root " + activeDatabase + " " + QString("reserveWords");
		fscanVal = system(query);
	    }
	    sqlDB->commit();
	}
    }
}

void frmgeno::relationStat_TableCreation()
{
    QStringList tabList;
    bool okStat;
    tabList = sqlDB->tables();
    
    if(sqlDB->isOpen())
    {
	okStat = false;
	for(QStringList::iterator it = tabList.begin(); it != tabList.end(); ++it)
	{
	    if((*it).upper() == tr("pk_fk_relations").upper())
	    {
		okStat = true;
		break;
	    }
	}

	if(okStat == false)
	{
	    QString query;
	    query = "create table pk_fk_relations(id int(2),fktable varchar(50), targettable varchar(50), constraint primary key(id));";
	    sqlDB->transaction();
	    QSqlQuery sq = sqlDB->exec(query);
	    sqlDB->commit();
	}
    }
}

void frmgeno::dbRelation_TableCreation()
{
    QStringList tabList;
    bool okStat;
    tabList = sqlDB->tables();
    
    if(sqlDB->isOpen())
    {
	okStat = false;
	for(QStringList::iterator it = tabList.begin(); it != tabList.end(); ++it)
	{
	    if((*it).upper() == tr("db_relations").upper())
	    {
		okStat = true;
		break;
	    }
	}
	
	if(okStat == false)
	{
	    int status = -1;
	    QString query = "create table db_relations(status char(1));";
	    status = system("mysql " + sqlDB->databaseName() + " -e \"" + query + "\"");
	    if(status > -1)
	    {
		query = "INSERT INTO db_relations VALUES('0');";
		status = system("mysql " + sqlDB->databaseName() + " -e \"" + query + "\"");
	    }
	}
    }
}
//~~~~~~~~~~~~~~~~~~~~~~~~defaultTablesCreation [STOP]~~~~~~~~~~~~~~~~~~~~~~~~

void frmgeno::showTheTableInOurApp(QString tableName)
{
    if(sqlDB->isOpen())
    {
	QString query = "",name = "";
	int viewIndex = 0;
	
	closeCurrentOpenTable(viewIndex);
	viewCur[viewIndex] = new QSqlCursor(tableName,true,sqlDB);
	name = "DataTable_" + QString::number(viewIndex);
	tabView[viewIndex] = new QDataTable(viewCur[viewIndex],true,wGeno,name);
	//connect(tabView[viewIndex],SIGNAL(pressed(int ,int ,int ,const QPoint &)),this,SLOT(dataTablePressed(int ,int ,int ,const QPoint &)));
	tabView[viewIndex]->setCaption(tableName);
	tabView[viewIndex]->setSelectionMode(QTable::Multi);
	tabView[viewIndex]->setAutoEdit(true);
	viewCur[viewIndex]->setMode(7);
	tabView[viewIndex]->refresh();
	tabView[viewIndex]->show();
	openFlag[viewIndex] = true;
    }
}

/*void frmgeno::dataTablePressed(int row,int col,int button,const QPoint &p)
{
    if(button == Qt::LeftButton)
    {
	currentRow = row;
	currentColumn = col;
	tableType = false; //Database Table
    }
}*/

void frmgeno::closeCurrentOpenTable(int viewIndex)
{
    if(openFlag[viewIndex] == true)
    {
	tabView[viewIndex]->close();
	viewCur[viewIndex]->clear();
	delete tabView[viewIndex];
	tabView[viewIndex] = 0;
	delete viewCur[viewIndex];
	viewCur[viewIndex] = 0;
	openFlag[viewIndex] = false;
    }
}

void frmgeno::destroyDataTable()
{
    delete curs;
    delete table;
}

void frmgeno::dropTheTableorViewFromDatabase(QString tableName)
{
    QString str;
    int retStat = -1;
    
    if(tableName.contains("view") > 0)
    {
	str = "Do you want to drop the view " + tableName + "on table " + tableName.section('_',0,0) + " from the Database ?";
	if(!QMessageBox::question(this, tr("View deletion from the database"),tr(str),tr("Drop view"),tr("Leave"),QString::null,0,1))
	{
	    str = "DROP TABLE " + tableName;
	    QSqlQuery sq = sqlDB->exec(str);
	    if(sq.isActive())
	    {
		activeDatabase = sqlDB->databaseName();
		connectionToDatabase();
		QMessageBox::information(this,"Successful query","View dropped successfully");
	    }
	    else
		QMessageBox::information(this,"Unsuccessful query","View drop is unsuccessful");
	}
    }
    else
    {
	str = "Do you want to drop the table " + tableName + " from the Database ?";
	if(!QMessageBox::question(this, tr("Table deletion from the database"),tr(str),tr("Drop table"),tr("Leave"),QString::null,0,1))
	{
	    if(relationWithOtherExist(tableName))
	    {
		retStat = QMessageBox::question(this, tr("Dependency alert!"),tr("This table has primary key ~ foreign key relationship with other tables.\nMake your decision carefully"),tr("Drop this table only"),tr("Drop all dependencies also"),tr("Leave"),0,2);
		if(retStat == 0)
		    dropTheTableOnly(tableName);
		else if(retStat == 1)
		    dropTheTableANDdependents(tableName);
	    }
	    else
		dropTheTableOnly(tableName);
	}
    }
}

bool frmgeno::relationWithOtherExist(QString tableName)
{
    bool retStat = false;
    
    QSqlCursor cascadeCur(tr("pk_fk_relations"),true,sqlDB);
    if(cascadeCur.select())
    {
	while(cascadeCur.next())
	{
	    cout << (cascadeCur.value(1)).toString() << endl;
	    if((cascadeCur.value(1)).toString() == tableName || (cascadeCur.value(2)).toString() == tableName)
	    {
		retStat = true;
		break;
	    }
	}
    }    
    return retStat;
}

void frmgeno::dropTheTableOnly(QString tableName)
{
    fscanVal = system("mysql " + sqlDB->databaseName() + " -e \"" + "delete from pk_fk_relations where fktable = \'" + tableName + "\';" + "\"");
    fscanVal = system("mysql " + sqlDB->databaseName() + " -e \"" + "delete from pk_fk_relations where targettable = \'" + tableName + "\';" + "\"");

    QSqlQuery sq = sqlDB->exec("drop table " + tableName + ";");
    if(sq.isActive())
    {
	activeDatabase = sqlDB->databaseName();
	connectionToDatabase();
	QMessageBox::information(this,"Successful query","Table dropped successfully");
    }
    else
	QMessageBox::information(this,"Unsuccessful query","Table drop is unsuccessful");
}

void frmgeno::dropTheTableANDdependents(QString tableName)
{
    QString query = "";
    QStringList tableList,tempList,cascadeList;
    
    QSqlQuery sq = sqlDB->exec("drop table " + tableName + ";");
    if(sq.isActive())
    {
	tableList.append(tableName);
	QSqlCursor cascadeCur(tr("pk_fk_relations"),true,sqlDB);
	while(1)
	{
	    for(QStringList::Iterator it = tableList.begin(); it != tableList.end(); ++it)
	    {
		if(cascadeCur.select())
		{
		    while(cascadeCur.next())
		    {
			if((cascadeCur.value(2)).toString() == *it)
			{
			    fscanVal = system("mysql " + sqlDB->databaseName() + " -e \"" + "drop table " + (cascadeCur.value(1)).toString() + "\";");
			    tempList.append((cascadeCur.value(1)).toString());
			    cascadeList.append("'" + (cascadeCur.value(1)).toString() + "'");
			}
		    }
		}
	    }

	    if(tempList.isEmpty())
		break;
	    else
	    {
		tableList = tempList;
		tempList.clear();
	    }
	}
	cascadeList.prepend("'" + tableName + "'");
	
	if(!cascadeList.isEmpty())
	{
	    for(QStringList::iterator it = cascadeList.begin(); it != cascadeList.end(); ++it)
	    {
		fscanVal = system("mysql " + sqlDB->databaseName() + " -e \"" + "delete from pk_fk_relations where fktable = \'" + (*it).remove("'") + "\';" + "\"");
		fscanVal = system("mysql " + sqlDB->databaseName() + " -e \"" + "delete from pk_fk_relations where targettable = \'" + (*it).remove("'") + "\';" + "\"");
	    }
	    QMessageBox::information(this,tr("Cascading table drop"),"'" + tableName + "' dropped successfully.\nCascading drop is executed\nfor the following tables: \n" + cascadeList.join(tr("\n")));
	}
    }
}

void frmgeno::databaseView_geometrySetting()
{
    const QRect rect_vBox = vb->geometry();
    int lv_x,lv_y,lv_width,lv_height;
    
    lv_x = rect_vBox.x() + 5;
    lv_y = rect_vBox.y() + 5;
    lv_width = rect_vBox.width() - (int)round(double(rect_vBox.width())/5.0);
    lv_height = rect_vBox.height() - 5;
    
    const QRect rect_lv(lv_x,lv_y,lv_width,25);
    lv->setGeometry(rect_lv);
    lv->showNormal();
}

//Database Connection Settings
void frmgeno::connectionToDatabase()
{
    QString dbName;
    int counter = 0;
    logoPath = workDir + "/images/mysql_logo.png";
    acPath = workDir + "/images/userS.png";
        
    if(lv->isHidden())
	lv->show();
    closeAllDatabases();
    fscanVal = lv->addColumn(QIconSet(QPixmap(QImage(logoPath))),"");
    fscanVal = lv->addColumn("Status");
    lv->setSelectionMode( QListView::Single );
    lv->setTreeStepSize(20);
    lv->setSelectionMode(QListView::Extended);
    //Adding User status in the list View
    QPixmap ac(acPath,"PNG");
    dRoot = new QListViewItem(lv,"ppm");
    dRoot->setExpandable(true);
    dRoot->setPixmap(0,ac);
     
    activeDbList.clear();
    QFile fp(".dbCreated.txt");
    if(fp.open(IO_ReadOnly | IO_Truncate))
    {
	QTextStream fpIn(&fp);
	fpIn.setEncoding(QTextStream::UnicodeUTF8 );
	while(!fpIn.atEnd())
	{
	    dbName = fpIn.readLine();
	    if(createConnections(dbName))
	    {
		activeDbList.append(dbName);
		dRoot->setText(1,"Activated");
		dRoot->widthChanged( 0 );
		dRoot->invalidateHeight();
		dRoot->setOpen(true);
		
		if(counter == 0)
		{
		    dbPath = workDir + "/images/popDb.png";
		    QPixmap popDb(dbPath,"PNG");
		    popDbase = new QListViewItem(dRoot,"Population Database");
		    popDbase->setExpandable(true);
		    popDbase->setPixmap(0,popDb);
		    popDbase->setText(1,"Open");
		    popDbase->widthChanged( 0 );
		    popDbase->invalidateHeight();
		    popDbase->setOpen(true);
		}
		
		popPath = workDir + "/images/sDatabase.png";
		//Adding Database Status in the list view
		QPixmap pop(popPath,"PNG");
		dbase = new QListViewItem(popDbase,dbName);
		dbase->setExpandable(false);
		dbase->setPixmap(0,pop);
		dbase->setText(1,"Close");
		dbase->widthChanged( 0 );
		dbase->invalidateHeight();
		dbase->setOpen(false);
		if(!(sqlDB->tables()).empty())
		    addDatabaseStatus();
		closeDatabase(); //This is done just for checking whether database connection is ok/not!
	    }
	    else
	    {
		dRoot->setText(1,"De activated");
		dRoot->setup();
		dRoot->widthChanged( 0 );
		dRoot->invalidateHeight();
		dRoot->setOpen(false);
		dRoot->repaint();
	    }
	    ++counter;
	}
	fp.close();
	if(counter > 0 && !activeDatabase.isEmpty())
	    createConnections(activeDatabase);
    }
}

void frmgeno::addDatabaseStatus()
{
    tablePath = workDir + "/images/table1.png";
    viewPath = workDir + "/images/view1.png";
    
    QStringList tabList = sqlDB->tables(QSql::Tables);
    dbase->setExpandable(true);
    
    for(QStringList::Iterator it = tabList.begin(); it != tabList.end(); ++it)
    {
	if(*it != tr("reserveWords") && *it != tr("standardtypes") && *it != tr("pk_fk_relations") && *it != tr("db_relations"))
	{
	    //Adding Database Tables Status in the list view
	    QListViewItem *item = new QListViewItem(dbase,*it);
	    item->setExpandable(false);
	    if((*it).contains("_view"))
	    {
		QPixmap pop(viewPath,"PNG");
		item->setPixmap(0,pop);
	    }
	    else
	    {
		QPixmap pop(tablePath,"PNG");
		item->setPixmap(0,pop);
	    }
	    item->setText(1,"");
	    item->setup();
	    item->widthChanged( 0 );
	    item->invalidateHeight();
	    item->setOpen(true);
	    item->repaint();
	}
    }
}	

bool frmgeno::createConnections(QString dbName)
{
    // Create the QMYSQL3 Database Connection QPSQL7
    if(QSqlDatabase::isDriverAvailable("QMYSQL3"))
    {
	sqlDB = QSqlDatabase::addDatabase( "QMYSQL3" );
	//sqlDB = QSqlDatabase::addDatabase( "QPSQL7" );
	if(!sqlDB) 
	{
	    qWarning( "Failed to connect to driver" );
	    //QMessageBox::information(this, "Connection Problem!","Failed to connect to the Database Driver");
	    return false;
	}
	else
	{
	    sqlDB->setDatabaseName(dbName);
	    sqlDB->setUserName( "root" ); // QMYSQL3
	    sqlDB->setPassword( "" ); // QMYSQL3
	    //sqlDB->setUserName( "ppm" ); //QPSQL7
	    //sqlDB->setPassword( "ppm" ); //QPSQL7~ppm:ppm 
	    sqlDB->setHostName( "localhost" );
	    if(!sqlDB->open()) 
	    {
		qWarning("Failed to open population genetics database: " + sqlDB->lastError().driverText());
		//QMessageBox::information(this, "Database Opening Problem!","Failed to open \nPopulation Genetics database: " + sqlDB->lastError().driverText() + "\n" + sqlDB->lastError().databaseText());
		qWarning(sqlDB->lastError().databaseText());
		return false;
	    }
	    else
		return true;
	}
    }
    else
	return false;
}

void frmgeno::singleClickOn_ListViewItem(QListViewItem *item, const QPoint &p, int column)
{
    isDoubleClick = false;
    if(column == 0 && item) //Column '0' contains table or Database name
    {
	QString dbName = item->text(column);
	if(!isClickMadeOnDatabaseItem(dbName))
	{
	    isSingleClick = true;
	    singleClickItem = item->text(column); //Column '0' contains table or Database name
	}
    }
}

void frmgeno::doubleClickOn_ListViewItem(QListViewItem *item, const QPoint &p, int column)
{
    isSingleClick = false;
    if(column == 0 && item) //Column '0' contains table or Database name
    {
	QString dbName = item->text(column);
	if(!isClickMadeOnDatabaseItem(dbName))
	{
	    isDoubleClick = true;
	    doubleClickItem = item->text(column);
	    clickOn_ListViewItem(1,item,p,column);
	}
    }
}

void frmgeno::clickOn_ListViewItem(int button,QListViewItem *item,const QPoint &pos, int column)
{
    bool okStat = false;
    QString dbName = "", query = "", db_Path = "";
    int status = pos.x();
  
    if(column == 0) //Column '0' contains table or Database name
    {
	dbName = item->text(0);
	okStat = isClickMadeOnDatabaseItem(dbName);
	if(okStat == true)
	{
	    if(button == Qt::RightButton)
	    {
		QString str = "Do you want to drop the Database \'" + dbName + "\' ?";
		if(!QMessageBox::question(this, tr("Database Deletion"),tr(str),tr("Drop Dtabase"),tr("Leave"),QString::null,0,1))
		{
		    query = "mysql -e \"drop database " + dbName + ";\"";
		    status = system(query);
		    if(status == -1)
			QMessageBox::information(this,"Query execution Status","Fail to drop Database");
		    else
		    {
			QMessageBox::information(this,"Query execution Status","Database dropped successfully");
			QFile fpRead(".dbCreated.txt");
			QFile fpWrite(".dbCreated_temp.txt");
			if(fpRead.open(IO_ReadOnly | IO_Truncate))
			{
			    if(fpWrite.open(IO_WriteOnly | IO_Truncate))
			    {
				QTextStream fpIn(&fpRead);
				fpIn.setEncoding(QTextStream::UnicodeUTF8 );
				QTextStream fpOut(&fpWrite);
				fpOut.setEncoding(QTextStream::UnicodeUTF8 );
				while(!fpIn.atEnd())
				{
				    db_Path = fpIn.readLine();
				    if(db_Path != dbName)
					fpOut << db_Path << endl;
				}
				fpRead.close();
				fpWrite.close();
				fscanVal = system("rm -f .dbCreated.txt");
				fscanVal = system("rename .dbCreated_temp.txt .dbCreated.txt .dbCreated_temp.txt");
				activeDbList.clear();
			    }
			}
		    }
		    activeDatabase = dbName;
		    connectionToDatabase();
		}
	    }
	    else if(button == Qt::LeftButton)
	    {
		activeDatabase = dbName;
		connectionToDatabase();
	    }
	    dataAnalyzerActionGeno->setVisible(false);
	    fnwizardActionGeno->setVisible(false);
	    mergeActionGeno->setVisible(false);
	    fnwizardActionGeno->setVisible(false);
	}
	else //Click is made on table icon
	{
	    //~~~~~~~Finding database to which table belongs [START]
	    okStat = false;
	    for(QStringList::Iterator it = activeDbList.begin(); it != activeDbList.end(); ++it)
	    {
		if((item->parent())->text(0) == *it)
		{
		    okStat = true;
		    break;
		}
	    }
	    //~~~~~~~Finding database to which table belongs [STOP]
	    if(okStat == true)
	    {
		closeDatabase();
		activeDatabase = (item->parent())->text(0);
		createConnections(activeDatabase);
		QStringList tableList = sqlDB->tables();
		if(button == Qt::LeftButton && column == 0)
		{
		    for(QStringList::Iterator it = tableList.begin(); it != tableList.end(); ++it)
		    {
			if(item->text(column) == *it)
			{
			    if(isSingleClick && singleClickItem == *it)
				; //Just signifies, the table is selected
			    else if(isDoubleClick && doubleClickItem == *it)
				showTheTableInOurApp(*it);
			    isSingleClick = isDoubleClick = false;
			    dataAnalyzerActionGeno->setVisible(true);
			    fnwizardActionGeno->setVisible(true);
			    mergeActionGeno->setVisible(true);
			     fnwizardActionGeno->setVisible(true);
			    break;
			}
		    }
		}
		else if(button == Qt::RightButton && column == 0)
		{
		    dataAnalyzerActionGeno->setVisible(false);
		    fnwizardActionGeno->setVisible(false);
		    mergeActionGeno->setVisible(false);
		    fnwizardActionGeno->setVisible(false);
		    for(QStringList::Iterator it = tableList.begin(); it != tableList.end(); ++it)
		    {
			if(item->text(column) == *it)
			{		
			    dropTheTableorViewFromDatabase(*it);
			    break;
			}
		    }
		}
		okStat = false;
	    }
	}
    }
}

bool frmgeno::isClickMadeOnDatabaseItem(QString dbName)
{
    bool okStat = false;
    
    //~~~~~~~~~~~~~~~~~Checking, if click is made on Database Name[START]
    for(QStringList::Iterator it = activeDbList.begin(); it != activeDbList.end(); ++it)
    {
	if(dbName == *it)
	{
	    okStat = true;
	    break;
	}
    }
    //~~~~~~~~~~~~~~~~~Checking, if click is made on Database Name[STOP]
    return okStat;
}

void frmgeno::onListItemExpansion(QListViewItem *lvItem)
{
    QString currentOpenDB = "", currentOpenTab = "";
    
    for(QStringList::Iterator it = activeDbList.begin(); it != activeDbList.end(); ++it)
    {
	if(*it == lvItem->text(0))
	{
	    lvItem->setText(1,"Open");
	    if(sqlDB != 0 && sqlDB->isOpen())
	    {
		currentOpenDB = sqlDB->databaseName();
		if(tabView[0])
		    currentOpenTab = tabView[0]->caption();
		closeDatabase();
	    }
	    createConnections(lvItem->text(0));
	    defaultTablesCreation();
	    closeDatabase();
	    createConnections(currentOpenDB);
	}
    }
}

void frmgeno::onListItemCollapse(QListViewItem *lvItem)
{
    for(QStringList::Iterator it = activeDbList.begin(); it != activeDbList.end(); ++it)
    {
	if(*it == lvItem->text(0))
	    lvItem->setText(1,"Close");
    }
}

void frmgeno::closeDatabase()
{
    if(sqlDB != 0 && sqlDB->isOpen())
    {
	closeCurrentOpenTable(0);
	sqlDB->close();
	sqlDB = 0;
    }	
}

void frmgeno::closeAllDatabases()
{
    if(lv->isHidden())
	lv->show();
    if(sqlDB != 0 && sqlDB->isOpen())
    {
	sqlDB->close();
	sqlDB = 0;
	lv->clear();
	lv->removeColumn(1);
	lv->removeColumn(0);
    }
    else
    {
	lv->clear();
	lv->removeColumn(1);
	lv->removeColumn(0);
    }
}

void frmgeno::deleteListViewItems(QListViewItemIterator *it)
{
	if(it->current())
	    deleteListViewItems(++it);
	delete it->current();
}
//Database Connection Settings[END]
//Initial mandatory Settings[END]
void frmgeno::tableCreate()
{
    bool ok = false;
    
    //~~~~~~~~~~~~~~~~~~~~~~
    isImport = false;
    indexingRequired = false;
    isSelectField = false;
    isTranspose = false;
    isPlinkformat = false;
    isValueChangeStat = false;
    isillumina = false;
    isAffy = false;
    runInSilentMode = false;
    //~~~~~~~~~~~~~~~~~~~~~~
    frmInput *inp = new frmInput(wGeno,tr("Data Importer"),true,0);
    inp->workDir = workDir;
    if(inp->exec() == QDialog::Accepted)
    {
	if(inp->dlgStatus == true)
	{
	    if(inp->otherFlag && inp->tDirect)
	    {
		tableDirectFlag = true;
		indexingRequired = true;
		isImport = false;
		isSelectField = false;
		isTranspose = false;
		isPlinkformat = false;
		isValueChangeStat = false;
		isillumina = false;
		isAffy = false;
	    }
	    else
	    {
		tableDirectFlag = false;
		if(inp->otherFlag)
		{
		    isImport = inp->importStat;
		    indexingRequired = inp->indexreqStat;
		    isSelectField = inp->selectedFieldStat;
		    isTranspose = inp->transposeStat;
		    isPlinkformat = inp->plinkformatStat;
		    isValueChangeStat = inp->valueChangeStat;
		}
		else if(inp->illuminaFlag)
		{
		    isImport = true;
		    indexingRequired = true;
		    isSelectField = false;
		    isTranspose = false;
		    isPlinkformat = false;
		    isValueChangeStat = false;
		    isillumina = inp->illuminaFlag;
		}
		else if(inp->affyFlag)
		{
		    isImport = true;
		    indexingRequired = true;
		    isSelectField = true;
		    isTranspose = false;
		    isPlinkformat = true;
		    isValueChangeStat = true;
		    isAffy = inp->affyFlag;
		}
		inp->close();
		delete inp;
		inp = NULL;
		
		if(isImport)
		{
		    QString dbName = QInputDialog::getItem(tr("Database selection"),tr("Select database "),activeDbList,0,false,&ok,this,tr("Database name selection"));
		    if(ok && !dbName.isEmpty())
		    {
			activeDatabase = dbName;
			connectionToDatabase();
			extendedCreateTable();
		    }
		}
		else
		    extendedCreateTable();
		isImport = false;
		indexingRequired = false;
		isSelectField = false;
		isTranspose = false;
		isPlinkformat = false;
		isValueChangeStat = false;
		isillumina = false;
		isAffy = false;
	    }
	}
    }
}

//Table section ~~~~~~~~~~~~~~~~~~~~~~~~[START]
void frmgeno::createNewTable()
{
    if(tabArrIndex < 50)
    {
	define_newTableStructure();
	/*QTable *newTab = new QTable(100,20,wGeno);
	connect(newTab,SIGNAL(pressed(int ,int ,int ,const QPoint &)),this,SLOT(normalTablePressed(int ,int ,int ,const QPoint &)));
	newTab->show();
	tabArr[tabArrIndex++] = newTab;*/
    }
    else
	QMessageBox::information(this,tr("Limit reached!"),tr("Maximum '50' tables can be opened at a time!"));
}

void frmgeno::define_newTableStructure()
{
    definetabstructure *deftstruc = new definetabstructure(wGeno,"tabledefinition",WDestructiveClose);
    deftstruc->loadDatabaseList(workDir);
    deftstruc->show();
}

/*void frmgeno::normalTablePressed(int row, int col, int button, const QPoint &p)
{
    if(button == Qt::LeftButton)
    {
	currentRow = row;
	currentColumn = col;
	tableType = true; //Normal Table
    }
}*/

void frmgeno::cutTableData()
{
    
}

/*void frmgeno::copyHeaderTableData()
{
    QHeader *head = tabView[0]->horizontalHeader();
    QTableSelection sel = selectionArea();
    if(!sel.isEmpty())
    {
	QString str = "";
	for(int i = sel.leftCol(); i <= sel.rightCol(); ++i)
	{
	    if(i - sel.leftCol() > 0)
		str += "\t";
	    str += head->label(i);
	}
	str += "\n";
	QApplication::clipboard()->setText(str);
	//////
	if(sel.numRows()*sel.numCols() <= 50000)
	{
	    QString str = "";
	    for (int i = 0; i < sel.numRows(); ++i) 
	    {
		if(i > 0)
		    str += "\n";
		for(int j = 0; j < sel.numCols(); ++j) 
		{
		    if(j > 0)
			str += "\t";
		    str += tabView[0]->text(sel.topRow() + i, sel.leftCol() + j);
		}
	    }
	    QApplication::clipboard()->setText(str);
	}
	else
	    QMessageBox::information(this,tr("Copy failed!"),tr("Maximum '50000' cells can be selected at a time!"));
	    //////
    }
}*/

void frmgeno::copyTableData()
{
    QTableSelection sel = selectionArea();
    if(!sel.isEmpty())
    {
	rowO = sel.topRow();
	colO = sel.leftCol();
	rowN = sel.bottomRow();
	colN = sel.rightCol();
	numRows = sel.numRows();
	numCols = sel.numCols();
	curSourceTab = tabView[0];
    }
}

void frmgeno::pasteTableData()
{
    QTableSelection sel = selectionArea();
    if(!sel.isEmpty())
    {
	int USEABLETHREAD = 0;
	int dest_RowO = sel.topRow();
	int dest_ColO = sel.leftCol();
	
	if(!tabArrFlag[activeTableID])
	{
	    tabArr[activeTableID]->setNumRows(dest_RowO + numRows);
	    tabArr[activeTableID]->setNumCols(dest_ColO + numCols);
	    tabArrFlag[activeTableID] = true;
	}
	else if(tabArrFlag[activeTableID])
	{
	    tabArr[activeTableID]->setNumRows(tabArr[activeTableID]->numRows() >= dest_RowO + numRows ? tabArr[activeTableID]->numRows() : dest_RowO + numRows);
	    tabArr[activeTableID]->setNumCols(tabArr[activeTableID]->numCols() >= dest_ColO + numCols ? tabArr[activeTableID]->numCols() : dest_ColO + numCols);
	}
	
	const QObjectList *objTree = tabArr[activeTableID]->objectTrees();
	cout << objTree->count() << endl;
	QLNode *p = objTree->currentNode();
	cout << (QPtrCollection::Item)p->getData() << endl;
	
	if(numRows < 1000)
	{
	    for(int i = 0; i < numRows; ++i)
	    {
		for(int j = 0; j < numCols; ++j)
		    tabArr[activeTableID]->setText(i + dest_RowO,j + dest_ColO, curSourceTab->text(i + rowO,j + colO));
	    }
	}
	else //thread operation
	{
	    USEABLETHREAD = (numRows < 10000 ? MAXCPTHREAD/2 : MAXCPTHREAD);
	    parllCopy *plCp[USEABLETHREAD];
	    int slotSize = numRows/USEABLETHREAD;
	    int excessPart = numRows%USEABLETHREAD;
	    
	    QTable *t = ++curSourceTab;
	    cout << t->currentRow() << endl;
	    QTable *t1 = (curSourceTab + 5);
	    if(t != t1)
		cout << t1->currentRow() << endl;
	    //exit(0);
	    for(int i = 0; i < USEABLETHREAD; ++i)
	    {
		if(i < USEABLETHREAD - 1)
		    plCp[i] = new parllCopy(curSourceTab + slotSize*i, rowO + slotSize*i, colO, slotSize, numCols, tabArr[activeTableID] + slotSize*i, dest_RowO + slotSize*i, dest_ColO);
		else
		    plCp[i] = new parllCopy(curSourceTab + slotSize*i, rowO + slotSize*i, colO, slotSize + excessPart, numCols, tabArr[activeTableID] + slotSize*i, dest_RowO + slotSize*i, dest_ColO);	
	    }

	    for(int i = 0; i < USEABLETHREAD; ++i)
		plCp[i]->start();
	    
	    for(int i = 0; i < USEABLETHREAD; ++i)
		plCp[i]->wait();
	}
    }
}

QTableSelection frmgeno::selectionArea()
{
    QTableSelection ts = tabView[0]->selection(0);
    if(ts.isEmpty())
	return QTableSelection(tabView[0]->currentRow(), tabView[0]->currentColumn(), tabView[0]->currentRow(), tabView[0]->currentColumn());
    else
	return tabView[0]->selection(0);
}
//EDIT section ~~~~~~~~~~~~~~~~~~~~~~~~[STOP]
//Columns Add, Modification, Del Events[START]
void frmgeno::addColumn()
{
    if(dataanalyzer)
	dataanalyzer->addColumn();
    else if(tabView[0])
    {
	//Prepare column list[START]
	QStringList colList;
	QString query = "";
	
	for(int i = 0; i < tabView[0]->numCols(); ++i)
	    colList.append(viewCur[0]->fieldName(i));
	//Prepare column list[STOP]
	frmcolumnadd *coladd = new frmcolumnadd(0,"colmod",true);
	coladd->initSettings(colList);
	if(coladd->exec() == QDialog::Accepted && coladd->position != -1)
	{
	    query = tr("ALTER TABLE ") + tabView[0]->caption() + tr(" ADD ") + coladd->colName + tr(" ") + coladd->dttype + tr("(") + coladd->Length + tr(")");
	    
	    switch(coladd->position)
	    {
	    case 0:
		query += tr(" FIRST;");
		break;
	    case 1:
		query += tr(" AFTER ") + coladd->selCol + tr(";");
		break;
	    case 2:
		query += tr(";");
	    }
	    coladd->close();
	    delete coladd;
	    coladd = 0;
	    if(executeQueryInDatabase(query))
		showTheTableInOurApp(tabView[0]->caption());
	    else
		QMessageBox::information(this,tr("Execution failure"),tr("Query execution failure!\nInform the developer"),QMessageBox::Ok);
	}
    }
    else
	QMessageBox::information(this,tr("No table"),tr("There is no open table!"),QMessageBox::Ok);
}

void frmgeno::modifyColumn()
{
    if(dataanalyzer)
	dataanalyzer->modifyColumn();
    else if(tabView[0])
    {
	//Prepare column list[START]
	QStringList colList,colList2,dttypeList,lenList;
	QString query = "";
	bool status = true;
	
	for(int i = 0; i < tabView[0]->numCols(); ++i)
	    colList.append(viewCur[0]->fieldName(i));
	query = tr("mysql ") + sqlDB->databaseName() + tr(" -e 'desc ") + tabView[0]->caption() +tr("' > .fieldInfo.txt");
	fscanVal = system(query);
	//Fetch field information [START]
	QString buffer = "";
	QFile fpRead(".fieldInfo.txt");
	if(fpRead.open(IO_ReadOnly | IO_Truncate))
	{
	    QTextStream fpIn(&fpRead);
	    fpIn.setEncoding(QTextStream::UnicodeUTF8);
	    
	    buffer = fpIn.readLine();
	    while(!fpIn.atEnd())
	    {
		buffer = fpIn.readLine();
		QStringList list = QStringList::split("\t",buffer);
		QStringList::iterator it = list.begin();
		++it;
		buffer = *it;
		buffer.remove(")");
		dttypeList.append(buffer.section('(',0,0));
		lenList.append(buffer.section('(',-1));
	    }
	    fpRead.close();
	    
	    frmcolmodify *colmod = new frmcolmodify(0,"colmod",true);
	    colmod->initSettings(colList,dttypeList,lenList);
	    if(colmod->exec() == QDialog::Accepted && colmod->retStat)
	    {
		colList2 = colmod->colNameList; //dttype list is used here to store new column names
		lenList = colmod->lengthList;
		int listLength = (int)colList.count();
		QStringList::iterator it1 = colList.begin();
		QStringList::iterator it2 = colList2.begin();
		QStringList::iterator it3 = dttypeList.begin();
		QStringList::iterator it4 = lenList.begin();
		for(int i = 0; i < listLength; ++i)
		{
		    if(*it1 != *it2)
		    {
			query = tr("ALTER TABLE ") + tabView[0]->caption() + tr(" CHANGE ") + *it1 + tr(" ") + *it2 + tr(" ") + *it3 + tr("(") + *it4 + tr(")");
			if(!executeQueryInDatabase(query))
			{
			    QMessageBox::information(this,tr("Execution failure"),tr("Query execution failure!\nInform the developer"),QMessageBox::Ok);
			    status = false;
			    break;
			}
			it1++;
			it2++;
			it3++;
			it4++;
		    }
		    else
		    {
			query = tr("ALTER TABLE ") + tabView[0]->caption() + tr(" MODIFY ") + *it1 + tr(" ") + *it3 + tr("(") + *it4 + tr(")");
			if(!executeQueryInDatabase(query))
			{
			    QMessageBox::information(this,tr("Execution failure"),tr("Query execution failure!\nInform the developer"),QMessageBox::Ok);
			    status = false;
			    break;
			}
			it1++;
			it2++;
			it3++;
			it4++;
		    }
		}
		
		if(status)
		    showTheTableInOurApp(tabView[0]->caption());
		delete colmod;
		colmod = 0;
	    }
	    fscanVal = system("rm -f .fieldInfo.txt");
	}
	//Fetch field information [STOP]
	//Prepare column list[STOP]
    }
    else
	QMessageBox::information(this,tr("No table"),tr("There is no open table!"),QMessageBox::Ok);
}

void frmgeno::dropColumn()
{
    if(dataanalyzer)
	dataanalyzer->dropColumn();
    else if(tabView[0])
    {
	QStringList dropColList;
	QString pkColumn,query;
	bool status = true;
	
	if(tabView[0]->numCols() > 1)
	{
	    QHeader *h = tabView[0]->horizontalHeader();
	    for(int i = 0; i < tabView[0]->numCols(); ++i) //identify columns to drop
	    {
		if(tabView[0]->isColumnSelected(i))
		    dropColList.append(h->label(i));
	    }
	    
	    pkColumn = h->label(0); //store primary key field name
	    for(QStringList::iterator it = dropColList.begin(); it != dropColList.end(); ++it)
	    {
		query = tr("ALTER TABLE ") + tabView[0]->caption() + tr(" DROP COLUMN ") + *it;
		if(!executeQueryInDatabase(query))
		{
		    QMessageBox::information(this,tr("Execution failure"),tr("'Column drop' query execution failure!\nInform the developer"),QMessageBox::Ok);
		    status = false;
		    break;
		}
		else
		{
		    if(pkColumn == *it) //primary key field selected for dropping
		    {
			QHeader *h1 = tabView[0]->horizontalHeader();
			pkColumn = h1->label(0);
			query = tr("ALTER TABLE ") + tabView[0]->caption() + tr(" ADD PRIMARY KEY(") + pkColumn + tr(")");
			if(!executeQueryInDatabase(query))
			{
			    QMessageBox::information(this,tr("Execution failure"),tr("Query execution failure!\nCan't assign primary key to the table\nInform the developer"),QMessageBox::Ok);
			    status = false;
			    break;
			}
			else
			    cout << "Primary key changed\n";
		    }
		}
	    }
	    
	    if(status)
		showTheTableInOurApp(tabView[0]->caption());
	}
	else
	    QMessageBox::information(this,tr("Single column table"),tr("Only one column exist in the table!\nIf you want to drop the column, drop the table either."),QMessageBox::Ok);
    }
    else
	QMessageBox::information(this,tr("No table"),tr("There is no open table!"),QMessageBox::Ok);
}

void frmgeno::renameOpenTable()
{
    bool ok = false;
    if(dataanalyzer)
	dataanalyzer->renameOpenTable();
    else if(tabView[0])
    {
	QString tbName = QInputDialog::getText("New table name","Enter new name:",QLineEdit::Normal,QString::null,&ok,this);
	if(ok && !tbName.isEmpty()) 
	{
	    if(checkDbNameSyntax(tbName))
	    {
		QString query = tr("RENAME TABLE ") + tabView[0]->caption() + tr(" TO ") + tbName;
		if(executeQueryInDatabase(query))
		{
		    connectionToDatabase();
		    showTheTableInOurApp(tbName);
		}
		else
		    QMessageBox::information(this,tr("Execution failure"),tr("Query execution failure!\nInform the developer"),QMessageBox::Ok);
	    }
	}
    }
    else
	QMessageBox::information(this,tr("No table"),tr("There is no open table!"),QMessageBox::Ok);
}

//Data manipulation language functions[START]
void frmgeno::insertDataInTheExistingTable()
{
    if(tabView[0])
    {
	if(tableDirectFlag)
	{
	    QString fullFilePath = "", fullFilePath1 = "";
	    
	    QFile fpRead("datasource.txt");
	    if(fpRead.open(IO_ReadOnly | IO_Truncate))
	    {
		QTextStream fpIn(&fpRead);
		fpIn.setEncoding(QTextStream::UnicodeUTF8);
		fullFilePath = fpIn.readLine();
		fpRead.close();
		fullFilePath1 = fullFilePath;
		
		if(dataCompatibilityCheckBeforeInsertOK(fullFilePath))
		{
		    if(fullFilePath1.section('/',-1) == (tabView[0]->caption() + tr(".txt")))
			fscanVal = system(tr("rename ") + fullFilePath1 + tr(" ") + fullFilePath1 + tr("1 ") + fullFilePath1);
		    fullFilePath1 = fullFilePath.section('/',0,fullFilePath.contains('/') - 1) + tr("/") + tabView[0]->caption();
		    fscanVal = system(tr("rename ") + fullFilePath + tr(" ") + fullFilePath1 + tr(" ") + fullFilePath);
		    directdataInsertionInTable(fullFilePath1);
		    showTheTableInOurApp(tabView[0]->caption());
		    fscanVal = system(tr("rename ") + fullFilePath1 + tr(".txt1 ") + fullFilePath1 + tr(".txt ") + fullFilePath1 + tr(".txt1"));
		    QMessageBox::information(this,tr("Table insertion"),tr("Insertion operation is successful!"),QMessageBox::Ok);
		}
	    }
	}
    }
    else
	QMessageBox::information(this,tr("No table"),tr("There is no open table!"),QMessageBox::Ok);
}

void frmgeno::updateDataInTheExistingTable()
{
    if(tabView[0])
    {
	if(tableDirectFlag)
	{
	    QString fullFilePath = "", fullFilePath1 = "";
	    
	    QFile fpRead("datasource.txt");
	    if(fpRead.open(IO_ReadOnly | IO_Truncate))
	    {
		QTextStream fpIn(&fpRead);
		fpIn.setEncoding(QTextStream::UnicodeUTF8);
		fullFilePath = fpIn.readLine();
		fpRead.close();
		fullFilePath1 = fullFilePath;
		
		QStringList dttypeList1,lenList1;
		QStringList dttypeList2,lenList2;
		examineInputFileStructure(fullFilePath,dttypeList1,lenList1);
		fetchTableStructure(dttypeList2,lenList2);
		
		if((int)dttypeList2.count() < (int)dttypeList1.count())
		    QMessageBox::information(this,tr("Less columns in the table"),tr("Input file contains ") + QString::number((int)dttypeList1.count()) + tr(" columns \nwhere as the table contains ") + QString::number((int)dttypeList2.count()) + tr(" columns\nYou need to add extra columns in the table : ") + tabView[0]->caption(),QMessageBox::Ok);
		else
		{
		    frmupdatetable *updtTab = new frmupdatetable(0,"",true,0);
		    QHeader *h = tabView[0]->horizontalHeader();
		    QStringList tabh;
		    for(int i = 0; i < tabView[0]->numCols(); ++i)
			tabh.append(h->label(i));
		    updtTab->initSettings(columnList,dttypeList1,lenList1,tabh,dttypeList2,lenList2);
		    if(updtTab->exec() == QDialog::Accepted)
		    {
			if(updtTab->statFlag == true)
			{
			    int *fid,*tid,index,i;
			    //Creating index list for fields in the file from where values will be fetched[START]
			    fid = new int[(int)(updtTab->sourceColList).count()];
			    index = 0;
			    for(QStringList::iterator it1 = (updtTab->sourceColList).begin(); it1 != (updtTab->sourceColList).end(); ++it1)
			    {
				i = 0;
				for(QStringList::iterator it = columnList.begin(); it != columnList.end(); ++it,++i)
				{
				    if(*it == *it1)
				    {
					fid[index] = i;
					++index;
				    }
				}
			    }
			    //Creating index list for fields in the file from where values will be fetched[STOP]
			    //Creating index list for fields in the table where values will be pasted[START]
			    tid = new int[(int)(updtTab->targetColList).count()];
			    index = 0;
			    for(QStringList::iterator it2 = (updtTab->targetColList).begin(); it2 != (updtTab->targetColList).end(); ++it2)
			    {
				i = 0;
				for(QStringList::iterator it = tabh.begin(); it != tabh.end(); ++it,++i)
				{
				    if(*it == *it2)
				    {
					tid[index] = i;
					++index;
				    }
				}
			    }
			    //Creating index list for fields in the table where values will be pasted[STOP]
			    performUpdateOperation(tabView[0]->caption(), fullFilePath, fid, tid);
			    QMessageBox::information(this,tr("Table update "),tr("Update operation is successful!"),QMessageBox::Ok);
			}
			delete updtTab;
			updtTab = 0;
		    }
		}
	    }
	    fscanVal = system("rm -f datasource.txt");
	}
    }
    else
	QMessageBox::information(this,tr("No table"),tr("There is no open table!"),QMessageBox::Ok);
}

bool frmgeno::dataCompatibilityCheckBeforeInsertOK(QString &fullPath)
{
    QString query = "";
    int val1;
    float val2;
    bool ok,flag;
    int tlen,dlen,tlen2,dlen2;
    QStringList dttypeList1,lenList1;
    QStringList dttypeList2,lenList2;
    
    flag = true;
    if(examineInputFileStructure(fullPath,dttypeList1,lenList1))
    {
	fetchTableStructure(dttypeList2,lenList2);
	
	//Now check it with existing table's data structure[START]
	int count = dttypeList1.count();
	QStringList::iterator it1 = dttypeList1.begin();  //Input file
	QStringList::iterator it2 = lenList1.begin();	//Input file
	QStringList::iterator it3 = dttypeList2.begin();  //Table
	QStringList::iterator it4 = lenList2.begin();	//Table
	
	for(int i = 0; i < count; ++i)
	{
	    if(*it1 == *it3)
	    {
		if(*it1 == tr("VARCHAR"))
		{
		    if((int)(*it4).length() < (int)(*it2).length())
		    {
			QMessageBox::information(this,tr("Shorter length"),tr("Length is not matching: \nColumn ") + QString::number(i + 2) + tr(" VARCHAR(") + *it4 + tr(") in the table \n\twith\nColumn ") + QString::number(i + 1) + tr(" VARCHAR(") + *it2 + tr(") in the input file"),QMessageBox::Ok);
			flag = false;
			break;
		    }
		}
		else if(*it1 == tr("INT"))
		{
		    if((int)(*it4).length() < (int)(*it2).length())
		    {
			QMessageBox::information(this,tr("Shorter length"),tr("Length is not matching: \nColumn ") + QString::number(i + 2) + tr(" INT(") + *it4 + tr(") in the table \n\twith\nColumn ") + QString::number(i + 1) + tr(" INT(") + *it2 + tr(") in the input file"),QMessageBox::Ok);
			flag = false;
			break;
		    }
		}
		else if(*it1 == tr("FLOAT"))
		{
		    tlen = (int)((*it4).length() - 1);
		    dlen =  (int)(((*it4).section(',',-1)).length());
		    tlen2 = (int)((*it2).length() - 1);
		    dlen2 =  (int)(((*it2).section(',',-1)).length());
		    
		    if(tlen < tlen2 || dlen < dlen2)
		    {
			QMessageBox::information(this,tr("Shorter length"),tr("Length is not matching: \nColumn ") + QString::number(i + 2) + tr(" FLOAT(") + *it4 + tr(") in the table \n\twith\nColumn ") + QString::number(i + 1) + tr(" FLOAT(") + *it2 + tr(") in the input file"),QMessageBox::Ok);
			flag = false;
			break;
		    }
		}
	    }
	    else
	    {
		QMessageBox::information(this,tr("Data type mismatch"),tr("Data type is not matching: \nColumn ") + QString::number(i + 2) + tr(" ") + *it3 + tr("(") + *it4 + tr(") in the table \n\twith\nColumn ") + QString::number(i + 1) + tr(" ") + *it1 + tr("(") + *it2 + tr(") in the input file"),QMessageBox::Ok);
		flag = false;
	    }
	    ++it1; ++it2; ++it3; ++it4;
	}
	//Now check it with existing table's data structure[STOP]
	if(!flag)
	    fscanVal = system(tr("rm -f ") + fullPath);
	return flag;
    }
    else
	return false;
}

bool frmgeno::examineInputFileStructure(QString &fullPath, QStringList &dttypeList, QStringList &lenList)
{
    int val1;
    float val2;
    bool ok;
    int tlen,dlen,tlen2,dlen2;
    
    baseRows = baseCols = 0;
    if(readNumberOfRows_ColsIn_File(fullPath,1,1,baseRows,baseCols))
    {
	//Calculate suggested data type and length[START]
	dttypeList.clear();
	lenList.clear();
	for(QStringList::iterator it = maxValSmplList[0].begin(); it != maxValSmplList[0].end(); it++)
	{
	    ok = false;
	    val1 = (*it).toInt(&ok);
	    if(ok)
	    {
		dttypeList.append("int");
		lenList.append(QString::number((int)(*it).length()));
	    }
	    else
	    {
		val2 = (*it).toFloat(&ok);
		if(ok)
		{
		    dttypeList.append("float");
		    tlen = (int)((*it).length() - 1);
		    dlen =  (int)(((*it).section('.',-1)).length());
		    lenList.append(QString::number(tlen) + tr(",") + QString::number(dlen));
		}
		else
		{
		    dttypeList.append("varchar");
		    lenList.append(QString::number((int)(*it).length()));
		}
	    }
	}
	//Calculate suggested data type and length[STOP]
	return true;
    }
    else
	return false;
}

void frmgeno::fetchTableStructure(QStringList &dttypeList, QStringList &lenList)
{
    //Fetch data type and length values from table structure definition[START]
    QString buffer = "", query = "";
    QStringList colList;
    query = tr("mysql ") + sqlDB->databaseName() + tr(" -e 'desc ") + tabView[0]->caption() +tr("' > .fieldInfo.txt");
    fscanVal = system(query);
    
    QFile fpRead(".fieldInfo.txt");
    if(fpRead.open(IO_ReadOnly | IO_Truncate))
    {
	QTextStream fpIn(&fpRead);
	fpIn.setEncoding(QTextStream::UnicodeUTF8);
	buffer = fpIn.readLine();
	while(!fpIn.atEnd())
	{
	    buffer = fpIn.readLine();
	    QStringList list = QStringList::split("\t",buffer);
	    QStringList::iterator it = list.begin();
	    colList.append(*it);
	    ++it;
	    buffer = *it;
	    buffer.remove(")");
	    dttypeList.append(buffer.section('(',0,0));
	    lenList.append(buffer.section('(',-1));
	}
	fpRead.close();
	fscanVal = system("rm -f .fieldInfo.txt");
    }
    //Fetch data type and length values from table structure definition[STOP]
}

void frmgeno::performUpdateOperation(QString targetTable, QString fullFilePath, int *fid, int *tid)
{
    QFile fpRead(fullFilePath);
    if(fpRead.open(IO_ReadOnly|IO_Truncate))
    {
	QSqlCursor *cur = 0;
	QSqlRecord *buffer = 0;
	QString rowSet;
	QStringList cellValueList;
	QProgressDialog *progressDlg = 0;
	QString keyValue;
	int progressStepSize = 0, progress = 0;
	int pivot = 0,index = 0;
	
	QTextStream fpIn(&fpRead);
	fpIn.setEncoding(QTextStream::UnicodeUTF8 );
	rowSet = fpIn.readLine(); //Read out the header
	
	cur = new QSqlCursor(targetTable,true,sqlDB);
	cur->setMode( QSqlCursor::Insert | QSqlCursor::Update );
	QSqlIndex pk = cur->primaryIndex();
	buffer = new QSqlRecord();
	//~~~~~~~~~~ Just for progressStepSize calculation[START]
	progressStepSize = baseRows/100;
	progressStepSize = (baseRows%100 == 0 ? progressStepSize : progressStepSize + 1);
	progressStepSize = 1;
	//~~~~~~~~~~ Just for progressStepSize calculation[END]
	progressDialogSettings(&progressDlg,baseRows/progressStepSize,tr("Table update"),tr("Table update in progress"));
	sqlDB->transaction();
	for(int i = 0; i < baseRows; ++i) //baseCols
	{
	    rowSet = fpIn.readLine(); //Read a single row seperated by tab
	    cellValueList.clear();
	    cellValueList = QStringList::split('\t',rowSet);
	    if(i%progressStepSize == 0)
	    {
		progressDlg->setProgress(++progress);
		progressDlg->setLabelText("Updating data from: " + fullFilePath.section('/',-1) + "\n[Completed: " + QString::number(i == 0  ? 1 : i) + " of " + QString::number(baseRows) + " records]");
		progressDlg->show();
		appQ->processEvents();
	    }

	    pivot = 0;
	    index = 1;
	    QStringList::Iterator it = cellValueList.begin();
	    keyValue = (*it).stripWhiteSpace(); ++it;
	    cur->setValue(0,QVariant(keyValue));
	    if(cur->select(pk, pk))
	    {
		if(cur->next()) //Update mode
		{
		    buffer = cur->primeUpdate();
		    for(; it != cellValueList.end(); ++it,++index)
		    {
			if(fid[pivot] == index)
			{
			    if((buffer->value(tid[pivot])).toString() != (*it).stripWhiteSpace())
				buffer->setValue(tid[pivot],(*it).stripWhiteSpace());
			    ++pivot;
			}
		    }
		    if(!cur->update())
			cout << "Update: " << cur->lastError().text() << endl;
		}
		else //Insert mode
		{
		    buffer = cur->primeInsert();
		    buffer->setValue(0,keyValue);
		    for(; it != cellValueList.end(); ++it,++index)
		    {
			if(fid[pivot] == index)
			{
			    buffer->setValue(tid[pivot],(*it).stripWhiteSpace());
			    ++pivot;
			}
		    }
		    if(cur->insert())
			cout << "Insert: " << cur->lastError().text() << endl;
		}
	    }
	}
	progressDlg->close();
	sqlDB->commit();
	cur->clear();
	//delete buffer;
	buffer = 0;
	//delete cur;
	cur = 0;
	fpRead.close();
	showTheTableInOurApp(targetTable);
	fscanVal = system(tr("rm -f ") + fullFilePath);
    }
}

void frmgeno::deleteDataInTheExistingTable()
{
    if(tabView[0])
    {
	int num_Rows = tabView[0]->numRows();
	int num_Cols = tabView[0]->numCols();
	QString rowIDs = "";
	bool pkDelNPermit = false;
	QStringList rowList;
	QStringList colList;
	
	for(int i = 0; i < num_Rows; ++i) //Recording offset and length
	{
	    if(tabView[0]->isRowSelected(i,true))
		rowIDs += tabView[0]->text(i,0) + tr(",");
	    else
	    {
		for(int j = 0; j < num_Cols; ++j) //Check for all columns are selected
		{
		    if(tabView[0]->isSelected(i,j))
		    {
			if(j == 0)
			    pkDelNPermit = true;
			else
			{
			    rowList.append(QString::number(i));
			    colList.append(QString::number(j));
			}
		    }
		}
	    }
	}
	
	rowIDs.truncate(rowIDs.length() - 1);
	if(rowIDs != "")
	    fullDeletion(rowIDs);
	else
	    partialDeletion(rowList,colList);
	
	if(pkDelNPermit)
	    QMessageBox::information(this,tr("Deletion restriction"),tr("You can not delete values from primary key field!\nYou have to select total row for deletion"),QMessageBox::Ok);
	showTheTableInOurApp(tabView[0]->caption());
    }
    else
	QMessageBox::information(this,tr("No table"),tr("There is no open table!"),QMessageBox::Ok);
}

void frmgeno::fullDeletion(QString rowIDs)
{
    QString query = "";
    QHeader *h = tabView[0]->horizontalHeader();
    query = tr("delete from ") + tabView[0]->caption() + tr(" where ") + h->label(0) + tr(" in(") + rowIDs + tr(")");
    
    if(!executeQueryInDatabase(query))
	QMessageBox::information(this,tr("Execution error"),tr("'DELETE' query execution error\nInform developer!"),QMessageBox::Ok);
}

void frmgeno::partialDeletion(QStringList rowList, QStringList colList)
{
    QSqlCursor *cur = 0;
    QSqlRecord *buffer = 0;
    QString keyValue = "";
    bool ok = false;
    int row,col;
    int cellCount = (int)rowList.count();
    QStringList::iterator it1 = rowList.begin();
    QStringList::iterator it2 = colList.begin();
    
    cur = new QSqlCursor(tabView[0]->caption(),true,sqlDB);
    cur->setMode(QSqlCursor::Update);
    QSqlIndex pk = cur->primaryIndex();
    buffer = new QSqlRecord();
    sqlDB->transaction();
    for(; it1 != rowList.end(); ++it1, ++it2)
    {
	row = (*it1).toInt(&ok);
	col = (*it2).toInt(&ok);
	keyValue = tabView[0]->text(row,0);
	cur->setValue(0,QVariant(keyValue));
	if(cur->select(pk, pk))
	{
	    if(cur->next()) //Update mode
	    {
		buffer = cur->primeUpdate();
		buffer->setValue(col,tr("0"));
		if(!cur->update())
		    cout << "Update: " << cur->lastError().text() << endl;
	    }
	}
    }
    sqlDB->commit();
    cur->clear();
    buffer = 0;
    cur = 0;
}
//Data manipulation language functions[STOP]

bool frmgeno::searchForTheName(QString text)
{
    bool stat = false;
    QStringList tabList = sqlDB->tables();
    
    for(QStringList::iterator it = tabList.begin(); it != tabList.end(); ++it)
    {
	if(*it == text)
	{
	    stat = true;
	    break;
	}
    }
    return stat;
}

bool frmgeno::executeQueryInDatabase(QString query)
{
    int status = -1;
    
    QString command = tr("mysql ") + sqlDB->databaseName() + tr(" -e \"") + query + tr("\"");
    status = system(command);
    if(status == -1)
	return false;
    else
	return true;
}

void frmgeno::directdataInsertionInTable(QString fileName)
{
    QString query = "";
    int count = 0;
    int recsafter = 0;
    QString rowSet = "";
    
    query = tr("mysqlimport --local --ignore-lines=1 --lines-terminated-by='\n' -l -u root ") + activeDatabase + " " + QString(fileName);
    fscanVal = system(query);
    
    query = "SELECT * FROM " + tabView[0]->caption() + ";";
    QSqlQuery squery(query,sqlDB);
    if(squery.isActive()) //This portion is to avoid problem with last record insertion
    {
	curs = new QSqlCursor(tabView[0]->caption(),true,sqlDB);
	curs->setMode(7);
	recsafter = squery.size();
	if(squery.seek(recsafter - 1))
	{
	    QSqlIndex pk = curs->primaryIndex();
	    curs->setValue(0,QVariant(squery.value(0).toString()));
	    if(curs->select( pk, pk ))
	    {
		if(curs->next())
		{
		    buffer = new QSqlRecord *[1];
		    buffer[0] = curs->primeUpdate();
		    if((buffer[0]->value(baseCols-1).toString() == "") || (buffer[0]->value(baseCols-1).toString() == NULL))
		    {
			QFile fpRead(fileName);
			if(fpRead.open(IO_ReadOnly | IO_Truncate))
			{
			    QTextStream fpIn(&fpRead);
			    fpIn.setEncoding(QTextStream::UnicodeUTF8 );
			    while(!fpIn.atEnd())
				fpIn >> rowSet;
			    fpRead.close();
			    buffer[0]->setValue(baseCols-1,rowSet.stripWhiteSpace());
			    if(curs->update())
			    {
				if(sqlDB->commit())
				    cout << "\nTransaction Commit\n";
			    }
			    else
				cout << "Update: " << curs->lastError().text() << endl;
			}
		    }
		    delete [] buffer;
		}
	    }
	}
	curs->clear();
	delete curs;
	curs = 0;
    }
    
    query = tr("rm -f ") + fileName;
    fscanVal = system(query);
    if(sqlDB->commit())
	cout << "\nTransaction Commit\n";
}
//Columns Add, Modification, Del Events[STOP]

void frmgeno::dataAnalyzer()
{
    int viewIndex = 0;
    
    if(isRelationExist())
    {
	dataanalyzer = new frmDataAnalyzer(wGeno,tr("Data Analyzer"),WDestructiveClose);
	dataanalyzer->initSettings(workDir,sqlDB,(singleClickItem == "" ? doubleClickItem : singleClickItem));
	dataanalyzer->show();
	if(openFlag[viewIndex] == true) //If table is open, close it.
	    tabView[viewIndex]->close();
    }
    else
	QMessageBox::information(this,tr("Not Allowed!"),tr("First confirm that all possible relations among tables\nin the database has been established"),QMessageBox::Ok);
}

bool frmgeno::isRelationExist()
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

void frmgeno::dataRearrange()
{
    frmdatarearrange *datreg = new frmdatarearrange(wGeno,"datreg",true,WDestructiveClose);
    datreg->initSettings(workDir);
    datreg->show();
}

void frmgeno::relateTable()
{
    if(tabView[0] != 0 && !tabView[0]->isHidden())
	tabView[0]->close();
    closeDatabase();
    tabRel = new frmrelationestablishment(wGeno,tr("Table Relation"),WDestructiveClose);
    tabRel->initSettings(workDir);
    tabRel->show();
}

void frmgeno::mergeTableWizard()
{
    int viewIndex = 0;
    int numTables = 0;
    QStringList tabList,parentList;
    QString dbName;
    
    if(isRelationExist())
    {
	if(numTableSelected(tabList,parentList,numTables) && numTables > 1)
	{
	    QStringList::iterator it = parentList.begin();
	    dbName = *it;
	    frmmerge *merge = new frmmerge(wGeno,tr("tablemerge"),true,WDestructiveClose);
	    merge->initSettings(workDir,sqlDB,tabList,dbName);
	    merge->show();
	    if(openFlag[viewIndex] == true) //If table is open, close it.
		tabView[viewIndex]->close();
	}
	else if(numTables == 1)
	    QMessageBox::information(this,tr("Operation not permitted"),tr("Select at least two tables"),QMessageBox::Ok);
    }
    else
	QMessageBox::information(this,tr("Operation not permitted"),tr("First confirm that all possible relations among tables\nin the database has been established"),QMessageBox::Ok);
}

bool frmgeno::numTableSelected(QStringList &tabList,QStringList &parentList, int &numTables)
{
    bool okStat = false;
    QString dbName = "";
    QListViewItem *lview = lv->firstChild();
    QListViewItemIterator it(lview);
    
    while(it.current())
    {
	if(it.current()->isSelected())
	{
	    okStat = isClickMadeOnDatabaseItem(((QListViewItem *)it.current())->text(0));
	    if(!okStat) //true ~ table name, false ~ database name
	    {
		tabList.append(((QListViewItem *)it.current())->text(0));
		parentList.append((((QListViewItem *)it.current())->parent())->text(0));
	    }
	    else
	    {
		QMessageBox::information(this,tr("Database name!"),tr("Database name can not be selected\nSelect only tables from the same database"),QMessageBox::Ok);
		okStat = true;
		break;
	    }
	}
	++it;
    }
    
    if(!okStat)
    {
	numTables = 0;
	for(QStringList::iterator it1 = parentList.begin(); it1 != parentList.end(); ++it1)
	{
	    //~~~~~~~Finding database to which table belongs [START]
	    okStat = false;
	    for(QStringList::Iterator it2 = activeDbList.begin(); it2 != activeDbList.end(); ++it2)
	    {
		if(*it1 == *it2)
		{
		    //Check whether the table is in same database as previous table checked[START]
		    if(dbName == "")
		    {
			dbName = *it2;
			++numTables;
		    }
		    else
		    {
			if(dbName != *it2)
			{
			    okStat = true;
			    numTables = 0;
			    QMessageBox::information(this,tr("Wrong selection!"),tr("Selected tables are not in the same database\nSelect only tables from the same database"),QMessageBox::Ok);
			    break;
			}
			else
			    ++numTables;
		    }
		    //Check whether the table is in same database as previous table checked[STOP]
		    break;
		}
	    }
	    //~~~~~~~Finding database to which table belongs [STOP]
	    if(okStat)
		break;
	}
    }
    return (okStat == true ? false : true);
}

void frmgeno::functionwizard()
{
    int viewIndex = 0;
    
    if(isRelationExist())
    {
	fnwizard *wizard = new fnwizard(wGeno,tr("fnwizard"),true,WDestructiveClose);
	wizard->initSettings(workDir,sqlDB,(singleClickItem == "" ? doubleClickItem : singleClickItem));
	wizard->show();
	if(openFlag[viewIndex] == true) //If table is open, close it.
	    tabView[viewIndex]->close();
    }
    else
	QMessageBox::information(this,tr("Not Allowed!"),tr("First confirm that all possible relations among tables\nin the database has been established"),QMessageBox::Ok);
}

void frmgeno::datasetSelection()
{
    QString dbName = QInputDialog::getItem(tr("Database selection"),tr("Select database "),activeDbList,0,false);
}

void frmgeno::fieldsCheckup(bool &acceptedFieldFlag)
{
    QString query;
    
    if((int)rejectedFieldList.count() == 0 && (int)acceptedFieldList.count() == 0) //Either interactive mode or no conflicting field/s was not found in previous iteration/s
    {
	//This part replace '  '  with '_'  from field name also check RESERVE WORD/s conflict[START]
	for(QStringList::Iterator it = columnList.begin(); it != columnList.end(); ++it)
	{
	    if((*it).contains(" ") > 0)
		(*it).replace(tr(QString(" ")),tr(QString("_")),false); //Replace '  '  with '_'  from field name
	    //~~~~~~Check weather any field name/s conflict RESERVE WORD/s or not~~~~~
	    query = "select * from reserveWords where res_words='" + (*it).upper() + "';";
	    QSqlQuery sq = sqlDB->exec(query);
	    if(sq.numRowsAffected())
		rejectedFieldList.append(*it);
	}
	//This part replace '  '  with '_'  from field name also check RESERVE WORD/s conflict[STOP]
    }
    else//Silent mode with conflicting field/s was  found in previous iteration/s so just perform field space replacement with '_'
    {
	for(QStringList::Iterator it = columnList.begin(); it != columnList.end(); ++it)
	{
	    if((*it).contains(" ") > 0)
		(*it).replace(tr(QString(" ")),tr(QString("_")),false); //Replace '  '  with '_'  from field name
	}
    }

    if((int)rejectedFieldList.count() > 0 && (int)acceptedFieldList.count() == 0) //For correction of conlictinglicting field name/s, can occure either in interactive mode or in silent mode with no conflicting field/s was not found in previous iteration/s
    {
	acceptedFieldList.clear();
	frmfieldedit *fldedit = new frmfieldedit(wGeno,tr("Field Edit"),true,0);
	fldedit->rejectedFieldList = rejectedFieldList;
	fldedit->sqlDB = sqlDB;
	fldedit->initSettings();
	if(fldedit->exec() == QDialog::Accepted)
	{
	    if(fldedit->dlgStatus == true)
	    {
		acceptedFieldList = fldedit->rejectedFieldList;
		fldedit->rejectedFieldList.clear();
		fldedit->close();
		delete fldedit;
		fldedit = NULL;
		//~~~~~~~~~~~~Now store the edited field names in the list[START]
		QStringList::Iterator it1 = acceptedFieldList.begin();
		for(QStringList::Iterator it2 = rejectedFieldList.begin(); it2 != rejectedFieldList.end(); ++it1,++it2)
		{
		    for(QStringList::Iterator it3 = columnList.begin(); it3 != columnList.end();++it3)
		    {
			if(*it3 == *it2)
			{
			    *it3 = *it1;
			    break;
			}
		    }
		}
		//~~~~~~~~~~~~Now store the edited field names in the list[STOP]
		acceptedFieldFlag = true;
	    }
	    else
		acceptedFieldFlag = false;
	}
    }
    else if((int)rejectedFieldList.count() > 0 && (int)acceptedFieldList.count() > 0) //Occure in case of silent mode with conflicting field/s was  found in previous iteration/s, so just perform update
    {
	//~~~~~~~~~~~~Now store the edited field names in the list[START]
	QStringList::Iterator it1 = acceptedFieldList.begin();
	for(QStringList::Iterator it2 = rejectedFieldList.begin(); it2 != rejectedFieldList.end(); ++it1,++it2)
	{
	    for(QStringList::Iterator it3 = columnList.begin(); it3 != columnList.end();++it3)
	    {
		if(*it3 == *it2)
		{
		    *it3 = *it1;
		    break;
		}
	    }
	}
	//~~~~~~~~~~~~Now store the edited field names in the list[STOP]
	acceptedFieldFlag = true;
    }
}

bool frmgeno::readNumberOfRows_ColsIn_File(QString &fileName,int index,int numLines,int &baseRows,int &baseCols)
{
    QString query = "", scanBuf = "", writeBuf = "";
    QString tempFileName = "", tempFilePath = "", tempFile_Name = "";
    QString temp_ImportFileName = "",importFileName = "";
    QStringList fieldList;
    QProgressDialog *progressDlg = 0;
    bool acceptedFieldFlag = true;
    int progress = 0,totalProgress = 0, progressStepSize = 0;
    FILE *gpIn;
    int index1,index2,length;
    char c = '\t';
    
    //This part only to set progressDialog's max value[START]
    scanBuf = "wc -l " + fileName + " > numLines.txt";
    fscanVal = system(scanBuf);
    gpIn = fopen("numLines.txt","r");
    fscanVal = fscanf(gpIn,"%d %[^\n]",&totalProgress,scanBuf.ascii());
    fclose(gpIn);
    gpIn = NULL;
    fscanVal = system("rm -f numLines.txt");
    //This part only to set progressDialog's max value[STOP]
    baseRows = baseCols = 0;
    needFileSplit = false;
    inFileSet.clear();
    //This portion to read number of base rows and columns are in the file
    columnList.clear();
    tempFileName = fileName;
    tempFilePath = fileName.section('/',0,fileName.contains('/') - 1);
    tempFile_Name = fileName.section('/',-1);
    tempFile_Name.remove('\\'); tempFile_Name.remove(" ");
    temp_ImportFileName = importFileName = tempFilePath + tr("/") + tempFile_Name.remove(".txt") + tr("-import.txt");
    tempFileName.remove("\\");
    temp_ImportFileName.remove("\\");
    
    QFile pathIn(tempFileName);
    QFile pathOut(temp_ImportFileName);
    if(pathIn.open(IO_ReadOnly | IO_Truncate) && pathOut.open(IO_WriteOnly | IO_Truncate))
    {
	QTextStream fpIn(&pathIn);
	QTextStream fpOut(&pathOut);
	baseRows = progress = 0;
	//~~~~~~~~~~ Just for progressStepSize calculation[START]
	progressStepSize = totalProgress/100;
	progressStepSize = (totalProgress%100 == 0 ? progressStepSize : progressStepSize + 1);
	//~~~~~~~~~~ Just for progressStepSize calculation[END]
	progressDialogSettings(&progressDlg,totalProgress/progressStepSize,"File checkup","File checkup in progress");
	while(!fpIn.atEnd())
	{
	    if(baseRows%progressStepSize == 0)
	    {
		progressDlg->setProgress(++progress);
		progressDlg->setLabelText(fileName.section('/',-1) + ": " + QString::number(index) + " of " + QString::number(numLines) + "\nFile checkup operation in Progress...\n" + QString::number(progress*progressStepSize) + " of " + QString::number(totalProgress) + " records processed");
		progressDlg->show();
		appQ->processEvents();
	    }
	    //~~~~~~This do~while loop to ommit comment lines from input data file [START]
	    do
	    {
		scanBuf = fpIn.readLine();
	    }while(scanBuf.contains("#",FALSE) > 0);
	    //~~~~~~This do~while loop to ommit comment lines from input data file [STOP]
	    if(baseRows == 0)
	    {
		columnList = QStringList::split('\t',scanBuf);
		if(indexingRequired)
		{
		    if(runInSilentMode)
			columnList.prepend(indexFld);
		    else
		    {
			int retVal = -1;
			while(1)
			{
			    bool ok = false;
			    indexFld = QInputDialog::getText("Auto index column","Enter auto index column name for the file to import:",QLineEdit::Normal,"AutoIn",&ok,this);
			    if(ok && !indexFld.isEmpty()) 
			    {
				if(checkDbNameSyntax(indexFld))
				{
				    columnList.prepend(indexFld);
				    break;
				}
				else
				    QMessageBox::information(this,tr("Incorrect column name"),tr("Problem with given name! \nColumn name can contains only charecter from 'A' to 'Z' case insensitive, \n'0' to '9' and underscore"));
			    }
			    else
			    {
				if(ok && indexFld.isEmpty())
				    QMessageBox::information(this,tr("Empty input!"),tr("Input is empty! \nGive column name  containing only charecter from 'A' to 'Z' case insensitive, \n'0' to '9' and underscore"));
				else
				{
				    retVal = QMessageBox::question(this, tr("Abort data import"),tr("Do you want to abort import operation?"),tr("&Yes"), tr("&No"),QString::null,2,2);
				    if(!retVal)
					break;
				}
			    }
			}
			if(!retVal)
			    break;
		    }
		}

		if((int)columnList.count() > MAXCOLS + (indexingRequired ? 2 : 1))
		{
		    needFileSplit = true;
		    if(QString(importFileName).contains("-import.txt"))
		    {
			query = "rm -f " + QString(importFileName);
			fscanVal = system(query);
		    }
		    break;
		}
		else
		{
		    fieldsCheckup(acceptedFieldFlag); //Perform space elimination within a field name and changing of field name/s conflict with reservewords
		    if(!acceptedFieldFlag)
			break;
		    else if(acceptedFieldFlag && !needFileSplit)
		    {
			writeBuf = columnList.join("\t");
			baseCols = columnList.count(); // Number of columns in data file
		    }
		}
	    }
	    else
	    {
		fieldList = QStringList::split('\t',scanBuf);
		if(baseCols >  (int)fieldList.count())
		{
		    //This portion of code fills up blank cell with "XX"...[START]
		    length = (int)scanBuf.length();
		    for(register int i = 0; i < length;)
		    {
			index1 = scanBuf.find(c,i,false);
			if(index1 > -1)
			{
			    i = index1 + 1;
			    index2 = scanBuf.find(c,i,false);
			    if(index2 == -1)
			    {
				if(i == length)
				    scanBuf = scanBuf.insert(i,"XX");
				break;
			    }
			    else if(abs(index1 - index2) == 1)
			    {
				scanBuf = scanBuf.insert(index2,"XX");
				length = (int)scanBuf.length();
				i = index2 + 2;
			    }
			    else
				i = index2;
			}
			else
			    break;
		    }
		    //This portion of code fills up blank cell with "XX"...[STOP]
		    fieldList = QStringList::split('\t',scanBuf); //Split field values in list 
		}
		if(indexingRequired) //Prepending auto indexing value
		    fieldList.prepend(QString::number(baseRows));
		writeBuf = fieldList.join("\t");
		//~~~~Below part to prepare a list of maximum length value of each field~~~~[START]
		if(baseRows == 1)
		{
		    maxValSmplList = new QStringList[1];
		    maxValSmplList[0].clear();
		    maxValSmplList[0] = fieldList;
		}
		else
		{
		    QStringList::Iterator it1 = fieldList.begin();
		    QStringList::Iterator it2 = maxValSmplList[0].begin();
		    for( ; it1 != fieldList.end(); ++it1,++it2)
		    {
			if((int)(*it1).length() > (int)(*it2).length())
			    *it2 = *it1;
		    }
		}
		//~~~~Below part to prepare a list of maximum length value of each field~~~~[STOP]
	    }
	    if(acceptedFieldFlag && !needFileSplit)
	    {
		fpOut << writeBuf << endl;
		baseRows++; //Number of rows in data file
	    }
	}
	progressDlg->close();
	--baseRows;
	pathIn.close();
	pathOut.close();
	if(acceptedFieldFlag && !needFileSplit)
	    fileName = importFileName;
    }
    
    if(acceptedFieldFlag)
    {
	if(!tableDirectFlag && needFileSplit)
	{
	    numSplitSets = (int)ceil(double(columnList.count() - (indexingRequired ? 2 : 1))/double(MAXCOLS));
	    return readNumberOfRows_ColsIn_File_NeedFileSplit(fileName,index,numLines,baseRows,baseCols);
	}
	else
	{
	    if(tableDirectFlag && needFileSplit)
	    {
		QMessageBox::information(this,tr("Large file"),tr("The file can not be stored in a single table.\nUse import method"));
		return false;
	    }
	    else
	    {
		if(baseRows > 0 && baseCols > 0)
		    return true;
		else	
		    return false;
	    }
	}
    }
    else
	return false;
}

bool frmgeno::readNumberOfRows_ColsIn_File_NeedFileSplit(QString &fileName,int &index,int &numLines,int &baseRows,int &baseCols)
{
    QString scanBuf = "", pkID1 = "", pkID2 = "";
    QString tempFileName = "",tempFilePath = "",tempFile_Name = "";
    QString temp_ImportFileName = "",importFileName = "";
    QStringList *inFile_Set_Data;
    QFile **pathOut;
    QTextStream **fpOut;
    QProgressDialog *progressDlg = 0;
    int counter = 0,totalProgress = 0,progress = 0,progressStepSize = 0; 
    FILE *gpIn;
    int index1 = 0,index2 = 0,length = 0;
    char c = '\t';
    bool acceptedFieldFlag = true;
        
    //This part only to set progressDialog's max value[START]
    scanBuf = "wc -l " + fileName + " > numLines.txt";
    fscanVal = system(scanBuf);
    gpIn = fopen("numLines.txt","r");
    fscanVal = fscanf(gpIn,"%d %s",&totalProgress,scanBuf.ascii());
    fclose(gpIn);
    gpIn = NULL;
    fscanVal = system("rm -f numLines.txt");
    
    //This part only to set progressDialog's max value[STOP]
    baseRows = baseCols = 0;
    inFileSet.clear();
    //This portion to read number of base rows and columns are in the file
    columnList.clear();
    inFile_Set_Columns = new QStringList[numSplitSets];
    inFile_Set_Data = new QStringList[numSplitSets];
    maxValSmplList = new QStringList[numSplitSets];
    pathOut = new QFile *[numSplitSets];
    fpOut = new QTextStream *[numSplitSets];
    
    tempFileName = fileName;
    tempFilePath = fileName.section('/',0,fileName.contains('/') - 1);
    tempFile_Name = fileName.section('/',-1);
    tempFile_Name.remove('\\'); tempFile_Name.remove(" ");
    for(register int itr = 0; itr < numSplitSets; ++itr)
    {	
	temp_ImportFileName = importFileName = tempFilePath + tr("/") + tempFile_Name.remove(".txt") + tr("_") + QString::number(itr + 1) + tr("-import.txt");
	inFileSet.append(importFileName);
	temp_ImportFileName.remove("\\");
	pathOut[itr] = new QFile(temp_ImportFileName);
	pathOut[itr]->open(IO_WriteOnly | IO_Truncate);
	fpOut[itr] = new QTextStream(pathOut[itr]);
	fpOut[itr]->setEncoding(QTextStream::UnicodeUTF8);
	//pathOut[itr]->close();
	maxValSmplList[itr].clear();
    }
    
    tempFileName.remove("\\");
    QFile pathIn(tempFileName);
    if(pathIn.open(IO_ReadOnly | IO_Truncate))
    {
	QTextStream fpIn(&pathIn);
	fpIn.setEncoding(QTextStream::UnicodeUTF8);
	baseRows = progress = 0;
	//~~~~~~~~~~ Just for progressStepSize calculation[START]
	progressStepSize = totalProgress/100;
	progressStepSize = (totalProgress%100 == 0 ? progressStepSize : progressStepSize + 1);
	//~~~~~~~~~~ Just for progressStepSize calculation[END]
	progressDialogSettings(&progressDlg,totalProgress/progressStepSize,"File splitting[large file]","File is large, splitting operation in progress");
	while(!fpIn.atEnd())
	{
	    if(baseRows%progressStepSize == 0)
	    {
		progressDlg->setProgress(++progress);
		progressDlg->setLabelText(fileName.section('/',-1) + ": " + QString::number(index) + " of " + QString::number(numLines) + "\nFile is large, splitting & checkup operation in Progress...\n" + QString::number(progress*progressStepSize) + " of " + QString::number(totalProgress) + " records processed");
		progressDlg->show();
		appQ->processEvents();
	    }
	    
	    //~~~~~~This do~while loop to ommit comment lines from input data file [START]
	    do
	    {
		scanBuf = fpIn.readLine();
	    }while(scanBuf.contains("#",FALSE) > 0);
	    //~~~~~~This do~while loop to ommit comment lines from input data file [STOP]
	    if(baseRows == 0)
	    {
		for(register int itr = 0; itr < numSplitSets; ++itr)
		    inFile_Set_Columns[itr].clear();
		columnList = QStringList::split('\t',scanBuf);
		if(indexingRequired)
		    columnList.prepend(indexFld);
		fieldsCheckup(acceptedFieldFlag); //Perform space elimination within a field name and changing of field name/s conflict with reservewords
		if(!acceptedFieldFlag)
		    break;
		else
		{
		    length = columnList.count();
		    baseCols = counter = 0;
		    for(QStringList::Iterator it = columnList.begin(); it != columnList.end(); ++it,++baseCols)
		    {
			if(baseCols == 0)
			    pkID1 = *it;
			if(indexingRequired && baseCols == 1)
			    pkID2 = *it;
			inFile_Set_Columns[counter].append(*it);
			if((baseCols == (counter + 1)*MAXCOLS + (indexingRequired ? 1 : 0)) || (baseCols == (int)columnList.count() - 1))
			{
			    if(counter > 0)
			    {
				if(indexingRequired)
				    inFile_Set_Columns[counter].prepend(pkID2);
				inFile_Set_Columns[counter].prepend(pkID1);
			    }
			    *fpOut[counter] << inFile_Set_Columns[counter].join("\t") << endl;
			    counter++;
			}
		    }
		}
	    }
	    else
	    {
		for(register int itr = 0; itr < numSplitSets; ++itr)
		    inFile_Set_Data[itr].clear();
		columnList = QStringList::split('\t',scanBuf);
		if(length > (int)columnList.count())
		{
		    //This portion of code fills up blank cell with "XX"...[START]
		    length = (int)scanBuf.length();
		    for(register int i = 0; i < length;)
		    {
			index1 = scanBuf.find(c,i,false);
			if(index1 > -1)
			{
			    i = index1 + 1;
			    index2 = scanBuf.find(c,i,false);
			    if(index2 == -1)
			    {
				if(i == length)
				    scanBuf = scanBuf.insert(i,"XX");
				break;
			    }
			    else if(abs(index1 - index2) == 1)
			    {
				scanBuf = scanBuf.insert(index2,"XX");
				length = (int)scanBuf.length();
				i = index2 + 2;
			    }
			    else
				i = index2;
			}
			else
			    break;
		    }
		    //This portion of code fills up blank cell with "XX"...[STOP]
		    columnList = QStringList::split('\t',scanBuf);  //Split field values in list 
		}
		if(indexingRequired) //Prepending auto indexing value
		    columnList.prepend(QString::number(baseRows));
		baseCols = counter = 0;
		for(QStringList::Iterator it = columnList.begin(); it != columnList.end(); ++it,++baseCols)
		{
		    if(baseCols == 0)
			pkID1 = *it;
		    if(indexingRequired && baseCols == 1)
			pkID2 = *it;
		    inFile_Set_Data[counter].append(*it); 
		    if((baseCols >= (counter + 1)*MAXCOLS + (indexingRequired ? 1 : 0)) || (baseCols == (int)columnList.count() - 1))
		    {
			if(counter > 0)
			{
			    if(indexingRequired)
				inFile_Set_Data[counter].prepend(pkID2);
			    inFile_Set_Data[counter].prepend(pkID1);
			}
			*fpOut[counter] << inFile_Set_Data[counter].join("\t") << endl;
			//~~~~Below part to prepare a list of maximum length value of each field~~~~[START]
			if(baseRows == 1)
			    maxValSmplList[counter] = inFile_Set_Data[counter];
			else
			{
			    QStringList::Iterator it1 = inFile_Set_Data[counter].begin();
			    QStringList::Iterator it2 = maxValSmplList[counter].begin();
			    for( ; it1 != inFile_Set_Data[counter].end(); ++it1,++it2)
			    {
				if((int)(*it1).length() > (int)(*it2).length())
				    *it2 = *it1;
			    }
			}
			//~~~~Below part to prepare a list of maximum length value of each field~~~~[STOP]
			counter++;
		    }
		}
	    }
	    if(acceptedFieldFlag)
		baseRows++;
	}
	progressDlg->close();
	pathIn.close();
	--baseRows;
	for(register int itr = 0; itr < numSplitSets; ++itr)
	    inFile_Set_Data[itr].clear();
	delete [] inFile_Set_Data;
	
	for(register int itr = 0; itr < numSplitSets; ++itr)
	    pathOut[itr]->close();
	delete [] pathOut;
	delete [] fpOut;
    }
   
    if(acceptedFieldFlag && baseRows > 0 && baseCols > 0)
	return true;
    else
	return false;
}

///////////////////////////////////Tratnspose Read[START]////////////////////////////////////
void frmgeno::bodyTransposeData(QString inputFile, QString fileName, int index, int numLines)
{
    FILE *fpIn = 0;
    int numRows = 0;
    QString scanBuf = "", query = "";
    bool ok = false;
    progressDlg *progressBar = 0;
    int h = 0, m = 0, s = 0, ms = 0;
    int statusT = 0, progress = 0;
    
    //This part only to find number of rows[START]
    cout << fileName << endl;
    query = "wc -l " + fileName + " > numLines.txt";
    fscanVal = system(query);
    fpIn = fopen("numLines.txt","r");
    fscanVal = fscanf(fpIn,"%d",&numRows);
    fclose(fpIn);
    fpIn = NULL;
    fscanVal = system("rm -f numLines.txt");
    
    ok = false;
    int value = QInputDialog::getInteger("Confirm: number of rows[Transpose operation]", "Number of rows\n(if it is wrong give correct value):",numRows,1,2147483647,1,&ok,this);
    if(ok)
	numRows = value;
    ok = false;
    //This part only to find number of rows[STOP]
    //Reading First Column value to change it in output file[START]
    QFile fpRead(fileName);
    if(fpRead.open(IO_ReadOnly | IO_Truncate))
    {
	QTextStream fpIn(&fpRead);
	fpIn.setEncoding(QTextStream::UnicodeUTF8);
	query = fpIn.readLine();
	fpRead.close();
	QStringList lst = QStringList::split('\t',query);
	QStringList::iterator it = lst.begin();
	scanBuf = *it;
    }
    //Reading First Column value to change it in output file[STOP]
    //Taking 'First Colun' name from User
    while(1)
    {
	QString header = QInputDialog::getText("Change first column name[Transpose operation]","Enter first column name:",QLineEdit::Normal,QString::null,&ok,this);
	if(ok && !header.isEmpty()) 
	{
	    if(checkDbNameSyntax(header)) //Checking given 'First Colun' name
	    {
		//Progress Dialog Settings[START]
		progressBar = new progressDlg(wGeno,"Progress",true,WDestructiveClose);
		progressBar->setCaption("File transpose");
		progressBar->parentProgress->setTotalSteps(numRows);
		progressBar->lblParent->setEnabled(true);
		progressBar->parentProgress->setCenterIndicator(true);
		progressBar->initSettings(true,false);
		//progressBar->childProgress->setTotalSteps(numRows);
		//progressBar->lblChild->setEnabled(true);
		//progressBar->childProgress->setCenterIndicator(true);
		//progressBar->initSettings(true,true);
		progressBar->show();
		setCursor(WaitCursor);
		//Progress Dialog Settings[STOP]
		//Time tracker: Initial[START]
		QTime t1 = QTime::currentTime();
		//Time tracker: Initial[STOP]
		
		fscanVal = system("./transpose.pl " + fileName + " " + fileName.section("/", 0, fileName.contains("/") - 1) + "/T" + fileName.section('/',-1) + " " + header + "&");
		fscanVal = system("rm -f .statusout.txt"); //Just a confirmation that the file is not present
		
		for(int i = 0; ; )
		{
		    fpIn = fopen(".statusout.txt","r");
		    if(fpIn)
		    {
			fscanVal = fscanf(fpIn,"%d",&statusT);
			fclose(fpIn);
		    }
		    fpIn = NULL;
		    
		    fpIn = fopen(".trackITR.txt","r");
		    if(fpIn)
		    {
			fscanVal = fscanf(fpIn,"%d",&progress);
			fclose(fpIn);
		    }
		    fpIn = NULL;
		    
		    //Time tracker: update status[START]
		    if(progress%1000 == 0)
		    {
			updateTimeElapsed(t1,ms,s,m,h);
			query = tr("Time elapsed: ") + QString::number(h) + tr(":") + QString::number(m) + tr(":") + QString::number(s) + tr(":") + QString::number(ms) + tr("[HH:MM:SS:MS]");
			progressBar->lblParent->setText(query);
		    }
		    //Time tracker: update status[STOP]
		    progressBar->parentProgress->setProgress(progress);
		    progressBar->show();
		    
		    if(statusT)
		    {
			updateTimeElapsed(t1,ms,s,m,h);
			query = tr("Time elapsed: ") + QString::number(h) + tr(":") + QString::number(m) + tr(":") + QString::number(s) + tr(":") + QString::number(ms) + tr("[HH:MM:SS:MS]");
			progressBar->lblParent->setText(query);
			fscanVal = system("rm -f .statusout.txt");
			setCursor(ArrowCursor);
			progressBar->isDone = true;
			QMessageBox::information(this,tr("File transpose"),tr("Input file: ") + inputFile.section('/', - 1) + tr("\nOutput file(Transposed data): T") + fileName.section('/', - 1) + tr("\nSaved in the directory: ") + fileName.section("/", 0, fileName.contains("/") - 1));
			saveTemporary(fileName.section('/',0,fileName.contains('/') - 1) + tr("/T") + fileName.section('/', - 1));
			progressBar->close();
			break;
		    }
		    appQ->processEvents();
		}
	    }
	    else
	    {
		QMessageBox::information(this,tr("Column naming status"),tr("\nProblem with given name! \nColumn name can contains only charecter from 'A' to 'Z' case insensitive, \n'0' to '9' and underscore"));
	    }
	}
	if(statusT)
	    break;
    }
}

void frmgeno::updateTimeElapsed(QTime &t1, int &ms, int &s, int &m, int &h)
{
    QTime t2 = QTime::currentTime();
    ms = (t2.hour()*360000 + t2.minute()*6000 + t2.second()*100 + t2.msec()) - (t1.hour()*360000 + t1.minute()*6000 + t1.second()*100 + t1.msec());
    s = ms/100; ms %= 100;
    m = s/60; s %= 60;
    h = m/60; m %= 60;
}
///////////////////////////////////////////Tratnspose Read[STOP]/////////////////////////////////////////////
void frmgeno::changeValueSet(QString fileName)
{
    QString rowScan = "";
    QStringList valueList;
    QString filePath = "", file_Name = "";
    
    filePath = fileName.section('/',0,fileName.contains('/') - 1);
    file_Name = fileName.section('/',-1);
    valuechangedlg *valdlg = new valuechangedlg(this,"valueChangeDlg",WDestructiveClose);
    if(valdlg->exec() == QDialog::Accepted)
    {
	valdlg->close();
	valdlg = 0;
	int counter = 0;
	QString replace = "", with = "",str = "";
	//Load value to change from file and create query[START]
	QFile fpRead(".valuetochange.txt");
	if(fpRead.open(IO_ReadOnly | IO_Truncate))
	{
	    QTextStream fpIn(&fpRead);
	    fpIn.setEncoding(QTextStream::UnicodeUTF8);
	    valueList.clear();
	    while(!fpIn.atEnd())
	    {
		rowScan = fpIn.readLine();
		valueList = QStringList::split("\t",rowScan);
		QStringList::Iterator it = valueList.begin();
		replace = *it; ++it; with = *it;
		if(counter == 0)
		    str += tr("sed 's/") + replace + tr("/") + with + tr("/g' ") + fileName + tr(" | ");
		else
		    str += tr("sed 's/") + replace + tr("/") + with + tr("/g' | ");
		++counter;
	    }
	    fpRead.close();
	    str.truncate((int)str.length()-3);
	    //~~~~Code to replace value[START]
	    QString query = str + " > " + filePath + tr("/C") + file_Name;
	    fscanVal = system(query);
	    if(fscanVal > -1)
		QMessageBox::information(this,tr("Value editing"),tr("Original file: ") + file_Name + tr("\nhas been saved as C") + file_Name + tr("\nby changing specified values under the directory\n") + filePath);
	    //Save file name in datadource1.txt temporarily[START]
	    saveTemporary(filePath + tr("/C") + file_Name);
	    //Save file name in datadource1txt temporarily[STOP]
	    //~~~~Code to replace value[STOP]
	}
	//Load value to change from file and create query[STOP]
    }
}
///////////////////////////////////////////Plink Format[START]/////////////////////////////////////////////
void frmgeno::getCharSet_PlinkFormat(QString fileName)
{
    bool ok;
    QString charset = "";
    //Load previous charset given[START]
    QFile fpRead(".charsetPlink.txt");
    if(fpRead.open(IO_ReadOnly | IO_Truncate))
    {
	QTextStream fpIn(&fpRead);
	fpIn.setEncoding(QTextStream::UnicodeUTF8);
	fpIn >> charset;
	fpRead.close();
	charset.replace(tr("|"),tr(","),false);
    }
    //Load previous charset given[STOP]
    while(1)
    {
	//Taking 'Cahrecter set' for Plink Formatting from user
	charset = QInputDialog::getText("Charecter set Input","Enter cahrecter set of the Genotypes\nfor converting to Plink format[example: a,A,B] case sensitive:",QLineEdit::Normal,charset,&ok,this);
	if(ok && !charset.isEmpty()) 
	{
	    bool okStat = false;
	    if(charset.contains(tr(",")) > 0)
	    {
		QStringList charList = QStringList::split(charset,",");
		for(QStringList::iterator it = charList.begin(); it != charList.end(); ++it)
		{
		    if((int)((*it).length()) > 1)
		    {
			okStat = true;
			break;
		    }
		}
		if(okStat == true)
		    QMessageBox::information(this,tr("Charecter set Input status"),tr("\nInput error!\nType charecters(case sensitive) separated by comma"));
	    }
	    else
	    {
		QMessageBox::information(this,tr("Charecter set Input status"),tr("\nInput error!\nType charecters(case sensitive) separated by comma"));
		okStat = true;
	    }
	    
	    if(okStat == false)
	    {
		charset.replace(tr(","),tr("|"),false);
		//Save charset given in file[START]
		QFile fpWrite(".charsetPlink.txt");
		if(fpWrite.open(IO_WriteOnly | IO_Truncate))
		{
		    QTextStream fpOut(&fpWrite);
		    fpOut.setEncoding(QTextStream::UnicodeUTF8);
		    fpOut << charset << endl;
		    fpWrite.close();
		}
		//Save charset given in file[STOP]
		generateData_PlinkFormat(fileName,charset);
		break;
	    }    
	}
	else
	    QMessageBox::information(this,tr("Charecter set Input status"),tr("\nInput error!\nInput may be blank!"));
    }
}

void frmgeno::generateData_PlinkFormat(QString fileName,QString charset)
{
    QString filePath = "", file_Name = "";
    filePath = fileName.section('/',0,fileName.contains('/') - 1);
    file_Name = fileName.section('/',-1);
    QString query = tr("sed 's/\\([") + charset + tr("]\\)\\([") + charset + tr("]\\)/\\1 \\2/g' ") + fileName + tr(" > ") + filePath + tr("/P") + file_Name; 
    fscanVal = system(query);
    //Save file name in datadource1.txt temporarily[START]
    saveTemporary(filePath + tr("/P") + file_Name);
    //Save file name in datadource1txt temporarily[STOP]
}

void frmgeno::saveTemporary(QString filename)
{
    QFile fpAppend("datasource1.txt");
    if(fpAppend.open(IO_WriteOnly | IO_Append))
    {
	QTextStream fpApp(&fpAppend);
	fpApp.setEncoding(QTextStream::UnicodeUTF8);
	fpApp << filename << endl;
	fpAppend.close();
    }
}
///////////////////////////////////////////Plink Format[STOP]/////////////////////////////////////////////
//////////////////////////////////Illumina Data Process[START]////////////////////////////////////////
void frmgeno::processIlluminaData(QString fileName)
{
    QString str1,str2,str3;
    
    QMessageBox::information(this,tr("Click to start execution"),tr("This operation will be executed using several steps \nand may take several times depending on the file size..."));
    str1 = fileName.section('/',0,fileName.contains('/') -1) + tr("/H") + fileName.section('/',-1);
    str2 = tr("cut -f 1-6 ") + fileName + tr(" > ") + str1;
    fscanVal = system(str2);
    
    if(fscanVal > -1)
    {
	QStringList headerNameList;
	if(getHeaderName(headerNameList))
	{
	    FILE *fpOut = 0;
	    int count = 0;
	    
	    str2 = fileName.section('/',0,fileName.contains('/') -1) + tr("/Header") + fileName.section('/',-1);
	    str2.replace("\\ ", " ");
	    fpOut = fopen(str2,"w");
	    for(QStringList::iterator it = headerNameList.begin(); it != headerNameList.end(); ++it,++count)
	    {
		fprintf(fpOut,"%s",(*it).ascii());
		if(count < (int)headerNameList.count() - 1)
		    fprintf(fpOut,"\t");
		else
		    fprintf(fpOut,"\n");
	    }
	    fclose(fpOut);
	    fpOut = 0;
	    fscanVal = system(tr("cat ") + str1 + tr(" >> ") + str2);
	    fscanVal = system(tr("rm -f ") + str1);
	    //Now generate body part
	    str1 = fileName.section('/',0,fileName.contains('/') -1) + tr("/temp") + fileName.section('/',-1);
	    fscanVal = system(tr("cut -f 2,7- ") + fileName + tr(" > ") + str1);
	    str3 = fileName.section('/',0,fileName.contains('/') -1) + tr("/Body") + fileName.section('/',-1);
	    correctifyFormat(str1, str3);
	    //bodyTransposeData(str3,1,1);
	    isImport = true;
	    indexingRequired = true;
	    //Update datasource.txt file
	    //str3 = fileName.section('/',0,fileName.contains('/') -1) + tr("/TBody") + fileName.section('/',-1);
	    FILE *fpWrite = fopen("datasource1.txt","a");
	    fprintf(fpWrite,"%s\n",str2.ascii());
	    fprintf(fpWrite,"%s\n",str3.ascii());
	    fclose(fpWrite);
	}
	else
	    fscanVal = system(tr("rm -f ") + str1);
    }
}

bool frmgeno::getHeaderName(QStringList &headerNameList)
{
    bool retStat = false;
    
    createheader *header = new createheader(wGeno,tr("New Header"),true,0);
    header->setCaption("Enter header names for the six fields");
    header->initSettings();
    if(header->exec() == QDialog::Accepted)
    {
	if(header->exitStatus)
	{
	    headerNameList = header->headerNameList;
	    header->close();
	    delete header;
	    header = 0;
	    retStat = true;
	}
	else
	    retStat = false;
    }
    return retStat;
}

void frmgeno::correctifyFormat(QString fileName, QString &destFileName)
{
    FILE *fpIn = 0;
    int numRows = 0;
    QString scanBuf = "", query = "";
    bool ok = false;
    progressDlg *progressBar = 0;
    int h = 0, m = 0, s = 0, ms = 0;
    int statusT = 0, progress = 0;
    
    //This part only to find number of rows[START]
    query = "wc -l " + fileName + " > numLines.txt";
    fscanVal = system(query);
    fpIn = fopen("numLines.txt","r");
    fscanVal = fscanf(fpIn,"%d",&numRows);
    fclose(fpIn);
    fpIn = NULL;
    fscanVal = system("rm -f numLines.txt");
    
    ok = false;
    int value = QInputDialog::getInteger("Confirm: number of rows[Transpose operation]", "Number of rows\n(if it is wrong give correct value):",numRows,1,2147483647,1,&ok,this);
    if(ok)
	numRows = value;
    ok = false;
    //This part only to find number of rows[STOP]
    //Reading First Column value to change it in output file[START]
    QFile fpRead(fileName);
    if(fpRead.open(IO_ReadOnly | IO_Truncate))
    {
	QTextStream fpIn(&fpRead);
	fpIn.setEncoding(QTextStream::UnicodeUTF8);
	query = fpIn.readLine();
	fpRead.close();
	QStringList lst = QStringList::split('\t',query);
	QStringList::iterator it = lst.begin();
	scanBuf = *it;
    }
    //Reading First Column value to change it in output file[STOP]
    //Taking 'First Colun' name from User
    while(1)
    {
	QString header = QInputDialog::getText("Change first column name[Transpose operation]","Enter first column name:",QLineEdit::Normal,QString::null,&ok,this);
	if(ok && !header.isEmpty()) 
	{
	    if(checkDbNameSyntax(header)) //Checking given 'First Colun' name
	    {
		//Progress Dialog Settings[START]
		progressBar = new progressDlg(wGeno,"Progress",true,WDestructiveClose);
		progressBar->setCaption("File transpose");
		progressBar->parentProgress->setTotalSteps(numRows);
		progressBar->lblParent->setEnabled(true);
		progressBar->parentProgress->setCenterIndicator(true);
		progressBar->initSettings(true,false);
		//progressBar->childProgress->setTotalSteps(numRows);
		//progressBar->lblChild->setEnabled(true);
		//progressBar->childProgress->setCenterIndicator(true);
		//progressBar->initSettings(true,true);
		progressBar->show();
		setCursor(WaitCursor);
		//Progress Dialog Settings[STOP]
		//Time tracker: Initial[START]
		QTime t1 = QTime::currentTime();
		//Time tracker: Initial[STOP]
		
		fscanVal = system("./transpose.pl " + fileName + " " + destFileName + " " + header + "&");
		fscanVal = system("rm -f .statusout.txt"); //Just a confirmation that the file is not present

		for(int i = 0; ; )
		{
		    fpIn = fopen(".statusout.txt","r");
		    if(fpIn)
		    {
			fscanVal = fscanf(fpIn,"%d",&statusT);
			fclose(fpIn);
		    }
		    fpIn = NULL;
		    
		    fpIn = fopen(".trackITR.txt","r");
		    if(fpIn)
		    {
			fscanVal = fscanf(fpIn,"%d",&progress);
			fclose(fpIn);
		    }
		    fpIn = NULL;
		    
		    //Time tracker: update status[START]
		    if(progress%1000 == 0)
		    {
			updateTimeElapsed(t1,ms,s,m,h);
			query = tr("Time elapsed: ") + QString::number(h) + tr(":") + QString::number(m) + tr(":") + QString::number(s) + tr(":") + QString::number(ms) + tr("[HH:MM:SS:MS]");
			progressBar->lblParent->setText(query);
		    }
		    //Time tracker: update status[STOP]
		    
		    progressBar->parentProgress->setProgress(progress);
		    progressBar->show();
		    
		    if(statusT)
		    {
			updateTimeElapsed(t1,ms,s,m,h);
			query = tr("Time elapsed: ") + QString::number(h) + tr(":") + QString::number(m) + tr(":") + QString::number(s) + tr(":") + QString::number(ms) + tr("[HH:MM:SS:MS]");
			progressBar->lblParent->setText(query);
			fscanVal = system("rm -f .statusout.txt");
			setCursor(ArrowCursor);
			QMessageBox::information(this,tr("File transpose"),tr("Input file: ") + fileName.section('/', - 1) + tr("\nOutput file(Transposed data): T") + fileName.section('/', - 1) + tr("\nSaved in the directory: ") + fileName.section("/", 0, fileName.contains("/") - 1));
			break;
		    }
		    appQ->processEvents();
		}
	    }
	    else
	    {
		QMessageBox::information(this,tr("Column naming status"),tr("\nProblem with given name! \nColumn name can contains only charecter from 'A' to 'Z' case insensitive, \n'0' to '9' and underscore"));
	    }
	}
	if(statusT)
	    break;
    }
}
//////////////////////////////////Illumina Data Process[STOP]/////////////////////////////////////////
///////////////////////////////////////////Selected Field Read[START]//////////////////////////////////
void frmgeno::initiateselectedFieldRead(QString tbName, int numCols)
{
    QString inp_fileName = "";
    QProgressDialog *progressDlg = 0;
    int progress = 0;
    bool okStat = false;
    bool mergeState = false;
    bool retValue = false;
    
    QFile fpRead("datasource.txt");
    if(fpRead.open(IO_ReadOnly | IO_Truncate))
    {
	QTextStream fpIn1(&fpRead);
	fpIn1.setEncoding(QTextStream::UnicodeUTF8);

	QFile fpWrite("datasource1.txt");
	if(fpWrite.open(IO_WriteOnly | IO_Truncate))
	{
	    QTextStream fpOut(&fpWrite);
	    fpOut.setEncoding(QTextStream::UnicodeUTF8);
	    isApplyAll_fldSeln = false;
	    isMerge = false;
	    for(register int i = 0; i < numCols; ++i)
	    {
		inp_fileName = fpIn1.readLine();
		if(!isApplyAll_fldSeln)
		    retValue = fieldSelection(inp_fileName);
		else
		{
		    if(isApplyAll_fldSeln && !okStat)
		    {
			progressDialogSettings(&progressDlg,numCols, tr("Field selection"), tr("Making a copy of selected fields in a separate File"));
			okStat = true;
		    }
		    progressDlg->setProgress(++progress);
		    progressDlg->setLabelText("Copy selected field \nFrom: " + inp_fileName.section('/',-1) + "\nTo: " + fileName.section('/',-1) + (isMerge ? tr("[merge mode]") : ""));
		    progressDlg->show();
		    appQ->processEvents();
		    updateFieldList(inp_fileName);
		}
		
		if(!abortStatus && retValue)
		{
		    if(!mergeState)
			fileName = inp_fileName.section('/',0,(int)inp_fileName.contains('/') - 1) + "/" + tbName + "-" + QString::number(i); //store temporary created file(desired)  from which data is imported in the database
		    copyselectedFields(inp_fileName,mergeState);
		    if(!mergeState && isMerge)
		    {
			fpOut << fileName << endl;
			mergeState = true;
		    }
		    else if(!mergeState)
			fpOut << fileName << endl;
		}
		else if(abortStatus)
		    break;
	    }
	    fpWrite.close();
	    if(okStat)
		progressDlg->close();
	}
	fpRead.close();
	fscanVal = system("rename datasource1.txt datasource.txt datasource1.txt");
    }
}

bool frmgeno::fieldSelection(QString fileName)
{
    bool retStat = false;
    
    dlgfieldseln *fldDlg = new dlgfieldseln(wGeno,tr("Field Selection"),true);
    fldDlg->initSettings(fileName,isMerge);
    if(fldDlg->exec() == QDialog::Accepted)
    {
	if(fldDlg->abortStatus)
	    abortStatus = fldDlg->abortStatus;
	else if(fldDlg->num_Rows)
	{
	    abortStatus = false;
	    fieldIds = fldDlg->fldLst;
	    numFields = fldDlg->num_Rows;
	    isApplyAll_fldSeln = fldDlg->applyToAll;
	    isMerge = fldDlg->mergeState;
	    fldDlg->close();
	    delete fldDlg;
	    fldDlg = NULL;
	    retStat = true;
	}
    }
    return retStat;
}

void frmgeno::updateFieldList(QString inp_fileName)
{
    QString str;
    int count = 1;
    
    QFile fpRead(".correctField.txt");
    if(fpRead.open(IO_ReadOnly | IO_Truncate))
    {
	QFile fpWrite(".correctField0.txt");
	if(fpWrite.open(IO_WriteOnly | IO_Truncate))
	{
	    QTextStream fpIn(&fpRead);
	    fpIn.setEncoding(QTextStream::UnicodeUTF8 );
	    QTextStream fpOut(&fpWrite);
	    fpOut.setEncoding(QTextStream::UnicodeUTF8 );
	    
	    while(!fpIn.atEnd())
	    {
		fpIn >> str;
		if(count > 1)
		    fpOut << str.replace(str.section("_",0,(int)str.contains("_") - 1),(inp_fileName.section('/',-1)).remove(tr(".txt"))) << endl;
		else
		    fpOut << str << endl;
		++count;
	    }
	    fpWrite.close();
	}
	fpRead.close();
	fscanVal = system("rename .correctField0.txt .correctField.txt .correctField0.txt");
    }
}

void frmgeno::copyselectedFields(QString inp_fileName,bool mergeState)
{
    QString str = "",query = "",scanBuf = "";
    int count = 1,numLines = 0;
    FILE *fp;
    int numLineToSkip = 0;
    
    QFile fpRead(".correctField.txt");
    if(fpRead.open(IO_ReadOnly | IO_Truncate))
    {
	QFile fpWrite(tr(".tempFile"));
	if(fpWrite.open(IO_WriteOnly | IO_Truncate))
	{
	    QTextStream fpIn(&fpRead);
	    fpIn.setEncoding(QTextStream::UnicodeUTF8 );
	    QTextStream fpOut(&fpWrite);
	    fpOut.setEncoding(QTextStream::UnicodeUTF8 );
	    
	    while(!fpIn.atEnd())
	    {
		fpIn >> str;
		query += str;
		if(count < (int)fieldIds.count())
		    query += "\t";
		++count;
	    }
	    fpOut << query << endl;;
	    fpWrite.close();
	}
	fpRead.close();
    }
    
    //Read number of lines in the file[START]
    str = tr("wc -l ") + inp_fileName + tr(" > numLines.txt");
    fscanVal = system(str.ascii());
    fp = fopen("numLines.txt","r");
    fscanVal = fscanf(fp,"%d %s",&numLines,str.ascii());
    fclose(fp);
    fp = NULL;
    fscanVal = system("rm -f numLines.txt");
    //Read number of lines in the file[STOP]
    //Read number of lines to skip[START]
    str = inp_fileName;
    str.remove('\\');
    QFile fpRead1(str);
    if(fpRead1.open(IO_ReadOnly | IO_Truncate))
    {
	QTextStream fpIn1(&fpRead1);
	fpIn1.setEncoding(QTextStream::UnicodeUTF8);
	do
	{
	    scanBuf = fpIn1.readLine();
	    //cout << scanBuf << endl;
	    numLineToSkip++;
	}while(scanBuf.contains("#",FALSE) > 0);
	fpRead1.close();
    }
    //cout << "numLineToSkip: " << numLineToSkip << endl;
    str = "";
    //Read number of lines to skip[STOP]
    query = tr("cut -f ");
    for(QStringList::Iterator it = fieldIds.begin(); it != fieldIds.end(); ++it)
	query += *it + ",";
    query.truncate(query.length() - 1);
    query += tr(" ") + inp_fileName + tr(" | tail -") + QString::number(numLines-numLineToSkip) + tr(" >> .tempFile");
    fscanVal = system(query);
    query = tr("tr -d '\\r' < .tempFile > ") + (isMerge ? fileName + "_temp" : fileName);
    fscanVal = system(query);
    fscanVal = system(tr("rm -f .tempFile"));
    if(isMerge && mergeState)
    {
	fscanVal = system(tr("rename ") + fileName + " " + fileName + tr("_temp2 ") + fileName);
	fscanVal = system(tr("join ") + fileName + tr("_temp2 ") + fileName + tr("_temp | tr ' ' '\t' > ") + fileName);
	fscanVal = system(tr("rm -f ") + fileName + tr("_temp2"));
	fscanVal = system(tr("rm -f ") + fileName + tr("_temp"));
    }
    else if(isMerge)
	fscanVal = system(tr("rename ") + fileName + tr("_temp ") + fileName + tr(" ") + fileName + tr("_temp"));
}

void frmgeno::reomveIntermediateFiles(QString filename)
{
    QString str = "",query = "";
    
    QFile fpRead(filename);
    if(fpRead.open(IO_ReadOnly | IO_Truncate))
    {
	QTextStream fpIn(&fpRead);
	fpIn.setEncoding(QTextStream::UnicodeUTF8);
	while(!fpIn.atEnd())
	{
	    str = fpIn.readLine();
	    query = tr("rm -f ") + str;
	    fscanVal = system(query);
	}
	fpRead.close();
	fscanVal = system(tr("rm -f ") + filename);
    }
}
///////////////////////////////////////////Selected Field Read[STOP]////////////////////////////////////////

void frmgeno::progressDialogSettings(QProgressDialog **progressDlg, int totalSteps, QString title, QString label)
{
    (*progressDlg) = new QProgressDialog(wGeno,tr(title),false,WDestructiveClose);
    (*progressDlg)->setCaption(title);
    (*progressDlg)->setTotalSteps(totalSteps);
    lblProgress = new QLabel(*progressDlg,label,WDestructiveClose);
    (*progressDlg)->setLabel(lblProgress);
    (*progressDlg)->setAutoReset(true);
}

void frmgeno::extendedCreateTable()
{
    int numLines = 0;
    bool sameGroup = false,ok = false; 
    QProgressDialog *progressDlg = 0;
    int progress = 0;
    QString inputFile = "";
    
    if(isillumina)
    {
	numLines = numRecordsInFile();
	QFile fpRead("datasource.txt");
	if(fpRead.open(IO_ReadOnly | IO_Truncate))
	{
	    FILE *fpWrite = fopen("datasource1.txt","w");
	    fclose(fpWrite);
	    QTextStream fpIn1(&fpRead);
	    fpIn1.setEncoding(QTextStream::UnicodeUTF8);
	    for(int i = 0; i < numLines; ++i)
	    {
		fileName = fpIn1.readLine();
		processIlluminaData(fileName);
	    }
	    fpRead.close();
	    fscanVal = system("rm -f datasource.txt");
	    fscanVal = system("rename datasource1.txt datasource.txt datasource1.txt");
	}
    }
    
    if(isSelectField)
    {
	if(isImport)
	{
	    while(1)
	    {
		QString tbName = QInputDialog::getText("Table name","Enter table name\n(where all imported data will be stored): ",QLineEdit::Normal,QString::null,&ok,this);
		if(ok && !tbName.isEmpty()) 
		{
		    if(checkDbNameSyntax(tbName))
		    {
			initiateselectedFieldRead(tbName,numRecordsInFile());
			break;
		    }
		    else
			QMessageBox::information(this,tr("Table name"),tr("Not permitted table name: ") + tbName + tr("\nProblem with given name! \nTable name can contains only charecter from 'A' to 'Z' case insensitive, \n'0' to '9' and underscore"));
		}
	    }
	}
	else
	{
	    while(1)
	    {
		QString tbName = QInputDialog::getText("File name","Enter file name\n(where selected field's data will be stored): ",QLineEdit::Normal,QString::null,&ok,this);
		if(ok && !tbName.isEmpty()) 
		{
		    if(checkDbNameSyntax(tbName))
		    {
			initiateselectedFieldRead(tbName,numRecordsInFile());
			if(!abortStatus)
			    QMessageBox::information(this,tr("Field selection"),tr("Current operation completed successfully!"));
			break;
		    }
		    else
			QMessageBox::information(this,tr("File name"),tr("Not permitted file name: ") + tbName + tr("\nProblem with given name! \nFile name can contains only charecter from 'A' to 'Z' case insensitive, \n'0' to '9' and underscore"));
		}
	    }
	}
    }
     
    if(isTranspose)
    {
	numLines = numRecordsInFile();
	QFile fpRead("datasource.txt");
	if(fpRead.open(IO_ReadOnly | IO_Truncate))
	{
	    QTextStream fpIn1(&fpRead);
	    fpIn1.setEncoding(QTextStream::UnicodeUTF8);
	    FILE *fpWrite = fopen("datasource1.txt","w");
	    fclose(fpWrite);
	    QString fileExtn = QInputDialog::getText("Transposed file extension", "Enter file extension: ", QLineEdit::Normal, tr(".ped"), &ok, this);
	    if(ok && !fileExtn.isEmpty())
	    {
		for(int i = 0; i < numLines; ++i)
		{
		    fileName = fpIn1.readLine();
		    inputFile = fileName;
		    fscanVal = system(tr("tr -d '\r' < ") + fileName + tr(" > ") + fileName.section('/',0,fileName.contains('/') - 1) + tr("/tempFile.txt"));
		    if(!fileName.endsWith(".ped",false))
		    {
			fileExtn.remove(".");
			QString extn = fileName.section('.',-1);
			fileName.truncate(fileName.length() - extn.length());
			fileName += tr("ped");
		    }
		    fscanVal = system(tr("rename ") + fileName.section('/',0,fileName.contains('/') - 1) + tr("/tempFile.txt ") + fileName + tr(" ") + fileName.section('/',0,fileName.contains('/') - 1) + tr("/tempFile.txt "));
		    if(fscanVal > -1)
		    {
			bodyTransposeData(inputFile,fileName,i,numLines);
			fscanVal = system(tr("rm -f ") + fileName);
		    }
		    else
			QMessageBox::information(this,tr("Renaming problem"),tr("File renaming problem occur from transpose operation part\nInform the developer..."));
		}
		fpRead.close();
		fscanVal = system("rm -f datasource.txt");
		fscanVal = system("rename datasource1.txt datasource.txt datasource1.txt");
	    }
	}
    }
    
    if(isValueChangeStat)
    {
	numLines = numRecordsInFile();
	QFile fpRead("datasource.txt");
	if(fpRead.open(IO_ReadOnly | IO_Truncate))
	{
	    QTextStream fpIn1(&fpRead);
	    fpIn1.setEncoding(QTextStream::UnicodeUTF8);
	    FILE *fpWrite = fopen("datasource1.txt","w");
	    fclose(fpWrite);
	    progressDialogSettings(&progressDlg,numLines,tr("Value editing"),tr("Value editing"));
	    for(int i = 0; i < numLines; ++i)
	    {
		progressDlg->setProgress(++progress);
		progressDlg->setLabelText(fileName.section('/',-1) + ":-\n[Completed: " + QString::number(i) + " of " + QString::number(numLines) + " files]");
		progressDlg->show();
		appQ->processEvents();
		fileName = fpIn1.readLine();
		changeValueSet(fileName);
	    }
	    progressDlg->close();
	    fpRead.close();
	    progress = 0;
	    progressDlg = 0;
	    //reomveIntermediateFiles();
	    fscanVal = system("rm -f datasource.txt");
	    fscanVal = system("rename datasource1.txt datasource.txt datasource1.txt");
	}
    }
    
    if(isPlinkformat)
    {
	numLines = numRecordsInFile();
	QFile fpRead("datasource.txt");
	if(fpRead.open(IO_ReadOnly | IO_Truncate))
	{
	    QTextStream fpIn1(&fpRead);
	    fpIn1.setEncoding(QTextStream::UnicodeUTF8);
	    FILE *fpWrite = fopen("datasource1.txt","w");
	    fclose(fpWrite);
	    progressDialogSettings(&progressDlg,numLines,tr("Space between charecters"),tr("Space between charecters"));
	    for(int i = 0; i < numLines; ++i)
	    {
		progressDlg->setProgress(++progress);
		progressDlg->setLabelText(fileName.section('/',-1) + ":-\n[Completed: " + QString::number(i) + " of " + QString::number(numLines) + " files]");
		progressDlg->show();
		appQ->processEvents();
		fileName = fpIn1.readLine();
		getCharSet_PlinkFormat(fileName);
	    }
	    progressDlg->close();
	    fpRead.close();
	    progress = 0;
	    progressDlg = 0;
	    fscanVal = system("rm -f datasource.txt");
	    fscanVal = system("rename datasource1.txt datasource.txt datasource1.txt");
	}
    }
                          
    if(isImport)
    {
	numLines = numRecordsInFile();
	specialFlag = (numLines > 1 ? true : false);
	if(numLines > 1)
	    sameGroup = checkForSameGroup(numLines);
	dataImportMonitoring(numLines,sameGroup);
    }
}

int frmgeno::numRecordsInFile()
{
    char tempStr[500];
    int numLines = 0;
    FILE *fpIn;
    
    sprintf(tempStr,"wc -l datasource.txt > numLines.txt");
    fscanVal = system(tempStr);
    fpIn = fopen("numLines.txt","r");
    fscanVal = fscanf(fpIn,"%d %s",&numLines,tempStr);
    fclose(fpIn);
    fpIn = NULL;
    sprintf(tempStr,"rm -f numLines.txt");
    fscanVal = system(tempStr);
    return numLines;
}

bool frmgeno::checkForSameGroup(int numLines)
{
    bool sameGroup = false; 
    
    if(numLines > 1)
    {
	QFile fpRead1("datasource.txt");
	if(fpRead1.open(IO_ReadOnly | IO_Truncate))
	{
	    QString propName1,propName2;
	    QTextStream fpIn1(&fpRead1);
	    
	    sameGroup = true;
	    fpIn1.setEncoding(QTextStream::UnicodeUTF8);
	    fileName = fpIn1.readLine();
	    propName1 = (fileName.section('/',-1)).section('-',0,0);
	    for(register int i = 1; i < numLines; ++i)
	    {
		fileName = fpIn1.readLine();
		propName2 = (fileName.section('/',-1)).section('-',0,0);
		if(propName1 != propName2)
		{
		    sameGroup = false;
		    break;
		}
	    }
	    fpRead1.close();
	}
    }
    return sameGroup;
}

void frmgeno::dataImportMonitoring(int numLines,bool sameGroup)
{
    int counter = 0,silentModePivot = -1,recsInserted = 0;
    frmsilentmodestatus *importStat = NULL;
    QString query;
    int *importedRecsList; //List of imported Records
    int totRecsUptoLastImport = 0; //Total Rocords in Table upto last insert
    int importMethod = 0; //1~'mysqlimport' 2~ 'import algo'
    
    //Data Import monitoring operation start from here
    QFile fpRead("datasource.txt");
    if(fpRead.open(IO_ReadOnly | IO_Truncate))
    {
	QTextStream fpIn1(&fpRead);
	fpIn1.setEncoding(QTextStream::UnicodeUTF8);
	importedRecsList = new int[numLines];

	for(int i = 0; i < numLines; ++i)
	{
	    fileName = fpIn1.readLine();
	    //If Table already exist in the database then it calculate number of rows exist in the table otherwise assign zero in the variable 'totRecsUptoLastImport'
	    if(i == 0)
	    {
		if(sameGroup)
		{
		    query = "SELECT * FROM " + (fileName.section('/',-1)).section('-',0,0) + ";";
		    QSqlQuery squery(query);
		    if(squery.numRowsAffected())
		    {
			totRecsUptoLastImport = squery.size();
			totRecsUptoLastImport = (totRecsUptoLastImport == -1 ? 0 : totRecsUptoLastImport);
		    }
		    else
			totRecsUptoLastImport = 0;
		}
		else
		    totRecsUptoLastImport = 0;
	    }
	    if(runInSilentMode == false) //If from second iteration program does not runs in silent mode then clear these two lists
	    {
		rejectedFieldList.clear();
		acceptedFieldList.clear();
	    }
	    
	    if(readNumberOfRows_ColsIn_File(fileName,i+1,numLines,baseRows,baseCols))
	    {
		if(needFileSplit) //Each file is splitted by MAXCOLS limit
		{
		    specialFlag = true;
		    counter = 0;
		    needFileSplit = false;
		    runInSilentMode = false;
		    activeDatabase = sqlDB->databaseName();
		    for(QStringList::Iterator it = inFileSet.begin(); it != inFileSet.end(); ++it,++counter)
		    {
			fileName = *it;
			columnList.clear();
			columnList =  inFile_Set_Columns[counter];
			baseCols = columnList.count();
			tableName = (fileName.section('/',-1)).remove(".txt");
			//Database Table creation/updatation part for import operation[START]
			frmTableCreate *tabSchema = new frmTableCreate(this,"Table Schema",WDestructiveClose);
			tabSchema->sqlDB = sqlDB;
			matchingColumn = -1;
			tabSchema->tableName = tableName;
			tabSchema->maxcols = MAXCOLS;
			tabSchema->columnList = columnList;
			tabSchema->maxValSmplList = maxValSmplList[counter];
			tabSchema->setTableAttrTypes(indexingRequired);
			tabSchema->runInSilentMode = (runInSilentMode ? true : false);
			tabSchema->setSilentRunMode(0);
			if((runInSilentMode == true && tabSchema->runCallsInSilentMode()) || tabSchema->exec() == QDialog::Accepted) //Running modes: Silent or Interactive
			{
			    if(tabSchema->tableStatus == true) //true ~ table creation/updation is successful
			    {
				//Updating name of the file to import[START]
				tableName = fileName.section('/',0,fileName.contains("/") - 1) + "/" + tabSchema->tableName + (tabSchema->tableName.contains("-import") > 0 ? "" : "-import"); //creating changed name for file to import
				fscanVal = system("rename " + fileName + " " + tableName + " " + fileName);
				fileName = tableName;
				tableName = tabSchema->tableName;
				matchingColumn = tabSchema->matchingColumn;
				//Updating name of the file to import[STOP]
				colSeqOrdered = true;
				if(frmTableCreate::updateFlag == true) //Update checkbox is checked so use second process(avoid mysqlimport)
				    colSeqOrdered = false;
				if(tabSchema->colIndexArrFlag)
				{
				    colIndexArr = tabSchema->colIndexArr;
				    colIndexArrFlag = tabSchema->colIndexArrFlag;
				    tabSchema->colIndexArr = NULL;
				    for(register int j = 0; j < (int)columnList.count() - 2; ++j)
				    {
					if(colIndexArr[j] > colIndexArr[j+1])
					{
					    colSeqOrdered = false;
					    break;
					}
				    }
				}
				else
				    colIndexArrFlag = false;
				runInSilentMode = tabSchema->runInSilentMode;
				tabSchema->close();
				createActionGeno->setVisible(false);
				if(runInSilentMode)
				{
				    if(!sqlDB->isOpen())
					connectionToDatabase();
				    if(counter < (int)inFileSet.count() - 1)
				    {
					straightForwardInsertionOfDataInTable(recsInserted,importMethod);
					if(recsInserted)
					{
					    if(importMethod == 0)
						importedRecsList[i] = recsInserted;
					    else if(importMethod == 1)
					    {
						importedRecsList[i] = recsInserted - totRecsUptoLastImport;
						totRecsUptoLastImport = recsInserted;
					    }
					    else
						importedRecsList[i] = recsInserted;
					}
				    }
				    else
				    {
					straightForwardInsertionOfDataInTable(recsInserted,importMethod);
					if(recsInserted)
					{
					    if(importMethod == 0)
						importedRecsList[i] = recsInserted;
					    else if(importMethod == 1)
					    {
						importedRecsList[i] = recsInserted - totRecsUptoLastImport;
						totRecsUptoLastImport = recsInserted;
					    }
					    else
						importedRecsList[i] = recsInserted;
					    recsInserted = 0;
					    for(register int j = 0; j < numLines; ++j)
						recsInserted += importedRecsList[j];
					    query = QString::number(recsInserted) + " records inserted or updated";
					    QMessageBox::information(this,"Query Result",query,QMessageBox::Ok);
					}
				    }
				}
				else if(!runInSilentMode)
				{
				    if(!sqlDB->isOpen())
					connectionToDatabase();
				    straightForwardInsertionOfDataInTable(recsInserted,importMethod);
				    if(recsInserted)
				    {
					query = QString::number(recsInserted) + " records inserted or updated";
					QMessageBox::information(this,"Query Result",query,QMessageBox::Ok);
				    }
				}
				createActionGeno->setVisible(true);
				if(colIndexArrFlag)
				{
				    delete colIndexArr;
				    colIndexArr = NULL;
				}
			    }
			    
			    if(QString(fileName).contains("-import"))
			    {
				query = "rm -f " + QString(fileName);
				fscanVal = system(query);
			    }
			}
			//Database Table creation/updatation part for import operation[END]
		    }
		    for(register int itr = 0; itr < (int)inFileSet.count(); ++itr)
		    {
			inFile_Set_Columns[itr].clear();
			maxValSmplList[itr].clear();
		    }	
		    delete [] inFile_Set_Columns;
		    delete [] maxValSmplList;
		    inFileSet.clear();
		    columnList.clear();
		    needFileSplit = false; //To avoid bug if any
		    runInSilentMode = false; //To avoid bug if any
		}
		else //Single File with no split, doesn't cross boundary of MAXCOLS
		{
		    tableName = (fileName.section('/',-1)).remove(".txt");
		    //Database Table creation/updatation part for import operation[START]
		    frmTableCreate *tabSchema = new frmTableCreate(this,"Table Schema",WDestructiveClose);	
		    tabSchema->sqlDB = sqlDB;
		    matchingColumn = -1;
		    tabSchema->tableName = tableName;
		    tabSchema->maxcols = MAXCOLS;
		    tabSchema->columnList = columnList;
		    tabSchema->maxValSmplList = maxValSmplList[0];
		    tabSchema->runInSilentMode = runInSilentMode; //For importing multiple file in single mode
		    tabSchema->setTableAttrTypes(indexingRequired);
		    if(sameGroup == false) //Files aren't within same group, so hide 'Run in Silent Mode' checkbox option
			tabSchema->hideSilentRun();
		    else
			tabSchema->setSilentRunMode(1);
		    if((runInSilentMode == true && tabSchema->runCallsInSilentMode()) || tabSchema->exec() == QDialog::Accepted)
		    {
			if(tabSchema->tableStatus == true)
			{
			    //Updating name of the file to import[START]
			    tableName = fileName.section('/',0,fileName.contains("/") - 1) + "/" + tabSchema->tableName + (tabSchema->tableName.contains("-import") > 0 ? "" : "-import"); //creating changed name for file to import
			    fscanVal = system("rename " + fileName + " " + tableName + " " + fileName);
			    fileName = tableName;
			    tableName = tabSchema->tableName;
			    matchingColumn = tabSchema->matchingColumn;
			    //Updating name of the file to import[STOP]
			    colSeqOrdered = true; //Fields are ordered
			    if(tabSchema->colIndexArrFlag)
			    {
				colIndexArr = tabSchema->colIndexArr;
				colIndexArrFlag = tabSchema->colIndexArrFlag;
				tabSchema->colIndexArr = NULL;
				for(register int j = 0; j < (int)columnList.count() - 2; ++j)
				{
				    if(colIndexArr[j] > colIndexArr[j+1])
				    {
					colSeqOrdered = false; //Fields are unordered
					break;
				    }
				}
			    }
			    else
				colIndexArrFlag = false;
			    if(colSeqOrdered && tabSchema->runInSilentMode && silentModePivot == -1) //Iff fields aren't unordered	
			    {
				runInSilentMode = tabSchema->runInSilentMode;
				silentModePivot = i;
			    }
			    if(frmTableCreate::updateFlag == true) //Update checkbox is checked so use second process(avoid mysqlimport)
				colSeqOrdered = false;
			    tabSchema->close();
			    createActionGeno->setVisible(false);
			    if(runInSilentMode)
			    {
				if(!sqlDB->isOpen())
				    connectionToDatabase();
				if(i < numLines - 1)
				{
				    straightForwardInsertionOfDataInTable(recsInserted,importMethod);
				    if(recsInserted)
				    {
					if(importMethod == 0)
					    importedRecsList[i] = recsInserted;
					else if(importMethod == 1)
					{
					    importedRecsList[i] = recsInserted - totRecsUptoLastImport;
					    totRecsUptoLastImport = recsInserted;
					}
					else
					    importedRecsList[i] = recsInserted;
					if(i > silentModePivot)
					{
					    importStat->updateStatus(i,importedRecsList);
					    //importStat->show();
					}
				    }
				}
				else
				{
				    straightForwardInsertionOfDataInTable(recsInserted,importMethod);
				    if(recsInserted)
				    {
					if(importMethod == 0)
					    importedRecsList[i] = recsInserted;
					else if(importMethod == 1)
					{
					    importedRecsList[i] = recsInserted - totRecsUptoLastImport;
					    totRecsUptoLastImport = recsInserted;
					}
					else
					    importedRecsList[i] = recsInserted;
					if(i > silentModePivot)
					{
					    importStat->updateStatus(i,importedRecsList);
					    //importStat->show();
					}
					recsInserted = 0;
					for(register int j = 0; j < numLines; ++j)
					    recsInserted += importedRecsList[j];
					query = QString::number(recsInserted) + " records inserted or updated";
					QMessageBox::information(this,"Query Result",query,QMessageBox::Ok);
				    }
				}
			    }
			    else if(!runInSilentMode)
			    {
				if(!sqlDB->isOpen())
				    connectionToDatabase();
				straightForwardInsertionOfDataInTable(recsInserted,importMethod);
				if(recsInserted)
				{
				    query = QString::number(recsInserted) + " records inserted or updated";
				    QMessageBox::information(this,"Query Result",query,QMessageBox::Ok);
				}
			    }
			    createActionGeno->setVisible(true);
			    if(colIndexArrFlag)
			    {
				delete colIndexArr;
				colIndexArr = NULL;
			    }
			}	
			if(QString(fileName).contains("-import"))
			{
			    query = "rm -f " + QString(fileName);
			    fscanVal = system(query);
			}
		    }
		    maxValSmplList[0].clear();
		    delete [] maxValSmplList;
		    columnList.clear();
		    needFileSplit = false;
		    //Database Table creation/updatation part for import operation[END]
		}
	    }
	    
	    if(runInSilentMode)
	    {
		if(i == silentModePivot)
		{
		    importStat = new frmsilentmodestatus(wGeno,tr("status"),false,WDestructiveClose);
		    importStat->workDir = workDir;
		    importStat->initSettings(numLines,silentModePivot,importedRecsList);
		    //importStat->show();
		}
	    }
	}
	if(!isTranspose)
	{
	    //statBar->clear();
	    delete [] importedRecsList;
	    importedRecsList = NULL;
	    if(runInSilentMode == true)
	    {
		runInSilentMode = false;
		importStat->show();
		rejectedFieldList.clear();
		acceptedFieldList.clear();
	    }
	}
	fpRead.close();
    }
    else
	QMessageBox::information(this, "File Opening Problem!","Can not open \'datasource.txt\' file for reading");
}

void frmgeno::straightForwardInsertionOfDataInTable(int &recsInserted, int  &importMethod)
{
    int Counter = 0,bufIndex = 0,recsbefore = 0, recsafter = 0, *mode,progress = 0;
    QString keyValue = "",keyValue1 = ""; //keyValue for first primary Key, keyValue1 for second primary Key(for Index required)
    QString keyValueM = "";
    QString query = "",column_list = "";
    QString rowSet;
    
    if(colSeqOrdered == true && (((frmTableCreate::oldTableAffect.count() + frmTableCreate::newTables.count()) == 1) && frmTableCreate::isNewFields == false)) //false ~ create or no create, true ~ alter: table operation
    {
	if(sqlDB->transaction())
	{
	    QStringList::iterator itf;
	    if(frmTableCreate::oldTableAffect.count())
		itf = frmTableCreate::oldTableAffect.begin();
	    else if(frmTableCreate::newTables.count())
		itf = frmTableCreate::newTables.begin();
	    query = "SELECT * FROM " + *itf + ";";
	    QSqlQuery squery1(query,sqlDB);
	    if(squery1.isActive()) //This portion is to avoid problem with last record insertion
		recsbefore = squery1.size();
	    Counter = columnList.count();
	    bufIndex = 1;
	    column_list = "";
	    statusBar()->message(tr("Data import from ") + fileName.section('/',-1) + tr("is in progress. Depending on data size it may take several minutes."));
	    statusBar()->show();
	    for(QStringList::Iterator it = columnList.begin(); it != columnList.end(); ++it,++bufIndex)
	    {
		column_list += *it;
		if(bufIndex <  Counter)
		    column_list += ",";
	    }

	    QString fileName2 = QString(fileName).section('/',0,-2) + "/" + *itf;
	    query = "cp " + QString(fileName) + " " + fileName2;
	    fscanVal = system(query);
	    query = "mysqlimport --local --ignore-lines=1 --lines-terminated-by='\n' -l -u root " + activeDatabase + " " + QString(fileName2);
	    fscanVal = system(query);
	    query = "SELECT * FROM " + *itf + ";";
	    QSqlQuery squery(query,sqlDB);
	    if(squery.isActive()) //This portion is to avoid problem with last record insertion
	    {
		curs = new QSqlCursor(*itf,true,sqlDB);
		if(!specialFlag)
		{
		    table = new QDataTable(curs,true,wGeno,"DataTable");
		    connect(table,SIGNAL(pressed(int ,int ,int ,const QPoint & )),this,SLOT(pressedOnTable(int ,int ,int ,const QPoint & )));
		    table->setSelectionMode(QTable::Multi);
		    table->setAutoEdit(true);
		}
		curs->setMode(7);
		recsafter = squery.size();
		if(squery.seek(recsafter - 1))
		{
		    QSqlIndex pk = curs->primaryIndex();
		    curs->setValue(0,QVariant(squery.value(0).toString()));
		    if(curs->select( pk, pk ))
		    {
			if(curs->next())
			{
			    buffer = new QSqlRecord *[1];
			    buffer[0] = curs->primeUpdate();
			    if((buffer[0]->value(baseCols-1).toString() == "") || (buffer[0]->value(baseCols-1).toString() == NULL))
			    {
				QFile fpRead(fileName2);
				if(fpRead.open(IO_ReadOnly | IO_Truncate))
				{
				    QTextStream fpIn(&fpRead);
				    fpIn.setEncoding(QTextStream::UnicodeUTF8 );
				    while(!fpIn.atEnd())
					fpIn >> rowSet;
				    fpRead.close();
				    buffer[0]->setValue(baseCols-1,rowSet.stripWhiteSpace());
				    if(curs->update())
				    {
					if(sqlDB->commit())
					    cout << "\nTransaction Commit\n";
				    }
				    else
					cout << "Update: " << curs->lastError().text() << endl;
				}
			    }
			    delete [] buffer;
			}
		    }
		    if(!specialFlag)
		    {
			table->refresh();
			table->show();
		    }
		}
	    }

	    query = "rm -f " + fileName2;
	    fscanVal = system(query);
	    recsInserted = recsafter;
	    if(sqlDB->commit())
		cout << "\nTransaction Commit\n";
	    if(!recsbefore && recsafter)
		importMethod  = 0;
	    else if(recsbefore && recsafter)
		importMethod  = 1;
	    statusBar()->clear();
	}
    }
    else
    {
	QStringList cellValueList;
	int itc = 0,startIndex = 0,tempCounter = 0,progressStepSize = 0;
	QProgressDialog *progressDlg = 0;
	QString pkFieldName; //carry the primary key field name for searching for record in table during record insertion/updation
	bool ok;
	int tempVar,*tabIdArr; //For creating id arrayes for those tables in which data insertion/updation is required
	//Initializing cursor/s
	Counter = frmTableCreate::oldTableAffect.count() + frmTableCreate::newTables.count();
	tabIdArr = new int[Counter];
	if(!specialFlag)
	    dtab = new QDataTable *[Counter];
	mode = new int[Counter];
	//Counter += frmTableCreate::oldTables.count(); //This portion is needed if it is required to insert new 'rs' values in old tables which are not under current updation
	cur = new QSqlCursor *[Counter];
	buffer = new QSqlRecord *[Counter];
	bufIndex = 0;
	for(QStringList::Iterator it = frmTableCreate::oldTableAffect.begin(); it != frmTableCreate::oldTableAffect.end(); ++it,++bufIndex)
	{
	    ok = false;
	    tempVar = ((*it).section("_",-1)).toInt(&ok,10);
	    if(ok)
		tabIdArr[bufIndex] = tempVar;
	    else
		tabIdArr[bufIndex] = 0;
	    cur[bufIndex] = new QSqlCursor(*it,true,sqlDB);
	    if(!specialFlag) //If Database Table is needed to show then allocate Data Table
	    {
		dtab[bufIndex] = new QDataTable(cur[bufIndex],true,wGeno,"DataTable");
		connect(dtab[bufIndex],SIGNAL(pressed(int ,int ,int ,const QPoint & )),this,SLOT(pressedOnTable(int ,int ,int ,const QPoint & )));
		dtab[bufIndex]->setSelectionMode(QTable::Multi);
		dtab[bufIndex]->setAutoEdit(true);
	    }
	}
	
	for(QStringList::Iterator it = frmTableCreate::newTables.begin(); it != frmTableCreate::newTables.end(); ++it,++bufIndex)
	{
	    ok = false;
	    tempVar = ((*it).section("_",-1)).toInt(&ok,10);
	    if(ok)
		tabIdArr[bufIndex] = tempVar;
	    else
		tabIdArr[bufIndex] = 0;
	    cur[bufIndex] = new QSqlCursor(*it,true,sqlDB);
	    if(!specialFlag) //If Database Table is needed to show then allocate Data Table
	    {
		dtab[bufIndex] = new QDataTable(cur[bufIndex],true,wGeno,"DataTable");
		connect(dtab[bufIndex],SIGNAL(pressed(int ,int ,int ,const QPoint & )),this,SLOT(pressedOnTable(int ,int ,int ,const QPoint & )));
		dtab[bufIndex]->setSelectionMode(QTable::Multi);
		dtab[bufIndex]->setAutoEdit(true);
	    }
	}
	//This portion is needed if it is required to insert new 'rs' values in old tables which are not under current updation
	//for(QStringList::Iterator it = frmTableCreate::oldTables.begin(); it != frmTableCreate::oldTables.end(); ++it,++bufIndex)
	//cur[bufIndex] = new QSqlCursor(*it,true,sqlDB);
	tempCounter = Counter - 1; //Use only for identufying start index of  column in old table / that in case of new table(generally be 1 less)
	//Counter -= frmTableCreate::oldTables.count();
	//Start of reading records from file
	if(sqlDB->transaction())
	{
	    QString fileName_adjusted = fileName;
	    fileName_adjusted.remove("\\");
	    QFile fpRead(fileName_adjusted);
	    if(fpRead.open(IO_ReadOnly|IO_Truncate))
	    {
		QTextStream fpIn(&fpRead);
		fpIn.setEncoding(QTextStream::UnicodeUTF8 );
		rowSet = fpIn.readLine(); //Read the table header from file
		recsafter = progress = 0;
		//~~~~~~~~~~ Just for progressStepSize calculation[START]
		progressStepSize = baseRows/100;
		progressStepSize = (baseRows%100 == 0 ? progressStepSize : progressStepSize + 1);
		//~~~~~~~~~~ Just for progressStepSize calculation[END]
		progressDialogSettings(&progressDlg,baseRows/progressStepSize,tr("Data import"),tr("Data import in progress"));
		pkFieldName = cur[0]->fieldName(0); //Store the pkFieldName
		cout << pkFieldName << endl;
		for(register int itr = 0; itr < baseRows; ++itr)
		{
		    //cout << itr << endl;
		    bufIndex = 0;
		    if(itr%progressStepSize == 0)
		    {
			progressDlg->setProgress(++progress);
			progressDlg->setLabelText("Importing data from: " + fileName.section('/',-1) + "\n[Completed: " + QString::number(itr == 0  ? 1 : itr) + " of " + QString::number(baseRows) + " records]");
			progressDlg->show();
			//appQ->processEvents();
		    }

		    rowSet = fpIn.readLine(); //Read a single row seperated by tab
		    cellValueList.clear();
		    cellValueList = QStringList::split('\t',rowSet);
		    QStringList::Iterator itcV = cellValueList.begin();
		    keyValue = (*itcV).stripWhiteSpace();
		    //cout << "keyValue: " << keyValue << endl;
		    if(indexingRequired)
		    {
			++itcV;
			keyValue1 = (*itcV).stripWhiteSpace();
		    }

		    for(bufIndex = 0; bufIndex < Counter; ++bufIndex)
		    {
			if(cur[bufIndex])
			{
			    //Get the matching field name[START]
			    if(matchingColumn >= 0)
			    {
				dtab[bufIndex]->refresh();
				QHeader *head = dtab[bufIndex]->horizontalHeader();
				for(int i = 0; i <= matchingColumn; ++i)
				    matchingColumnName = head->label(i);
			    }
			    //Get the matching field name[STOP]
			    try
			    {
				if(matchingColumn >= 0)
				{
				    //cout << "matchingColumnName: " << matchingColumnName << endl;
				    cur[bufIndex]->select(matchingColumnName + "='" + (indexingRequired ? keyValue1 : keyValue) + "'");
				    if(cur[bufIndex]->next())
				    {
					buffer[bufIndex] = cur[bufIndex]->primeUpdate();
					mode[bufIndex] = 2;
				    }
				}
				else
				{
				    QSqlIndex pk = cur[bufIndex]->primaryIndex();
				    cur[bufIndex]->setValue(0,QVariant(keyValue));
				    if(cur[bufIndex]->select( pk, pk ))
				    {
					if(cur[bufIndex]->next())
					{
					    //cout << "\nUpdate\n";
					    buffer[bufIndex] = cur[bufIndex]->primeUpdate();
					    mode[bufIndex] = 2;
					}
					else
					{
					    //cout << "\nInsert\n";
					    buffer[bufIndex] = cur[bufIndex]->primeInsert();
					    buffer[bufIndex]->setValue(0,keyValue);
					    if(indexingRequired)
						buffer[bufIndex]->setValue(1,keyValue1);
					    mode[bufIndex] = 1;
					}
				    }
				}
			    }
			    catch(...)
			    {
				QMessageBox::information(this,tr("Import Error!"),tr("Cursor initiation problem, kindly report to the developer"));
				exit(0);
			    }
			}
			else
			{
			    QMessageBox::information(this,tr("Import Error!"),tr("Cursor allocation problem, kindly report to the developer"));
				exit(0);
			}
		    }
		    ++itcV;
		    for(itc = 0; itcV != cellValueList.end(); ++itcV,++itc)
		    {
			bufIndex = 0;
			while(colIndexArr[itc] > (tabIdArr[bufIndex] + 1)*(MAXCOLS + (indexingRequired ? 2 : 1)) - 1) //Indentify Table
			    ++bufIndex;
			startIndex = colIndexArr[itc] - tabIdArr[bufIndex]*(MAXCOLS + (indexingRequired ? 2 : 1));
			switch(mode[bufIndex])
			{
			case 1:
			    //cout << "\nINSERT: StartIndex: " << startIndex << endl;
			    buffer[bufIndex]->setValue(startIndex,(*itcV).stripWhiteSpace());
			    break;
			case 2:
			    //cout << "\nUPDATE: StartIndex: " << startIndex << ": " << cellValueList.count() << endl;
			    if((buffer[bufIndex]->value(startIndex)).isNull() || (buffer[bufIndex]->value(startIndex).toString() == ""))
			    {
				buffer[bufIndex]->setValue(startIndex,(*itcV).stripWhiteSpace());
				//cout << "\nUPDATE: StartIndex: " << startIndex << endl;
			    }
			    if((buffer[bufIndex]->value(startIndex).toString() != (*itcV).stripWhiteSpace()) && frmTableCreate::updateFlag == true)
				buffer[bufIndex]->setValue(startIndex,(*itcV).stripWhiteSpace());
			}
		    }

		    for(register int i = 0; i < Counter; ++i)
		    {
			switch(mode[i])
			{
			case 1:
			    if(cur[i]->canInsert())
			    {
				if(cur[i]->insert())
				    ++recsafter;
				else if(cur[i]->lastError().text())
				    cout << "Insert: " << cur[i]->lastError().text() << endl;
			    }
			    break;
			case 2:
			    if(cur[i]->canUpdate())
			    {
				if(cur[i]->update())
				    ++recsafter;
				else if(cur[i]->lastError().text())
				    cout << "Update: " << cur[i]->lastError().text() << endl;
			    }
			}
			sqlDB->commit();
			buffer[i]->clearValues(TRUE);
		    }
		    //cout << "\nCompleted: " << itr + 1 << " of " << baseRows << endl;
		    //This portion to insert new 'rs' values in old tables which are not under current updation[START]
		    /*int startIndex = Counter;
		    for(QStringList::Iterator it = frmTableCreate::oldTables.begin(); it != frmTableCreate::oldTables.end(); ++it,++startIndex)	
		    {
			query = cur[startIndex]->fieldName(0) + "=" + keyValue;
			cur[startIndex]->select(query);
			if(!cur[startIndex]->next())
			{
			    buffer[startIndex] = cur[startIndex]->primeInsert();
			    buffer[startIndex]->setValue(0,keyValue);
			    if(!cur[startIndex]->insert())
			    {
				if(cur[startIndex]->lastError().text())
				    cout << "\nERROR: " << cur[startIndex]->lastError().text() << endl;
			    }
			}
		    }*/
		    //This portion to insert new 'rs' values in old tables which are not under current updation[END]
		}
		progressDlg->close();
		//for(register int i = 0; i < (int)columnList.count() - 1; ++i)
		  //  cout << colIndexArr[i] << "\t";
		//cout << endl;
		fpRead.close();
		if(sqlDB->commit())
		    cout << "\nTransaction Commit\n";
		for(register int i = 0; i < Counter; ++i)
		{
		    if(!specialFlag)
		    {
			dtab[i]->refresh();
			dtab[i]->show();
		    }
		}
		//~~~~~~~~~~This portion of code deallocate cursors and buffers and mode array~~~~~~~[SATART]
		//Counter += frmTableCreate::oldTables.count();
		for(register int i = 0; i < Counter; ++i)
		    cur[i]->clear();
		delete [] buffer;
		delete [] cur;
		delete [] mode;
		//~~~~~~~~~~This portion of code deallocate cursors and buffers~~~~~~~[STOP]
		/*if(runInSilentMode)
		{
		    for(QStringList::Iterator it = frmTableCreate::oldTableAffect.begin(); it != frmTableCreate::oldTableAffect.end(); ++it)
		    {
			query = "SELECT * FROM " + *it + ";";
			cout << query << endl;
			QSqlQuery squery(query);
			cout << "\nVV: " << squery.numRowsAffected() << endl;
			if(squery.numRowsAffected()) //This portion is to avoid problem with last record insertion
			    recsInserted = squery.size();
		    }
		}
		else*/
		recsInserted = (recsafter/Counter < baseRows ? baseRows : recsafter/Counter);
		importMethod  = 2;
	    }
	}
	delete [] tabIdArr;
    }
}	

/////////Overloading standard event classes[START]
/*void frmgeno::keyPressEvent(QKeyEvent *e)
{
    e->accept();
    if(e->key() == Qt::Key_Delete)
	deleteTableContents();
}*/

void frmgeno::timerEvent(QTimerEvent *e)
{
    //DataTable Closing Management[START]
    QWidgetList windows = wGeno->windowList();
    if(windows.count() > 1) 
    {
	for(register int i = 1; i < int(windows.count()); ++i) 
	{
	    QWidget *window = windows.at(i);
	    if(window->isHidden())
	    {
		if(window->name() == QString("DataTable"))
		{
		    QSqlCursor *cursor = ((QDataTable *)window)->sqlCursor();
		    cursor->clear();
		    delete cursor;
		    cursor = NULL;
		    window->close();
		    delete window;
		}
		else if(QString((window->name())).contains(QString("DataTable_")) > 0)
		{
		    int viewIndex;
		    for(viewIndex = 0; viewIndex < MAXVIEWS; ++viewIndex)
		    {
			if(openFlag[viewIndex] && !tabView[viewIndex]->isShown())
			{
			    delete tabView[viewIndex];
			    tabView[viewIndex] = 0;
			    viewCur[viewIndex]->clear();
			    delete viewCur[viewIndex];
			    viewCur[viewIndex] = 0;
			    openFlag[viewIndex] = false;
			    break;
			}
		    }
		}
		window = 0;
	    }
	}
    }
    //DataTable Closing Management[STOP]
    //Table Management[START]
    int delIndex = -1;
    for(int i = 0; i < tabArrIndex; ++i)
    {
	if(tabArr[i] != 0 && tabArr[i]->isHidden())
	{
	    //delete [] tabArr[i];
	    tabArr[i] = 0;
	    tabArrFlag[i] = false;
	    delIndex = i;
	}
    }
    
    if(delIndex > -1)
    {
	if(delIndex < tabArrIndex - 1)
	{
	    for(int i = delIndex + 1; i < tabArrIndex; ++i)
	    {
		tabArr[i - 1] =  tabArr[i];
		tabArr[i] = 0;
	    }
	}
	--tabArrIndex;
    }
    
    if(tabArrIndex > 0)
    {
	for(int index = 0; index < tabArrIndex; ++index)
	{
	    if(tabArr[index]->hasMouse())
	    {
		activeTableID = index;
		break;
	    }
	}
    }
    //Table Management[STOP]

    if(lv->isMaximized())
	lv->showNormal();

    if(dataanalyzer)
    {
	if(dataanalyzer->isShown())
	{
	    serviceStartActionDb->setEnabled(false);
	    serviceStopActionDb->setEnabled(false);
	    createActionDb->setEnabled(false);
	    connectActionDb->setEnabled(false);
	    closeActionDb->setEnabled(false);
	    createActionGeno->setEnabled(false);
	    tabrelateActionGeno->setEnabled(false);
	    dataAnalyzerActionGeno->setEnabled(false);
	    mergeActionGeno->setEnabled(false);
	    fnwizardActionGeno->setVisible(false);
	    settingsGeno->setItemEnabled(0,false);
	    settingsGeno->setItemEnabled(1,false);
	    lv->setEnabled(false);
	}
	else
	{
	    serviceStartActionDb->setEnabled(true);
	    serviceStopActionDb->setEnabled(true);
	    createActionDb->setEnabled(true);
	    connectActionDb->setEnabled(true);
	    closeActionDb->setEnabled(true);
	    createActionGeno->setEnabled(true);
	    tabrelateActionGeno->setEnabled(true);
	    dataAnalyzerActionGeno->setEnabled(true);
	    mergeActionGeno->setEnabled(true);
	    fnwizardActionGeno->setVisible(false);
	    settingsGeno->setItemEnabled(0,true);
	    settingsGeno->setItemEnabled(1,true);
	    lv->setEnabled(true);
	    dataanalyzer = 0;
	}
    }
    
    if(tabRel)
    {
	if(tabRel->isShown())
	{
	    serviceStartActionDb->setEnabled(false);
	    serviceStopActionDb->setEnabled(false);
	    createActionDb->setEnabled(false);
	    connectActionDb->setEnabled(false);
	    closeActionDb->setEnabled(false);
	    createActionGeno->setEnabled(false);
	    tabrelateActionGeno->setEnabled(false);
	    dataAnalyzerActionGeno->setEnabled(false);
	    mergeActionGeno->setEnabled(false);
	    fnwizardActionGeno->setVisible(false);
	    settingsGeno->setItemEnabled(0,false);
	    settingsGeno->setItemEnabled(1,false);
	    lv->setEnabled(false);
	}
	else
	{
	    serviceStartActionDb->setEnabled(true);
	    serviceStopActionDb->setEnabled(true);
	    createActionDb->setEnabled(true);
	    connectActionDb->setEnabled(true);
	    closeActionDb->setEnabled(true);
	    createActionGeno->setEnabled(true);
	    tabrelateActionGeno->setEnabled(true);
	    dataAnalyzerActionGeno->setEnabled(true);
	    mergeActionGeno->setEnabled(true);
	    fnwizardActionGeno->setVisible(false);
	    settingsGeno->setItemEnabled(0,true);
	    settingsGeno->setItemEnabled(1,true);
	    lv->setEnabled(true);
	    tabRel = 0;
	}
    }
}

void frmgeno::closeEvent(QCloseEvent *e)
{
    destroy();
    //system("/sbin/service mysqld stop");
    e->accept();
}
/////////Overloading standard event classes[STOP]

void frmgeno::destroy()
{
  
}

void frmgeno::destroy_WhenDatabaseConnectionIsClosed()
{
//Listview Items [Start]
delete dRoot;
dRoot = NULL;
delete dbase;
dbase = NULL;
//Listview Items Related[Stop]
}
