/****************************************************************************
*
*				������� ������ PreprocessSqueeze
*
****************************************************************************/

#include "PreprocessSqueeze.h"
#include "TemplateRecognizer.h"

using namespace AxiomLib;

#define noValue -1.0
#define minRatio 1.1
#define maxRatio 10.0

// ���������� ������
PreprocessSqueeze::PreprocessSqueeze (void) {
	squeezeParam = 1.0;
	squeezeParamMin = 0.1;
	squeezeParamMax = 10.0;
	LagrangeDegree = 4;
	factorials.resize(LagrangeDegree + 1);
	factorials[0] = 1;
	for (unsigned int i = 1; i <= LagrangeDegree; i++)
		factorials[i] = factorials[i-1] * (double) i;
	goalStrategy = NULL;
} 


// ���������� ������
PreprocessSqueeze::~PreprocessSqueeze (void) {
	if (goalStrategy != NULL) delete goalStrategy;
	
}


/****************************************************************************
*						PreprocessSqueeze::initFromEnv
*
*       Description:	������� ������������� ��������� ������ �� ������, 
*						���������� �� ������ ���������
*       Parameters:		env - ���������, ���������� ����������� ���������
*       Returns:		0
*       Throws:			-
*       Author:			dk
*       History:
*
****************************************************************************/
int PreprocessSqueeze::initFromEnv (const Environment& env) {
	// ��������� ���������� ������ �� ���������
	// ���� ���� �� ���������� �� ����������, �� ������������ �������� �� ���������
	env.getDoubleParamValue (squeezeParam, "squeezeParam");
	env.getDoubleParamValue (squeezeParamMin, "squeezeParamMin");
	env.getDoubleParamValue (squeezeParamMax, "squeezeParamMax");
	// �������� ������������ ������������� ����������
	if ((squeezeParamMin > squeezeParamMax) || (squeezeParamMin < 0.0) || 
		(squeezeParam < squeezeParamMin) || (squeezeParam > squeezeParamMax))
		throw AxiomLibException ("PreprocessSqueeze::initFromEnv : incompatible parameters in environment.");
	// ��������� ��������� LagrangeDegree
	int tmpInt;
	if (env.getIntParamValue (tmpInt, "LagrangeCoeffDegree") >= 0) {
		if ((tmpInt < 1) || (tmpInt > 20))
			throw AxiomLibException ("PreprocessSqueeze::initFromEnv : LagrangeCoeffDegree of an incompatible value in environment.");
		LagrangeDegree = (unsigned int) tmpInt;
		factorials.resize(LagrangeDegree + 1);
		factorials[0] = 1.0;
		for (unsigned int i = 1; i <= LagrangeDegree; i++)
			factorials[i] = factorials[i-1] * (double) i;
	}
	// �������������� ��������� ���������� ������� �������
	std::string goalStrategyName;
	GoalStrategyFactory gsf;
	if (env.getStringParamValue(goalStrategyName, "goalClass") >= 0) {
		goalStrategy = gsf.create(goalStrategyName);
		goalStrategy->setParamsFromEnv(env);
	}
	return 0;
}


/****************************************************************************
*						PreprocessSqueeze::copy
*
*       Description:	������� �������� ����� ��������� �������������
*       Parameters:		void
*       Returns:		Preprocess* - ��������� �� ��������� ����� ������� ���������������
*       Throws:			-
*       Author:			dk
*       History:
*
****************************************************************************/
Preprocess* PreprocessSqueeze::copy (void) {
	PreprocessSqueeze* toReturn;
	toReturn = new PreprocessSqueeze;
	toReturn->setParams (this->squeezeParam, this->squeezeParamMin, this->squeezeParamMax, this->LagrangeDegree);
	return (Preprocess*) toReturn;
}


/****************************************************************************
*						PreprocessSqueeze::setParams
*
*       Description:	������� ������������� ����������� ��� ������� ������ ���������
*       Parameters:		sp - ������������� ���������� ������ squeezeParam
*						spMin - ����������-��������� �������� ��� ���������� squeezeParam
*						spMax - �����������-��������� �������� ��� ���������� squeezeParam
*       Returns:		0
*       Throws:			-
*       Author:			dk
*       History:
*
****************************************************************************/
int PreprocessSqueeze::setParams (double sp, double spMin, double spMax, unsigned int ld) {
	if ((spMin > spMax) || (spMin < 0.0) || (sp < spMin) || (sp > spMax))
		throw AxiomLibException ("PreprocessSqueeze::setParams : incompatible input parameters.");
	squeezeParam = sp;
	squeezeParamMin = spMin;
	squeezeParamMax = spMax;
	LagrangeDegree = ld;
	if ((ld < 1) || (ld > 20))
		throw AxiomLibException ("PreprocessSqueeze::setParams : LagrangeCoeffDegree of an incompatible value.");
	factorials.resize(LagrangeDegree + 1);
	factorials[0] = 1.0;
	for (unsigned int i = 1; i <= LagrangeDegree; i++)
		factorials[i] = factorials[i-1] * (double) i;
	return 0;
}


/****************************************************************************
*						PreprocessSqueeze::setParams
*
*       Description:	������� ������������� ����������� ��� ������� ������ ���������
*       Parameters:		sp - ������������� ���������� ������ squeezeParam
*       Returns:		0
*       Throws:			-
*       Author:			dk
*       History:
*
****************************************************************************/
int PreprocessSqueeze::setParams (double sp) {
	if ((sp < squeezeParamMin) || (sp > squeezeParamMax))
		throw AxiomLibException ("PreprocessSqueeze::setParams : incompatible input parameter.");
	squeezeParam = sp;
	return 0;
}


/****************************************************************************
*						PreprocessSqueeze::learn
*
*       Description:	������� �������� ������� ��������� �������������
*       Parameters:		templateRecognizer - ������ ��������������, �� �������
*						��������� �������� �������� ������� ��������� �������������
*       Returns:		0
*       Throws:			-
*       Author:			dk
*       History:
*
****************************************************************************/
double PreprocessSqueeze::learn (TemplateRecognizer &templateRecognizer) {
	// 1. �������� ��������� ��������
	if (goalStrategy == NULL)
		throw AxiomLibException("PreprocessSqueeze::learn: goalClass is not set in PreprocessSqueeze, but needed for learn function.");

	// 2. ���������� ���������� ��� ������
	// 2.1. ��������� �������� ���������, �� ������� ����� ����� ������������ ��������
	std::vector <double> curSqueezeParams;
	curSqueezeParams.resize(5);
	// ������� ��������� �������� ��������� ������� curSqueezeParams
	curSqueezeParams[2] = squeezeParam;
	curSqueezeParams[0] = squeezeParam / maxRatio;
	curSqueezeParams[4] = squeezeParam * maxRatio;
	if (curSqueezeParams[0] < squeezeParamMin) 
		curSqueezeParams[0] = squeezeParamMin;
	if (curSqueezeParams[4] > squeezeParamMax) 
		curSqueezeParams[4] = squeezeParamMax;
	curSqueezeParams[1] = 0.5*(curSqueezeParams[0] + curSqueezeParams[2]);		
	curSqueezeParams[3] = 0.5*(curSqueezeParams[2] + curSqueezeParams[4]);

	// 2.2. ������ ��� �������� ������� �������, ��������������� ��������� �� curSqueezeParams
	std::vector <double> curRes; 
	curRes.resize(5);
	for (unsigned int i=0; i < 5; i++) {
		curRes[i] = noValue;
	}
	
	// 3. ������ ������������ ��������� ������ 
	return sublearn (templateRecognizer, curRes, curSqueezeParams);
}


/****************************************************************************
*						PreprocessSqueeze::sublearn
*
*       Description:	��������������� ������� ��� ��������� ��������. 
*						���������� ���������� ����� ���������� �������� ���������
*						precision. ��� ���� �������� ������ � ������ ������ 
*						������������ ����� � 2 ����. �������� ������ - ������
*						���������� ������ ��������� precMinDelta.
*       Parameters:		templateRecognizer - ������, �� ������ ����������� ���������� ������� �������
*						curRes - ������ �������� ������� ������� ��� �������� precision �� ������� curSqueezeParams
*						curSqueezeParams - ������ �������� precision �� ������� ������� ����� ������� ��������
*						comparisonTable - ������� ��������� ������������ �������� ������� curRes
*       Returns:		double - 
*       Throws:			AxiomLibExceprion - ���� ������� ������� � ��������������� �����������
*       Author:			dk
*       History:
*
*****************************************************************************/
double PreprocessSqueeze::sublearn (TemplateRecognizer &templateRecognizer, std::vector <double> &curRes, std::vector <double> &curSqueezeParams) {
	// ����������� �������� ������� ������� ��� �������� ��������� squeezeParam �� ������� curSqueezeParams
	int resFirst, resSecond;
	for (unsigned int i = 0; i < 5; i++) {
		if (curRes[i] == noValue) {
			squeezeParam = curSqueezeParams[i];
			templateRecognizer.run(resFirst, resSecond);
			curRes[i] = goalStrategy->compute (resFirst, resSecond);
		}
	}
	if ((chooseBest (curRes, curSqueezeParams)) || ((curSqueezeParams[3] / curSqueezeParams[2]) < minRatio) ){
		squeezeParam = curSqueezeParams[2];
		return curRes[2];
	} else 
		return sublearn (templateRecognizer, curRes, curSqueezeParams);
}


// ��������������� ������� - ����� �������������� ��� ��������� ���������
inline bool PreprocessSqueeze::chooseBest (std::vector <double> &curRes, std::vector <double> &curSqueezeParams) const {
	// ����������� ������� ��������
	double bestRes = curRes[2];
	unsigned int bestInd = 2;
	for (unsigned int h = 0; h < 5; h++) {
		if (curRes[h] < bestRes) {
			bestRes = curRes[h];
			bestInd = h;
		}
	}
	switch (bestInd) {
		case 0:
			curSqueezeParams[2] = curSqueezeParams[0];
			curSqueezeParams[4] = curSqueezeParams[1];
			curSqueezeParams[3] = 0.5*(curSqueezeParams[0] + curSqueezeParams[1]);
			curSqueezeParams[0] *= curSqueezeParams[2] / curSqueezeParams[4];
			if (curSqueezeParams[0] < squeezeParamMin)
				curSqueezeParams[0] = squeezeParamMin;
			curSqueezeParams[1] = 0.5*(curSqueezeParams[0] + curSqueezeParams[2]);
			curRes[2] = curRes[0];
			curRes[4] = curRes[1];
			curRes[0] = noValue;
			curRes[1] = noValue;
			curRes[3] = noValue;
			break;
		case 1:
			curSqueezeParams[4] = curSqueezeParams[2];
			curSqueezeParams[2] = curSqueezeParams[1];
			curSqueezeParams[3] = 0.5*(curSqueezeParams[2] + curSqueezeParams[4]);
			curSqueezeParams[1] = 0.5*(curSqueezeParams[0] + curSqueezeParams[2]);
			curRes[4] = curRes[2];
			curRes[2] = curRes[1];
			curRes[3] = noValue;
			curRes[1] = noValue;
			break;
		case 2:
			curSqueezeParams[0] = curSqueezeParams[1];
			curSqueezeParams[4] = curSqueezeParams[3];
			curSqueezeParams[1] = 0.5*(curSqueezeParams[1] + curSqueezeParams[2]);
			curSqueezeParams[3] = 0.5*(curSqueezeParams[2] + curSqueezeParams[3]);
			curRes[0] = curRes[1];
			curRes[4] = curRes[3];
			curRes[1] = noValue;
			curRes[3] = noValue;
			break;
		case 3:
			curSqueezeParams[0] = curSqueezeParams[2];
			curSqueezeParams[2] = curSqueezeParams[3];
			curSqueezeParams[1] = 0.5*(curSqueezeParams[0] + curSqueezeParams[2]);
			curSqueezeParams[3] = 0.5*(curSqueezeParams[2] + curSqueezeParams[4]);
			curRes[0] = curRes[2];
			curRes[2] = curRes[3];
			curRes[1] = noValue;
			curRes[3] = noValue;
			break;
		case 4:
			curSqueezeParams[2] = curSqueezeParams[4];
			curSqueezeParams[0] = curSqueezeParams[3];
			curSqueezeParams[1] = 0.5*(curSqueezeParams[3] + curSqueezeParams[4]);
			curSqueezeParams[4] *= curSqueezeParams[2] / curSqueezeParams[0];
			if (curSqueezeParams[4] > squeezeParamMax)
				curSqueezeParams[4] = squeezeParamMax;			
			curSqueezeParams[3] = 0.5*(curSqueezeParams[2] + curSqueezeParams[4]);
			curRes[2] = curRes[4];
			curRes[0] = curRes[3];
			curRes[1] = noValue;
			curRes[3] = noValue;
			curRes[4] = noValue;
			break;
		default:
			throw AxiomLibException("PreprocessSqueeze::chooseBest: internal error - unrecgnized value of managment param.");
			break;
	}
	return false;
}


/****************************************************************************
*						PreprocessSqueeze::run
*
*       Description:	�������� ������� ������ - ������ ��������� �������������
*       Parameters:		dataSet - ������������� ����� ������
*       Returns:		0
*       Throws:			-
*       Author:			dk
*       History:
*
****************************************************************************/
int PreprocessSqueeze::run (DataSet &dataSet) {

	// ���������� ���������� ����������
	bool bres; // ��������� ����������, ������������ � �����
	std::vector <double> row; // ���� ����� ������������ ���� �� dataSet'a
	std::vector <double> newRow; // ���� ����� ������������ ���� �� dataSet'a
	std::vector <int> corrMarkUp; // ����� ����� ������ ���������� ������� �������� ����
	std::vector <int> newCorrMarkUp; // ����� ����� ������ ���������� ������� �������� ����
	
	// ������ ����� �������� ����� � dataSet'e
	int numOfTests = 0;
	std::vector<int> numOfTS;
	bool c = dataSet.getTestSize (numOfTests, numOfTS);
			
	// �������� ���������������� ���� ����������
	// omp_set_dynamic(1);
	// #pragma omp parallel for schedule(dynamic, 1)
	// for ...
	//cout << "\n	process  " << omp_get_num_threads() << endl;

	// ���� �� ���� ������ �� dataSet'a
	for (int t = 0; t < numOfTests; t++) {
		// ����������� ���������� �������� ������� ����� � ��������� ��
		bres = dataSet.getTSByIndexFromTests (corrMarkUp, t, -1);
		if (bres) {
			apply (newCorrMarkUp, corrMarkUp);
			bres = dataSet.setTSByIndexFromTests (newCorrMarkUp, t, -1);
			if (!bres)
				throw AxiomLibException("Error in PreprocessSqueeze::run: for unknown reason a row of correct value coult not be changed in dataSet.");
		}
		else {
			throw AxiomLibException("Error in PreprocessSqueeze::run: correct-params are missing in Tests in DataSets.");
		}
		// ���� �� ���� ������������ �������� ���������� ����� �� dataSet'a � �� ����������
		for (int prm = 0; prm < (int) numOfTS[t]; prm++) {
			// ���������� ���������� ��� ��� ����� ���������� ����
			bres = dataSet.getTSByIndexFromTests (row, t, prm);
			if (bres) {
				apply (newRow, row);
				bres = dataSet.setTSByIndexFromTests (newRow, t, prm);
				if (!bres)
					throw AxiomLibException("Error in PreprocessSqueeze::run: for unknown reason a row coult not be changed in dataSet.");
			}
			else {
				throw AxiomLibException("Error in PreprocessSqueeze::run: some params are missing in Tests in DataSets.");
			}
		}
	}

	// ���������� ���������� ���������� �������
	int numOfClasses;
	std::vector<int> numOfMultiTS;
	std::vector<std::vector<int> > numOfTSSets;

	bres = dataSet.getClassSize (numOfClasses, numOfMultiTS, numOfTSSets);
	std::vector <double> vec; // ������ 
	std::vector <double> newVec; // ������ 

	// ���������� ������� ������ ��������������
	for (int i = 0; i < numOfClasses; i++) {
		for (int j = 0; j < numOfMultiTS[i]; j++) {
			for (int p = 0; p < numOfTSSets[i][j]; p++) {
				bres = dataSet.getTSByIndexFromClass (vec, i, j, p);
				if (bres) {
					apply (newVec, vec);
					bres = dataSet.setTSByIndexFromClass (newVec, i, j, p);
					if (!bres)
						throw AxiomLibException("Error in PreprocessSqueeze::run: new alue for class-row could not be set.");
				}
			}
		}
	}

	return 0;
}


/****************************************************************************
*						PreprocessSqueeze::apply
*
*       Description:	���������� ������� ���������� ������������� ��� ������ 
*						�������� ������� ���� �������������� �����
*       Parameters:		newVec - ��������������� �������� �������
*						vec - ������ �������� �������
*       Returns:		true - ���� �������������� ��������� �������
*						false - ���� �������������� ��������� ���������
*       Throws:			-
*       Author:			dk
*       History:
*
****************************************************************************/
bool PreprocessSqueeze::apply (std::vector <double> &newVec, std::vector <double> &vec) const {
	// �������� ��������� �������
	unsigned int size = vec.size();
	if (size < 1) {
		newVec.clear();
		return false;
	}

	// ���������� ����� ���������������� �������
	unsigned int newSize = (unsigned int) round (((double) size )* squeezeParam);
	if (newSize < 1)
		newSize = 1;

	// ��� �������� ���������� ��������� ���� ������ � ������� �������
	if (newSize == size) { // ������ �������� �������������� �� ����������
		newVec.assign (vec.begin(), vec.end());
		return true;
	}
	
	if (newSize > size) { // ������ ���������� ��������� ������������
		// �������� �� ����������� ������������� �������������� ��������� ������������
		if (newSize <= LagrangeDegree) {
			newVec.assign (vec.begin(), vec.end());
			return false;
		}
		newVec.resize (newSize);
		
		// ���������� �������� ��������� �������
		unsigned int j; // ������ ��������� ����, ��������������� �������� ������� ����� i
		double ld; // ���������� ��� ��������� ����������������� ��������� ��������
		double relativeShift; // ���������� ��� �������� ������� ����� ���������� �����, � ������� ������� �������� � ��������� � ����������� ����� j
		unsigned int left; // �������� LagrangeDegree - ��� ����������� �������� ������������ �����
		unsigned int right; // �������� LagrangeDegree - ��� ����������� �������� ������������ �����
		left = (unsigned int) round ( (double) LagrangeDegree / 2.0);
		right = LagrangeDegree - left;
		for (unsigned int i = 0; i < newSize; i++) {
			j = (unsigned int) round ((double) i / squeezeParam);
			if (j < left)
				j = left;
			if ((size - j) <= right)
				j = size - right - 1;
			// ���������� �������� � ����� i �� ���������� ������������
			newVec[i] = 0.0;
			relativeShift = (double) i / squeezeParam - j + left;
			for (int t = 0; t <= (int) LagrangeDegree; t++) {
				// ������� ��������� �������� ld
				ld = 1.0;
				// ���������
				for (int n = 0; n <= (int) LagrangeDegree; n++) {
					if (n==t) continue;
					ld *= relativeShift - n;
				}
				// �����������
				ld /= factorials[t] * factorials[LagrangeDegree - t];
				if ( ((LagrangeDegree - t)%2) != 0) 
					ld *= -1.0;
				// ���������� ��������� �������� � �����
				newVec[i] += vec[j-left+t] * ld;
			}
		}
		return true;
	}

	if (newSize < size) { // ������ ���������� ��������� ������� ����� �������� ���� � ����� ��������
		newVec.resize (newSize);
		// ���������� �������� ��������� ������ �������
		unsigned int left; // �������� �������������� ������� - ��� ����������� �������� ������������ �����
		unsigned int right; // �������� �������������� ������� - ��� ����������� �������� ������������ �����
		double j; // ����������� ����� ����� �� ������ ���
		double delta; // ������ �������������� �������
		double deltaHalf; // �������� �� ������� �������
		double curDelta; // ������� �������� ����� �������������� ������� - ��������� ��� ��������� �����
		delta = 1.0 / squeezeParam;
		deltaHalf = delta / 2.0;
		double posLeft; // ������� �� ��� �������, ����� ������� �������������� �������
		double posRight; // ������� �� ��� �������, ������ ������� �������������� �������
		double pointLeft; // ������� �� ��� �������, ����� ������� ����������� �����
		double pointRight; // ������� �� ��� �������, ������ ������� ����������� �����
		for (unsigned int i = 0; i < newSize; i++) {
			j = (double) i / squeezeParam;
			// ���������� left � right
			posLeft = j - deltaHalf;
			if (posLeft < 0.0)
				posLeft = 0.0;
			posRight = j + deltaHalf;
			if (posRight > (size - 1))
				posRight = size - 1;
			curDelta = posRight - posLeft;
			left = (unsigned int) round (posLeft);
			right = (unsigned int) round (posRight);
			// ��������� �������� ������ ���� � ����� i
			newVec[i] = 0.0;
			for (unsigned int h = left; h <= right; h++) {
				pointLeft = h - 0.5;
				pointRight = h + 0.5;
				if (pointLeft < posLeft)
					pointLeft = posLeft;
				if (pointRight > posRight)
					pointRight = posRight;
				newVec[i] += vec[h] * (pointRight - pointLeft);
			}
			newVec[i] /= curDelta;
		}
		return true;
	}

	return false;
}


/****************************************************************************
*						PreprocessSqueeze::apply
*
*       Description:	���������� ������� ���������� ������������� ��� ������ 
*						�������� ������� ���� ����� �����
*       Parameters:		newVec - ��������������� �������� �������
*						vec - ������ �������� �������
*       Returns:		true - ���� �������������� ��������� �������
*						false - ���� �������������� ��������� ���������
*       Throws:			-
*       Author:			dk
*       History:
*
****************************************************************************/
bool PreprocessSqueeze::apply (std::vector <int> &newVec, std::vector <int> &vec) const {
	// �������� ��������� �������
	unsigned int size = vec.size();
	if (size < 1) {
		newVec.clear();
		return false;
	}

	// ���������� ����� ���������������� �������
	unsigned int newSize = (unsigned int) round (((double) size )* squeezeParam);
	if (newSize < 1)
		newSize = 1;

	// ��� �������� ���������� ��������� ���� ������ � ������� �������
	if (newSize == size) { // ������ �������� �������������� �� ����������
		newVec.assign (vec.begin(), vec.end());
		return true;
	}
	
	if (newSize > size) { // ������ ���������� ��������� ������������
		// �������� �� ����������� ������������� �������������� ��������� ������������
		if (newSize <= LagrangeDegree) { // ��������� ��� �������, ��� ��� ��� ��������� ��� �������������� ������� ����� ������ � ���� ������ �������, �� ������ ������������
			newVec.assign (vec.begin(), vec.end());
			return false;
		}
		newVec.resize (newSize);
		// ���������� �������� ��������� �������
		int j; // ������ ��������� ����, ��������������� �������� ������� ����� i
		for (unsigned int i = 0; i < newSize; i++) {
			j = round( (double) i / squeezeParam );
			if (j >= (int) size)
				j = size - 1;
			newVec[i] = vec[j];
		}
		return true;
	}

	if (newSize < size) { // ������ ���������� ��������� ������� ����� �������� ���� � ����� ��������
		newVec.resize (newSize);
		// ���������� �������� ��������� ������ �������
		int left; // �������� �������������� ������� - ��� ����������� �������� ������������ �����
		int right; // �������� �������������� ������� - ��� ����������� �������� ������������ �����
		int curVal; // ������� ��������, ������� �������� ����� ���������� �� ������� ���� � �����
		for (unsigned int i = 0; i < newSize; i++) {
			// ���������� left � right
			left = round ( ((double) i - 0.5) / squeezeParam );
			if (left < 0)
				left = 0;
			right = round ( ((double) i + 0.5) / squeezeParam );
			if (right >= (int) size)
				right = size - 1;
			curVal = 0;
			for (int h = left; h <= right; h++) {
				if (vec[h] > 0) {
					curVal = vec[h];
					break;
				}
			}
			newVec[i] = curVal;
		}
		return true;
	}

	return false;
}
