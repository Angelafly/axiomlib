/***************************************************************************
*					AxiomSetBrowser
*
*	Description:	�����, ���������� ���������������� ���� � ��������� 
*					������ ������ � ����������� �� ���
*	Author:			dk
*	History:
*
****************************************************************************/
#ifndef AXIOM_SET_BROWSER_H
#define AXIOM_SET_BROWSER_H

#include "ui_AxiomSetBrowser.h"
#include "AxiomSetStructure.h"
#include "AxiomStructure.h"
#include "AxiomBrowser.h"
#include "Environment.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/fstream.hpp"
#include "boost/tokenizer.hpp"
#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QString>
#include <QTextStream>

class AxiomSetBrowser : public QWidget, public Ui::asBrowser {

	Q_OBJECT

 private:

	// ���� � ����������, ���������� ���������� ������� ������
	std::string axiomSetsPath;

	// ������ ��������� �������� ������ ������
	std::vector <AxiomSetStructure> assVec;

	// ���������� ��� ���������� � ������� ���������
	typedef boost::tokenizer<boost::char_separator<char> > token_separator;

	// ������� ��������� �������� ������ ������ � ���������� �� ��� � ��������� ����������
	void findAxiomSets(const std::string pathToAxiomSets);
	// ������� ������� ���������� �������� ������� � ��������� ������ ������
	void clearTable ();
	// ������� ��������� �������� ������� ������ � �������
	void addAxiomSet (std::string &path);
	// ������� ������ ������ �� ����� �������� ������� ������ �� ���������� ���������
	int readFromAS (std::string &filePath, AxiomSetStructure &ass);
	// ������� ������ �������� ��������� �������
	void showAxiomDescription (AxiomSetStructure &ass, const int axiomNum) const;

 public:
	
	// ����������� ������
	AxiomSetBrowser(const std::string pathToAxiomSets);
	// ���������� ������
	~AxiomSetBrowser();

 public slots:

	// �������, ���������� �������� �������������, �������� �������� ������: 
	void cellCalled(int , int ); // ������������� ������ ��������� ��� ����������� �������, �������� ���� � ������������ ��������� �������
	void refreshForm(); // ������������� ������ ���������� �����, �������� ����� ���������� ����� ����������� ���������� � ����������� ������� � ��������� ������
	
}; // end of class

#endif // AXIOM_SET_BROWSER_H
