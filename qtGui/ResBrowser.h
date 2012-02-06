/***************************************************************************
*					ResBrowser
*
*	Description:	�����, ���������� ���������������� ���� � ������������
*					����������� �������������
*	Author:			dk
*	History:
*
****************************************************************************/
#ifndef RES_BROWSER_H
#define RES_BROWSER_H

#include "ui_ResBrowser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/fstream.hpp"
#include "boost/tokenizer.hpp"
#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QString>
#include <QProcess>
#include <QTextStream>
#ifdef Q_OS_WIN32
	#include <windows.h>
#endif

class ResBrowser : public QWidget, public Ui::resBrowser {

	Q_OBJECT

 private:

	 // ���� � �����, ���������� ������������ ����������
	 std::string pathToResults;
	 // ���� � �������� ������, �������� ������� ������������
	 std::map <int, std::string> axiomSetDirs;

	 // ���������� ��� ���������� � ������� ���������
	 typedef boost::tokenizer<boost::char_separator<char> > token_separator;

	 // ������� ������: 
	 // ������� ��������� �� ���������� � ������ ������ ������ � �������� �� �����������
	 void findAxiomSets(const std::string pathToDir);
	 // ������� ��������� ��������� ������� ������ � ����
	 void addAxiomSetResults (std::string axiomSetPath, std::string &axiomSetName);
	 // ������� ������� ��� ������, ������������ � ������� ����
	 void clearForm ();
	 // ������� ������� ���� �������
	 void clearTableFields();
	 // ������� ������ ���� ����������� ���� � ��� ��� ������ ��� ������� � ��������� ������
	 void setFormEnabledOrNot();
	 // ������� ��������� ���� � ��������� �������������� ����������
	 void openFile (std::string &path);
	 // ������� �������� ����������� ����������� ����
	 void showFile (std::string &path, int whatToShow);

 public:
	
	 // ����������� ������	
	 ResBrowser(const std::string pathToDir);
	 // ���������� ������
	 ~ResBrowser();

 public slots:
	
	 // �������, ���������� �������� �������������, �������� �������� ������: 
	 void refreshForm (); // ���������� ��� ������� �� ������ ����������, �������� ���������� ����������� ����
	 void showRow(); // ������������� ������ ����������� ����, �������� ��� ����������� �����������
	 void showResults(); // ������������� ������ ����������� ����������� �������������, �������� �� ����������� ���
	 void loadContents (int index); // ������� ��������� ���������� ����������� ��� ��������� ������� ������
	 void muCellCalled (int row, int column); // ������������� ������ ����� � ��������� �������� ��������� ����������
	 void resCellCalled (int row, int column); // ������������� ������ ����� � ��������� ����������� �������������
	
}; // end of class

#endif // RES_BROWSER_H
