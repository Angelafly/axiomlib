/****************************************************************************
*			Environment
*
*	Description:	�����, ���������� �� ������ � ������������� � ����������� ���������,
*				� ��� ����� � ������������ ������ � ������ ������
*	Author:		gevor
*	History:
*
****************************************************************************/

#ifndef __AXIOMS_LIB_ENVIRONMENT_HXX
#define __AXIOMS_LIB_ENVIRONMENT_HXX

#include <iostream>      
#include <cctype>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"

#include "AxiomLibException.h"

namespace AxiomLib {

class Environment {

 private:

	// ����� ������: http://stackoverflow.com/questions/1801892/making-mapfind-operation-case-insensitive
	/************************************************************************/
	/* Comparator for case-insensitive comparison in STL assos. containers  */
	/************************************************************************/
	struct ci_less : std::binary_function<std::string, std::string, bool>
	{
	  // case-independent (ci) compare_less binary function
	  struct nocase_compare : public std::binary_function<unsigned char,unsigned char,bool> 
	  {
		bool operator() (const unsigned char& c1, const unsigned char& c2) const {
			return std::tolower (c1) < std::tolower (c2); 
		}
	  };
	  bool operator() (const std::string & s1, const std::string & s2) const {
		return std::lexicographical_compare 
		  (s1.begin (), s1.end (),   // source range
		  s2.begin (), s2.end (),   // dest range
		  nocase_compare ());  // comparison
	  }
	};
	
	typedef std::multimap<std::string, std::string, ci_less> MapType;
  std::multimap<std::string, std::string, ci_less> configFileParams; // ��������� �� ����������������� �����

 protected:
  
  // ����������� ���� - ������� ��� ���������� � ��������� ����
  typedef boost::tokenizer<boost::char_separator<char> >      		token_separator;
  typedef MapType::const_iterator		mm_c_iter;
  
  // ���������� ��������������� ������� - �������� ����� ������� ��� ����� ������ ����������������� �����
  // � � ������������ � ���������� ���������� ����� ����� ���������� ���������� ���������� ������
  signed int processTokens (token_separator tokens);
  // �������� ����� ��������� ���������� � ����������� ����� ������
  int testOut (void);
  // ������� ����������� ������ - ������� ��� ������� � ��������� � ������ � � ����� �����
  int moveSpaces (std::string &str);
  // ������� ���������� � ��������� �������
  int createDir (std::string dir);
 public:
	
  // ����������� � �������� ��������� ���������� �� ���������
  Environment (void) {};	

  // ������ ��������� ���������������� ����
  signed int readConfigFile(void);
  // ������ ���������������� ���� � �������� ������
  signed int readConfigFile(const char* filename);

  // ������ ��������� �������� �� ���� ��� ������� ���������
  signed int readConfigParams (int argc, char** argv);

  // ������ ��������� �� ������
  signed int readConfigParams(std::list<const char*>);

  // ������ ���������
  template<class ForwardIterator, class ConvertFunctor>
  int readConfigParams(ForwardIterator begin, ForwardIterator end, ConvertFunctor convert);

  // ��������� ��������� ���� � ���������� (�������� ���� � ��������� ������ � ����������)
  signed int saveFile (const std::string fileName);
  
  void writeToStream(std::ostream& ostream) const;
  std::string getStringRepresentation() const;
  
  void getParamNames(std::vector<std::string>& result) const;
  
  // �������� �������� � ������ name
  int getStringParamValue(std::string& value, const std::string& name) const;
  int getDoubleParamValue(double& value, const std::string& name) const;
  int getIntParamValue(signed int& value, const std::string& name) const;
  int getBoolParamValue(bool& value, const std::string& name) const;
  int getStringSetParamValue(std::set<std::string>& value, const std::string& name) const;
  int getDoubleSetParamValue(std::set<double>& value, const std::string& name) const;
  int getIntSetParamValue(std::set<int>& value, const std::string& name) const;
  
  // ����� ������� ������ �������� ���������
  template<class T>
  int getParamValue(T& value, const std::string& name) const;
  
  // ��������� (����������) ���������
  template<class T>
  void setParamValue(const T& value, const std::string& name);
  void setParamValues(const std::set<std::string>& value, const std::string& name);
  
  // � ������ ���������� �������� ������� ����������
  template<class T>
  void getMandatoryParamValue(T& value, const std::string& name) const;
  
  // ������� �������� �� ���������
  int undefineParam(const std::string& name);
  
  bool paramDefined(const std::string& paramName) const;
  
  void clear();

  // �������� ����������� ����������� ������� ���������
  Environment& operator += (const Environment& second);
  
  // ���������� ��� ���������, ������������ � ����� ����������,
  // � �������� �� other
  Environment& operator %= (const Environment& other);

}; // end of class

/****************************************************************************
*					Environment::readConfigParams
*
*	Description:	������ ���������, �������� � ���� ������������������, ��������
*					��������� � �������� ����������
*	Parameters:	-
*	Returns:		0
*	Throws:		-
*	Author:		wictor
*	History:
*
****************************************************************************/
template<class ForwardIterator, class ConvertFunctor>
int Environment::readConfigParams(ForwardIterator begin,
		ForwardIterator end,
		ConvertFunctor convert) {
	signed int toRet = 0;
	std::string name, value;
	ForwardIterator current = begin;
	while(current != end) {
		name = convert(*current);
		current++;
		if(current == end) break;
		value = convert(*current);
		configFileParams.insert(std::make_pair(name, value));
		current++;
	}
	return toRet;
}

template<class T>
int Environment::getParamValue(T& value, const std::string& name) const {
	std::string buffer;
	if(getStringParamValue(buffer, name) != 0) {
		return -1;
	}
	try {
		value = boost::lexical_cast<T>(buffer);
		return 0;
	} catch (boost::bad_lexical_cast) {
		return -1;
	}
}

template<class T>
void Environment::getMandatoryParamValue(T &value, const std::string &name) const {
	if(getParamValue<T>(value, name) != 0) {
		throw AxiomLibException("Mandatory parameter '" + name + "' not found in the environment");
	}
}


/****************************************************************************
*				Environment::setParamValue
*
*	Description:	������� ������������� ����� �������� ���������
*					� ������� ��� ��� ������ ���������
*	Parameters:	T& value - ��������
*				const std::string& name - ��� ���������
*	Returns:	-
*	Throws:		-
*	Author:		wictor
*	History:
*
****************************************************************************/
template<class T>
void Environment::setParamValue(const T &value, const std::string &name) {
	configFileParams.erase(name);
	
	configFileParams.insert(std::make_pair(name, boost::lexical_cast<std::string>(value)));
}


}; //  end of namespace

#endif /* __AXIOMS_LIB_ENVIRONMENT_HXX */
