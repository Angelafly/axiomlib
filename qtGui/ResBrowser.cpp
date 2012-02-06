/****************************************************************************
*					������� ������ ResBrowser.h
*
*	Description:	������� ���� � ������������ ����������� ������������� - ����������
*	Author:			dk
*	History:	
*
****************************************************************************/
#include "ResBrowser.h"

// ��������� ������ � ������ ��������� ������
#define str_axiomSet_ending		".as"
#define str_dataFile_ending		".csv"
#define str_markUpFile_start	"mu_"
#define str_no_table_data		"n/a"

// ���������� ��� ��������� ����� ��� Unix
#define str_defCsvProc		"oocalc"

// �������� ������������ �����, ����������� ��� ������������ ����������� ������
#define str_showRowProc		"show_rows"

// ��������� ����������� ������
#define str_alg_win_end		".exe"
#define str_alg_unix_end	".sh"

// ����������� ����������� - ���� > 0 - �� ��� ������ ����� ������ �������������� ���� � ��������� ������� ������.
#define	check_axiomSet_file_presence 1


/****************************************************************************
*					ResBrowser::ResBrowser
*
*	Description:	����������� ������, ������ ����������� ��������� ���������
*					� ����������� ������� ���
*	Parameters:		parent - ������, �� �������� ������ ����� ��� �����������
*					flags - ��������� ������� ����, ��� ������� QT
*	Returns:		-
*	Author:			dk
*	History:		
*
****************************************************************************/
ResBrowser::ResBrowser(const std::string pathToDir) {
	axiomSetDirs.clear();
	pathToResults = pathToDir;

	setupUi(this);

	refreshForm ();	

	connect( comboBox_axiomSets, SIGNAL( activated( int ) ), this, SLOT( loadContents( int ) ) );
	connect( tableWidget_markUps, SIGNAL( cellActivated(int , int ) ), this, SLOT( muCellCalled(int , int ) ) );
	connect( tableWidget_results, SIGNAL( cellActivated(int , int ) ), this, SLOT( resCellCalled(int , int ) ) );
	connect( pushButton_refresh, SIGNAL( clicked() ), this, SLOT( refreshForm() ) );
	connect( pushButton_showRow, SIGNAL( clicked() ), this, SLOT( showRow() ) );
	connect( pushButton_showResults, SIGNAL( clicked() ), this, SLOT( showResults() ) );

}


/****************************************************************************
*					ResBrowser::~ResBrowser
*
*	Description:	������ ���������� ������
*	Parameters:		-
*	Returns:		-
*	Author:			dk
*	History:		
*
****************************************************************************/
ResBrowser::~ResBrowser() {

}


/****************************************************************************
*					ResBrowser::refreshForm
*
*	Description:	�������, ���������� ���������� ����������� ���� ����� 
*					������ ������� �� ����������� ���������� � ������������
*	Parameters:		-
*	Returns:		-
*	Author:			dk
*	History:		
*
****************************************************************************/
void ResBrowser::refreshForm () {
	clearForm();
	findAxiomSets (pathToResults);
	loadContents (0);
}


/****************************************************************************
*					ResBrowser::muCellCalled
*
*	Description:	������� �������� �������� ����� � ��������� ��������� 
*					��������� �������� ��������� �������� ������
*	Parameters:		row - ����� ���� ���������� ����� � ������� � ������� �������� 
*					column - ����� ������� ���������� �����
*	Returns:		-
*	Author:			dk
*	History:		
*
****************************************************************************/
void ResBrowser::muCellCalled (int row, int column) {
	std::map <int, std::string>::const_iterator c_iter;
	c_iter = axiomSetDirs.find (comboBox_axiomSets->currentIndex());
	if (c_iter == axiomSetDirs.end())
		return;
	std::string str;
	str = c_iter->second;
	str.append ("/");
	QLabel *item;
	item = (QLabel *) tableWidget_markUps->cellWidget(row, column);
	str.append (item->text().toStdString());
	openFile (str);
}


/****************************************************************************
*					ResBrowser::resCellCalled
*
*	Description:	������� �������� �������� ����� � ��������� �����������
*					������������� ����������� ����
*	Parameters:		row - ��� ���������� ��� �������� ����� � ������� � ������� �����������
*					column - ������� ���������� ��� �������� �����
*	Returns:		-
*	Author:			dk
*	History:		
*
****************************************************************************/
void ResBrowser::resCellCalled (int row, int column) {
	std::map <int, std::string>::const_iterator c_iter;
	c_iter = axiomSetDirs.find (comboBox_axiomSets->currentIndex());
	if (c_iter == axiomSetDirs.end())
		return;
	std::string str;
	str = c_iter->second;
	str.append ("/");
	QLabel *item;
	item = (QLabel *) tableWidget_results->cellWidget(row, column);
	str.append (item->text().toStdString());
	openFile (str);	
}


/****************************************************************************
*					ResBrowser::openFile
*
*	Description:	������� �������� �������� ���������� ����� � ����������� ��� ���������
*	Parameters:		path - ���� � ������������ �����
*	Returns:		-
*	Author:			dk
*	History:		
*
****************************************************************************/
void ResBrowser::openFile (std::string &path) {
	#ifdef Q_OS_WIN32
		ShellExecuteA(NULL, "open", (char*) path.c_str(), NULL, NULL, SW_SHOWNORMAL);
	#else
		// �������� ��������
		QProcess *process = new QProcess;
		
		QStringList args;
		args.clear();
		args << (QString) path.c_str();
		
		// ������ ��������
		process->start (str_defCsvProc, args);
	#endif
}


/****************************************************************************
*					ResBrowser::clearForm
*
*	Description:	������� ������� ���������� ����� ����
*	Parameters:		-
*	Returns:		-
*	Author:			dk
*	History:		
*
****************************************************************************/
void ResBrowser::clearForm () {
	axiomSetDirs.clear();
	comboBox_axiomSets->clear();
	clearTableFields();
	setFormEnabledOrNot();
}


/****************************************************************************
*					ResBrowser::clearTableFields
*
*	Description:	������� ������� ���������� ����� ������ � ������� �������� � ����������
*	Parameters:		-
*	Returns:		-
*	Author:			dk
*	History:		
*
****************************************************************************/
void ResBrowser::clearTableFields() {
	tableWidget_markUps->clearContents();
	int size = tableWidget_markUps->rowCount();
	for (int j = 0; j < size; j++)
		tableWidget_markUps->removeRow (0);
	
	tableWidget_results->clearContents();
	int size2 = tableWidget_results->rowCount();
	for (int j = 0; j < size2; j++)
		tableWidget_results->removeRow (0);

}


/****************************************************************************
*					ResBrowser::setFormEnabledOrNot
*
*	Description:	������� ������ ���� �����������, ���� � ��� �� ���������� 
*					����������� �� �� ����� ������� ������
*	Parameters:		-
*	Returns:		-
*	Author:			dk
*	History:		
*
****************************************************************************/
void ResBrowser::setFormEnabledOrNot() {
	if (comboBox_axiomSets->count() > 0)
		comboBox_axiomSets->setEnabled(true);
	else
		comboBox_axiomSets->setEnabled(false);
	if (tableWidget_markUps->rowCount() > 0)
		tableWidget_markUps->setEnabled(true);
	else
		tableWidget_markUps->setEnabled(false);
	if (tableWidget_results->rowCount() > 0)
		tableWidget_results->setEnabled(true);
	else
		tableWidget_results->setEnabled(false);
}


/****************************************************************************
*					ResBrowser::findAxiomSets
*
*	Description:	������� ������ ���������� � ������� �������� ����������� 
*					������ ������.
*	Parameters:		pathToDir - ���� � ����������, � ������� ������ ����������
*						�� �������� ������
*	Returns:		-
*	Author:			dk
*	History:		
*
****************************************************************************/
void ResBrowser::findAxiomSets(const std::string pathToDir) {
	// ������� ���� ��� ������ ������ ������
	boost::filesystem::path fullPath( pathToDir );
	
	std::string fileNameLastHalf (str_axiomSet_ending); // ������ - �������� ������ ������������ ����� ������ � ���������� ������ ��� ������� ��������������
	std::string curFileName, curDirName; // ��������� ����������, �������� ��� �������� ������������ � ������������ ����� 
	
	// �������� - ���������� �� �������� ���� � ��������
	if ( !boost::filesystem::exists( fullPath ) ) {
		clearForm ();
		return;
	}
		
	// ��������� - ���������� �� ������ ����
	if ( boost::filesystem::is_directory( fullPath ) ) {
		boost::filesystem::directory_iterator end_iter;
		for ( boost::filesystem::directory_iterator dir_itr( fullPath ); dir_itr != end_iter; ++dir_itr ) {
			if ( boost::filesystem::is_directory( *dir_itr ) ) {
				curDirName = dir_itr->leaf();
				/*std::string::size_type ind = curDirName.find_first_not_of ("0123456789");
				if ((ind != std::string::npos) && (ind != 0))
					ind += 1;
				else 
					continue;
				curDirName = curDirName.substr (ind); // ������ � curDirName ���������� �������������� ��� ������� ������
				*/
				// ���� ���� � ��������� ������� ������
				#if check_axiomSet_file_presence
				for ( boost::filesystem::directory_iterator file_itr( fullPath ); file_itr != end_iter; ++file_itr ) {
					curFileName = file_itr->leaf();
					if (curFileName.find(fileNameLastHalf) == (curFileName.size() - fileNameLastHalf.size())) {
						// ������ ������� ���� - ��� ���� � ��������� ������� ������ 
						curFileName = curFileName.substr(0, curFileName.size() - fileNameLastHalf.size());
						if (curDirName == curFileName) {
							addAxiomSetResults (dir_itr->string(), curDirName);
						}
					}
				}
				#else
				addAxiomSetResults (dir_itr->string(), curDirName);
				#endif
        	}
		}
	} else {
		clearForm ();
	}
	setFormEnabledOrNot();
}


/****************************************************************************
*					ResBrowser::addAxiomSetResults
*
*	Description:	������� ��������� �� ���������� ������������� ������� ������
*	Parameters:		axiomSetPath - ��������� ���� � ����������� �� ������� ������
*					axiomSetName - ��� �������������� ������� ������
*	Returns:		-
*	Author:			dk
*	History:		
*
****************************************************************************/
void ResBrowser::addAxiomSetResults (std::string axiomSetPath, std::string &axiomSetName) {
	QString str("");
	QTextStream(&str) << comboBox_axiomSets->count() + 1 << ". ";
	str.append(axiomSetName.c_str());
	comboBox_axiomSets->addItem (str);
	axiomSetDirs[comboBox_axiomSets->count() - 1] = axiomSetPath;
}


/****************************************************************************
*					ResBrowser::loadContents
*
*	Description:	������� ���������� ��� ����� � ���������� � ������������ 
*					��� ��������� ������� ������
*	Parameters:		index - ����� ������� ������ � axiomSetDirs
*	Returns:		-
*	Author:			dk
*	History:		
*
****************************************************************************/
void ResBrowser::loadContents (int index) {
	// �������� ����������� ����� ��� ������ �������
	clearTableFields();
	
	// ����� ������ ��� �������� ���������� �� ��������� ������� ������
	std::map <int, std::string>::const_iterator c_iter;
	c_iter = axiomSetDirs.find(index);
	if (c_iter == axiomSetDirs.end())
		return;

	// ���������� � ����������� ����������� � �����
	boost::filesystem::path path( c_iter->second );

	std::string fileLastHalf (str_dataFile_ending);
	std::string muFileFirstHalf (str_markUpFile_start);
	std::string curFileName; // ��������� ����������, �������� ��� �������� ������������ � ������������ ����� 
	
	// �������� - ���������� �� �������� ���� � ��������
	if ( !boost::filesystem::exists( path ) )
		return;
	// ��������� - ���������� �� ������ ����
	if ( !boost::filesystem::is_directory( path ) )
		return;

	// �������� �� ���������� � ����������� ����������
	int curRow = 0;
	boost::filesystem::directory_iterator end_iter;
	for ( boost::filesystem::directory_iterator file_itr( path ); file_itr != end_iter; ++file_itr ) {
		if ( boost::filesystem::is_directory (*file_itr) )
			continue;
		curFileName = file_itr->leaf();
		if (curFileName.find(fileLastHalf) == (curFileName.size() - fileLastHalf.size())) {
			// ������ ���� ������������ ��� ������ ���� � ������
			if (curFileName.find(muFileFirstHalf) == 0) {
				// ������ ��� ���� � ��������� ��������� ����������
				curRow = tableWidget_markUps->rowCount();
				tableWidget_markUps->insertRow(curRow);
				tableWidget_markUps->setCellWidget (curRow,0,new QLabel(curFileName.c_str()));
			} else {
				// ������ ��� ���� � ��������� �������� �����
				curRow = tableWidget_results->rowCount();
				tableWidget_results->insertRow(curRow);
				tableWidget_results->setCellWidget (curRow,0,new QLabel(curFileName.c_str()));
			}
		}
	}
	setFormEnabledOrNot();
}


/****************************************************************************
*					ResBrowser::showRow
*
*	Description:	������� �������� ����������� ����������� ��������� ����������
*	Parameters:		-
*	Returns:		-
*	Author:			dk
*	History:		
*
****************************************************************************/
void ResBrowser::showRow () {
	int curCol = -1;
	int curRow = -1;
	curCol = tableWidget_results->currentColumn();
	curRow = tableWidget_results->currentRow();
	if ((curCol < 0)|| (curRow < 0))
		return;
	std::map <int, std::string>::const_iterator c_iter;
	c_iter = axiomSetDirs.find (comboBox_axiomSets->currentIndex());
	if (c_iter == axiomSetDirs.end())
		return;
	std::string str = c_iter->second;
	str.append ("/");
	QLabel *item;
	item = (QLabel *) tableWidget_results->cellWidget(curRow, curCol);
	str.append (item->text().toStdString());
	// ������ � str ���������� ������ ���� � csv-�����, ������ �� �������� ���������� ����������
	showFile (str, 0);
}


/****************************************************************************
*					ResBrowser::showResults
*
*	Description:	������� �������� ����������� ����������� ����������� �������������
*	Parameters:		-
*	Returns:		-
*	Author:			dk
*	History:		
*
****************************************************************************/
void ResBrowser::showResults () {
	int curCol = -1;
	int curRow = -1;
	curCol = tableWidget_results->currentColumn();
	curRow = tableWidget_results->currentRow();
	if ((curCol < 0)|| (curRow < 0))
		return;
	std::map <int, std::string>::const_iterator c_iter;
	c_iter = axiomSetDirs.find (comboBox_axiomSets->currentIndex());
	if (c_iter == axiomSetDirs.end())
		return;
	std::string str = c_iter->second;
	str.append ("/");
	QLabel *item;
	item = (QLabel *) tableWidget_results->cellWidget(curRow, curCol);
	str.append (item->text().toStdString());
	// ������ � str ���������� ������ ���� � csv-�����, ������ �� �������� ���������� ����������
	showFile (str, 1);
}


/****************************************************************************
*					ResBrowser::showFile
*
*	Description:	������� ��������� ����������� ����������� ������ �� �����.
*	Parameters:		path - ���� � �����, ������ �� �������� ���������� ����������
*					whatToShow - ���� 0, �� ������������ ���� ����������,
*								 ���� 1, �� ������������ ���������� �������������
*	Returns:		-
*	Author:			dk
*	History:		
*
****************************************************************************/
void ResBrowser::showFile (std::string &path, int whatToShow) {
	QString tmpStr;
	QTextStream(&tmpStr) << whatToShow;
	#ifdef Q_OS_WIN32
		std::string exeFile (str_showRowProc);
		exeFile.append (str_alg_win_end);
		path.append (" ");
		path.append (tmpStr.toStdString());
		ShellExecuteA(NULL, "open", (char*) exeFile.c_str(), (char*) path.c_str(), NULL, SW_SHOWNORMAL);
	#else
		// �������� ��������
		QProcess *process = new QProcess;
		
		QStringList args;
		args.clear();
		args << (QString) "arspds";
		args << (QString) path.c_str();
		args << tmpStr;
		
		std::string exeFile ("./");
		exeFile.append (str_showRowProc);
		exeFile.append (str_alg_unix_end);
		// ������ ��������
		process->start (exeFile.c_str(), args);
	#endif
}
