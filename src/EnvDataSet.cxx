/****************************************************************************
*				EnvDataSet.cxx
*
*	Description:	������� ������ EnvDataSet - ����������
*	Author:		dk
*	History:		
*
****************************************************************************/
#include "EnvDataSet.h"

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/fstream.hpp"

using namespace AxiomLib;


// ����������� ������
EnvDataSet::EnvDataSet(void) {
	// defaults: none of the parameters are set
	
	// DataSet Parameters
	nullStr = "";
	normalStr = "";
	analizeNames.resize(0);
	
	//AxiomSet Parameters
	saveTo = "";
}


// ���������� ������
EnvDataSet::~EnvDataSet(void) {

}


/****************************************************************************
*					EnvDataSet::moveSpaces
*
*	Description:	������� ����������� ������ - ������� ��� ������� � ��������� � ������ � � ����� �����
*	Parameters:	str - ������������� ������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
int EnvDataSet::moveSpaces (std::string &str) {
	// �������� - ������ �� ������
	if (str.size() == 0) {
		return 0;
	}
	// �������������
	if ((str[0] == '\t') || (str[0] == ' ') || (str[str.size() - 1] == '\t') || (str[str.size() - 1] == ' ')) {
		int j = 0;
		while (j < (int) str.size()) {
			if ((str[j] != '\t') && (str[j] != ' ')) {
				break;
			}
			j++;
		}
		if (j == str.size()) {
			// ������ ���� �� �������� � ���������
			str.resize(0);
			return 0;
		}
		// ������ j - ������ ������� ������� ��������� �� ��������� � �������
		int l = (int) str.size() - 1;
		while (l >= j) {
			if ((str[l] != '\t') && (str[l] != ' ')) {
				break;
			}
			l--;
		}
		// ������ � l - ������ ���������� ������� ��������� �� ��������� � �������
		std::string newStr;
		newStr.resize(l - j + 1);
		int h = 0;
		for (int i = j; i <= l; i++) {
			newStr[h] = str[i];
			h++;
		}
		str= newStr;
	}
	return 0;
}


/****************************************************************************
*					EnvDataSet::testOut
*
*	Description:	�������� ����� ��������� ���������� � ����������� ����� ������
*	Parameters:	-
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
int EnvDataSet::testOut (void) {
	std::cout << "\n nullStr is '" << nullStr << "'\n";
	std::cout << "\n normalStr is '" << normalStr << "'\n";
	for (unsigned int i = 0; i < analizeNames.size(); i++) {
		std::cout << " analizeNames[" << i+1 << "] is '" << analizeNames[i] << "'\n";
	}
	std::cout << " saveTo is " << saveTo << "\n";
	return 0;
}


/****************************************************************************
*					EnvDataSet::readConfigFile
*
*	Description:	������� ������ ����������������� ����� � ���������� �������������� ����� ������� ������
*	Parameters:	���������� ������� �������� ������ ���������� ���� � ����������������� �����
*	Returns:		0
*	Throws:		AxiomLibException - ���� �� ������� ����������������� ����� � �������� �����
*	Author:		dk
*	History:
*
****************************************************************************/
signed int EnvDataSet::readConfigFile(std::string confDir, std::string confName) {
	this->analizeNames.resize(0);
	std::string str;
	std::string fileName(confDir);
	fileName.append("/");
	fileName.append(confName);
	fileName.append("/");
	fileName.append(confName);
	fileName.append(".conf");
	//std::cout << "\n fileName is '" << fileName << "'\n";
	int i = 0;
	// ��������� - ���������� �� ����
	if ( !boost::filesystem::exists( fileName ) ) {
		//std::cout << "\n fileName is '" << fileName << "'\n";
		throw AxiomLibException("Error in EnvDataSet::ReadConfigFile: cannot find the config file at path "+fileName);
		return 1;
	}
	tmpNull = 0;
	tmpNormal = 0;
	tmpSaveTo = 0;
	// ��������� ����
	boost::filesystem::ifstream file( fileName );
    boost::char_separator<char> sep("';:\t \r\n");
	// ���� - ����������� �� ����� ������ ����� � ��� ��������� � ��� ������ �������, 
	// ������� ����� ���� ���������� �� ��������� �������������� �������
	while (i = std::getline(file, str) && i != EOF)  {
		if (!str.empty()) {
			moveSpaces (str);
			token_separator tokens(str, sep);
			processTokens (tokens);
		}
 	}
	file.close();
	//testOut();
	//std::cout << "\n Press Enter";
	//std::cin.get();
	return 0;
}


/****************************************************************************
*					EnvDataSet::readConfigFile
*
*	Description:	������� ������ ����������������� ����� � ���������� �������������� ����� ������� ������
*	Parameters:	�������� ����������� �� ������ env
*	Returns:		0
*	Throws:		AxiomLibException - ���� �� ������� ����������������� ����� � �������� �����
*	Author:		dk
*	History:
*
****************************************************************************/
signed int EnvDataSet::readConfigFile(Environment &env) {
	std::string baseDataSetDir;
	std::string dataSetName;
	env.getStringParamValue(baseDataSetDir, "BaseDataSetDir");
	env.getStringParamValue(dataSetName, "DataSet");
	return readConfigFile (baseDataSetDir, dataSetName);
}


/****************************************************************************
*					EnvDataSet::processTokens
*
*	Description:	���������� ��������������� ������� - �������� ����� ������� ��� ����� ������ ����������������� �����
*				� � ������������ � ���������� ���������� ����� ����� ���������� ���������� ���������� ������
*	Parameters:	����� ������� ��� �������
*	Returns:		0
*	Throws:		AxiomLibException - ���� ������ ������� ����������������� �� ��������� � �������� ��������� � ../format.x
*	Author:		dk
*	History:
*
****************************************************************************/
signed int EnvDataSet::processTokens (token_separator tokens) {
	std::string strTemp;
	for (token_separator::iterator tok_iter = tokens.begin();  tok_iter != tokens.end();  ++tok_iter) {
		// �������� ������� ������ �� ���������� ���� ������ - ���� ��������� - �� ��������� �������������� ���� ������ ��������� �������
		// ���� ���������� �� ������� - ������ ������. ���� �� �������� - ������ ���� throw AxiomLibException
		
		// DataSet Parameters
		if (*tok_iter == "null") {
			if (tmpNull > 0) {
				throw AxiomLibException ("Error in EnvDataSet::processTokens : error in config file - too many definitions of null.");
			}
			++tok_iter;
			if (tok_iter != tokens.end()) {
				this->nullStr = *tok_iter;
				tmpNull++;
			}
			else {
				throw AxiomLibException ("Error in EnvDataSet::processTokens : error in config file - 'null'-string is wrong.");
			}
			break;
		}
		if (*tok_iter == "normalClass") {
			if (tmpNormal > 0) {
				throw AxiomLibException ("Error in EnvDataSet::processTokens : error in config file - too many definitions of normalStr.");
			}
			++tok_iter;
			if (tok_iter != tokens.end()) {
				this->normalStr = *tok_iter;
				tmpNormal++;
			}
			else {
				throw AxiomLibException ("Error in EnvDataSet::processTokens : error in config file - 'Normal'-string is wrong.");
			}
			break;
		}
		if (*tok_iter == "analizeParameter") {
			++tok_iter;
			if (tok_iter != tokens.end()) {
				this->analizeNames.push_back(*tok_iter);
			}
			else {
				throw AxiomLibException ("Error in EnvDataSet::processTokens : error in config file - analizeParameter is wrong.");
			}
			break;
		}
		
		// AxiomSet Parameters
		if (*tok_iter == "saveTo") {
			if (tmpSaveTo > 0) {
				throw AxiomLibException ("Error in EnvDataSet::processTokens : error in config file - too many defenitions of saveTo.");
			}
			++tok_iter;
			if (tok_iter != tokens.end()) {
				saveTo = *tok_iter;
				tmpSaveTo++;
			}
			else {
				throw AxiomLibException ("Error in EnvDataSet::processTokens : error in config file - saveTo is wrong.");
			}
			break;
		}
		
		// Something else - if it is not commented - then this is throw
		strTemp = *tok_iter;
		if ( (strTemp.size() > 0) && ( (strTemp.size() < 2) || (strTemp[0] != '/') || (strTemp[1] != '/') ) ) {
			//std::cout << "\n wrong string is \n" << strTemp << "\n";
			throw AxiomLibException ("Error in EnvDataSet::processTokens : conf - file has a wrong format.");
		}
		else { // comments or empty string
			break;
		}
	}
	return 0;
}

// Returning DataSet Parameters

/****************************************************************************
*				EnvDataSet::getNullStr
*
*	Description:	������� ���������� ������ ������� ������������ ��������� ���������� � ������� �������� ������ �����
*	Parameters:	retNull - ������������� ������
*	Returns:		0 - ���� ��� ������ � ���������������� �����
*				-1 - ���� ��� �� ������ � ���������������� �����
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
int EnvDataSet::getNullStr (std::string &retNull) {
	retNull = nullStr;
	if (nullStr == "") { // �� ���� �� ������
		return -1;
	}
	return 0;
}


/****************************************************************************
*				EnvDataSet::getNormalStr
*
*	Description:	������� ���������� ������ ������� ������������ ���������� ��������� ����� ������� ��������������
*	Parameters:	retNormal - ������������� ������
*	Returns:		0 - ���� ��� ������ � ���������������� �����
*				-1 - ���� ��� �� ������ � ���������������� �����
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
int EnvDataSet::getNormalStr (std::string &retNormal) {
	retNormal = normalStr;
	if (normalStr == "") { // �� ���� �� ������
		return -1;
	}
	return 0;
}


/****************************************************************************
*				EnvDataSet::getAnalizeName
*
*	Description:	������� ���������� ������ ������� ������������ ���������� � �������� ������� � ������� ����
*	Parameters:	-
*	Returns:		-
*	Throws:		AxiomLibException - ���� ������ ������ �� ������� �����������
*	Author:		dk
*	History:
*
****************************************************************************/
std::string EnvDataSet::getAnalizeName (const int i) const {
	if ( (i < 0) || (i >= (int) analizeNames.size()) ) {
		throw AxiomLibException ("Error in EnvDataSet::getAnalizeName : input parameter out of range.");
	}
	return analizeNames[i];
}


/****************************************************************************
*				EnvDataSet::getAnalizeNameSize
*
*	Description:	������� ���������� ����� ��������� � ������ � ���������� ����������
*				���� ����� ����� ��������� ����� 0 - �� �������� ����� ��������� �� ���� ����������
*	Parameters:	-
*	Returns:		-
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
int EnvDataSet::getAnalizeNameSize (void) const {
	return (int) analizeNames.size();
}


/****************************************************************************
*				EnvDataSet::getWhereToSave
*
*	Description:	������� ���������� ��������� ��� ������ AxiomSet
*	Parameters:	whereToSave - ��� ������������� ������ ��������
*	Returns:		0 - ���� ��� ������ � ���������������� �����
*				-1 - ���� ��� �� ������ � ���������������� �����
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
int EnvDataSet::getWhereToSave (std::string &whereToSave) {
	whereToSave = saveTo;
	if (saveTo == "") { // �� ���� �� ������
		return -1;
	}
	return 0;
}
