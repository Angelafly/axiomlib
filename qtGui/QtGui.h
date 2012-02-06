/***************************************************************************
*					QtGui
*
*	Description:	�����, ���������� ���������������� ��������� ���� ���.
*	Author:			dk
*	History:
*
****************************************************************************/
#ifndef QTGUI_H
#define QTGUI_H

//#include <QObjectCleanupHandler>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <iostream>

#include <QtGui>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/tokenizer.hpp>

#include "ui_QtGui.h"
#include "AxiomSetBrowser.h"
#include "ResBrowser.h"

class QtGui :public QMainWindow, public Ui::mainWindow {

	Q_OBJECT

 private:

	QProcess *process; // ������ �� ����������� �� ��� ������� �������
	FILE *logFile; // ������ �� ��� ���� ��� �������� ������������ ��������
	bool writeToLog; // ������ �������� ���������� - ��������� �� ���-���� ��� ���
	bool curSysIsWindows; // ������ �������� ��� ����������� - ����������� �� ��� �� ������������ �������� Windows
	bool isExtendedGUI; // ���� ����, ��� ������������ ����������� GUI ��� FuzzyData
	unsigned int procNum; // ����� ��������� ��� ������������� MPI
	struct tm timeStart; // ����� ������� �������� ��������
	

	// ���������� ��� ���������� ��������� � ������� ���������
	typedef boost::tokenizer<boost::char_separator<char> > token_separator;

	//QObjectCleanupHandler cleanupHandler;

	void checkExtendedGUI();
	void getDataSetNamePath(const QString &dataSetFullPath, QString &dataSetName, QString &dataSetPath);
 public:
	
	// ����������� ������
	QtGui(QWidget *parent = 0, Qt::WFlags flags = 0);
	// ���������� ������
	~QtGui();

	// ���������� ���� � ������������ ����� �������� �������� ��� ���������� ���������
	QString getAlgPath(QString algLabel, QString workDir, QString &algName);
	// �������� ��� �� ����������� ���� ����� �������� ���� ���������
	bool checkFieldsEntered ();
	// ���������� ��������� �� ������� ���������
	int systemMsgHandler (std::string &str);
	// ������� ������������� �������� ���������� ���������� ��������
	void moveProgressBar (std::string str);
	// ������� �������� ������ ������ �� ������, ���������� ���� � ����� ��� �����
	const char* removeSplashes (std::string &str);
	// ������� �������� ������ � ������
	void addSplashes (QString &str);
	// ������� ��������� ������ � �������
	void addQuotes (QString &str);
	// ������� ��������� ������ � ������ ����������
	int addArgsToList (const std::string str, QStringList &args);
	// ������� ������� ����� ���-���� ������ �����������, ���� �� ����������
	void createLogFile ();
	// ������� ������� ���������� �� ���������� ������
	int createDir (std::string dir);
	// ������� ���������� � ���-���� ���������� ������
	void toLogFile (std::string &str);
	// ������� ������������ ������ � ���-���� � ��������� ���
	void closeLogFile ();

 public slots:
	
	// �������, ���������� �������� �������������, �������� �������� ������ � ���:
    void getPathParams(); // ������������� ������ ���� ��� ����������������� �����
	void getPathDataSet(); // ������������� ������ ���� ��� ������ ������
	void getPathWhereToSave(); // ������������� ������ ����, �� �������� ��������� ����������
	void getPathWorkingDir(); // ������������� ������ ���� ������� ����������
	void getPathMpiexec(); // ������������� ������ ���� � ������������ ����� mpiexec
	void editConfig(); // ������������� ������ �������������� ����������������� �����
    void clear();  // ������������� ������ �������� ��������� ���� ����������� �����������
    void startLog(); // ���������� ��� ������ ������ � ���-����
	void about(); // ����� ���������� � ���
	void checkBoxFunc(); // ���������� ��� ����� �� ������� ���������� �����������
	void checkBoxUseMpi(); // ���������� ��� ����� �� ������� ������������� mpi
	void startChgState (); // �������� ������� ��� ��� ��� ������� �� ������ ������
	void start(); // ���������� ��� ������� �� ������ ������, �������� ������ �������� ��������
	void stop(); // ���������� ��� ������� �� ����, ������������� ���������� �������� ��������
	void showVersion(); // ���������� ������� ������ ���
	void showHelp(); // ���������� ����������� ���������� �� ������������� ���
	void runASBrowser(); // �������� ���� � ������������ ������ ������
	void runResBrowser();  // �������� ���� � ������������ ����������� �������������
	void clearAllFields(); // ������� ���������� ���� ����� ���
	void exit(); // ��������� �������� ���� ���
	void processOutput(); // ����������� ��� ����������� ������ �� �������� ��������, ������������ ��.

}; // end of class

#endif // QTGUI_H
