/****************************************************************************
*					Environment
*
*	Description:	�����, ���������� �� ������ � ������������� � �����������
*	Author:			dk
*	History:
*
****************************************************************************/
#ifndef __qtGui_ENVIRONMENT_HXX
#define __qtGui_ENVIRONMENT_HXX

#include <iostream>      
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <set>
#include <boost/tokenizer.hpp>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/fstream.hpp"

// �����������, ������������ �� ��������� ��� �������� ������� ���������, �� �������� �������� ���������
#define str_def_map_value	"__def_second_param_name_"

class Environment {

 private:
	
  // ��������� �� ���������� �����
  std::multimap < std::pair <std::string, std::string> , std::string> params;

 protected:
  
  // ����������� ���� - ������� ��� ���������� � ��������� ����
  typedef boost::tokenizer<boost::char_separator<char> >	token_separator;
  typedef std::multimap <std::pair <std::string, std::string>, std::string>::const_iterator	mm_c_iter;
  
  // ���������� ��������������� ������� - �������� ����� ������� ��� ����� ������ ����������������� ����� 
  // � � ������������ � ���������� ���������� ����� ����� ���������� ���������� ���������� ������
  int processTokens (token_separator tokens);
  
  // ������� ����������� ������ - ������� ��� ������� � ��������� � ������ � � ����� �����
  int moveSpaces (std::string &str);
  
 public:
	
  // ����������� ������
  Environment (void);
	
  // ������ ���������������� ���� � �������� ������
  int readFile (const std::string &filename);
	
  // �������� �������� �� name1 � name2
  int getStringParamValue (std::string& value, const std::string& name1, const std::string& name2 = str_def_map_value) const;
  int getDoubleParamValue (double& value, const std::string& name1, const std::string& name2 = str_def_map_value) const;
  int getIntParamValue    (int& value, const std::string& name1, const std::string& name2 = str_def_map_value) const;
  int getStringSetParamValue(std::set<std::string>& value, const std::string& name1, const std::string& name2 = str_def_map_value) const;
  int getDoubleSetParamValue(std::set<double>& value, const std::string& name1, const std::string& name2 = str_def_map_value) const;

}; // end of class

#endif /* __qtGui_ENVIRONMENT_HXX */
