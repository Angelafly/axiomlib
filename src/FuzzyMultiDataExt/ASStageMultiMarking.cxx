#include "ASStageMultiMarking.h"
#include "../ReducedRecognizerFactory.h"
#include "../GoalStrategyFactory.h"
#include "../Logger.h"

#include "AXStage.h"

#include "ASStage.h"
// �������� ���������� ������������ ������ ������� � ������� ��������� �������, ������� ������� ��� ���� ���������
//  (������ ���� ���������� �������, ��� ��� �� ����������� ��� ����� ����� ���������� ������)
#define int_max_size_of_vector_axiomExprSet		100
// ����� �������� ����� �������� � ��������������� �������
#define eps			0.0000000001
// ������������ ����� ������ ������, ������� ����� ���� ������� �� ������ ���� ��������� ��� ����� �������� ������� ������
#define int_max_number_of_renew_axiom_set		10

using namespace AxiomLib;
using namespace AxiomLib::FuzzyMultiDataExt;

void debug_common_bool(std::vector<std::vector<std::vector<bool> > > common){

    std::vector<std::vector<bool> > one_line;
    std::vector<bool> one_point;
    std::cout << "Find\t" << common.size() << "common subsequence\n";

    for (unsigned int i = 0; i < common.size(); i++){
        std::cout << "Common subsequence ?" << i << "\n";
        one_line.clear();
        one_line = common[i];

        for (unsigned int j = 0; j < one_line.size(); j++){
            std::cout << "Point " << j << "\n";
            one_point.clear();
            one_point = one_line[j];

            for (unsigned int k = 0; k < one_point.size(); k++){
                std::cout << one_point[k];
            }
        }
        std::cout << "\n\n";
    }
}

void debug_common_int(std::vector<std::vector<int> > common){

    std::vector<int> one_line;
    std::cout << "Find\t" << common.size() << "common subsequence\n";

    for (unsigned int i = 0; i < common.size(); i++){
        std::cout << "Common subsequence ?" << i << "\n";
        one_line.clear();
        one_line = common[i];

        for (unsigned int j = 0; j < one_line.size(); j++){
            std::cout << one_line[j];
        }
        std::cout << "\n\n";
    }
}

// From BaseClass
AxiomExprSetPlus &ASStageMultiMarking::getAS(int n){
    return bestAxiomSets[n];
}

const AxiomExprSetPlus &ASStageMultiMarking::getAS(int n) const{
    return bestAxiomSets[n];
}

int ASStageMultiMarking::getASSize() const{
    return bestAxiomSets.size();
}

void ASStageMultiMarking::recalculateMatterASFunc(AxiomExprSetPlus& as){}

void ASStageMultiMarking::setAxiomSets(const std::vector<AxiomExprSetPlus>& initialAS){
    bestAxiomSets = initialAS;
}

void ASStageMultiMarking::initFromEnv(const Environment& env){
    if (env.getIntParamValue (ccNumPoints, "ccNumPoints") < 0)
            throw AxiomLibException("FuzzyMultiDataExt_AS::setParamsFromEnv : ccNumPoints is undefined.");

    // �������������� ��������� ���������� ������� �������
    std::string goalStrategyName;
    GoalStrategyFactory gsf;
    if (env.getStringParamValue(goalStrategyName, "goalClass") < 0)
            throw AxiomLibException("FuzzyMultiDataExt_AS::initFromEnv: goal-class is undefined.");
    goalStrategy = PGoalStrategy(gsf.create(goalStrategyName));
    goalStrategy->setParamsFromEnv(env);

    // ��������� ������, ���� ��� ������ �������� �������� ������� �������� ��� ������������ � ������ ��������
    if (env.getDoubleParamValue (shiftErrIn, "shiftErrIn") < 0)
            throw AxiomLibException("FuzzyMultiDataExt_AS::setParamsFromEnv : shiftErrIn is undefined.");
    // ������� ���������� ����� ��������� �������� (��� ������ �������� �� ����� �� �������)
    if (env.getDoubleParamValue (extendMarkUpLensLevel, "extendMarkUpLensLevel") < 0)
            throw AxiomLibException("FuzzyMultiDataExt_AS::setParamsFromEnv : extendMarkUpLensLevel is undefined.");
    if (env.getIntParamValue (maxAxiomSetPopSize, "maxAxiomSetPopSize") < 0)
            throw AxiomLibException("FuzzyMultiDataExt_AS::setParamsFromEnv : maxAxiomSetPopSize is undefined.");
    if (env.getIntParamValue (maxNumberOfSteps, "maxNumberOfSteps") < 0)
            throw AxiomLibException("FuzzyMultiDataExt_AS::setParamsFromEnv : maxNumberOfSteps is undefined.");
    if (env.getDoubleParamValue (bestAxiomSetGoal, "bestAxiomSetGoal") < 0)
            throw AxiomLibException("FuzzyMultiDataExt_AS::setParamsFromEnv : bestAxiomSetGoal is undefined.");
    if (env.getIntParamValue (numberOfBestAxiomSets, "numberOfBestAxiomSets") < 0)
            throw AxiomLibException("FuzzyMultiDataExt_AS::setParamsFromEnv : numberOfBestAxiomSets is undefined.");
    if (env.getDoubleParamValue (percentBestAxiomSets, "percentBestAxiomSets") < 0)
            throw AxiomLibException("FuzzyMultiDataExt_AS::setParamsFromEnv : percentBestAxiomSets is undefined.");
    if ((percentBestAxiomSets <= 0.0) || (percentBestAxiomSets > 1.0))
            throw AxiomLibException("FuzzyMultiDataExt_AS::setParamsFromEnv : percentBestAxiomSets is out of range.");

    // ������� ��� ������ ���������� ����� ���������������������
    std::string name_m;
    if (env.getStringParamValue (name_m, "DTWMetric") < 0)
            throw AxiomLibException("FuzzyMultiDataExt_AS::setParamsFromEnv : m is undefined.");
    else
        m=m->getMetric(name_m) ;
    // ������� ��� ��������������.
    if (env.getStringParamValue (name_m, "mForRecognize") < 0)
            throw AxiomLibException("FuzzyMultiDataExt_AS::setParamsFromEnv : mForRecognize is undefined.");
    else
        mForRecognize=mForRecognize->getMetric(name_m) ;
    //����� ��� ��������� ��� ������� � ��������������
    if (env.getDoubleParamValue (porog, "porog") < 0)
            throw AxiomLibException("FuzzyMultiDataExt_AS::setParamsFromEnv : porog is undefined.");

    if (env.getDoubleParamValue (porogForDTW, "porogForDTW") < 0)
            throw AxiomLibException("FuzzyMultiDataExt_AS::setParamsFromEnv : porogForDTW is undefined.");

    if (env.getBoolParamValue (areMultiMark, "areMultiMark") < 0)
            throw AxiomLibException("FuzzyMultiDataExt_AS::setParamsFromEnv : areMultiMark is undefined.");

    // �������� ��� ��������� ��� ��������������(������ ����)
    if (env.getDoubleParamValue (mStretch, "mStretch") < 0)
            throw AxiomLibException("FuzzyMultiDataExt_AS::setParamsFromEnv : mStretch is undefined.");
    if (env.getIntParamValue (maxNOP, "maxNOP") < 0)
            throw AxiomLibException("FuzzyMultiDataExt_AS::setParamsFromEnv : maxNOP is undefined.");

}


ASStageMultiMarking::ASStageMultiMarking(FuzzyDataSet* fuzzyDataSet,
                                         AXStage* stage2): fuzzyDataSet(fuzzyDataSet), stage2(stage2), logger(Logger::getInstance()){
}

/****************************************************************************
*					FuzzyMultiDataExt_AS::chooseBestMarkUp
*
*	Description:	������� �������� �� ���� ��������� ��� �������� ������
*	Parameters:		as - ������� ������, ��� ������� ���������� �������� ���������� ���������� ����
*					abType - ��� ���������� ���������, ��� �������� ���������� ��������
*					markUp - ��������� ������ �������, �.�. ��������� ������� �������� ��������� ���������� ������� ����
*					genMarkUps - �������������� ����� ��������� � �������� ���������� ���������� ���������
*					errFirstVal - ����������� ����� ������ I ���� ��� ������ �� �������� ������� ������� ��������� ��������
*					errSecondVal - ����������� ����� ������ II ���� ��� ������ �� �������� ������� ������� ��������� ��������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int ASStageMultiMarking::chooseBestMarkUp (AxiomExprSetPlus &as, int abType, std::vector <std::vector <bool> > &markUp, const std::vector <std::vector <std::vector <bool> > > &genMarkUps, int &errFirstVal, int &errSecondVal) {

    // ��������� ������ ������� ���������� � �������� ��������� ���������� ��������� ����
        if (genMarkUps.size() < 1) {
                markUp.clear();
                errFirstVal = 0;
                errSecondVal = 0;
                return 1;
        }
        if (genMarkUps.size() == 1) {
                markUp = genMarkUps[0];
                double tmpGoalVal;
                matterAxiomSetFunc (as, abType, genMarkUps[0], tmpGoalVal, errFirstVal, errSecondVal);
                return 0;
        }
        // �������������� ����������� ���������
        double goalVal, goalTmp;
        int errFirstTmp, errSecondTmp;
        int curBestIndex = 0;
        // ��������� ����� ������ I � II ���� ��� ��������� �������� �������� ��������� ���������� ��������� ���� � �������� ������ �������� ������� �������
        matterAxiomSetFunc (as, abType, genMarkUps[0], goalVal, errFirstVal, errSecondVal);
        for (unsigned int i = 1; i < genMarkUps.size(); i++) {
            matterAxiomSetFunc (as, abType, genMarkUps[i], goalTmp, errFirstTmp, errSecondTmp);
                if (goalTmp < goalVal) {
                        goalVal = goalTmp;
                        errFirstVal = errFirstTmp;
                        errSecondVal = errSecondTmp;
                        curBestIndex = i;
                }
        }
        // ��������� �������������� ������ ��������
        markUp = genMarkUps[curBestIndex];
        return 0;
}
double minimum (const std::vector<double> &r){
        if(r.empty()) {
                throw AxiomLibException("minimum(): vector is empty");
        }

        double min=r[0];
        for(int i=0;i<r.size();i++){
                if (r[i]<min)
                        min=r[i];
        }
        return min;
}
std::vector<int> ASStageMultiMarking::convert(std::vector<double> resultInDouble){  
    std::vector<int> result;
    int toPush=0;
    for (int i=0;i<resultInDouble.size();i++){
        if (resultInDouble[i] < porogForDTW){
            toPush=1;
        }
        else{
            toPush=0;
        }
        result.push_back(toPush);
    }
return result;
}

// ������� ������� ������ �������� ethalon � markUp � ������ ����������� ������ � result
void ASStageMultiMarking::recognize (std::vector <std::vector<bool> > &markUp, const std::vector <std::vector<bool> > &genMarkUp, std::vector <int> &result){

    std::vector<double> resultInDouble(markUp.size(), 1.0);
    std::vector<double> temp_result;
    int windowSizeLowerLimit = std::max(int((1.0/mStretch)*genMarkUp.size()), 1);
    int windowSizeUpperLimit = std::max(int((mStretch)*genMarkUp.size()), 1);

        /*std::cerr << "windowSizeLowerLimit = " << windowSizeLowerLimit << ", windowSizeUpperLimit = " << windowSizeUpperLimit
			  << ", markUp.size() = " << markUp.size()
                                 << ", genMarkUp.size() = " << genMarkUp.size() << std::endl;*/
	if(genMarkUp.size() > 0) {
		for (int s=windowSizeUpperLimit;s<markUp.size();s++){
				//Logger::debug("Window : " + boost::lexical_cast<std::string>((1.0/mStretch)*etalon[i].size()) + " " + boost::lexical_cast<std::string>((mStretch)*etalon[i].size()));
                                //Logger::debug("Computing DTW...");
                        temp_result.clear();
			mForRecognize->computeDTWForMetric( markUp, s , windowSizeLowerLimit, windowSizeUpperLimit, genMarkUp, temp_result);
                                //Logger::debug("Setting distance...");
                        resultInDouble[s] = minimum(temp_result);
		}
        }

    result = convert(resultInDouble);
}


/****************************************************************************
*					FuzzyMultiDataExt_AS::matterAxiomSetFunc
*
*	Description:	������� ������� ������� ��� ������� ������:
*					1. �������� ���������� ��������� ������� � ������������
*					   ����������� ���������� ��������
*					2. �������� ���������� ����������� ������� � �������
*					   �������� ������� �������
*	Parameters:		as - ������� ������, ��� ������� ��������� �������� ������� �������
*	Returns:		double - �������� ������� �������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
double ASStageMultiMarking::matterAxiomSetFunc (AxiomExprSetPlus &as)  {
    int numOfClasses;
        std::vector <int> numOfMultiTS;
        std::vector < std::vector <int> > numOfTS;

        // ��������� ���������� �� ��������� ������ ������
        fuzzyDataSet->getClassSize (numOfClasses, numOfMultiTS, numOfTS);
        // ��� ������� ������ ���������� ��������� ��������� ����������� ���������� ��������
        std::vector < std::vector <std::vector <bool> > > resMarkUps, genMarkUps;
        int errFirstVal, errSecondVal;
        as.errFirst = 0;
        as.errSecond = 0;
        as.multiMarkUps.resize(numOfClasses);
        as.errors.resize(numOfClasses);
        std::vector <std::vector <bool> > razmetka;
        for (int abType = 0; abType < numOfClasses; abType++) {
                genMarkUps.clear();
                resMarkUps.resize (numOfMultiTS[abType]);
                for (int multiTS = 0; multiTS < numOfMultiTS[abType]; multiTS++) {
                        createRefMarkUp (as,FuzzyDataSet::Reference,abType,multiTS,resMarkUps[multiTS]);
                        razmetka=resMarkUps[multiTS];
                }
                // �������� �������� - ������� 0 � ������ � � ����� �������� - ����� ����� ����� ���� ������ �����
                simplifyMarkUps (resMarkUps);
                // �� ��������� ���������� �������� ���������� ����������� ��������� - ���������� �� ����� � ��������� �������� ���������� ��������
                createMarkUpVariants (genMarkUps, resMarkUps);
                // �������� ������� �������� - ������� 0 � ������ � � �����. ���� �������� �������� ��������� ��������� � ���� ��������
                simplifyMarkUps (genMarkUps);
                // ��������� ����������� �������� �������� - ���� ����� ����� �������� �� �������
                if (genMarkUps.size() < 1) {
                        logger->writeComment("Warinig: couldn't find common subsequence, creating simple markup");
                        createSimpleMarkUpVariants (genMarkUps, (int) as.axioms.size());
                }
                // �������� �� ���� �������� ������ ��� ������� ������ ���������� ��������� � ��������� ��
                 chooseBestMarkUp (as, abType, as.multiMarkUps[abType], genMarkUps, errFirstVal, errSecondVal);

                as.errors[abType].first = errFirstVal;
                as.errors[abType].second = errSecondVal;

                as.errFirst += errFirstVal;
                as.errSecond += errSecondVal;
        }
        // ���������� ������ �������� ������� �������� ��� ������ ������� ������ � ������ ��������� �������� ��������� ����������

        as.goal = goalStrategy->compute(as.errFirst, as.errSecond);
        return as.goal;
}

double ASStageMultiMarking::matterAxiomSetFunc(AxiomExprSetPlus &as, const std::vector<std::vector<std::vector<std::vector<bool> > > >& markupVariants) {
    int numOfClasses = fuzzyDataSet->getClassCount();
        int errFirstVal, errSecondVal;
        as.errFirst = 0;
        as.errSecond = 0;
        as.multiMarkUps.resize(numOfClasses);
        as.errors.resize(numOfClasses);
        for (int abType = 0; abType < numOfClasses; abType++) {
                // �������� �� ���� �������� ������ ��� ������� ������ ���������� ��������� � ��������� ��
                chooseBestMarkUp (as, abType, as.multiMarkUps[abType], markupVariants[abType], errFirstVal, errSecondVal);
                as.errors[abType].first = errFirstVal;
                as.errors[abType].second = errSecondVal;
                as.errFirst += errFirstVal;
                as.errSecond += errSecondVal;
        }
        // ���������� ������ �������� ������� �������� ��� ������ ������� ������ � ������ ��������� �������� ��������� ����������

        as.goal = goalStrategy->compute(as.errFirst, as.errSecond);
        return as.goal;
}

/****************************************************************************
*					FuzzyMultiDataExt_AS::matterAxiomSetFunc
*
*	Description:	������� ������� ������� ��� ������� ������ � �������� ���������
*					�������� ��������� ����������
*	Parameters:		as - ������� ������, ��� ������� ��������� �������� ������� �������
*					markUps - �������� ��������� ����������
*	Returns:		double - �������� ������� �������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/

double ASStageMultiMarking::matterAxiomSetFunc (AxiomExprSetPlus &as, std::vector <std::vector <std::vector <bool> > > &markUps) {
    int tmpFirst, tmpSecond;
        double tmpGoal;
        as.errFirst = 0;
        as.errSecond = 0;
        as.errors.resize(markUps.size());
        for (int abType = 0; abType < (int) markUps.size(); abType++) {
                matterAxiomSetFunc (as, abType, markUps[abType], tmpGoal, tmpFirst, tmpSecond);
                as.errors[abType] = std::make_pair(tmpFirst, tmpSecond);
                as.errFirst += tmpFirst;
                as.errSecond += tmpSecond;
        }
        // ���������� �������� ������� ������� ��� ����������� ����� ������ I � II ����
        as.goal = goalStrategy->compute(as.errFirst, as.errSecond);
        return as.goal;
}


/****************************************************************************
*					FuzzyMultiDataExt_AS::matterAxiomSetFunc
*
*	Description:	������� ������� ������� ��� ������� ������ ��� ���������
*					���� ���������� ��������� � ��������� �������� ��������
*					��������� ���������� ����� ����
*	Parameters:		as - ������� ������, ��� ������� ��������� �������� ������� �������
*					abType - ��� ���������� ���������, ��� �������� ��������� �������� ������� �������
*					genMarkUp - ������� �������� ��������� ���������� ��������� ���� ���������� ���������
*					goalVal - ����������� �������� ������� �������
*					errFirstVal - ����������� �������� ����� ������ I ����
*					errSecondVal - ����������� �������� ����� ������ II ����
*	Returns:		double - �������� ������� �������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/

double ASStageMultiMarking::matterAxiomSetFunc (AxiomExprSetPlus &as, int abType, const std::vector <std::vector<bool> > &genMarkUp, double &goalVal, int &errFirstVal, int &errSecondVal) {
    int numOfClasses;
        std::vector <int> numOfMultiTS;
        std::vector < std::vector <int> > numOfTS;
        // �������� ���������� � ����� ���������� ����������� �������
        fuzzyDataSet->getTestSize (numOfClasses, numOfMultiTS, numOfTS);
        if ((abType < 0) || (abType >= numOfClasses))
                throw AxiomLibException("FuzzyMultiDataExt_AS::matterAxiomSetFunc: incorrect input number of abnormal type.");
        // �������������� ������ - ����� ��������� ����� ������������� �������� ������
        std::vector <bool> dims;
        dims.resize (fuzzyDataSet->paramNamesSize(), false);
        for (unsigned int axNum = 0; axNum < as.axioms.size(); axNum++) {
                for (unsigned int ecNumI = 0; ecNumI < as.axioms[axNum]->expression.size(); ecNumI++) {
                        for (unsigned int ecNumII = 0; ecNumII < as.axioms[axNum]->expression[ecNumI].size(); ecNumII++) {
                                if ((as.axioms[axNum]->expression[ecNumI][ecNumII].dimension < 0) || (as.axioms[axNum]->expression[ecNumI][ecNumII].dimension >= (int) dims.size()))
                                        throw AxiomLibException("FuzzyMultiDataExt_AS::matterAxiomSetFunc : incomplete definition of elementary condition, it's data set dimension is out of range.");
                                dims[as.axioms[axNum]->expression[ecNumI][ecNumII].dimension] = true;
                        }
                }
        }
        // �������������� ��������� ��� �������� ����� ������
        errFirstVal = 0;
        errSecondVal = 0;
        // �������� ��������� ���������� ��� ��������� ���� ���������� ��������� � ��������� ��������������
        std::vector <std::vector<bool> > curMarkUp;
        std::vector <int> result;
        int num;
        for (int t = 0; t < (int) fuzzyDataSet->getMutiTSCount(FuzzyDataSet::Testing, abType); t++) {
                // �������� ���������� ����������� ������� �������� ������ as
                createRefMarkUp(as,FuzzyDataSet::Testing,abType, t, curMarkUp);
                // ������������� ���������� ��������� � �������� ����
                 recognize (curMarkUp, genMarkUp, result);
                // ���������� ����� ������ ������� � ������� ����
                num = getStatistic (result);
                // ������������ ����� ������
                if (num == 0)
                        errSecondVal++;
                else
                        errFirstVal += num - 1;
        }
        // ��������� �� ���� ����� ����������� ���������
        int numNormalMultiTS;
        std::vector <int> numNormalTS;
        fuzzyDataSet->getNormalTestSize (numNormalMultiTS, numNormalTS);
        for (int t = 0; t < (int) fuzzyDataSet->getMutiTSCount(FuzzyDataSet::Testing, -1); t++) {
                // ��������� ���������� ����������� ���������
                createRefMarkUp(as,FuzzyDataSet::Testing,t, numNormalTS[t],curMarkUp);
                // ������������� ���������� ��������� � �������� ����
                recognize (curMarkUp, genMarkUp, result);
                // ���������� ����� ������ ������� � ������� ����
                num = getStatistic (result);
                // ������������ ����� ������over
                errFirstVal += num;
        }

        // ���������� �������� ������� ������� ��� ����������� ����� ������ I � II ����
        goalVal = goalStrategy->compute(errFirstVal, errSecondVal);
        return goalVal;
}


/****************************************************************************
*					FuzzyMultiDataExt_AS::matterAxiomSetFunc
*
*	Description:	������� ������� ������� ��� ������� ������ ��������� � �����
*	Parameters:		baseDir - �������� ��������, ��� ����� ��������� �������� ������� ������
*					fileName - ���� � ��������� ������� ������, ��� ������� ��������� �������� ������� �������
*					errFirst - ����� ������ I ����
*					errSecond - ����� ������ II ����
*	Returns:		double - �������� ������� �������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
/*
double ASStageMultiMarking::matterAxiomSetFunc (const std::string baseDir, const std::string fileName, int &errFirst, int &errSecond) const {
        // ������������� ������� ������ �� ���������� �����
        AxiomExprSetPlus aesp;
        AxiomSetBase axiomSetBase;
        AxiomExprSetStructure aess;
        std::string axiomSetPath = baseDir;
        axiomSetPath.append("/");
        axiomSetPath.append(fileName);
        // ��������� ���������� � ������� ������ � aess
        std::vector <std::string> dataSetParams;
        dataSetParams = fuzzyDataSet->getParamNames();
        axiomSetBase.readFromAS (axiomSetPath, aess, dataSetParams);
        aesp.initAxiomSetFromStructure (aess);
        // ���������� �������� ������� �������
        matterAxiomSetFunc (aesp);
        // ���������� ����������� � ����� �� �������
        errFirst = aesp.errFirst;
        errSecond = aesp.errSecond;
        double toReturn = aesp.goal;
        aesp.clear();
        return toReturn;
}

/****************************************************************************
*					FuzzyMultiDataExt_AS::getStatistic
*
*	Description:	������� ������� ����� ��������� ��������� ��������
*	Parameters:		row - ���, � ������� ������ ��������� ������
*	Returns:		int - ����� ��������� ��������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
inline int ASStageMultiMarking::getStatistic (std::vector <int> &row) {
    int num = 0;
        int i = 0;
        int j;
        int len = (int) row.size();
        while (i < len) {
                if (row[i] > 0) {
                        num++;
                        j = i+1;
                        while (j < len) {
                                if (row[j]!=row[j-1])
                                        break;
                                j++;
                        }
                        if ((j - i) > ccNumPoints)
                                num += j - i - ccNumPoints;
                        i = j;
                } else {
                        i++;
                }
        }
        return num;
}


/****************************************************************************
*					ASStageMultiMarking::createSimpleMarkUpVariants
*
*	Description:	������� ������� ����� ������� ������� �������� ���������� ���������� ���������
*					����� �������� ��, �������� �� ����� ������.
*	Parameters:		genMarkUps - ����������� ������ ��������� ��������� ��������� ��������
*						��� ���������� ���� ���������� ���������
*					numOfAxioms - ����� ������ � ������� ������, ��� ������� ��������� �������� ��������
*	Returns:		int - ����� ��������� ��������
*	Throws:			-
*	Author:			armkor
*	History:
*
****************************************************************************/

inline int ASStageMultiMarking::createSimpleMarkUpVariants (std::vector<std::vector<std::vector<bool> > > &genMarkUps, const int numOfAxioms) const {

     for (int i = 0; i < numOfAxioms; i++) {
         std::vector<std::vector<bool> > line;
             std::vector<bool> temp;
             for (int j=0;j<numOfAxioms;j++){
                 temp.push_back(false);
             }
             temp[i]=true;
             line.push_back(temp);
             genMarkUps.push_back(line);
     }
     return numOfAxioms;
}

void ASStageMultiMarking::createRefMarkUp ( AxiomExprSetPlus &as, FuzzyDataSet::DataSetDivisionType division, int classNo, int multiTSNo, std::vector <std::vector<bool> >& result){

    result.clear();
    std::vector<std::vector<double> > row;

    int multiTSLen = fuzzyDataSet->getMultiTSLength(division, classNo, multiTSNo);
    result.resize(multiTSLen);

    for (int i=0;i<fuzzyDataSet->getDimensionCount();i++){
        std::vector<double> temp;
        fuzzyDataSet->getTSByIndex(division,temp,classNo,multiTSNo,i);
        row.push_back(temp);
    }
       // ���������, ��� �������� ��������. ����� �� ���������(�������� ������)
    as.enter (result,row, 0, row[0].size());
}


inline int ASStageMultiMarking::simplifyMarkUps (std::vector <std::vector <std::vector<bool> > > &markUps) const {
    int k, l;
    std::vector <std::vector<bool> > line;
    std::vector<bool> point;
    bool stop=false;

    for (int i =0;i< (int) markUps.size(); i++) {
        line.clear();line=markUps[i];
        // ������� ��� 0 � ������ ������� - �� �� ���������
        k = 0;
        stop=false;
        while (k < (int) line.size()) {
            point.clear();point=line[k];
            for (int j=0;j<point.size();j++){
                if (point[j]){
                    stop=true;
                }
            }
            if (stop){
                break;
            }
                k++;
        }
        stop=false;
        // �� ��������� ��� 0 � ����� ������� ��������
        l = (int) line.size() - 1;
        while (l >= k) {
            point.clear();point=line[l];

            for (int j=0;j<point.size();j++){
                if (point[j]){
                    stop=true;
                }
            }
            if (stop){
                break;
            }
            l--;
        }

        if (l < k) {
                // ���� ���� ������ 0 - �� ������� �������� �� ������
                markUps.erase(markUps.begin() + i);
        } else {
                // ������� 0 � ������ � � ����� ������� ��������
                markUps[i].erase(markUps[i].begin() + l + 1, markUps[i].end());
                markUps[i].erase(markUps[i].begin(), markUps[i].begin() + k);
        }
}
return 0;

}



void ASStageMultiMarking::createAllMarkUpVariants(AxiomExprSetPlus &as, std::vector<std::vector<std::vector<std::vector<bool> > > > &markUpVariants) {
    int numOfClasses;
        std::vector <int> numOfMultiTS;
        std::vector < std::vector <int> > numOfTS;
        // ��������� ���������� �� ��������� ������ ������
        fuzzyDataSet->getClassSize (numOfClasses, numOfMultiTS, numOfTS);
        // ��� ������� ������ ���������� ��������� ��������� ����������� ���������� ��������
        std::vector <std::vector <std::vector<bool> > > resMarkUps;
        markUpVariants.resize(numOfClasses);
        for (int abType = 0; abType < numOfClasses; abType++) {
                markUpVariants[abType].clear();
                resMarkUps.resize (numOfMultiTS[abType]);

                // �������� ��������
                for (int multiTS = 0; multiTS < numOfMultiTS[abType]; multiTS++) {
                        createRefMarkUp (as, FuzzyDataSet::Reference, abType, multiTS, resMarkUps[multiTS]);
                }

                // �������� �������� - ������� 0 � ������ � � ����� �������� - ����� ����� ����� ���� ������ �����
                simplifyMarkUps (resMarkUps);

                // �� ��������� ���������� �������� ���������� ����������� ��������� - ���������� �� ����� � ��������� �������� ���������� ��������
                createMarkUpVariants (markUpVariants[abType], resMarkUps);
                // �������� ������� �������� - ������� 0 � ������ � � �����. ���� �������� �������� ��������� ��������� � ���� ��������
                simplifyMarkUps (markUpVariants[abType]);
                // ��������� ����������� �������� �������� - ���� ����� ����� �������� �� �������
                if (markUpVariants[abType].size() < 1) {
                        //logger->writeDebug("Warning: couldn't find common subsequence, creating simple markup");
                        createSimpleMarkUpVariants (markUpVariants[abType], (int) as.axioms.size());
                }
        }
}
/****************************************************************************
*					FuzzyMultiDataExt_AS::run()
*
*	Description: ������ �������� ���������
*	Parameters:		-
*	Returns:		-
*	Throws:			-
*	Author:			armkor
*	History:
*
****************************************************************************/
std::vector<std::vector<int > > ASStageMultiMarking::stringIn(std::vector<std::vector<std::vector<bool> > > &in,int &numberOfAxiom){
    std::vector<std::vector<int > > result;
    std::vector<bool> point;
    std::vector<std::vector<bool> > oneLine;
    point.clear();oneLine.clear();
    int number;

    for (int i=0;i<in.size();i++){
        oneLine=in[i];
        std::vector<int > lineOfResult;
        for (int j=0;j<oneLine.size();j++){

            number=0;
            point.clear();
            point=oneLine[j];
            numberOfAxiom=point.size();
            for (int k=0;k<point.size();k++){
                if (point[k]){
                    number++;
                    lineOfResult.push_back(k+1);
                }
            }
            if (number==0){
                //������ ���������
                lineOfResult.push_back(-1);
                lineOfResult.push_back(-1);
            }
            //�����������
            lineOfResult.push_back(-1);
        }
        result.push_back(lineOfResult);
    }
    return result;
}

std::vector<std::vector<std::vector<bool> > > ASStageMultiMarking::stringOut(std::vector<std::vector<int > > &genMarkUps,int& numberOfAxiom){
    std::vector<std::vector<std::vector<bool> > > result;
    // ���������� ����������
    std::vector<int > oneLine;
    for (int i=0;i<genMarkUps.size();i++){
        // ������� � ����� ����������
        std::vector<std::vector<bool> > lineOfResult;

        oneLine.clear();
        oneLine=genMarkUps[i];
        int j=0;

        while (j < oneLine.size()){
            // �������������� �����
            std::vector<bool> pointOfResult;
            pointOfResult.resize(numberOfAxiom);
            for (int k=0;k<pointOfResult.size();k++){
                pointOfResult[k]=false;
            }

            if((j+1)<oneLine.size()){
                //������ ���������
                if (oneLine[j]==-1 && oneLine[j+1]==-1){
                    lineOfResult.push_back(pointOfResult);
                    j=j+2;
                }
                else{
                    while(oneLine[j]!=-1 && j < oneLine.size()){
                        pointOfResult[oneLine[j]-1]=true;
                        j++;
                    }
                    lineOfResult.push_back(pointOfResult);
                }
            }
            j++;
        }

    result.push_back(lineOfResult);
    }
    return result;
}


inline int ASStageMultiMarking::createMarkUpVariants (std::vector<std::vector<std::vector<bool> > > &genMarkUps,std::vector<std::vector<std::vector<bool> > >  &resMarkUps) {
    if (!areMultiMark){
        std::vector<std::vector<int > > _resMarkUps,_genMarkUps;
        int numberOfAxiom;
        _resMarkUps = stringIn(resMarkUps,numberOfAxiom);

        std::vector<std::vector<int > > temp;
        _genMarkUps.push_back(_resMarkUps[0]);
        temp.push_back(_resMarkUps[0]);

        for (int i=1;i<_resMarkUps.size();i++){
            for (int j=0;j<_genMarkUps.size();j++){
                std::vector<std::vector<int> > common =findCommonSubsequence(_resMarkUps[i],_genMarkUps[j],distanceFunctionForAxiom,choiceFunctionForAxiom,this->porog,this->maxNOP);
                debug_common_int(common);
                if (common.size()>0){
                    for (int k=0;k<common.size();k++){
                        temp.push_back(common[k]);
                    }
                }
            }
            _genMarkUps=temp;
            temp.clear();
        }
        genMarkUps=stringOut(_genMarkUps,numberOfAxiom);
    }
    else {
        std::vector<std::vector<std::vector<bool> > > temp;
        genMarkUps.push_back(resMarkUps[0]);
        temp.push_back(resMarkUps[0]);
        for (int i=1;i<resMarkUps.size();i++){
            for (int j=0;j<genMarkUps.size();j++){
                std::vector<std::vector<std::vector<bool> > > common =findCommonSubsequence(resMarkUps[i],genMarkUps[j],DistanceFunctor(this->m),choiceFunctionForMultiMark,this->porog,this->maxNOP);
                debug_common_bool(common);
                if (common.size()>0){
                    for (int k=0;k<common.size();k++){
                        temp.push_back(common[k]);
                    }
                }
            }
            genMarkUps=temp;
            temp.clear();
        }

    }
        return 0;
}



// From run

/****************************************************************************
*					FuzzyMultiDataExt_AS::sortAxiomSets
*
*	Description:	������� ��������� ������ ������ ������ �� ���������� ������� ��������
*	Parameters:		axiomSets - ������ ����������� ������ ������
*					indecies - ������ �������� ������ ������ � ������� ���������� �������� ������� �������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int ASStageMultiMarking::sortAxiomSets (const std::vector <AxiomExprSetPlus> &axiomSets, std::vector <int> &indecies) const {
    indecies.resize (axiomSets.size());
        for (int i = 0; i < (int) axiomSets.size(); i++)
                indecies[i] = i;
        int tmp;
        for (int i = (int) axiomSets.size() - 1; i > 0; i--) {
                for (int j = 0; j < i; j++) {
                        if (axiomSets[indecies[j]].goal > axiomSets[indecies[j+1]].goal) {
                                tmp = indecies[j+1];
                                indecies[j+1] = indecies[j];
                                indecies[j] = tmp;
                        }
                }
        }
        return 0;
}
/****************************************************************************
*					FuzzyMultiDataExt_AS::addAxiomSets
*
*	Description:	������� ��������� ������� ������ �� ������ �������, �������
*					������� ����������� �� ������� �������, � ������ ������.
*	Parameters:		nextStepAxiomSets - ������ ������ ������, � ������� ����������� ����� �������
*					newAxiomSets - ������ ������, �� �������� ����������� ������� ������
*					indicesOfBestSets - ������ �������� ����������� ������ ������
*	Returns:		0
*	Throws:			AxiomLibException - ���� ������, � ������� ��������� ������� ������ �� ����
*	Author:			dk
*	History:
*
****************************************************************************/
int ASStageMultiMarking::addAxiomSets(std::vector <AxiomExprSetPlus> &nextStepAxiomSets, std::vector <AxiomExprSetPlus> &newAxiomSets, std::vector <int> &indicesOfBestSets) const {
    if (nextStepAxiomSets.size() > 0)
                throw AxiomLibException("Error in FuzzyMultiDataExt_AS::addAxiomSets: input arguments out of consistency.");
        // ���������� ������� ������ ������ ���������
        unsigned int toAdd = 0;
        for (unsigned int i = 0; i < indicesOfBestSets.size(); i++) {
                if ((indicesOfBestSets[i] >= 0 ) && (indicesOfBestSets[i] < (int) newAxiomSets.size()))
                        toAdd++;
        }
        // ������� ��� ����� ������� ������ � ������ ���� ������ � ������������ � ��������� ��������
        unsigned int curSize = 0; // nextStepAxiomSets.size();
        nextStepAxiomSets.resize (toAdd);
        for (unsigned int i = 0; i < indicesOfBestSets.size(); i++) {
                if ((indicesOfBestSets[i] >= 0 ) && (indicesOfBestSets[i] < (int) newAxiomSets.size())) {
                        nextStepAxiomSets[curSize] = newAxiomSets[indicesOfBestSets[i]];
                        curSize++;
                }
        }
        return 0;
}
/****************************************************************************
*					FuzzyMultiDataExt_AS::chooseIndicesOfBestAxiomSets
*
*	Description:	������� �������� �� ������� ������ ������ �������� �����
*					������, ������ �� �������� ������� ������� ������ ����
*					������ ��������� ��������.
*	Parameters:		newAxiomSets - ������ ������ ������, �� ������� ���������� ������
*					indicesOfBestSets - ����������� ������ �������� ������ ������ ������
*					goal - �������� ������� �������, ��� �������� ������ ������ ������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int ASStageMultiMarking::chooseIndicesOfBestAxiomSets (std::vector <AxiomExprSetPlus> &newAxiomSets, std::vector <int> &indicesOfBestSets, double &goal) const {
    // ��������� ������ ������ ������ �� �������� ������� �������
    std::vector <int> indecies;
    sortAxiomSets (newAxiomSets, indecies);
    // ���� ������ �������������� ������� ������
    int first = 0;
    while (first < (int) indecies.size()) {
            if (newAxiomSets[indecies[first]].goal > -eps)
                    break;
            first++;
    }
    // ���� ������� ������, ������� � �������� ��� ������� ������ ����� �������� ������� ������� ������ ��������� goal
    int last = first;
    while (last < (int) indecies.size()) {
            if ((newAxiomSets[indecies[last]].goal - goal) > eps)
                    break;
            last++;
    }
    // ���� ����� �� ������� - �� �������
    if ((first == (int) indecies.size()) || (first == last)) {
            indicesOfBestSets.clear();
            return 0;
    }

    // ���������� ������������ ����� ��������� � �������������� �������
    //  �������� ����� ������ ������ ������ � �������� ���������
    int numBest, numRand, num;
    numBest = round ((double) int_max_number_of_renew_axiom_set * percentBestAxiomSets);
    if (numBest > (last - first)) {
            numBest = last - first;
            numRand = round ((double) numBest * (1.0 - percentBestAxiomSets) / percentBestAxiomSets);
            num = numBest + numRand;
    } else {
            numRand = int_max_number_of_renew_axiom_set - numBest;
            num = int_max_number_of_renew_axiom_set;
    }

    // ���� �������� ������� ����� ������ ������
    if (((int) newAxiomSets.size() - first) <= num) {
            num = (int) newAxiomSets.size() - first;
            indicesOfBestSets.resize (num);
            for (int i = 0; i < num; i++, first++)
                    indicesOfBestSets[i] = indecies[first];
            return 0;
    }

    // �������� ��������� ������� ������
    int chooseDelta;
    chooseDelta = indecies.size() - first - numBest;
    std::vector <int> jRand;
    jRand.resize (numRand);
    int cur;
    int condition;
    for (int t = 0; t < numRand; t++) {
            // �������� �������� ����� �� ������� [0, chooseDelta-1]
            cur = first + numBest + (int) (((double) rand () / ((double) RAND_MAX + 1.0)) * (double) chooseDelta);
            // ��������� - �� �������� �� ��� ������ - ���� ��������, �� ��������
            do {
                    condition = false;
                    for (int s = 0; s < t; s++) {
                            if (cur == jRand[s]) {
                                    cur++;
                                    condition = true;
                                    break;
                            }
                    }
            } while (condition);
            // ��������� ��������� �����
            jRand[t] = cur;
            // ��������� �������� ������
            chooseDelta--;
    }
    // ��������� ��� ��������� �������
    indicesOfBestSets.resize (num);
    cur = first;
    for (int i = 0; i < numBest; i++, cur++)
            indicesOfBestSets[i] = indecies[cur];
    cur = 0;
    for (int i = numBest; i < num; i++, cur++)
            indicesOfBestSets[i] = indecies[jRand[cur]];
    return 0;

}
/****************************************************************************
*					FuzzyMultiDataExt_AS::cutDownAxiomSets
*
*	Description:	������� ��������� ����� ������ ������ �� ������� ������� ��
*					��������� � ���������� ������ ��������.
*	Parameters:		axiomSets - ������ ������ ������, ������� �������� ����������
*					�� ��������� �������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int ASStageMultiMarking::cutDownAxiomSets (std::vector <AxiomExprSetPlus> &axiomSets) const {
        // �������� ������� ����������
    if ((maxAxiomSetPopSize < 1) || ( (int) axiomSets.size() <= maxAxiomSetPopSize))
                return 0;

        // ���������� ������ �� �������� ������� �������
        std::vector <int> indecies;
        sortAxiomSets (axiomSets, indecies);

        // ����������� ����� ������ ������, ����������� � ���������
        unsigned int numBestToSave, numToChoose, numFrom;
        numBestToSave = (unsigned int) round (percentBestAxiomSets*maxAxiomSetPopSize);
        numToChoose = (unsigned int) maxAxiomSetPopSize - numBestToSave;

        numFrom = axiomSets.size() - numBestToSave;
        std::vector <bool> chosen;
        chosen.resize (numFrom, false);
        int curChoice;
        std::vector <int> toSave;
        toSave.resize (maxAxiomSetPopSize);
        // ������� � ������ ����������� ��������� - �������� ����� ������
        for (unsigned int i = 0; i < numBestToSave; i++) {
                toSave[i] = indecies[i];
        }
        // ��������� ����� ����������
        for (unsigned int i = 0; i < numToChoose; i++) {
                curChoice = round (((double) rand() / (double) RAND_MAX) * (double) (numFrom - i - 1));
                for (unsigned int t = (unsigned int) curChoice; t < numFrom; t++)
                        if (!chosen[t]) {
                                chosen[t] = true;
                                toSave[numBestToSave + i] = indecies[numBestToSave + t];
                                break;
                        }
        }

        // ���������� ������� ��������� ��� ���������� ��������� �� ��������
        unsigned int tmpInt;
        for (unsigned int i = maxAxiomSetPopSize - 1; i > 0; i--) {
                for (unsigned int j = 0; j < i; j++) {
                        if (toSave[j] < toSave[j+1]) {
                                tmpInt = toSave[j+1];
                                toSave[j+1] = toSave[j];
                                toSave[j] = tmpInt;
                        }
                }
        }

        // �������� ����������� ������ ������
        // ������� ���������� ��������� ������� ������
        if ((toSave[0] + 1) < (int) axiomSets.size()) {
                for (unsigned int j = toSave[0] + 1; j < axiomSets.size(); j++)
                        axiomSets[j].clear();
                axiomSets.erase (axiomSets.begin() + toSave[0] + 1, axiomSets.end());
        }
        for (int i = 1; i < maxAxiomSetPopSize; i++) {
                if ((toSave[i] + 1) < toSave[i-1]) {
                        // ������� ���������� ��������� ������� ������
                        for (int j = toSave[i] + 1; j < toSave[i-1]; j++)
                                axiomSets[j].clear();
                        // ������� �������� ��������
                        axiomSets.erase (axiomSets.begin() + toSave[i] + 1, axiomSets.begin() + toSave[i-1]);
                }
        }
        // ������� ���������� ��������� ������� ������
        if (toSave[maxAxiomSetPopSize-1] > 0) {
                for (int j = 0; j < toSave[maxAxiomSetPopSize-1]; j++) {
                        axiomSets[j].clear();
                }
                axiomSets.erase (axiomSets.begin(), axiomSets.begin() + toSave[maxAxiomSetPopSize-1]);
        }
        return 0;
}

/****************************************************************************
*					FuzzyMultiDataExt_AS::sortBestAxiomSets
*
*	Description:	������� ��������� ����� ��������� ������� ������ ������ ������
*					�� ��������� �� ��������� ��� � ���������������� ����� ��������
*	Parameters:		-
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int ASStageMultiMarking::sortBestAxiomSets (void) {
        // ���� ����� ��������� � ������� ������ ������ ������ ������ ��������� ��������� - �� ������ �������
    if ((int) bestAxiomSets.size() <= numberOfBestAxiomSets)
                return 0;
        // ���� ����� ���, ������� ������ ������ - ������ 1, �� ������ ������� ��� ��������
        if (numberOfBestAxiomSets < 1) {
                for (unsigned int i = 0; i < bestAxiomSets.size(); i++)
                        bestAxiomSets[i].clear();
                bestAxiomSets.clear();
                return 0;
        }
        // �������� �� ������� �������� ����� ������ ������ ������ � ������� ��
        int numToDel;
        numToDel = (int) bestAxiomSets.size() - numberOfBestAxiomSets;
        std::vector <int> indeciesToDel;
        indeciesToDel.resize(numToDel);
        for (int i = 0; i < numToDel; i++) {
                indeciesToDel[i] = i;
        }
        int curIndex;
        double curMin;
        for (int i = numToDel; i < (int) bestAxiomSets.size(); i++) {
                curMin = bestAxiomSets[i].goal;
                curIndex = -1;
                for (int j = 0; j < numToDel; j++) {
                        if (curMin > bestAxiomSets[indeciesToDel[j]].goal) {
                                curMin = bestAxiomSets[indeciesToDel[j]].goal;
                                curIndex = j;
                        }
                }
                if (curIndex >= 0) {
                        indeciesToDel[curIndex] = i;
                }
        }
        // ������ �������� ��������� - ��������� ��� �� ��������
        for (int i = numToDel - 1; i > 0; i--) {
                for (int j = 0; j < i; j++) {
                        if (indeciesToDel[j] < indeciesToDel[j+1]) {
                                curIndex = indeciesToDel[j+1];
                                indeciesToDel[j+1] = indeciesToDel[j];
                                indeciesToDel[j] = curIndex;
                        }
                }
        }
        // ������� ������� ������ � ������������ � ��������� �������� ��������
        for (int i = 0; i < numToDel; i++) {
                bestAxiomSets[indeciesToDel[i]].clear();
                bestAxiomSets.erase (bestAxiomSets.begin() + indeciesToDel[i]);
        }
        return 0;
}
/****************************************************************************
*					FuzzyMultiDataExt_AS::addToBestAxiomSets
*
*	Description:	������� ��������� �������� ����� ������ ������ � ������� ������ ������,
*					���� ��� ���� ��� ����� ��� �������� ������� ������ �� ������� �� ������
*					�������.
*	Parameters:		axiomExprSetPlus - ����������� ������� ������
*	Returns:		-1 - ���� � ���� ���������� �������� ������
*					0  - ���� ������� ������ ���� ���������
*					1  - ���� ������� ������� � ������ ������� ������ ������, ��� � �������
*						 � ������� ������ ������, ������� ������ ������� �� ���� ���������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int ASStageMultiMarking::addToBestAxiomSets (std::vector <AxiomExprSetPlus> &axiomSets) {
        // �������� ���������� ������� bestAxiomSets �� ��������� ����������
    std::vector <AxiomExprSetPlus> tmpBest;
        tmpBest.resize (bestAxiomSets.size());
        for (unsigned int i = 0; i < bestAxiomSets.size(); i++) {
                tmpBest[i] = bestAxiomSets[i];
                bestAxiomSets[i].clear();
        }
        // ����������� ������� tmpBest � axiomSets �� �����������
        std::vector <int> indeciesSets, indeciesBest;
        sortAxiomSets (axiomSets, indeciesSets);
        sortAxiomSets (tmpBest, indeciesBest);
        // �������� ����� ������� ������ ���������
        unsigned int s = 0;
        unsigned int b = 0;
        unsigned int sMax = axiomSets.size();
        unsigned int bMax = tmpBest.size();
        unsigned int upTo = int_max_size_of_vector_axiomExprSet;
        if ((sMax + bMax) < upTo) {
                upTo = sMax + bMax;
                s = sMax;
                b = bMax;
        }
        while ((s + b) < upTo) {
                if (b >= bMax) {
                        s++;
                        continue;
                }
                if (s >= sMax) {
                        b++;
                        continue;
                }
                if (tmpBest[indeciesBest[b]].goal < axiomSets[indeciesSets[s]].goal)
                        b++;
                else
                        s++;
        }

        // ��������� ��������� ������� ������ � ������ ������
        bestAxiomSets.resize (upTo);
        for (unsigned int i = 0; i < b; i++)
                bestAxiomSets[i] = tmpBest[i];
        for (unsigned int i = 0; i < s; i++, b++)
                bestAxiomSets[b] = axiomSets[i];

        // ������� �������������� �������
        for (unsigned int i = 0; i < axiomSets.size(); i++)
                axiomSets[i].clear();
        axiomSets.clear();
        for (unsigned int i = 0; i < tmpBest.size(); i++)
                tmpBest[i].clear();
        tmpBest.clear();
        return 0;
}


/****************************************************************************
*					FuzzyMultiDataExt_AS::run()
*
*	Description: ���������� ������� ������
*	Parameters:		-
*	Returns:		-
*	Throws:			-
*	Author:			armkor
*	History:
*
****************************************************************************/

void ASStageMultiMarking::run(){
    std::cerr << " Start run";
    // stage2 ������ ������� ��� ������� ������ ���������� ���������
    std::vector<int> sizeVector;
    stage2->getAXSize(sizeVector);
    logger->writeComment("Enter AxiomCreator");
    //  ������� ������ ���� ������ - ����� ��� ����� ���� ������������ ��� ������������ ������ ������
    int numOfAxioms = 0;
    for (unsigned int i = 0; i < sizeVector.size(); i++) {
            for(int j = 0; j < sizeVector[i]; j++) {
                    if(stage2->isAXSelected(i, j)) {
                            ++numOfAxioms;
                    }
            }
    }

    // ������ ������, ����������� �� ���� ������������ ������ �� ������������ �������
    std::vector <AxiomExpr> bestAxioms;
    bestAxioms.reserve(numOfAxioms);

    for (unsigned int i = 0; i < sizeVector.size(); i++) {
            for(int j = 0; j < sizeVector[i]; j++) {
                    if(stage2->isAXSelected(i, j)) {
                            AxiomExpr ax = stage2->getAX(i, j);
                            bestAxioms.push_back(ax);
                    }
            }
    }

    bestAxiomSets.clear();

    // ������� ����� ������ ������
    std::vector <AxiomExprSetPlus> axiomSets, newAxiomSets;
    std::vector < std::vector <AxiomExprSetPlus> > nextStepAxiomSets;
    axiomSets.resize(numOfAxioms);

    logger->writeComment("Create one axiom system");
    // ������� ����� ������ ������, ������ �� ������� ����� ��������� ������ ���� �������
	//#pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < numOfAxioms; i++) {
        // ��������� ������� ������ �� ����� �������
        axiomSets[i].addAxiom (bestAxioms[i]);
        // ������������� ����������
        axiomSets[i].axiomsIndex.push_back(i);
        // ��������� �������� ������� ������� ��� ����� ������� ������ + ���� ���� ��������
        matterAxiomSetFunc (axiomSets[i]);

    }
    std::cerr << "Step 1\n";
    // ������������ �������� ���������� ������� ������
    bool condition = true;
    std::vector <int> indicesOfBestSets;
    nextStepAxiomSets.clear();
    int iterNum = 0;
    double defMaxGoal;
    defMaxGoal = -1.0; // (axiomSets[axSet].goal + 1)*2.0; // ������ defMaxGoal ���������� ������� (�������� ������ axiomSets[axSet].goal - ���� � ������ �������������� �����������)
    // ������������� ������ ������� �� ����� ������
    unsigned int wholeNum = 0;
    newAxiomSets.resize (bestAxioms.size());
    while (condition) {
            // �������������� ���������� ��� ������� ������� ������ ������
            wholeNum = 0;
            nextStepAxiomSets.resize (axiomSets.size());
            for (unsigned int axSet = 0; axSet < axiomSets.size(); axSet++) {
                    // ������� ���������� ������� � �������������� ������������
                    for (unsigned int u = 0; u < newAxiomSets.size(); u++)
                            newAxiomSets[u].clear();

                    // ��� ������ ������� - ������� ����� ������� �� ������ ������ ������� � ������� axiomSets[axSet]

                    // ��� ���� ����� � bestAxioms ��������� �������. ������ ������ � axiomsIndex �����������
                    int size=axiomSets[axSet].axiomsIndex.size();
                    unsigned int lastAxiomInSet=axiomSets[axSet].axiomsIndex[size-1];

					//#pragma omp parallel for schedule(dynamic, 1)
                    for (int ax = lastAxiomInSet + 1; ax < (int) bestAxioms.size(); ax++) {
                            // ��� ���� ������, ��� �� �������� � ��������������� ������� ������ ������� ����� �������
                                    // ������� ����� ������� ������ �� ������ ��������� - �������� � ��� ��� ����������� �������
                                    newAxiomSets[ax] = axiomSets[axSet];
                                    tryAddAxiom(newAxiomSets[ax], bestAxioms[ax], ax);

                    }
                    //��� �� ����� ���������, � ���� �� ������� ��-�� ������� ��������
                    std::cerr << "Step 2\n";
                    // �������� �������� ����� ������ ������ ������ � ��������� �� � ��������� �� ��������� ���� ���������
                    indicesOfBestSets.clear();
                    chooseIndicesOfBestAxiomSets (newAxiomSets, indicesOfBestSets, axiomSets[axSet].goal);
                    wholeNum += indicesOfBestSets.size();
                    addAxiomSets(nextStepAxiomSets[axSet], newAxiomSets, indicesOfBestSets);
            }
            std::cerr << "Step 3\n";
            // ��������� �������� ������� ������ � ����������� ������ ����������� � �� ���� ���� ������� �� ����������
            addToBestAxiomSets (axiomSets);
            // ��������� ������� ������ ��� ���������� ���� � axiomSets
            axiomSets.resize(wholeNum);
            unsigned int cur = 0;
            double curBestVal = -1.0;

            for (unsigned int y = 0; y < nextStepAxiomSets.size(); y++) {
                    for (unsigned int u = 0; u < nextStepAxiomSets[y].size(); u++, cur++) {
                            axiomSets[cur] = nextStepAxiomSets[y][u];
                            nextStepAxiomSets[y][u].clear();
                            if ((curBestVal < -eps) || ((curBestVal - axiomSets[cur].goal) > eps))
                                    curBestVal = axiomSets[cur].goal;
                    }
                    nextStepAxiomSets[y].clear();
            }
            nextStepAxiomSets.clear();
            std::cerr << "Step 3_1\n";
            // - ������, ��� ��� ���� ����� ������ ������ ��� �������������� � ������� chooseIndicesOfBestAxiomSets
            // ������������ ����� ������ ������, ����� ��� �� ����� ������������ �� ��������� �������������� ���������
            cutDownAxiomSets (axiomSets);

            // ����������� ����� ��������, ����������� ����������
            iterNum++;

            // ��������� �������� ��������:
            // - �������, ���� �� ���� ������� �� ����� ����� ������� ������ � ������ ��������� ������� �������
            if (axiomSets.size() < 1) {
                    logger->writeComment("Stopping because no new axiom sets created");
                    condition = false;
                    break;
            }
            // - �������, ���� ����� �������� ������ ��������� �������
            if ((iterNum >= maxNumberOfSteps) || ((curBestVal - bestAxiomSetGoal) < eps)) {
                    if(iterNum >= maxNumberOfSteps) {
                            logger->writeComment("Stopping because maximum number of iterations reached");
                    } else {
                            logger->writeComment("Stopping because best axiom set goal value reached");
                    }
                    // ��������� �������� ������� ������ � ����������� ������ ����������� � �� ���� ���� ������� �� ����������
                    addToBestAxiomSets (axiomSets);
                    condition = false;
                    break;
            }
    }

    // ���������� ������ ������ ������ �� ��������� ����� ���������
  std::cerr << "Step 4\n";
    sortBestAxiomSets ();
    std::sort(bestAxiomSets.begin(), bestAxiomSets.end());
std::cerr << "Step 5\n";
}

/****************************************************************************
*					::run()
*
*	Description: ���������� �������.������� �������� ��������� ����������.
*                    ������� ������� �������.
*	Parameters:		-
*	Returns:		-
*	Throws:			-
*	Author:			armkor
*	History:
*
****************************************************************************/

int ASStageMultiMarking::tryAddAxiom(AxiomExprSetPlus &as, const AxiomExpr &ax, int axIndex) {
    as.addAxiom(ax);
    as.axiomsIndex.push_back(axIndex);
    std::vector<std::vector<std::vector<std::vector<bool> > > > markUpVariants;
    createAllMarkUpVariants(as, markUpVariants);
    matterAxiomSetFunc (as, markUpVariants);
    return -1;
}
