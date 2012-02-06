/****************************************************************************
*				EnvDataSet
*
*	Description:	���� ������ ������������ ��������� ������ ������
*	Author:		dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_ENV_DATA_SET_HXX
#define __AXIOMS_LIB_ENV_DATA_SET_HXX

#include "./elem_conditions/ElemCondition.h"
#include "ElemConditionsFactory.h"
#include "AxiomLibException.h"
#include "all_axioms.h"
#include "Axiom.h"
#include "Environment.h"
#include <iostream>      
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
//includes for using boost
#include <boost/tokenizer.hpp>

namespace AxiomLib {

class EnvDataSet {

 private:

 protected:
	// DataSet Parameters
	// ������, ������� ��������� ��������� ���������� � �������� �����
	std::string nullStr;

	// ������ - ������ ������������ ����� ���������������, �������� ������������ ���������� ��������� �������
	std::string normalStr;
	
	// ������ � ������� �������������� ����������
	std::vector<std::string> analizeNames;
	
	//AxiomSet Parameters
	std::string saveTo;
	
	// this class functions
	// ����������� ���� - ������� ��� ���������� � ��������� ����
	typedef boost::tokenizer<boost::char_separator<char> > token_separator;
  
	// ���������� ��������������� ������� - �������� ����� ������� ��� ����� ������ ����������������� ����� 
	// � � ������������ � ���������� ���������� ����� ����� ���������� ���������� ���������� ������
	signed int processTokens (token_separator tokens);

	// ��������������� ���������� ��� ����������� ������������� ����������������� �����
	int tmpNull;
	int tmpNormal;
	int tmpSaveTo;
	
	// �������� ����� ��������� ���������� � ����������� ����� ������ 
	int testOut (void);
 
 public:
	
	// ����������� � �������� ��������� ���������� �� ���������
	EnvDataSet (void);
	
	// ����������� � �������� ��������� ���������� �� ���������
	~EnvDataSet (void);
	
	// ���������� ������ � ������������ ��������� ���������� � ������ ��������
	int getNullStr (std::string &retNull);
	
	// ���������� ������, ������� ������������ ���������� ��������� ����� ������� ���������� ���������
	int getNormalStr (std::string &retNormal);
	
	// ���������� ������ � ��������� ��������� � �������� ��������
	std::string getAnalizeName (const int i) const;
	
	// ���������� ����� ��������� � ������� ����� � ���������� ����������
	int getAnalizeNameSize (void) const;
	
	// ������� ���������� ��� ������������ ������ ������� ������
	int getWhereToSave (std::string &whereToSave);
	
	// ������� ����������� ������ - ������� ��� ������� � ��������� � ������ � � ����� �����
	int moveSpaces (std::string &str);
	
	// ������ ���������������� ���� � �������� ������
	signed int readConfigFile(std::string confDir, std::string confName);
	
	// ������ ���������������� ���� � �������� ������
	signed int readConfigFile(Environment &env);

}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_ENV_DATA_SET_HXX */
