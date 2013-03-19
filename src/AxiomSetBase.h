/****************************************************************************
*				AxiomSetBase
*
*	Description:	�����, ���������� �� ������ ���� ������
*	Author:		dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_READAXIOMSET_HXX
#define __AXIOMS_LIB_READAXIOMSET_HXX

#include "./elem_conditions/ElemCondition.h"
#include "ElemConditionsFactory.h"
#include "AxiomBase.h"
#include "AxiomLibException.h"
#include "Environment.h"
#include "all_axioms.h"
#include "Axiom.h"
#include "AxiomSetStructure.h"
#include "AxiomStructure.h"
#include "AxiomExprSetStructure.h"
#include "AxiomExprStructure.h"
#include "ElemCondPlusStruture.h"
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

class AxiomSetBase {

 private:
	
 protected:
	
	// ���������� ��� ������� ���������
	typedef boost::tokenizer<boost::char_separator<char> >      token_separator;
	
	// ������� ������ ���������� � ������ � ��������� ����������� ���������
	bool checkName (std::string name1,signed int from,signed int upTo, std::string name2) const;
	
	// ������� ���������� ����� ����� ���������� �� ���� � �����, � �� ������ �����
	bool compareStringAndPath (std::string fileName, std::string path, const char* n3) const;
	
 public:
	// ���������� �������� � privat ! ! ! - ����� �������� ���� ! ! !
	// ��� ��������� � ������ ���������� ������ ������ ������
	AxiomSets axiomSets;
	
	// ������ �����������
	AxiomSetBase (void);
	
	// ������� ���������� ������ ������ - ������ ��������� ������� �� ����������������� �����
	signed int readSets (const char* axiomSetBaseDir);

	// ������� ���������� ���������� ������� ������
	signed int readFromAS (std::string filePath, AxiomSetStructure &ass);

	// ������� ���������� ���������� ������� ������ 
	signed int readFromAS (std::string filePath, AxiomExprSetStructure &aess) const;

	// ������� ���������� ���������� ������� ������ (� ��������� ����������� ������ ������, �� ������� ��������� ������������ �������)
	signed int readFromAS (std::string filePath, AxiomExprSetStructure &aess, const std::vector<std::string> &paramNames) const;

	// ������� ���������� ���������� ������� ������ �� ��� ��������� ����� (� ��������� ����������� ������ ������, �� ������� ��������� ������������ �������)
	signed int readFromAS (boost::filesystem::ifstream &file, AxiomExprSetStructure &aess, const std::vector<std::string> &paramNames) const;

	// ������� ������ ������� ������ � ����
	signed int saveToAS (std::string baseDir, AxiomSetStructure &ass, int first = -1, int second = -1) const;

	// ������� ������ ������� ������ � ����
	signed int saveToAS (std::string baseDir, AxiomExprSetStructure &aess, int first = -1, int second = -1) const;

	// ������� ������ ������� ������ � ���� (� ��������� ����������� ������ ������, �� ������� ��������� ������������ �������)
	signed int saveToAS (std::string baseDir, AxiomExprSetStructure &aess, std::vector <std::string> &paramNames, int first = -1, int second = -1) const;

	// ������� ������ ������� ������ � ��� �������� ���� (� ��������� ����������� ������ ������, �� ������� ��������� ������������ �������)
	signed int saveToAS (std::ofstream &file, AxiomExprSetStructure &aess, std::vector <std::string> &paramNames) const;
	
}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_READAXIOMSET_HXX */
