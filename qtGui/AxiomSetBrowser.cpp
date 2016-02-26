/****************************************************************************
*					������� ������ AxiomSetBrowser.h
*
*	Description:	������� ���� � ��������� ������ ������ - ����������
*	Author:			dk
*	History:	
*
****************************************************************************/
#include "AxiomSetBrowser.h"

// ��������� �������� ������
#define str_axiomSet_ending	".as"
#define str_axiomSet_stat_ending	".stat"

// ����������� ��� ������� � ��������� ������ ������
#define str_no_data		"n/a"
#define int_column_with_axiomSet	1


/****************************************************************************
*					AxiomSetBrowser::AxiomSetBrowser
*
*	Description:	����������� ������, ������ ����������� ��������� ���������
*					� ����������� ������� ���
*	Parameters:		pathToAxiomSets - ������ � ����� � ����������, � ������� 
*						���������� �������� ������ ������
*	Returns:		-
*	Author:			dk
*	History:		
*
****************************************************************************/
AxiomSetBrowser::AxiomSetBrowser(const std::string pathToAxiomSets) {
	assVec.clear();
	axiomSetsPath = pathToAxiomSets;

	setupUi(this);

	refreshForm();

	connect( tableWidget, SIGNAL( cellActivated(int , int ) ), this, SLOT( cellCalled(int , int ) ) );
	connect( pushButton_refresh, SIGNAL( clicked() ), this, SLOT( refreshForm() ) );
}


/****************************************************************************
*					AxiomSetBrowser::~AxiomSetBrowser
*
*	Description:	������ ���������� ������
*	Parameters:		-
*	Returns:		-
*	Author:			dk
*	History:		
*
****************************************************************************/
AxiomSetBrowser::~AxiomSetBrowser() {	

}


/****************************************************************************
*					AxiomSetBrowser::refreshForm
*
*	Description:	������� �������� ���������� ������� � ��������� ������ ������
*	Parameters:		-
*	Returns:		-
*	Author:			dk
*	History:		
*
****************************************************************************/
void AxiomSetBrowser::refreshForm() {
	clearTable();
	findAxiomSets (axiomSetsPath);
}


/****************************************************************************
*					AxiomSetBrowser::cellCalled
*
*	Description:	������� ��������� ����� ������� � ������� ������ �� �� 
*					��������� � ������� � ��������� ������ ������ � �������� 
*					�������, ������� �������� ����� ���� ����������� �������.
*	Parameters:		row - ���, ���������� ��������� �������, � ������� � ��������� ������ ������
*					column - �������, ���������� ��������� �������, � ������� � ��������� ������ ������
*	Returns:		-
*	Author:			dk
*	History:		
*
****************************************************************************/
void AxiomSetBrowser::cellCalled (int row, int /*column*/) {
	int curCol = tableWidget->currentColumn();
	int curRow = tableWidget->currentRow();
	if (curCol == int_column_with_axiomSet) {
		if ((curRow >= 0) && (curRow < (int) assVec.size()))
			showAxiomDescription (assVec[curRow], row);
	}
}


/****************************************************************************
*					AxiomSetBrowser::showAxiomDescription
*
*	Description:	������� �������� ���� � ������������ ��������� � ����������
*					��������� ������� 
*	Parameters:		ass - ��������� ������� ������, � ������� ���������� ��������� �������
*					axiomNum - ����� ��������� ������� � ������� ������
*	Returns:		-
*	Author:			dk
*	History:		
*
****************************************************************************/
void AxiomSetBrowser::showAxiomDescription (AxiomSetStructure &ass, const int axiomNum) const {
	if ((axiomNum < 0) || (axiomNum >= (int) ass.axioms.size()))
		return;
	AxiomBrowser* axBrowser;
	axBrowser = new AxiomBrowser(ass.axioms[axiomNum]);
	axBrowser->show();
}


/****************************************************************************
*					AxiomSetBrowser::clearTable
*
*	Description:	������� ������� ���������� ������� � ���������� ������ ������
*	Parameters:		-
*	Returns:		-
*	Author:			dk
*	History:		
*
****************************************************************************/
void AxiomSetBrowser::clearTable () {
	tableWidget->clearContents();
	int size = tableWidget->rowCount();
	for (int j = 0; j < size; j++)
		tableWidget->removeRow (0);
	assVec.clear();
}


/****************************************************************************
*					AxiomSetBrowser::findAxiomSets
*
*	Description:	������� ������ ���������� � ������� ������ � ��������� 
*					������ ������
*	Parameters:		pathToAxiomSets - ���� � ����������, ���������� ������������ 
*						������� ������
*	Returns:		-
*	Author:			dk
*	History:		
*
****************************************************************************/
void AxiomSetBrowser::findAxiomSets(const std::string pathToAxiomSets) {
	// ������� ���� ��� ������ ������ ������
	boost::filesystem::path fullPath( pathToAxiomSets );
	
	std::string fileNameLastHalf (str_axiomSet_ending); // ������ - �������� ������ ������������ ����� ������ � ���������� ������ ��� ������� ��������������
	std::string curFileName; // ��������� ����������, �������� ��� �������� ������������ � ������������ ����� 
	
	// �������� - ���������� �� �������� ���� � ��������
	if ( !boost::filesystem::exists( fullPath ) ) {
		return;
	}
	
	// ��������� - ���������� �� ������ ����
	if ( boost::filesystem::is_directory( fullPath ) ) {
		boost::filesystem::directory_iterator end_iter;
		for ( boost::filesystem::directory_iterator dir_itr( fullPath ); dir_itr != end_iter; ++dir_itr ) {
			if ( !boost::filesystem::is_directory( *dir_itr ) ) {
								curFileName = dir_itr->path().filename().c_str();
				if (curFileName.find(fileNameLastHalf) == (curFileName.size() - fileNameLastHalf.size())) {
					// ������ ����� ����������� ���� - ������������ ��� ����������
					curFileName = (*dir_itr).path().string();
					addAxiomSet (curFileName);
				}
        	}
		}
	}

}


/****************************************************************************
*					AxiomSetBrowser::addAxiomSet
*
*	Description:	������� ��������� �������� ������� ������ � ��������������� 
*					������� � ������ ����. ��� ���� ��� ������������� - ����������� 
*					���� � ��������� ���������� �� ������ ������� ������.
*	Parameters:		path - ���� � ����� � ��������� ����������� ������� ������
*	Returns:		-
*	Author:			dk
*	History:		
*
****************************************************************************/
void AxiomSetBrowser::addAxiomSet (std::string &path) {
	AxiomSetStructure ass;
	// ��������� ���������� ����� � ���������� ��������� ��������
	if (readFromAS (path, ass) >= 0) {
		int curRow = tableWidget->rowCount();
		if (tableWidget->columnCount() < 2)
			return; // ������� � �������� ����������
		// ��������� ��������� �������� �� ���������� ����������
		assVec.push_back(ass);
		// ������� ����� ������ ��� ����������� ��������� ������� ������
		tableWidget->insertRow(curRow);
		tableWidget->setCellWidget (curRow,0,new QLabel(ass.axiomSetName.c_str()));

		QTableWidget *newTab = new QTableWidget (ass.axioms.size(), 1);
		QStringList names;
		names.clear();
		names << "Axiom Name";
		newTab->setHorizontalHeaderLabels(names);
		for(unsigned int i = 0; i < ass.axioms.size(); i++) {
			newTab->setCellWidget (i,0,new QLabel (ass.axioms[i].axiomName.c_str()));
		}
		newTab->adjustSize();
		connect( newTab, SIGNAL( cellActivated(int , int ) ), this, SLOT( cellCalled(int , int ) ) );
		tableWidget->setCellWidget (curRow, 1, newTab);
		// ����������� ���������� �� ������� ������
		if (tableWidget->columnCount() >= 6) {
			// ���������� ����� �� �����������
			std::string asEnding (str_axiomSet_ending);
			std::string statFile = path.substr (0, path.size() - asEnding.size());
			statFile.append(str_axiomSet_stat_ending);
			Environment env;
			env.readFile(statFile);
			// ����������� ��������
			double dbl;
			QString qstr;
			if ((env.getDoubleParamValue(dbl, "axiomSet", "firstError") >= 0) && (dbl >= 0.0)) {
				qstr.clear();
				QTextStream(&qstr) << dbl;
				tableWidget->setCellWidget (curRow,3,new QLabel (qstr));
			} else {
				QLabel *label = new QLabel (str_no_data);
				label->setEnabled (false);
				tableWidget->setCellWidget (curRow,3,label);
			}
			if ((env.getDoubleParamValue(dbl, "axiomSet", "secondError") >= 0) && (dbl >= 0.0)) {
				qstr.clear();
				QTextStream(&qstr) << dbl;
				tableWidget->setCellWidget (curRow,2,new QLabel (qstr));
			} else {
				QLabel *label = new QLabel (str_no_data);
				label->setEnabled (false);
				tableWidget->setCellWidget (curRow,2,label);
			}
			if ((env.getDoubleParamValue(dbl, "axiomSet", "anomCount") >= 0) && (dbl >= 0.0)) {
				qstr.clear();
				QTextStream(&qstr) << dbl;
				tableWidget->setCellWidget (curRow,4,new QLabel (qstr));
			} else {
				QLabel *label = new QLabel (str_no_data);
				label->setEnabled (false);
				tableWidget->setCellWidget (curRow,4,label);
			}
			if ((env.getDoubleParamValue(dbl, "axiomSet", "normCount") >= 0) && (dbl >= 0.0)) {
				qstr.clear();
				QTextStream(&qstr) << dbl;
				tableWidget->setCellWidget (curRow,5,new QLabel (qstr));
			} else {
				QLabel *label = new QLabel (str_no_data);
				label->setEnabled (false);
				tableWidget->setCellWidget (curRow,5,label);
			}
		}
	}
}


/****************************************************************************
*					AxiomSetBrowser::readFromAS
*
*	Description:	������� ��������� �� ����� �������� ������� ������ �� 
*					���������� �������������
*	Parameters:		filePath - ���� � ����� � ��������� ������� ������
*					ass - ����������� �� ����� ��������� ������� ������
*	Returns:		<0 - ���� � ���� ���������� ��������� ��������� ������
*					0 - ���� ��������� ���� ������� ���������
*	Author:			dk
*	History:		
*
****************************************************************************/
int AxiomSetBrowser::readFromAS (std::string &filePath, AxiomSetStructure &ass) {
	std::string str, curFile;
	int i = 0;

	ass.axioms.clear();
	
	// ��������� ����
	boost::filesystem::ifstream file( filePath );

	boost::char_separator<char> sep(" :;	");
	
	if (i = std::getline(file, str) && i != EOF) {
		// ������ ��� ������� ������ � ���������� � ��������� �����
		token_separator tokens(str, sep);
		token_separator::iterator tok_name = tokens.begin();
		curFile.assign(*tok_name);
		if (filePath.find (*tok_name) == std::string::npos) {
			// ��� ������� ���������� - � �������� ����� � � ��� ���������� - ������ ������
			file.close();
			return -1;
		}
		// ���������� ��� �����
		ass.axiomSetName = *tok_name;
	} else {
		// ���� ����
		file.close();
		return -2;
	}
	
	if (i = std::getline(file, str) && i != EOF) {
		// ������ ��� ����� ������ ������������ � ������ �������
		token_separator tokens(str, sep);
		token_separator::iterator tok_name = tokens.begin();
		if ((*tok_name).empty()) {
			// ��� ����� ����� ������
			file.close();
			return -3;
		}		
		ass.axiomBankDir = *tok_name;
	}
	else {
		// ���� ���������� ����������
		file.close();
		return -4;
	}
	
	// ����������� ��������� ���������� ����� ������ � ��������� ����������� �������
	int currentAxiomNumber = 0;
	if (i = std::getline(file, str) && i != EOF)  {
		// ���������, ������� ����� ��������� � ���������� � ass
		AxiomStructure as;
		as.axiomParameters.clear();
		as.ecNames.clear();
		// ��������� ��������� ��� �������� ����� ������� � ����� ��������� �������
		std::string currentParam;
		std::string currentEC;
		std::map<std::string, std::string> readParams;
		
		int toSave = 0; // ���� ������������� ��������� ��������� �������
		int condition = 0;
		while (condition == 0) {
			// ��������� ������ �� ������
			token_separator tokens(str, sep);
			token_separator::iterator tok_iter = tokens.begin();
			if ( tok_iter == tokens.end()) {
				// ������ ������
				if (i = std::getline(file, str) && i != EOF) {
					// ������������ ��������� ������
					continue;
				} else {
					// ���� ����
					condition = 1;
					break;
				}
			}
			const char* tempNumber = (*tok_iter).c_str();
			// ������� - ����������� ����� �������
			if (atof(tempNumber) > 0) {
				// ������ �������� �������
				if (toSave > 0) {
					if (!currentEC.empty()) {
						as.axiomParameters[currentEC] = readParams;
					}
					// ��������� ����� ���������� ������� 
					(ass.axioms).push_back(as);
				}
				++tok_iter;
				if ( tok_iter == tokens.end()) {
					// ������ ����� ������������
					file.close();
					return -5;
				}
				currentAxiomNumber++;
				toSave = 1;
				as.axiomName = *tok_iter;
				as.axiomParameters.clear();
				as.ecNames.clear();
				currentParam = "";
				currentEC = "";
				if (i = std::getline(file, str) && i != EOF) { 
					// ����� �������� ������� ���-�� ���� - ���� �� ����� � ������� �����
					continue;
				}
				else { 
					// ����� �������� ������� ������ ��� - ����� 1 �������
					condition = 1;
					break;
				}
			} else {
				// ������� - ������ ������������ �������, ��� ��������� � ��������
				if (*tok_iter == currentEC) {
					//������ ���������� ���������� ���������� ������� ������������� �������
				} else {
					if (!currentEC.empty()) {
						as.axiomParameters[currentEC] = readParams;
					}
					currentEC = *tok_iter;
					readParams.clear();
					as.ecNames.push_back (*tok_iter);
				}
				++tok_iter;
				if ( tok_iter == tokens.end()) {
					// ������ ����� ������������
					file.close();
					return -6;
				}
				currentParam = *tok_iter;
				++tok_iter;
				if ( tok_iter == tokens.end()) {
					// ������ ����� ������������
					file.close();
					return -7;
				}
				if (*tok_iter != "->") {
					// ������ ����� ������������
					file.close();
					return -8;
				}
				++tok_iter;
				if ( tok_iter == tokens.end()) {
					// ������ ����� ������������
					file.close();
					return -9;
				}
				readParams[currentParam] = *tok_iter;
				if (i = std::getline(file, str) && i != EOF) {
					// ����� - ���-�� - ���� - ���� �� ����� � ������� �����
					continue;
				} else { 
					condition = 1;
					break;
				}
			}
		}
		// ��������� - ���� ���� �� ���������� ��������� ������
		if (toSave > 0) {
			if (!currentEC.empty()) {
				as.axiomParameters[currentEC] = readParams;
			}
			//��������� ����� ��������� ������� 
			(ass.axioms).push_back(as);
		}
	}
	
	file.close();
	
	return 0;
}
