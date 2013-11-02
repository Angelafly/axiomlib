#ifndef ASSTAGESIMPLE_H
#define ASSTAGESIMPLE_H

#include "../ReducedRecognizer.h"
#include "../GoalStrategy.h"
#include "../SatPointSet.h"

#include "../Environment.h"

#include "../FuzzyDataSet.h"

#include "Common.h"
#include "ForwardDeclarations.h"
#include "ASStage.h"

namespace AxiomLib {

namespace FuzzyMultiDataExt {

class ASStageSimple : public ASStage
{
public:
	ASStageSimple(FuzzyDataSet* fuzzyDataSet, 
	                     AXStage* stage2);
	
	void initFromEnv(const Environment& env);
	
	void run();
	
	// ������� ������� � ������ ������ ������
	void setAxiomSets(const std::vector<AxiomExprSetPlus>& initialAS);
	
	AxiomExprSetPlus &getAS(int n);
	const AxiomExprSetPlus &getAS(int n) const;
	int getASSize() const;
	
	void recalculateMatterASFunc(AxiomExprSetPlus& as) const;

private:
	class AXSatPointSet {
	public:
		AXSatPointSet(const AxiomExpr &ax, const FuzzyDataSet& fuzzyDataSet) {
			int classCount = fuzzyDataSet.getClassCount();
			
			refSatPoints.reserve(classCount);
			testSatPoints.reserve(classCount + 1);
			
			testSatPoints.push_back(
					PSatPointSet(new SatPointSet(ax, fuzzyDataSet, FuzzyDataSet::Testing, -1))
				);

			
			for(int i = 0; i < classCount; i++) {
				refSatPoints.push_back
						(
							PSatPointSet(new SatPointSet(ax, fuzzyDataSet, FuzzyDataSet::Reference, i))
						);
				testSatPoints.push_back
						(
							PSatPointSet(new SatPointSet(ax, fuzzyDataSet, FuzzyDataSet::Testing, i))
						);				
			}
		}
		
		const SatPointSet& getRefSatPointSet(int classNo) const { return *(refSatPoints[classNo]); }
		const SatPointSet& getTestSatPointSet(int classNo) const { 
			if(classNo < -1) {
				classNo = -1;
			}
			return *(testSatPoints[classNo + 1]); 
		}
	private:
		std::vector<PSatPointSet> refSatPoints, testSatPoints;
	};
	
	std::vector<AxiomExprSetPlus> bestAxiomSets;
	
	const FuzzyDataSet* fuzzyDataSet;
	const AXStage* stage2;

	// ��� ���������� (1/����� ��������) � ������� �������
	double singleAxiomMarkupPenalty;
	
	mutable std::vector<AXSatPointSet> satPoints;
	
	void setSatPointsSize(int size) const;
	void addToSatPoints(AxiomExpr& ax) const;
	
	const SatPointSet& getSatPoints(const AxiomExpr& ax, FuzzyDataSet::DataSetDivisionType type, int classNo) const;
	
	void performMarkUp(const AxiomExprSetPlus &as, FuzzyDataSet::DataSetDivisionType, int classNo, int multiTSNo, std::vector<int>& result) const;
	
	Logger* logger;
	
	// ��������� ��������� ���������� ������ ������
	// ��������� ������, ���� ��� ������ �������� �������� ������� �������� ��� ������������ � ������ ��������
	double shiftErrIn;
	// ������� ���������� �������� (���������� ��� ������ �������� �� ����� �� �������)
	double extendMarkUpLensLevel;
	// ������������ ����� ������ � ���������
	int maxAxiomSetPopSize;
	// ������������ ����� ����� � ��������� ���������� ������ ������
	int maxNumberOfSteps;
	// �������� ��������� ��� ��������� ���������� ������� ������ - ��������� �������� ������� �������
	double bestAxiomSetGoal;
	// ���� ������ ����������� ������ ������ ��� ������� ��������
	int numberOfBestAxiomSets;
	// ������� ������ ������ ������ � ���������, ������� ����������� ��� �������� �� ��������� ��� - (1 - percentBestAxiomSets) - ���������� ��������
	double percentBestAxiomSets;

	typedef boost::shared_ptr<ReducedRecognizer> PReducedRecognizer;
	// ����� ��� ������������� ��������� ���������� � ������������� ����
	PReducedRecognizer recognizer;

	// ����� �������� ����� ������ I � II ���� �� �������� �������
	//CompareStatistic* compareStatistic;
	int ccNumPoints; // ����� ����� � �������, ����� �������� ��������� ������ �� ������ ������ ����� � ���� ������� (������������ ��� �������� ����������)

	typedef boost::shared_ptr<GoalStrategy> PGoalStrategy;
	// ��������� �������� ������� ������� �� ������� I � II ����
	PGoalStrategy goalStrategy;
	
	// ��������� ������� � ������� ������ � ������������� ������� �������, 
	// ���� ������� ���������� � ��������� ���������� ���������
	int tryAddAxiom(AxiomExprSetPlus& as, const AxiomExpr& ax, int axIndex) const;
	
	void createAllMarkUpVariants(const AxiomExprSetPlus& as, std::vector<std::vector<std::vector<int> > >& markUpVariants) const;
	
	static bool markupsContain(const std::vector<std::vector<std::vector<int> > >& markUpVariants, int axiomNumber);
	
	int cutDownAxiomSets (std::vector <AxiomExprSetPlus> &axiomSets) const;
	
	int chooseIndicesOfBestAxiomSets (std::vector <AxiomExprSetPlus> &newAxiomSets, std::vector <int> &indicesOfBestSets, double &goal) const;
	
	int addAxiomSets(std::vector <AxiomExprSetPlus> &nextStepAxiomSets, std::vector <AxiomExprSetPlus> &newAxiomSets, std::vector <int> &indicesOfBestSets) const;
	
	int sortBestAxiomSets (void);
	
	int addToBestAxiomSets (std::vector <AxiomExprSetPlus> &axiomSets);
	
	int sortAxiomSets (const std::vector <AxiomExprSetPlus> &axiomSets, std::vector <int> &indecies) const;
	
	int createSimpleMarkUpVariants (std::vector < std::vector <int> > &genMarkUps, const int numOfAxioms) const;
	
	double matterAxiomSetFunc (AxiomExprSetPlus &as) const;
	
	double matterAxiomSetFunc (AxiomExprSetPlus &as, std::vector <std::vector <int> > &markUps) const;
	
	double matterAxiomSetFunc (AxiomExprSetPlus &as, const std::vector <std::vector <std::vector<int> > > &markupVariants) const;
	
	double matterAxiomSetFunc (AxiomExprSetPlus &as, int abType, const std::vector <int> &genMarkUp, double &goalVal, int &errFirstVal, int &errSecondVal) const;
	
	double matterAxiomSetFunc (const std::string baseDir, const std::string fileName, int &errFirst, int &errSecond) const;
	
	int getStatistic (std::vector <int> &row) const;
	
	int chooseBestMarkUp (AxiomExprSetPlus &as, int abType, std::vector <int> &markUp, const std::vector <std::vector <int> > &genMarkUps, int &errFirstVal, int &errSecondVal) const;
	
	int createMarkUpVariants (std::vector < std::vector <int> > &genMarkUps, std::vector < std::vector <int> > &resMarkUps) const;
	
	int createRefMarkUp (std::vector <int> &markUp, const AxiomExprSetPlus &as, const int abType, const int multiTSNumber, std::vector < std::vector <int> > &numOfTS) const;
	
	int createTestMarkUp (std::vector <int> &markUp, AxiomExprSetPlus &as, std::vector <bool> &dims, int &abType, int &indexTest, int &numOfTestTS) const;
	
	int createTestMarkUp (std::vector <int> &markUp, AxiomExprSetPlus &as, std::vector <bool> &dims, int &indexTest, int &numOfTestTS) const;
	
	int simplifyMarkUps (std::vector <std::vector <int> > &markUps) const;
	
	int getSubsequences (const std::vector <int> &str1, const std::vector <int> &str2, int i, int j, const std::vector < std::vector <int> > &square, std::vector < std::vector <int> > &res) const;
	
	int getSquare (const std::vector <int> &str1, const std::vector <int> &str2, std::vector < std::vector <int> > &square) const;
	
};

}

}

#endif // ASSTAGESIMPLE_H
