#ifndef AXSTAGESIMPLE_H
#define AXSTAGESIMPLE_H

#include "../AxiomExprPlus.h"
#include "../FuzzyMultiDataExtAlgorithm.h"

namespace AxiomLib {

namespace FuzzyMultiDataExt {

class AXStageSimple
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

	FuzzyMultiDataExtAlgorithm* getParent() const { return parent; }

private:

	// ����� ����� ���������� �� ��������� � ���������� ���������
	//typedef boost::tuples::tuple<PSatPointSet, PSatPointSet> ReducedSatPointSet;

	FuzzyMultiDataExtAlgorithm* parent;
	const FuzzyDataSet* fuzzyDataSet;
	const ECStage* stage1;

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


};

}

}

#endif // AXSTAGESIMPLE_H
