#pragma once

#ifndef FUZZYMULTIDATAEXT_EC_CLUSTERING_H
#define FUZZYMULTIDATAEXT_EC_CLUSTERING_H

#include "../Environment.h"
#include "../FuzzyDataSet.h"
#include "Common.h"
#include "ECStage.h"
#include "ForwardDeclarations.h"
#include "ClusteringFeatures/ClusteringFeature.h"
#include "ClusteringRealizations\ClusteringModel.h"
#include "ElemCondClustering.h"

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

	// List of clustering features using for transform strip to vector of features
	std::list<ClusteringFeature*> clusteringFeatures;

	// List of elementary conditions
	std::list<ElemCondClustering*> elemCond;
	
	// Count of features in list "clusteringFeatures"
	int featuresCount;

	int dimensions;

	// result data after getting strips and trasforming into vector of feature 
	vector<vector<vector<double> > > resultFeatures;

	// count of strips which are randomly taken from trajectory
	int stripsCount;

	int stripLength;

	// k-means cluster's count
	int k;
	
	// Realization of clustering algorithm
	std::vector<ClusteringModel*> clusteringModels;

	void handleTrajectory(const std::vector<double>& trajectory, int dimension);
};

};
};
#endif // FUZZYMULTIDATAEXT_EC_CLUSTERING_H