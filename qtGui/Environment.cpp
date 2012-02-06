/****************************************************************************
*					������� ������ Environment.h
*
*	Description:	������� ������, ����������� �� ���������� ���������� �� ����� - ����������
*	Author:			dk
*	History:		
*
****************************************************************************/
#include "Environment.h"

/****************************************************************************
*					Environment::Environment
*
*	Description:	����������� ������, ������� �������� ���������� ������ �� ���������
*	Parameters:		-
*	Returns:		-
*	Author:			dk
*	History:
*
****************************************************************************/
Environment::Environment (void) {
	params.clear();
}

/****************************************************************************
*					Environment::getStringParamValue
*
*	Description:	������� ���������� ������ �������� ��������� �� name1 � name2 
*					� ���� ������.
*	Parameters:		std::string& value - ���� ������ ��������
*					std::string name1 - ��� ���������
*					std::string name2 - ��� ���������
*	Returns:		0 - ���� ����� �������� �������
*					-1 - ���� ������ ��������� �� �������
*	Author:			dk
*	History:
*
****************************************************************************/
int Environment::getStringParamValue(std::string& value, const std::string& name1, const std::string& name2) const {
	std::multimap <std::pair <std::string, std::string>, std::string>::const_iterator i = params.find(std::make_pair (name1, name2));
	// �������� - ���� �� ����� �������
	if (i == params.end())
		return -1;
	// ���� ���� - ���������� ������ ���������
	value = i->second;
	return 0;
}

/****************************************************************************
*					Environment::getDoubleParamValue
*
*	Description:	������� ���������� ������ �������� ��������� �� name1 � name2 
*					� ���� �������� �����.
*	Parameters:		double& value - ���� ������ ��������
*					std::string name1 - ��� ���������
*					std::string name2 - ��� ���������
*	Returns:		0 - ���� ����� �������� �������
*					-1 - ���� ������ ��������� �� �������
*	Author:			dk
*	History:
*
****************************************************************************/
int Environment::getDoubleParamValue(double& value, const std::string& name1, const std::string& name2) const {
	std::multimap<std::pair<std::string, std::string>, std::string>::const_iterator i = params.find(std::make_pair (name1, name2));
	// �������� - ���� �� ����� �������
	if (i == params.end())
		return -1;
	// �������� � double
	value = atof(i->second.c_str());
	return 0;
}

/****************************************************************************
*					Environment::getIntParamValue
*
*	Description:	������� ���������� ������ �������� ��������� �� name1 � name2 
*					� ���� ������ �����.
*	Parameters:		signed int& value - ���� ������ ��������
*					std::string name1 - ��� ���������
*					std::string name2 - ��� ���������
*	Returns:		0 - ���� ����� �������� �������
*					-1 - ���� ������ ��������� �� �������
*	Author:			dk
*	History:
*
****************************************************************************/
int Environment::getIntParamValue(int &value, const std::string &name1, const std::string &name2) const {
  	std::multimap<std::pair<std::string, std::string>, std::string>::const_iterator i = params.find(std::make_pair (name1, name2));
	// �������� - ���� �� ����� �������
	if (i == params.end())
		return -1;
	// �������� � double
	value = atoi(i->second.c_str());
	return 0;
}

/****************************************************************************
*					Environment::getStringSetParamValue
*
*	Description:	������� ���������� ������ �������� ��������� �� name1 � name2 
*					� ���� ������ �����.
*	Parameters:		std::set <std::string>& value - ���� ������ ��������
*					std::string name1 - ��� ���������
*					std::string name2 - ��� ���������
*	Returns:		0 - ���� ����� �������� �������
*					-1 - ���� ������ ��������� �� �������
*	Author:			dk
*	History:
*
****************************************************************************/
int Environment::getStringSetParamValue(std::set<std::string> &value, const std::string& name1, const std::string& name2) const {
	std::multimap<std::pair<std::string, std::string>, std::string>::const_iterator i = params.find(std::make_pair (name1, name2));
	// �������� - ���� �� ����� �������
	if (i == params.end())
		return -1;
	// ���� ���� - ��������� ���� ���������
	std::pair<mm_c_iter,mm_c_iter> runner = params.equal_range(std::make_pair (name1, name2));
	for (mm_c_iter i = runner.first; i != runner.second; ++i)
		value.insert(i->second);
	return 0;
}

/****************************************************************************
*					Environment::getDoubleSetParamValue
*
*	Description:	������� ���������� ��� �������� ��������� �� name1 � name2 
*					� ���� ������ �����.
*	Parameters:		std::set<double>& value - ���� ������ ��������
*					std::string name1 - ��� ���������
*					std::string name2 - ��� ���������
*	Returns:		0 - ���� ����� �������� �������
*					-1 - ���� ������ ��������� �� �������
*	Author:			dk
*	History:
*
****************************************************************************/
int Environment::getDoubleSetParamValue (std::set<double>& value, const std::string& name1, const std::string& name2) const {
	std::multimap<std::pair<std::string, std::string>, std::string>::const_iterator i = params.find(std::make_pair (name1, name2));
	// �������� - ���� �� ����� �������
	if (i == params.end())
		return -1;
	// ���� ���� - ��������� ���� ���������
	std::pair<mm_c_iter,mm_c_iter> runner = params.equal_range(std::make_pair (name1, name2));
	for (mm_c_iter i = runner.first; i != runner.second; ++i)
		value.insert(atof(i->second.c_str()));
  return 0;
}

/****************************************************************************
*					Environment::moveSpaces
*
*	Description:	������� ����������� ������ - ������� ��� ������� � ��������� � ������ � � �����
*	Parameters:		str - ������������� ������
*	Returns:		int - ����� ������ ������ str
*	Author:			dk
*	History:
*
****************************************************************************/
int Environment::moveSpaces (std::string &str) {
	// �������� - ������ �� ������
	if (str.size() == 0) {
		return 0;
	}
	// �������������
	if ((str[0] == '\t') || (str[0] == ' ') || (str[str.size() - 1] == '\t') || (str[str.size() - 1] == ' ')) {
		// �������� � j - ������ ������� ������� ��������� �� ��������� � �������
		std::string::size_type j = str.find_first_not_of ("\t ");
		if (j == std::string::npos) {
			// ������ ���� �� �������� � ���������
			str.clear();
			return 0;
		}
		// �������� � l - ������ ���������� ������� ��������� �� ��������� � �������
		std::string::size_type l = str.find_last_not_of ("\t ");
		str = str.substr (j, l - j + 1);
	}
	return (int) str.size();
}


/****************************************************************************
*					Environment::readFile
*
*	Description:	������� ������ ����� � ������ ������� ������
*	Parameters:		fileName - ���� � ������������ �����
*	Returns:		0 - ���� ��������� ���������
*					-1 - �����
*	Author:			dk
*	History:
*
****************************************************************************/
int Environment::readFile(const std::string &fileName) {
	// ��������� - ���������� �� ����
	boost::filesystem::path fPath (fileName);
	if ( !boost::filesystem::exists( fPath ) ) {
		return -1;
	}

	// ��������� ����
	boost::filesystem::ifstream file( fileName );
	// ������ ���������
	boost::char_separator<char> sep("';:\t");

	// ���� - ����������� �� ����� ������ ����� � ��� ��������� � ��� ������ �������, 
	// ������� ����� ���� ���������� �� ��������� �������������� �������
	std::string str;
	while ( !file.eof() )  {
		std::getline(file, str);
		if (!str.empty()) {
			moveSpaces(str);
			token_separator tokens(str, sep);
			processTokens (tokens);
		}
	} 

	// ��������� ����, ��� ok
	file.close();
	return 0;
}


/****************************************************************************
*					Environment::processTokens
*
*	Description:	������ ������� �������, ��������� multimap
*	Parameters:		tokens - ����� ������� ��� �������
*	Returns:		0
*	Author:			dk
*	History:
*
****************************************************************************/
int Environment::processTokens (token_separator tokens) {
	for (token_separator::iterator tok_iter = tokens.begin();  tok_iter != tokens.end();  ++tok_iter) {
		std::string strTemp = *tok_iter;
		if ( (strTemp.size() < 1) || (strTemp[0] == ' ' ) ) { // ���� ������ �����, ���� ���������� � ������� - � ������ ��� �� �����
			continue;
		}
		if (strTemp.size() >= 2 && strTemp[0] == '/' && strTemp[1] == '/')
			return 0;

		// ��������� �� param � value
		std::string name1 = *tok_iter;
		++tok_iter;
		if (tok_iter == tokens.end()) {
			return 0;
		}
		std::string name2 = *tok_iter;
		++tok_iter;
		if (tok_iter == tokens.end()) {
			return 0;
		}
		std::string value = *tok_iter;

		// ������� � ��� multimap
		params.insert(std::make_pair (std::make_pair (name1, name2), value));
	}
	return 0;
}
