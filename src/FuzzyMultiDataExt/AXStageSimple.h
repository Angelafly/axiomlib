#ifndef AXSTAGESIMPLE_H
#define AXSTAGESIMPLE_H

#include "AXStage.h"

namespace AxiomLib {

namespace FuzzyMultiDataExt {

class AXStageSimple : public AXStage
{
public:
	AXStageSimple(FuzzyDataSet* fuzzyDataSet,
				  ECStage* stage1);

	void initFromEnv(const Environment& env);

	// ������� ������� � ������ ������
	void setAxioms(const std::vector<std::vector<AxiomExprPlus> > &initial);

	const AxiomExprPlus &getAX(int aType, int n) const;
	bool isAXSelected(int aType, int n) const;
	void setAXSelected(int aType, int n, bool value);
	void getAXSize(std::vector<int> &result) const;
	int getAXSize() const;
	int getAXSize(int aType) const;

	void recalculateMatterAxiomFunc(AxiomExprPlus& ax, int abType);

	void run();

private:
	typedef std::vector<std::vector<AXSelection> > AXMultiVector;

	// ����� ����� ���������� �� ��������� � ���������� ���������
	//typedef boost::tuples::tuple<PSatPointSet, PSatPointSet> ReducedSatPointSet;

	AXMultiVector axioms;

	std::vector<std::vector<ReducedSatPointSet> > ecSatPoints;
	std::vector<std::vector<ReducedSatPointSet> > axSatPoints;

	// ��������� ������ ����� ���������� ��� ������� ���� ���������� ���������
	// ��� ������������ ��� ����, ����� ��������� ����� ��������� ������
	mutable std::vector<ReducedSatPointSet> m_temporary1, m_temporary2;

	// ��������� ��������� ��������� ������
	// ������������ ����� ������ � ���������
	int maxAxiomPopSize;
	// ������������ ����� �������� ��������� ������ ������
	int maxIterNum;
	// ������������ �������� ������� ������� ��� �������, ���� ��� �����������, �� ����� ����� ������ ���������������
	double maxAxiomRes;
	// ������� ������ ������ ���������, ������� ����������� ��� ���������� ���������
	double percentBestAxioms;
	// ��� ������������� � ������� �������
	double goalOccurenceWeight;

	std::string axiomNameTemplate;

	Logger* logger;

	void formAxioms(int abnormalType);
	int cutDownBestAxioms (std::vector <AXSelection> &axioms) const;
	void cutDownByOccurrence(std::vector <AXSelection> &axioms, int iterNum, int maxIterNum) const;
	int getPriority (std::vector <AXSelection> &vec, std::vector <unsigned int> &pos) const;
	int combineAxioms (AxiomExprPlus &axiomFirst, AxiomExprPlus &axiomSec, AxiomExprPlus &axiomExprPlus, const int abType) const;

	double matterAxiomFunc(AxiomExprPlus &ax, const int abType) const;
	double matterAxiomFunc (AxiomExprPlus &ax, const int abType, const ReducedSatPointSet& satPointSet) const;

	ReducedSatPointSet conjunction(int aType, const std::vector<ElemCondPlus> &ecs, ReducedSatPointSet& storage) const;
	// s1 |= s2
	static ReducedSatPointSet& disjunction(ReducedSatPointSet& s1, const ReducedSatPointSet& s2);

	void setSatPointsSize(int size);
	void setSatPointsSize(int aType, int size);

	void addToEcSatPoints(int aType, ElemCondPlus &ecPlus);

	ReducedSatPointSet createAxiomSatPoints(int abType, const AxiomExprPlus& ax, ReducedSatPointSet& storage, ReducedSatPointSet& storageAux) const;
	void createAxiomSatPoints(int abType, std::vector<AXSelection>& axVector);

	ReducedSatPointSet getOr(int abType, const AxiomExprPlus& first, const AxiomExprPlus& second, ReducedSatPointSet& storage) const;
	ReducedSatPointSet getAnd(int abType, const AxiomExprPlus& first, const AxiomExprPlus& second, ReducedSatPointSet& storage) const;

	const ReducedSatPointSet& getEcSatPointSet(int aType, const ElemCondPlus &ecPlus) const;
	const ReducedSatPointSet& getAxSatPointSet(int aType, const AxiomExprPlus& ax) const;

	void setNames();

	std::string getAXName(int abType, int n) const;

};

}

}

#endif // AXSTAGESIMPLE_H
