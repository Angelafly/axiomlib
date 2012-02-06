/****************************************************************************
*				AxiomBase
*
*	Description:	�����, ���������� �� ������ ���� ������
*	Author:		dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_READAXIOMBASE_HXX
#define __AXIOMS_LIB_READAXIOMBASE_HXX

#include "./elem_conditions/ElemCondition.h"
#include "ElemConditionsFactory.h"
#include "AxiomLibException.h"
#include "AxiomExprStructure.h"
#include "Environment.h"
#include "all_axioms.h"
#include "Axiom.h"
#include <iostream>      
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
//includes for using boost
#include "boost/tokenizer.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/fstream.hpp"

namespace AxiomLib {

class AxiomBase {

 private:

	// ����� ���� ������ � ���� ������
	std::map <std::string, std::vector<std::string> > axioms;

	// ��� ���������� ����� ������
	std::string name;
	
 protected:
	
	// ���������� ��� ������� ���������
	typedef boost::tokenizer<boost::char_separator<char> >      token_separator;
	
	// ������� ������ ������ �� ax-�����
	signed int readFromAX (std::string fileName, std::vector<std::string> &vec);
	
	// ������� ������ ������ �� x-�����
	signed int readFromX (std::string fileName, std::string &strName);
	
	// ������� ��������� ���� � ��������
	bool checkName (std::string name1, int from, int upTo, std::string name2) const;

	// ������� ��������� ������ � ������  ����� � ������ � ����� ����� - ��������� ���������� ������ �������� ������
	bool compareStringAndPath (std::string a, std::string b, const char* n3) const;
	
 public:
 
	
	// ������� ���������� �����  ������ � ��������� ���������
	int size (void) const;
	
	// �� ������ ��������� � private ! ! !
	// ��� ���������� ����� ������
	//std::string name;
 
	// ������ �����������
	AxiomBase (void);
	
	// ������� ������ ��� ������� �� ��������, ���� ������� ������ � ����� ������������
	signed int readAxioms (const char* axiomBaseDir);
	
	// ������ ������ ������ ������������ ������� �������������� ����������� �������, ���� ������ ������ ���� �� ������� ������� � �������� ������
	signed int getVectorOfEC (std::vector<std::string>& toReturn, std::string axiomName);

	// ������� ���������� �������� ������� � �� ���������� � ��������� AxiomExprStructure
	signed int readFromAX (const std::string& filePath, AxiomExprStructure &aes) const;

	// ������� ���������� �������� ������� � �� ���������� � ��������� AxiomExprStructure
	signed int readFromAX (const std::string &filePath, AxiomExprStructure &aes, const std::vector<std::string> &paramNames) const;

	// ������ �� ��� ��������� ����� ������ �������� �������
	signed int readFromAX (boost::filesystem::ifstream &file, AxiomExprStructure &aes, const std::vector<std::string> &paramNames) const;

	// ������� ������ ������� � ����
	signed int saveToAX (const std::string& baseDir, AxiomExprStructure &aes) const;

	// ������� ������ ������� � ����
	signed int saveToAX (const std::string& baseDir, AxiomExprStructure &aes, const std::vector<std::string> &paramNames) const;

	// ������� ������ ������� � ��� �������� ����
	signed int saveToAX (std::ofstream &file, AxiomExprStructure &aes, const std::vector<std::string> &paramNames) const;

	// ������� ���������� ���������� ������������� ������� �� �����
	signed int readFromEC (const std::string& filePath, ElemCondPlusStruture &ecps) const;

	// ������� ���������� ���������� ������������� ������� �� �����
	signed int readFromEC (const std::string &filePath, ElemCondPlusStruture &ecps, const std::vector<std::string> &paramNames) const;

	// ������ �� ��� ��������� ����� ������ ������������� �������
	signed int readFromEC (boost::filesystem::ifstream &file, ElemCondPlusStruture &ecps, const std::vector<std::string> &paramNames) const;

	// ������� ������ ���������� ������������� ������� � ����
	signed int saveToEC (const std::string &baseDir, const std::string &fileName, ElemCondPlusStruture &ecps) const;

	// ������� ������ ���������� ������������� ������� � ����
	signed int saveToEC (const std::string &baseDir, const std::string &fileName, ElemCondPlusStruture &ecps, const std::string &paramName) const;

	// ������� ������ ���������� ������������� ������� � ����
	signed int saveToEC (const std::string &baseDir, const std::string &fileName, ElemCondPlusStruture &ecps, const std::vector <std::string> &paramNames) const;

	// ������� ������ ���������� ������������� ������� � ��� �������� ����
	signed int saveToEC (std::ofstream &file, ElemCondPlusStruture &ecps, const std::string &paramName) const;

	// ������ �� ����� �� ������� �������� ������
	signed int readFromList (const std::string &filePath, std::vector <std::vector <std::vector <std::vector <std::string> > > > &strList) const;

	// ������ �� ����� �� ������� �������� ������
	signed int readFromList (const std::string &filePath, std::vector <std::vector <std::string> >  &strList) const;

	// ������� ������ � ���� ������ � ���������� ������
	signed int saveToList (const std::string &baseDir, const std::string &fileName, const std::vector <std::vector <std::vector <std::vector <std::string> > > > &strList) const;

	// ������� ������ � ���� ������ � ���������� ������
	signed int saveToList (const std::string& baseDir, const std::string& fileName, const std::vector <std::vector <std::string> > &strList) const;
	
	// ���������, ����� �� ��� ����� �������� ����������
	static bool checkExtension(const std::string& fileName, const std::string& extension);
	
	// ��������� ����� ����� �������� ����������, ���� ��� ��� ��� �� �����
	static void checkedAddExtension(std::string& fileName, const std::string& extension);
	
}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_READAXIOMBASE_HXX */
