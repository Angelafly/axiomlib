/****************************************************************************
*				Environment.cxx
*
*	Description:	������� ������ Environment - ����������
*	Author:		gevor,dk
*	History:		
*
****************************************************************************/

#include <algorithm>
#include <iterator>
#include <list>
#include <iostream>
#include <sstream>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

#include "AxiomLibException.h"
#include "Environment.h"

using namespace AxiomLib;
using namespace std;

#define str_default_config_file_name	"./dataset/axiomlib.conf"

/****************************************************************************
*				Environment::getStringParamValue
*
*	Description:	������� ���������� ������ �������� ��������� � ������ name � ���� ������
*	Parameters:	string& value - ���� ������ ��������
*			string name - ��� ���������
*	Returns:	0 - ���� ����� �������� �������
*			-1 - ���� ������ ��������� �� �������
*	Throws:		-
*	Author:		gevor
*	History:
*
****************************************************************************/
int Environment::getStringParamValue(string& value, const string& name) const
{
  MapType::const_iterator i = configFileParams.find(name);
  
  // �������� - ���� �� ����� �������
  if (i == configFileParams.end())
    return -1;

  // ���� ���� - ���������� ������ ���������
  value = (*i).second;
  return 0;
}

/****************************************************************************
*				Environment::getDoubleParamValue
*
*	Description:	������� ���������� ������ �������� ��������� � ������ name � ���� ������� �����
*	Parameters:	double& value - ���� ������ ��������
*				string name - ��� ���������
*	Returns:		0 - ���� ����� �������� �������
*				-1 - ���� ������ ��������� �� �������, ��� �� �� �������� � double
*	Throws:		-
*	Author:		gevor
*	History:
*
****************************************************************************/
int Environment::getDoubleParamValue(double& value, const string& name) const
{
  MapType::const_iterator i = configFileParams.find(name);
  
  // �������� - ���� �� ����� �������
  if (i == configFileParams.end())
    return -1;

  // �������� � double
  value = atof((*i).second.c_str());
  return 0;
}

/****************************************************************************
*				Environment::getIntParamValue
*
*	Description:	������� ���������� ������ �������� ��������� � ������ name � ���� ������
*	Parameters:	signed int& value - ���� ������ ��������
*				string name - ��� ���������
*	Returns:		0 - ���� ����� �������� �������
*				-1 - ���� ������ ��������� �� �������, ��� �� �� �������� � int
*	Throws:		-
*	Author:		gevor
*	History:
*
****************************************************************************/
int Environment::getIntParamValue(signed int& value, const string& name) const
{
  MapType::const_iterator i = configFileParams.find(name);
  
  // �������� - ���� �� ����� �������
  if (i == configFileParams.end())
    return -1;

  // �������� � double
  value = atoi((*i).second.c_str());
  return 0;
}

/****************************************************************************
*					Environment::getBoolParamValue
*
*	Description:	������� ���������� ������ �������� ��������� � ������ name ��� ��������� ����������
*					(������� �������������� - case-insensitive, "true" ��� "false")
*	Parameters:	bool& value - ���� ������ ��������
*				string name - ��� ���������
*	Returns:		0 - ���� ����� �������� �������
*				-1 - ���� ������ ��������� �� �������, ��� �� �� �������� � bool (�� ���������� �������)
*	Throws:		-
*	Author:		wictor
*	History:
*
****************************************************************************/
int Environment::getBoolParamValue(bool &value, const std::string &name) const {
	std::string str;
	if(getStringParamValue(str, name) < 0) {
		return -1;
	} else {
		boost::algorithm::to_lower(str);
		if (str == "true") {
			value = true;
		} else if (str == "false") {
			value = false;
		} else {
			return -1;	
		}
		return 0;	
	}	
}

/****************************************************************************
*				Environment::getStringSetParamValue
*
*	Description:	������� ���������� ��� �������� ��������� � ������ name � ���� ������
*	Parameters:	string& value - ���� ������ ��������
*			string name - ��� ���������
*	Returns:	0 - ���� ����� �������� �������
*			-1 - ���� ������ ��������� �� �������
*	Throws:		-
*	Author:		gevor
*	History:
*
****************************************************************************/
int Environment::getStringSetParamValue(set<string>& value, const string& name) const
{
  MapType::const_iterator i = configFileParams.find(name);
  
  // �������� - ���� �� ����� ������� ������, ���� ����
  if (i == configFileParams.end())
    return -1;

  // ���� ���� - ��������� ���� ���������
  pair<mm_c_iter,mm_c_iter> runner = configFileParams.equal_range(name);
  for (mm_c_iter i = runner.first; i != runner.second; ++i)
    value.insert((*i).second);
  return 0;
}

/****************************************************************************
*				Environment::getDoubleSetParamValue
*
*	Description:	������� ���������� ��� �������� ��������� � ������ name � ���� ������ �����
*	Parameters:	string& value - ���� ������ ��������
*				string name - ��� ���������
*	Returns:		0 - ���� ����� �������� �������
*				-1 - ���� ������ ��������� �� �������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
int Environment::getDoubleSetParamValue (std::set<double>& value, const std::string& name) const 
{
  MapType::const_iterator i = configFileParams.find(name);
  
  // �������� - ���� �� ����� ������� ������, ���� ����
  if (i == configFileParams.end())
    return -1;

  // ���� ���� - ��������� ���� ���������
  pair<mm_c_iter,mm_c_iter> runner = configFileParams.equal_range(name);
  for (mm_c_iter i = runner.first; i != runner.second; ++i)
    value.insert(atof((*i).second.c_str()));
  return 0;
}

/****************************************************************************
*				Environment::getIntSetParamValue
*
*	Description:	������� ���������� ��� �������� ��������� � ������ name � ���� ������ �����
*	Parameters:	std::set<int>& value - ���� ������ �������� 
*				(�������� �����������, ��, ��� ��� ��� �����, �� ���������)
*				string name - ��� ���������
*	Returns:		0 - ���� ����� �������� �������
*				-1 - ���� ������ ��������� �� �������
*	Throws:		-
*	Author:		wictor
*	History:
*
****************************************************************************/
int Environment::getIntSetParamValue(std::set<int>& value, const std::string& name) const {
	std::set<std::string> stringSet;
	if(getStringSetParamValue(stringSet, name) < 0) {
		return -1;
	}
	// ��������� ���������� - ����� � ������ ������ ������ � value �� ������
	std::list<int> result;
	// ��������� ������ � �����
	BOOST_FOREACH(const std::string &str, stringSet) {
		try {
			result.push_back(boost::lexical_cast<int>(str));
		} catch(boost::bad_lexical_cast &) {
			return -1;
		}
	}
	// �������� result � value
	value.insert(result.begin(), result.end());
	return 0;
}

void Environment::setParamValues(const std::set<std::string> &value, const std::string &name) {
	configFileParams.erase(name);
	for(auto it = value.begin(); it != value.end(); ++it) {
		configFileParams.insert(std::make_pair(name, *it));
	}
}

/****************************************************************************
*				Environment::undefineParam
*
*	Description:	������� ������� �������� �� ���������
*	Parameters:	string name - ��� ���������
*	Returns:		0 - ���� ����� �������� �������
*				-1 - ���� ������ ��������� �� �������
*	Throws:		-
*	Author:		wictor
*	History:
*
****************************************************************************/
int Environment::undefineParam(const std::string &name) {
	int erasedCount = configFileParams.erase(name);
	return erasedCount > 0 ? 0 : -1;
}

bool Environment::paramDefined(const std::string &paramName) const {
	return configFileParams.find(paramName) != configFileParams.end();
}

void Environment::clear() {
	configFileParams.clear();
}

/****************************************************************************
*					Environment::moveSpaces
*
*	Description:	������� ����������� ������ - ������� ��� ������� � ��������� � ������ � � ����� �����
*	Parameters:	str - ������������� ������
*	Returns:		0
*	Throws:		-
*	Author:		dk
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
*					Environment::testOut
*
*	Description:	�������� ����� ���� ��������� ���������� � stdout
*	Parameters:	-
*	Returns:		0
*	Throws:		-
*	Author:		gevor
*	History:
*
****************************************************************************/
int Environment::testOut (void) {
	return 0;
}


/****************************************************************************
*					Environment::createDir
*
*	Description:	������� ���������� �� ���������� ������
*	Parameters:		dir - ������ � ������� ��� �������� ����������
*	Returns:		0 - ���� ���������� ������� ��� ����������
*					-1 - ���� ���������� ������� �� �������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int Environment::createDir (std::string dir) {
	// �������� �������� ���������
	if (dir.size() < 1)
		return -1;

	boost::filesystem::path path (dir);
	// �������� - ���������� �� ��������� ����������
	if ( boost::filesystem::exists(path) ) {
		if ( !boost::filesystem::is_directory(path) ) {
			boost::filesystem::create_directory(path);
		}
		return 0;
	}

	// ���� ���������� � ��������� ������ �� ����������
	int index;
	index = (int) dir.find_last_of ("/\\");
	if (index > 0) {
		if (index == (dir.size() - 1)) {
			dir.erase(index, 1);
			return createDir (dir);
		}
		if (createDir (dir.substr(0, index)) < 0)
			return -1;
		boost::filesystem::create_directory(path);
		return 0;
	} else {
		// ������ ������ � ���� �� �������� - ����� ����������, �������, ������ ������ ������������
		return -1;
	}

	// ���� ����� �� ������ - ������� ���������� ������
	return -1;
}


/****************************************************************************
*					Environment::saveFile
*
*	Description:	���������� ���������� ����� � ����� � ������������
*	Parameters:		fileName - ����� �����, ������� ��������� � ����������
*	Returns:		0 - ���� ��� ���������
*					-1 - ���� � ������ ������ Environment �� ����� ���� ��� ���������� ����������
*					-2 - ���� ��������� ���� ��� ���������� ����������� �� �������� �����������
*					-3 - ���� �� ���������� ��������� ��� ����������� ����
*					-4 - ���� �� ������� �������� ��� ������������ ���� �� ���������� ����
*					-5 - ���� �� ������� ������� ��������� ����, �� �������� ��������� ����
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
signed int Environment::saveFile (const std::string fileName) {
	std::string whereToSave;
	// ���� ��� ��������� - ���� ��������� ���� - �� ������� �� �������
	if (getStringParamValue (whereToSave, "PathToSaveResults") < 0)
		return -1;
	boost::filesystem::path path (whereToSave);
	if ( !boost::filesystem::exists( path ) ) {
		if (createDir (whereToSave) < 0)
			return -5;
	} else {
		if ( !boost::filesystem::is_directory( path ) )
			return -2;
	}
	int indexName, indexSlash;
	indexName = (int) fileName.find_last_of("/\\");
	indexSlash = (int) whereToSave.find_last_of("/\\");
	if (indexSlash != (whereToSave.size() - 1))
		whereToSave.append ("/");
	whereToSave.append (fileName.substr (indexName + 1, fileName.size() - indexName - 1));
	boost::filesystem::path fPath (fileName);
	boost::filesystem::path fPathNew (whereToSave);
	if ( !boost::filesystem::exists( fPath ) ) {
		return -3;
	}
	if ( boost::filesystem::exists( fPathNew ) ) {
		if (!boost::filesystem::remove( fPathNew ))
			return -4;
	}
	boost::filesystem::copy_file (fPath, fPathNew);
	return 0;
}

void Environment::writeToStream(std::ostream &ostr) const {
	// TODO �������� � escape'� ������������
	ostr<<"{ ";
	auto it = configFileParams.begin();
	ostr<<"\""<<it->first<<"\""<<":"<<"\""<<it->second<<"\"";
	++it;
	for(; it != configFileParams.end(); ++it) {
		ostr<<", \""<<it->first<<"\""<<":"<<"\""<<it->second<<"\"";
	}
	ostr<<" }";
}

std::string Environment::getStringRepresentation() const {
	std::ostringstream ostr;
	writeToStream(ostr);
	return ostr.str();
}

void Environment::getParamNames(std::vector<std::string> &result) const {
	if(configFileParams.size() == 0) {
		return;
	}
	
	result.reserve(configFileParams.size());
	auto it = configFileParams.begin();
	result.push_back(it->first);
	
	for(++it; it != configFileParams.end(); ++it) {
		if(it->first != result.back()) {
			result.push_back(it->first);
		}
	}
}

/****************************************************************************
*					Environment::readConfigParams
*
*	Description:	������ ��������� �������� �� ���� ��� ������� ���������
*	Parameters:	-
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int Environment::readConfigParams (int argc, char** argv) {
	signed int toRet = 0;
	std::string name, value;
	int p = 2;
	while (p < argc) {
		name.assign (argv[p-1]);
		value.assign (argv[p]);
		configFileParams.insert(make_pair(name, value));
		p += 2;
	}
	return toRet;
}


/****************************************************************************
*					Environment::readConfigFile
*
*	Description:	������� ������ ����������������� ����� �� ���������
*	Parameters:	-
*	Returns:	0
*	Throws:		-
*	Author:		gevor
*	History:
*
****************************************************************************/
signed int Environment::readConfigFile(void) {
  readConfigFile(str_default_config_file_name);
  return 0;
}


/****************************************************************************
*					Environment::readConfigFile
*
*	Description:	������� ������ ������� ����������������� �����
*	Parameters:	���������� �������� �������� ������ ���������� ���� � ����������������� �����
*	Returns:	0
*	Throws:		AxiomLibException - ���� �� ������� ����������������� ����� � �������� �����
*	Author:		gevor
*	History:
*
****************************************************************************/
signed int Environment::readConfigFile(const char* filename) {
  std::string fileName(filename);
  // ��������� - ���������� �� ����
  boost::filesystem::path fPath (filename);
  bool tempBool = boost::filesystem::exists( fPath );
  if ( !boost::filesystem::exists( fileName ) ) {
    throw AxiomLibException("Error in Environment::ReadConfigFile: cannot find the config file.");
  }

  // ��������� ����
  boost::filesystem::ifstream file( fileName );
  // ������ ���������
  boost::char_separator<char> sep("';:\t ");

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
*	Parameters:	����� ������� ��� �������
*	Returns:	0
*	Throws:		AxiomLibException - ���� ������ ������� ����������������� �� ��������� � �������� ��������� � ../format.x
*	Author:		dk,gevor
*	History:
*
****************************************************************************/
signed int Environment::processTokens (token_separator tokens) {
  for (token_separator::iterator tok_iter = tokens.begin();  tok_iter != tokens.end();  ++tok_iter) {
    string strTemp = *tok_iter;
    if ( (strTemp.size() < 1) || (strTemp[0] == ' ' ) ) {
		// ���� ������ �����, ���� ���������� � ������� - � ������ ��� �� �����
		continue;
    }
	if (strTemp.size() >= 2 && strTemp[0] == '/' && strTemp[1] == '/') {
		// ������ ������ - ��� ������ � ����������
		return 0;
	}

    // ��������� �� param � value
    // (!!!) - �� �������� � ���������
    string name = *tok_iter;
    //cerr << "Name token: " << *tok_iter << "\n";
	
    ++tok_iter;
    if (tok_iter == tokens.end()) {
      throw AxiomLibException ("Error in Environment::processTokens : error in config file - value not set. name = '" + name + "'");
    }
	//cerr << "Value token: " << *tok_iter << "\n";
	
    string value = *tok_iter;

    // ������� � ��� multimap
    configFileParams.insert(make_pair(name, value));
  }    
  return 0;
}


/****************************************************************************
*				Environment::operator +=
*
*	Description:	�������� ����������� ����������� ������� ���������
*	Parameters:		second - ����������� ����� ���������
*	Returns:		*this
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
Environment& Environment::operator += (const Environment& second) {
	if(this == &second) {
		return *this;
	}
	
	MapType::const_iterator iter;
	// ������� �������� ��� ����������� ����������
	for (iter = second.configFileParams.begin(); iter != second.configFileParams.end(); ++iter ) {
			this->configFileParams.erase (iter->first);		
	}
	// ��������� ����� �������� ��� ����������� ����������
	for (iter = second.configFileParams.begin(); iter != second.configFileParams.end(); ++iter ) {
		this->configFileParams.insert(make_pair(iter->first, iter->second));	
	}
	return *this;
}

Environment& Environment::operator %= (const Environment& other) {
	if(this == &other) {
		return *this;
	}
	
	std::set<std::string> stringSet;
	std::vector<std::string> paramNames;
	getParamNames(paramNames);
	for(auto it = paramNames.begin(); it != paramNames.end(); ++it) {
		stringSet.clear();
		if(!other.getStringSetParamValue(stringSet, *it)) {
			this->setParamValues(stringSet, *it);
		}
	}
	
	return *this;
}
