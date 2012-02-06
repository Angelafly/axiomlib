/****************************************************************************
*					������� ������ FullFuzzyDataSet
*
*	Description:	����� FullFuzzyDataSet - ����������
*	Author:			dk
*	History:	
*
****************************************************************************/

#include "FullFuzzyDataSet.h"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/fstream.hpp"

// �����������, ������������ ��� ���������� ������
#define reference_str				"reference"
#define testing_str					"testing"
#define file_name_first_half_str	"ref"
#define csv_file_ending_str			".csv"

using namespace AxiomLib;

/****************************************************************************
*				FullFuzzyDataSet::FullFuzzyDataSet
*
*	Description:	������ ����������� � �������������� ���������� ������ �� ���������
*	Parameters:		-
*	Returns:		-
*	Throws:			-
*	Author:			dk
*	History:		-
*
****************************************************************************/
FullFuzzyDataSet::FullFuzzyDataSet (void) {
	// ������� ���������� ���������� ������ �� ���������
	this->normal.assign ("normal");
}


/****************************************************************************
*					FullFuzzyDataSet::setNormalStr
*
*	Description:	������� ������������� ����� ����������� ��� ������ �
*					�������� � ��������� ������� ��������� ���������� ����������� 
*					���������
*	Parameters:		env - ����� � ������� �� ����������������� �����
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:		-
*
****************************************************************************/
signed int FullFuzzyDataSet::setNormalStr (EnvDataSet &envDataSet) {
	int ret;
	std::string normalStr;
	ret = envDataSet.getNormalStr(normalStr);
	if (ret == 0) {
		normal = normalStr;
	}
	return 0;
}


/****************************************************************************
*				FullFuzzyDataSet::readNormalClassTS
*
*	Description:	������ ��������� ���� ��� ������ �������������, �� ������� 
*					���������� ���������� ��������� �������.
*					����, ����� �� ���� ��������� � $path/reference/. 
*					��� ������� ��������, ����� �� ���� ������� � ��� 
*					.csv-������.
*	Parameters:		path - ������ ���������� ����� �������� ���������� ������ ������
*					normalTS - ����������� �������� ������ �����
*	Returns:		0 - ���� ��� ���� �������� ��������� (����� throw)
*	Throws:			AxiomLibException  - ���� ���������� $path/reference/ - �� ���������� 
*										��� ���� ����� ����������, �� ��� ����
*	Author:			dk
*	History:
*
****************************************************************************/
signed int FullFuzzyDataSet::readNormalClassTS (std::string &path, ClassTS &normalTS) {
	// �������� - ������ �� ����������� ��� ���������� ����������� ���������
	if (normal.size() < 1)
		throw AxiomLibException ("Error in FullFuzzyDataSet::readNormalClassTS: string for normal class is not set.");

	// ������� ���������� ����������� ����������
	normalTS.clear();

	// ������� ����� ��� ������ � ������������ � ������� 
	std::string nPath = path;
	nPath.append ("/");
	nPath.append (normal);
	boost::filesystem::path classPath( nPath ); // ��������� �� ���������� � ������������ ����������� ���������
	boost::filesystem::path filePath( nPath ); // ����� �������������� ����� ������ �� ������������� ���������� � ������ ������ ������������� "normal"
	
	// ������ - �������� ������ ������������ ����� ������ � ���������� ������ ��� ������� ��������������
	std::string fileNameFirstHalf (file_name_first_half_str);
	std::string fileNameLastHalf (csv_file_ending_str);
	std::string aa; // ��������� ����������, �������� ��� �������� ������������ � ������������ ����� 
	
	// �������� - ���������� �� �������� ���� � �����
	if ( !boost::filesystem::exists( classPath ) )
		throw AxiomLibException ("Error in FullFuzzyDataSet::readNormalClassTS: path for normal behavior does not exists.");
	
	// ��������� - ���������� �� ������ ���� - ��� �������������� � ��������� ��������� ������ ������
	if ( boost::filesystem::is_directory( classPath ) ) {
		// ���� �� ������ ��������� ����������
		
		readClassTS(nPath, normalTS);
		
//		boost::filesystem::directory_iterator end_iter_int;
//		for ( boost::filesystem::directory_iterator dir_itr_int( classPath ); dir_itr_int != end_iter_int; ++dir_itr_int ) {
//			filePath = *dir_itr_int;
//			// ��������� �������� ����� � ������� � char* - ����� �������� �� ������������ (�� ./formats.x)
//			aa = dir_itr_int->leaf();
//			if ((checkName (aa, -1, 0, fileNameFirstHalf)) && (checkName (aa, (int) aa.size() - 5, (int) aa.size() - 4, fileNameLastHalf))) {
//				//�������� ���� ������� ���� � �������� � ���������� ��������� ����� multiTS
//				MultiTS multiTStemp;
//				aa = (*dir_itr_int).string();
//				this->readFromCSV (aa, multiTStemp);
//				/*// Test Output
//				std::vector<double> vecTemp;
//				std::cout << "\n MultiTS \n";
//				for (int nR = 0; nR < multiTStemp.size(); nR++) {
//					multiTStemp.getTSByIndex(vecTemp, nR);
//					std::cout << "\n";
//					for (int fR = 0; fR < vecTemp.size(); fR++) {
//						std::cout << "  " << vecTemp[fR];
//					}
//				}*/
//				// ��������� ��������� multiTStemp � ���������� normalTS
//				normalTS.push_back(multiTStemp);
//			}
//		}
	
	} else {// ��������� ���� �� �������� ��������� - ������� ���������� ��������� ����� �� �����������
		throw AxiomLibException ("Error in FullFuzzyDataSet::readNormalClassTS: wrong path - path is not a dir, it is a file.");
	}
	
	return 0;
}


/****************************************************************************
*					FullFuzzyDataSet::ReadDataSet
*
*	Description:	������� ������ ���� ������ �� ������ ������, ������������
*					� ��������� ��������
*	Parameters:		const char* path - ���� � ��������, ����������� ����� ������
*	Returns:		0
*	Throws:			AxiomLibExeption - ���� ������� �����������, ��� ���� ������
*					� ������� ������ ������
*	Author:			dk
*	History:
*
****************************************************************************/
signed int FullFuzzyDataSet::readDataSet (std::string dataSetDir, std::string dataSetName) {
  // ����� ����� ������ �������:
  // 0. ��������� ������������ ����
  // 1. ������ ���� � ���������� ������� � ��������� ������ �������� �������;
  //    ���� ������� ������� ����������� ������������
  // 2. ������ ���� � ���������� ���������� � ��������� ������ �������� ����������
  //    ���� ������� ���������� ����������� ������������
  // 3. ������ ��������� ����
  //    ����, ����� �� ���� ��������� � $path/reference/. ��� ��������
  //    ������ �����, � �������� ��������� ���� �� ����� ��������.
  //        ��� ������� ��������, ����� �� ���� ������� � ��� .csv-������,
  //        ������ ��, �� ������� ��������� � ������������� ����.
  //	������ ���������� ����������� ���������.
  // 4. ������ �������� �������
  //    ����, ����� �� ���� ��������� � $path/testing/. ��� ��������
  //    ������ �����, � �������� ��������� ���� �� ����� ��������.
  //        ��� ������� ��������, ����� �� ���� ������� � ��� .csv-������,
  //        ������ ��, �� ������� ��������� � ������������� ����.
  //	������ ���������� ����������� ���������.

  //0 - 1 - 2 ������ ����������� � ��������� �������: 
	
	std::string curPath;
	std::string baseDataSetDir = dataSetDir;
	baseDataSetDir.append("/");
	baseDataSetDir.append(dataSetName);
	
	this->readParams(baseDataSetDir);
  
  //3 �����

	// ������ ����������, ����������� � ����������� ���������
	curPath = baseDataSetDir;
	curPath.append ("/");
	curPath.append (reference_str);
	readNormalClassTS (curPath, referenceNormalTS);
	
	// ������ ����������, ���������� ������� ���������� ���������
	readReferenceTS (curPath, referenceClassesTS);
	// �������� �� ������������ ����������:
	// - ��� ������� ������ �������������� ��������� ����� �� ���� �� ���� ��������� ���
	// - ��������� ��� �� ������ �������������� ������
	int countMultiTS = 0;
	for (unsigned int h = 0; h < referenceClassesTS.size(); h++) {
		if ((referenceClassesTS[h]).size() == 0) {
			countMultiTS++;
		}
	}
	if ((referenceClassesTS.size() != classNames.size()) || (countMultiTS > 0)) {
		throw AxiomLibException ("Error in FullFuzzyDataSet::readDataSet: cannot find all Classes TSs in reference dataset directory.");
	}
	
  //4 �����

	// ������ ����������, ����������� � ����������� ���������
	curPath = baseDataSetDir;
	curPath.append ("/");
	curPath.append (testing_str);
	readNormalClassTS (curPath, testingNormalTS);

	// ������ ����������, ���������� ������� ���������� ���������
	readReferenceTS(curPath, testingClassesTS);
	// �������� �� ������������ ����������:
	// - ��� ������� ������ �������������� ��������� ����� �� ���� �� ���� ��������� ���
	// - ��������� ��� �� ������ �������������� ������
	countMultiTS = 0;
	for (unsigned int h = 0; h < testingClassesTS.size(); h++) {
		if ((testingClassesTS[h]).size() == 0) {
			countMultiTS++;
		}
	}
	if ((testingClassesTS.size() != classNames.size()) || (countMultiTS > 0)) {
		throw AxiomLibException ("Error in FullFuzzyDataSet::readDataSet: cannot find all Classes TSs in testing dataset directory.");
	}

	return 0;
}


/****************************************************************************
*				FullFuzzyDataSet::getNormalTSFromClass
*
*	Description:	������� ���������� ��������� ���, ��������������� ����������� 
*					���������, �� ��������� ������� �� ������������ - ������ 
*					����������, ������ ���� � ����������.
*	Parameters:		vec - ����������� ��������� ���
*					indexMultiTS - ����� ���������� ������ ��������� ��������� ���
*					indexTS - ����� ���������� ���� � ����������
*	Returns:		true - ���� ��� ������� ������
*					false - ���� ��� �� �����-�� �������� �� ������ (�.�. ���� ��� ���)
*	Throws:			-
*	Author:			dk
*	History:		-
*
****************************************************************************/
bool FullFuzzyDataSet::getNormalTSFromClass (std::vector<double> &vec, int indexMultiTS, int indexTS) const {
	if ((indexMultiTS < (int) (referenceNormalTS.size())) && (indexMultiTS >=0)) {
		return (referenceNormalTS[indexMultiTS]).getTSByIndex (vec, indexTS);
	}
	return false;
}


/****************************************************************************
*					FullFuzzyDataSet::getNormalTSFromTest
*
*	Description:	������� ���������� ��������� ���, ��������������� ����������� 
*					���������, �� ����������� ������� �� ������������ - ������ 
*					����������, ������ ���� � ����������.
*	Parameters:		vec - ����������� ��������� ���
*					indexMultiTS - ����� ���������� ������ ��������� ��������� ���
*					indexTS - ����� ���������� ���� � ����������
*	Returns:		true - ���� ��� ������� ������
*					false - ���� ��� �� �����-�� �������� �� ������ (�.�. ���� ��� ���)
*	Throws:			-
*	Author:			dk
*	History:		-
*
****************************************************************************/
bool FullFuzzyDataSet::getNormalTSFromTest (std::vector<double> &vec, int indexMultiTS, int indexTS) const {
	if ((indexMultiTS < (int) (testingNormalTS.size())) && (indexMultiTS >=0)) {
		return (testingNormalTS[indexMultiTS]).getTSByIndex (vec, indexTS);
	}
	return false;
}


/****************************************************************************
*					FullFuzzyDataSet::getNormalClassSize
*
*	Description:	������� ������ ���������� � ����� ��������� ����� 
*					�������������� ����������� ��������� � ��������� �������
*	Parameters:		numOfMultiTS - ����� �����������
*					numOfTS - ������ ����� ����� � �����������
*	Returns:		true
*	Throws:			-
*	Author:			dk
*	History:		-
*
****************************************************************************/
bool FullFuzzyDataSet::getNormalClassSize (int &numOfMultiTS, std::vector<int> &numOfTS) const {
	numOfMultiTS = referenceNormalTS.size();
	numOfTS.resize (numOfMultiTS);
	for (int i = 0; i < numOfMultiTS; i++) {
		numOfTS[i] = (int) referenceNormalTS[i].size();
	}
	return true;
}


/****************************************************************************
*					FullFuzzyDataSet::getNormalTestSize
*
*	Description:	������� ������ ���������� � ����� ��������� ����� 
*					�������������� ����������� ��������� � ����������� �������
*	Parameters:		numOfMultiTS - ����� �����������
*					numOfTS - ������ ����� ����� � �����������
*	Returns:		true
*	Throws:			-
*	Author:			dk
*	History:		-
*
****************************************************************************/
bool FullFuzzyDataSet::getNormalTestSize (int &numOfMultiTS, std::vector<int> &numOfTS) const {
	numOfMultiTS = testingNormalTS.size();
	numOfTS.resize (numOfMultiTS);
	for (int i = 0; i < numOfMultiTS; i++) {
		numOfTS[i] = (int) testingNormalTS[i].size();
	}
	return true;
}


/****************************************************************************
*					FullFuzzyDataSet::getTSByIndexFromTest
*
*	Description:	������� ���������� ��������� ��� �� ��������� ������� �� 
*					������������ - ������ ������, ������ ���������� � ������, 
*					������ ���� � ����������
*	Parameters:		vec - ����������� ��������� ���
*					indexClass - ����� ������ �������������� �� ���������� 
*						������������� ������� ������, ������ ��������� ���
*					indexMultiTS - ����� ���������� ������ ��������� ��������� ���
*					indexTS - ����� ���������� ���� � ����������
*	Returns:		true - ���� ��� ������� ������
*					false - ���� ��� �� �����-�� �������� �� ������ (�.�. ���� ��� ���)
*	Throws:			-
*	Author:			dk
*	History:		-
*
****************************************************************************/
bool FullFuzzyDataSet::getTSByIndexFromTest (std::vector<double> &vec, int indexClass, int indexMultiTS, int indexTS) const {
	if ((indexClass < (int) testingClassesTS.size()) && (indexClass >=0)) {
		if ((indexMultiTS < (int) (testingClassesTS[indexClass]).size()) && (indexMultiTS >=0)) {
			return (testingClassesTS[indexClass][indexMultiTS]).getTSByIndex (vec, indexTS);
		}
	}
	return false;
}


/****************************************************************************
*					FullFuzzyDataSet::getTestSize
*
*	Description:	������� ������ ���������� � ����� ��������� ����� 
*					� ���������� ������ testingClassesTS.
*	Parameters:		numOfClasses - ����������� ����� ������� ��������������
*					numOfMultiTS - ������ ����� ����������� � ������ �� �������
*					numOfTS - ������ �������� ����� ����� � �������
*	Returns:		true
*	Throws:			-
*	Author:			dk
*	History:		-
*
****************************************************************************/
bool FullFuzzyDataSet::getTestSize (int &numOfClasses, std::vector<int> &numOfMultiTS, std::vector<std::vector<int> > &numOfTS) const {
	numOfClasses = (int) testingClassesTS.size();
	numOfMultiTS.resize(numOfClasses);
	numOfTS.resize(numOfClasses);
	for (int i = 0; i < numOfClasses; i++) {
		numOfMultiTS[i] = (int) (testingClassesTS[i]).size();
		(numOfTS[i]).resize(numOfMultiTS[i]);
		for (int j = 0; j < numOfMultiTS[i]; j++) {
			numOfTS[i][j] = (testingClassesTS[i][j]).size();
		}
	}
	return true;
}
