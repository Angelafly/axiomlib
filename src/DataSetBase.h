#ifndef DATASETBASE_H
#define DATASETBASE_H

#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

#include "EnvDataSet.h"
#include "MultiTS.h"

namespace AxiomLib {

class DataSetBase
{
public:
    DataSetBase();
	
	// ������� ������ ����������
	signed int readParams(std::string path);
	
	// ������ ������� �� ����� baseDataSetDir
	signed int readDataSet(const std::string &baseDataSetDir);

	// �������� ���������� � ��������� ��������� � ������������ ������������� �� ��� �����
	int placeOfParam(std::string parameter);

	// �������� ���������� �� ������ ��������� �� ��� ������ � ������������ �������������
	std::string paramName(int parameter) const;

	// �������� ���������� �� ������ ����������� ��������� �� ��� ������ � ������������ �������������
	std::string className(int parameter) const;
	
	// ����� ������� ���������� ��������� � ������������ �������
	std::vector<std::string> getClassNames() const;

	// �������� ���������� ��� ���� ��������� ���������� ������ ������ � �� ������������ �������������
	const std::vector <std::string>& getParamNames (void) const;
	
	// ������� �� ������� ������������ ������� ������ ������� ���������� � ������������ ������������� �� ������� ������� ������ ������� ������
	int getParamNums(std::vector<int>& toReturn, const Environment &env, EnvDataSet &envDataSet);
	
	int getParamNums(std::vector<int>& toReturn) const;
	
	std::vector<int> getParamNums() const;
	
	// ��������� ���������� � ����� ��������� � ������� �������� ����������
	int paramNamesSize (void) const;
	
	// ������� ������������� ����� ����������� ��� ��������� ����� ���� ���������� �� ��������� ����
	int setNullStr (const std::string newNullStr);
	
	// ������� ������������� ����� ����������� ��� ��������� ����� ���� ���������� �� ��������� ����
	int setNullStr (EnvDataSet &envDataSet);
	
	// ������� ������������� ����� ����������� ��� ����� ������� � ��������� ����������
	int setNameStr (const std::string newNameStr);	
	
protected:
	// �������� ������� ��������� �������� (�������� �� ����� class_names)
	std::vector<std::string> classNames;
  
	// �������� ���������� (������� ������)
	// � ���� ������� ��� ���� � ������������ �������
	// ��� ������� ������ ��� �������� ����������� � ������������� �������
	std::vector<std::string> paramNames;
	
	// ������ ������������ ���������� (������������)
	std::vector<int> paramNums;

	// ������ - ������ ������������ ������� ���� ������� �������������� � *-ref.csv ������
	std::string name;

	// ������ - ������ ������������ ���������� �� ������ ������� ������������� ������ ���� �� ��������� ������� � *-ref.csv ������
	std::string null;
  
	// ���������� ��� ������� ���������
   typedef boost::tokenizer<boost::char_separator<char> >      token_separator;
	
	void setupParamNums(const Environment& env, const EnvDataSet& envDataSet);
   
   // ������� ���������� ������ ������� ��������� �������� whatToFind ��� -1, ���� ������� �� ������
	// ������� �������� � Common.h
//   template<class T> 
//   static typename std::vector<T>::size_type isIn(const T& whatToFind, const std::vector<T> &whereToFind);
 
   // ������� ���������� ����� - ���������� � ���������� ������ - ������������ �� ���� ���� �������� � ����
   signed int getIntByName (std::string str);
 
   // ������� ������ ������ �� csv-�����
   signed int readFromCSV (std::string fileName, MultiTS &multiTS);
   
   // ������� ������ ������ �� ����� � ����������� � ��������� � ���� �������, ���������� ��������������� ��������� ���������
   signed int readFromRefCSV (std::string fileName, MultiTS &multiTS);
   
   // ������� ������ ������ ���������� ������
   signed int readClassTS(const std::string& dirName, ClassTS &classTS);
   
   // ������� ������� �� ������ � ������ ��������� ���� ��� ������� �� ������� � ��������� ����������
   signed int readReferenceTS (const std::string &path, ReferenceClassesTS &refClassesTS);

   // ������� ��������� ���� ����� (���� ������), ������ ������ ����� ��������� ����� * � ? ��� ���������� ����������� ������� ��� ������ ��������
   static bool checkName (std::string name1, int from, int upTo, std::string name2);   

   // ������� ��������� ������������ ����� ����� �������
   static bool checkRefCSVName(const std::string &name);
private:
};

//template<class T>
//typename std::vector<T>::size_type DataSetBase::isIn(const T &whatToFind, const std::vector<T> &whereToFind) {
//	auto iter = std::find(whereToFind.begin(), whereToFind.end(), whatToFind);
//	return iter != whereToFind.end() ? std::distance(whereToFind.begin(), iter) : -1;
//}

};

#endif // DATASETBASE_H
