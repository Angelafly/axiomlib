#pragma once

#ifndef FUZZYMULTIDATAEXT_EC_CLUSTERING_H
#define FUZZYMULTIDATAEXT_EC_CLUSTERING_H

#include "../Environment.h"
#include "../FuzzyDataSet.h"
#include "Common.h"
#include "ECStage.h"
#include "ForwardDeclarations.h"
#include "ClusteringFeatures/ClusteringFeature.h"
#include "ClusteringRealizations/ClusteringModel.h"
#include "ElemCondClustering.h"
#include "ECClusteringConfiguration.h"

using namespace std;

namespace AxiomLib {

namespace FuzzyMultiDataExt {

class ECStageClustering : public ECStage {
public: 
	ECStageClustering(FuzzyDataSet* fuzzyDataSet, ECTypeStage* stage0);
	
	virtual int initFromEnv(const Environment& env);
	
	virtual void run(); 

	/*
		� �-��� run ������ �����������:
		1) ������ �� ���������� N �������� ���������� �����
		2) ������� ������� ������� ������� � ������ ��������� (��. ����� ClusteringMark)
		3) ������������� ���������� ��������
	*/
	
	// ������� ������� � ������ ��
	// aType - ��� ���������� ���������
	// dimension - �����������
	// type - ��� ������������� ������� (�������� ������������� + ����� ��� + ����� � ����� ��������)
	// n - ����� � �������
private:
	Logger* logger;
	
	std::vector<ECClusteringConfiguration*> clusteringConfigurations;	
};

};
};
#endif // FUZZYMULTIDATAEXT_EC_CLUSTERING_H
