/***************************************************************************
*					TemplateManager
*
*	Description:	����� ����������� �������� ������������� �������� ��� 
*					������ ��������������.
*	Author:			dk
*	History:
*
****************************************************************************/

#ifndef _Template_Manager_HXX
#define _Template_Manager_HXX

#include "AxiomLibException.h"
#include "AxiomBase.h"
#include "Round.h"
#include "Environment.h"
#include "ExtGenetics.h"
#include "TemplateRecognizer.h"
#include "PreprocessFactory.h"
#include "TemplateGenetics.h"
#include "TemplateGeneticsFactory.h"
#include "DataSet.h"
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#include <set>

namespace AxiomLib {

class TemplateManager {

 private:
	
	// ����������� ����� ��������, ������� ����� ������������� �� ����� ��������
	int numOfTemplates;

	// ������ �������� ���������������
	std::vector <TemplateRecognizer> templateRecognizers;

	// �������� ����� ������, �� ������� ������������ ��������
	DataSet dataSet;

	// ������ �������� ���������� � ������������ ������������� �� ������� ����� ������ - ����� ��������� ��������
	// ��� ���������� ���������� - �������� ����������� ���� � ��������� ����� ����������� ��������� �������
	std::vector<int> params;

	std::vector < TemplateGenetics* > templateGenetics;

	// ��������� ��� ���������� ������ �������
	std::string axiomSetBaseDir;
	std::string axiomName;
	std::string whereToSave;
	bool outputToFile;

 protected:

 public:

	// ������ �����������: 
	TemplateManager (void);  
  
	// �����������, ������������ ��� ���������� �������� ������ �������
	~TemplateManager (void);

	// ������� ������������� ��������� ������ �� ������, ���������� �� ������ ���������
	int initFromEnv (const Environment& env);

	// ������� �������� �������� ���������������
	int generateTemplates (const Environment& env);
	
	// �������� ������� ������ - ������ ������� �������������� �� �������
	int run (void);

	// ������� �������� ��������������� �� �������� ������ ������ � ������� ����������� � ����������� �����
	int runExecuteOnly (void);

}; // end of class

}; // end of namespace

#endif /* _Template_Manager_HXX */
