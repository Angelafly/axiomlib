/****************************************************************************
*
*				������� ������ StartStrategy
*
****************************************************************************/

#include "StartStrategy.h"

using namespace AxiomLib;


// ���������� ������ � �������� ��������� ���������� �� ���������
StartStrategy::StartStrategy (void) {
	gaVector.clear();
	popVector.clear();
	treatVector.clear();
} 


// ���������� ������ - �������� ��� ��������� � ������ ������ ������������ �������
StartStrategy::~StartStrategy (void) {}


/****************************************************************************
*				StartStrategy::initFromEnvStrart
*
*	Description:	������� ������������� ���������� ������ ����� ������ ���������������� ������ envStart
*	Parameters:	-
*	Returns:		-
*	Throws:		AxiomLibException - 
*	Author:		dk
*	History:
*
****************************************************************************/
void StartStrategy::init (void) {
	// ���������� ���������� ������� ������
	Environment envStart;
	envStart.readConfigFile("./axiomlib_start.conf"); // ���� ����� ������� ��� ���� ����� ��������� - �� ����� ������ �������� �� ���� - ����� ����� ���� � ����������� � ���� ��������� 
	
	// ���������� ����� ������� � ������ �� ������� ������������� � ���������� ����������� 
	// (�� ���� ���� numOfStarts ����� 5, � ����� ������������ ������� ������������� ���������� 10, 
	// �� ����� ����� ������� 50 ������� �� ������� ������� ������ - �� ������� ������ ������������� � ���������� ����������� )
	if (envStart.getIntParamValue(numOfStarts, "numOfStarts") < 0)
		throw AxiomLibException("StartStrategy::init : data set directory is undefined.");	
	
	// ������������� ������� �������������:
	// 	��������� ��� �������� ������������ ������� ������������� 
	// 	�� �������� ������ ������������� �������� � ������������ ������� ������� ��������������� ������
	TreatmentFactory tf;
	std::set<std::string> names;
	if (envStart.getStringSetParamValue (names, "Treatment") < 0) // ����� _ � ������� ��������� ������� ������ ���� � ������ TREATMENT - ��� ����� �������� �����
		throw AxiomLibException("Error in StartStrategy::init : treatment strategies are undefined.");
	if (names.size() < 1) 
		throw AxiomLibException("Error in StartStrategy::init : incorrect number of treatment strategies.");
	treatVector.resize(names.size() * numOfStarts);
	std::set<std::string>::iterator iter;
	iter = names.begin();
	for (int g = 0; g < (int) names.size(); g++) {
		if (iter == names.end()) 
			throw AxiomLibException("Error in StartStrategy::init : error in creating treat Strategies.");
		for (int i = 0; i < numOfStarts; i++) {
			treatVector[g * numOfStarts + i] = tf.create (*iter);
		}
		iter++;
	}
	
	// ������������� ��������� ������ ������ - �� ������ �� ����������������� ����� ������ ������������ ����������� ������ ������ � ��������� ���
	std::string datasetDir, datasetName;
	if (envStart.getStringParamValue(datasetDir, "BaseDataSetDir") < 0)
		throw AxiomLibException("StartStrategy::init : data set directory is undefined.");
	if (envStart.getStringParamValue(datasetName, "DataSet") < 0)
		throw AxiomLibException("StartStrategy::init : data set is undefined.");
	// ��������� ����������� ��� ������� ������ ��������� � ������ ������
	EnvDataSet envDataSet;
	envDataSet.readConfigFile (datasetDir, datasetName);
	// ��������� ����������� ����������� NullStr - ����������� ��������� � ������ ����� ���� ������ ���� ���������� ���������
	dataSet.setNullStr (envDataSet);
	// ���������� ��������� ����� ������ - ������� ��� �� ���������� �������������
	dataSet.readDataSet(datasetDir, datasetName);
	// ��������������� � ������ ������ ������ �������� ���������� � ������������ �������������� �� ������� ����� ����� �������� ��������
	dataSet.getParamNums(params, envStart, envDataSet);
	
	// ����������� ����� ����� ������ ����� �������� �������� ��������� ��� ������� ������������� ��������� ������� ������� ������
	if (envStart.getStringParamValue(bgConfigFile, "bgConfigFile") < 0)
		throw AxiomLibException("StartStrategy::init : bgConfigFile is undefined.");
	
	// ���������� ���������� �� ����� �������� � ������� ��������� ���������� ���������������� �� ������� �����
	//...
}


/****************************************************************************
*				StartStrategy::sort
*
*	Description:	������� ���������� ������� �������� mask �� �������� ��������  resVector
*	Parameters:	mask - ����������� ������
*				resVector - ������� �������� - �� �������� ������� ����������
*	Returns:		0
*	Throws:		AxiomLibException - ���� ������� ������ �����
*	Author:		dk
*	History:
*
****************************************************************************/
inline int StartStrategy::sort (std::vector<int> &mask, std::vector<double> &resVector) const {
	// �������� ������� ���������
	if ( (mask.size() != resVector.size()) || (mask.size() < 1) )
		throw AxiomLibException ("Error in StartStrategy::sort: input parameters are of wrong length.");
	// ���������� ���������� �� �������� - ���������� � ������� ���������� �� goals �������� � mask
	int j = 0;
	for (int i = 0; i < (int) resVector.size(); i++){
		int j = 0;
		// ����� ����������
		while ( j < i ) {
			if (resVector[i] < resVector[mask[j]]) {
				break;
			}
			j++;
		}
		// �������� ����, � ���� goal ������
		for (int k = i; k > j; k--) {
			mask[k] = mask[k-1];
		}
		// ���������� ���������� �������
		mask[j] = i;
	}
	return 0;
}


/****************************************************************************
*				StartStrategy::deleteProcs
*
*	Description:	������� ������� ��������������� � ����������� �� ������ 
*				�� �������� � ������� alive - ���� ��� false - �� ��������������� 
*				������� ��������� �� ������� ����������������, ��, ���������.
*	Parameters:	alive - ������ �������� - ����� ��������������� �������� � ������������, � ����� ���
*	Returns:		0
*	Throws:		AxiomLibException - ���� ����� �������� ������� �������� � ������ ���������� �������� ������
*	Author:		dk
*	History:
*
****************************************************************************/
inline int StartStrategy::deleteProcs (std::vector<bool> &alive) {
	// �������� �� ������������ ���� ��������
	if ( (alive.size() != treatVector.size()) || (alive.size() != gaVector.size()) || (alive.size() != popVector.size()) )
		throw AxiomLibException("Error in StartStrategy::deleteProcs: input vector's length is incorrect.");
	// ���������� ���������� ��� �������� ��������� � alive ��������� ��������
	std::vector<Treatment *>::iterator iter1;
	std::vector <BaseGenetics>::iterator iter2;
	std::vector <AxiomSetPop>::iterator iter3;
	vector <bool>::iterator witer;
	// �������� ����� ��� �����������, ��� ������ ������� ������ �� ���� -  ����� �� �����
	int flag = 0;
	while (flag == 0) {
		flag = 1;
		// ��������� ���������� �� ������ ��������
		witer = alive.begin();
		iter1 = treatVector.begin();
		iter2 = gaVector.begin();
		iter3 = popVector.begin();
		// ����� �� ������� - ��� ��������� ���� ����� ������� - ������� � �������� ������ �� �������� ������
		while ( (iter1 != treatVector.end()) && (iter2 != gaVector.end()) && (iter3 !=popVector.end()) ) {
			if (*witer == false) {
				alive.erase(witer);
				delete *iter1;
				treatVector.erase(iter1);
				gaVector.erase(iter2);
				popVector.erase(iter3);
				flag = 0;
				break;
			}
			witer++;
			iter1++;
			iter2++;
			iter3++;
		}
	}
	return 0;
}


// �������� �� ����� ������������� ������� ���������� (���������� � ������������� ������ �������� � ������� �� ������� ���������������)
/****************************************************************************
*				StartStrategy::reduce
*
*	Description:	������� ���������� ���������� ������������ ���������������� � ����������� 
*				� ����������� �� ������ ���� i � ����������� (�������������� �������� � mask)
*	Parameters:	mask - ������ �������� ����������������� � ������� ��������� ������� �������
*				i - ����� ���� �� ������� ���������� ������ ����������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
inline int StartStrategy::reduce(const int i, const std::vector<int> &mask) {
	// ������� ������ �������� - ����� ��������������� � ������ ����������� �������� 
	std::vector <bool> alive;
	alive.resize(mask.size());
	for (unsigned int a = 0; a < alive.size(); a++)
		alive[a] = true; // ������� ��������� ��� ���������������
	// ������ ����������� ��������� 
	alive[mask[mask.size() - 1]] = false; // ����������� ��������� �� �������� �������������� � �����������
	// ��������������� ������ ��������� � ���������� ������ 
	deleteProcs (alive);
	return 0;
}


//  �������� �� ����� ������������� �������, ������ ���������
/****************************************************************************
*				StartStrategy::countNumOfIters
*
*	Description:	������� ���������� ����� �������� �� �� ������ ��������
*	Parameters:	i - ����� ���� ��������� �����
*	Returns:		����� �������� ��
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
inline int StartStrategy::countNumOfIters(const int i) {
	return 2;
}


/****************************************************************************
*				StartStrategy::saveToFile
*
*	Description:	������� ���������� ��������������� � ����������� � �������������� �������� ������ � ����
*	Parameters:	i - ������ � ������� ����������������
*	Returns:		0 - ���� ���������� ������ �������
*				-1 - ���� ���������� �� ����
*	Throws:		AxiomLibException - ���� ��������� ������ ������� �� ��������� ������� ��� ���������� ��������� ������ �� �����������
*	Author:		dk
*	History:
*
****************************************************************************/
int StartStrategy::saveToFile (const int i) const {
	// �������� �������� ���������
	if ( (i < 0) || (i >= (int) treatVector.size()) || (treatVector.size() != gaVector.size()) || (treatVector.size() != popVector.size()) )
		throw AxiomLibException("Error in StartStrategy::saveToFile: base vectors are of different size or input index is out of range.");
	// ���������� i-�� ��������������� � ����������� � ������ �������� ������ �� ��������������� ��������� 
	// ��������� ����� - ���� ��������� - ������� �� ����������������� ����� ��� ������������� � �������� �� ���� ������� �� ���������� ���������� ������
	
	// ... saving... - ���� �� ������� - ���� �� ��������� ������ (����� ����� � ������� ����� ����)
	
	return -1;
}


/****************************************************************************
*				StartStrategy::start
*
*	Description:	�������� ������� ������ - ������ ��������� ������ �������
*				��������������� � �����������. ��� ����������� ������
*				�������� �� ������������ ����������������� �����.
*	Parameters:	-
*	Returns:		-
*	Throws:		AxiomLibException - ���� ���������� ���������� ������ ���� ����������� ���������� ��� ��������
*	Author:		dk
*	History:
*
****************************************************************************/
void StartStrategy::start (void) {
	// ��������������� ��� (��������) ������ ������� (������ � �����):
	// 1. ����������� ���������� ������� ������
	//	(����� ��� ��������� ������� ��, ��������� ���������������� � ��.)
	// 2. ������� ��������� ���������� ������ �� ��������� ���������� 
	//	(���� ������ ��������� ������� ��, ���������� ���������� ��� ���, �������� �������� ��������� ���������, ���� �������� ������� � ������ ������� ��, ���������� ������ ������)
	// 3. �������� ���� ������� : 
	// 	3.0, �������� - �� ������� �� �� � 1-�� ��������������� � 1-�� ������ ���������� 
	//		(� ���� ������ ������� ����� �� �����)
	//	3.1. ��� ������� ���������� ������ ������������� ��������� ����� ������ ��� ������������� � ������� �����������
	//	3.2. �� ����� �������������� ������ ������ - ������ ������ ����������� ������� ������  � ������������ ������ ������ ������
	//	3.3. ��������� ���������� ������ ������� ������������ ���������� ������� ������ ������ 
	//		(���������� �����������, ��������� ��������� ����������� � ��������� �������������, ������� ������������ ������� �� � ��������� � ��� �������������� ��������)
	//	3.4. ��������� ���������� ������� ������� �� � ������������ � ����� ������� ����� 
	//		(��������� ���������� ����������� ���������������� - ����� ���� �������, �������� ������� � ���������� ������������ � ��������� � ���� ����������� ������� treatVector)
	// 4. ������� � ������ ��������������� � ������ ������ ���������� - ��������� ������ � ��� - � ���� �������, ������ ���������� - �������� ������� ������� � ��.
	
	// ���������� ���������
	// 1. ����������� ����������
	init ();
	
	// 2. �������� ����������� ��������� ���������� �� ����� � �������� ����
	if (/*numOfStarts **/ treatVector.size() <= 0) 
		throw AxiomLibException("StartStrategy::start: wrong number of starts.");
	popVector.resize(/*numOfStarts **/ treatVector.size());
	gaVector.resize(/*numOfStarts * */treatVector.size());
	Environment env; // ������ �������� ��� ������� ��
	env.readConfigFile(bgConfigFile.c_str());
	// ��������� ��������������� ��������
	for (unsigned int i = 0; i < gaVector.size(); i++) {
		(gaVector[i]).shortInitFromEnv(env);
		(gaVector[i]).setParamsOfDataSet(params);
	}
	
	// 3. �������� ���� ������� 
	bool exitCycleCond = false; // ������� ���������� ��������� ����� (�� ���� � ��������� ����� ������ �������� � 1-�� ����� ������������ ���������������� � ��� ����������)
	int curNumOfIter; // ������� ������������ ����� �������� �� �� ������ �� ���������������� � ������� ����������
	std::vector <int> mask;
	std::vector <double> resVector;
	resVector.resize(treatVector.size());
	int num = 0; // ����� �������� 
	while (exitCycleCond == false) {
		num++;
		std::cout << "\n	num is " << num;
		// 3.0. ��������� ���������� exitCycleCond
		if (gaVector.size() <= 1) {
			exitCycleCond = true;
		}
		if ( ( treatVector.size() != gaVector.size() ) || (treatVector.size() != popVector.size()) ) {
			std::cout << "\n 	treatVector.size() is " << treatVector.size() << "	gaVector.size() is " << gaVector.size() << "	popVector.size() is " << popVector.size() << "\n";
			throw AxiomLibException("Error in StartStrategy::start : base vectors are of different size.");
		}
		curNumOfIter = countNumOfIters(num); // ��������� ����� �������� ��
		for (unsigned int i = 0; i < treatVector.size(); i++) { // ������ ���� ������� ��������� ���������� �� i
			// 3.1. ������ ��������� ������������� ������ ������
			DataSet newDataSet; // ��������� ������ ��� ��������� ������ ������ ����� ������������� ������� ����������������
			(treatVector[i])->process(newDataSet, dataSet);
			// 3.2. ������ ������� ������� ������
			(gaVector[i]).setDataSet(newDataSet); // ��������� ����������� ���������� ��� ������� ��
			(gaVector[i]).setNumOfIter (curNumOfIter);
			//std::cout << "\n		i is " << i+1 << "	in";
			resVector[i] = (gaVector[i]).run (popVector[i]); // ���������� ������ �� ������� ����������� ������� ������
			//std::cout << "\n		i is " << i+1 << "	out";
			(treatVector[i])->addRes(resVector[i]); // ���������� � ������� �������� ��������������� ����������� �� ������ ����� ����� - ���������� ��� ����������� ��� �������� ���������
		}
		if (exitCycleCond == true) break;
		//std::cout << "\n gaVector.size() is " << gaVector.size();
		//std::cout << "\n Going to reduce. Press enter.";
		//std::cin.get();
		//throw AxiomLibException (" TEST ");
		// 3.3. ���������� �� �������� ��������� ������� �������
		mask.resize(resVector.size());
		for (int j = 0; j < (int) mask.size(); j++)
			mask[j] = j;
		sort (mask, resVector);
		reduce(num, mask); // ������� ���������� ���������� ������������ ���������������� � ����������� � ����������� �� ������ ���� i � ����������� (�������������� �������� � mask)
		// 3.4. ��������� treatVector � ������������ � ��� �������� �, ��������, �������� ���������� ����� ���������������� � ����������� � ����������� �� ���������
		for (unsigned int g = 0; g < treatVector.size(); g++) {
			(treatVector[g])->change (); // ��������� ����������
			// �������� ����� ����������������� � ����������� (�� ������ ������� g-��)
			// add(g); // ���� �� ����������� - �� � ����� �� - ��� ������������ ����������
		}
	}
	 std::cout << "\n	Finish \n";
	// 4. ���������� ������ �� �������� ���������������, ��� ����������, ������� ������, �� ������� ���� ���������� ��������� ����������
	if ( (treatVector.size() != 1) || (gaVector.size() != 1) || (popVector.size() != 1) )
		throw AxiomLibException("Error in StartStrategy::start : The result is multiple or absent.");
	saveToFile (0); // ��������� ������������ �������������� - �� ������ � 0-� �������� �������
}
