/****************************************************************************
*				OccClustering.h
*
*	Description: ������� ����� ��� �������� ������������� �� ������ ����������
*	Author:		wictor
*	History:	
*
****************************************************************************/
#ifndef OCCCLUSTERING_H
#define OCCCLUSTERING_H

#include "../FuzzyDataSet.h"
#include "../SatPointSet.h"
#include "../IntInterval.h"
#include "Heuristics.h"

namespace AxiomLib {
namespace FuzzyMultiDataExt {

//struct ClusteringInfo {
//	int maxClusterSize;
//	int minMaxClusterSize;
//};

// ������� ����� ��� ��������, ����������� �������������
// �� ������ ����������
class OccClustering : public Heuristics
{
public:
    OccClustering();
		
protected:
	void initFromEnvImpl(const Environment &env);
	
	// �������� ������������� �� ��� �������,
	// ���������� ��������� -- �������������
	// ������� ������� ��� ���
	template<class Axiom>
	bool clusterize(const Axiom& axiom, int abType, const FuzzyDataSet& dataSet) const;	
	
private:
	int m_maxClusterSize;
	int m_minMaxClusterSize;
	int m_minDistance;
	
	IntInterval getNextCluster(const SatPointSet& satPointSet, int multiTSNo, int &currentPoint, int minClusterDistance) const;
};

template<class Axiom>
bool OccClustering::clusterize(const Axiom& axiom, int abType, const FuzzyDataSet& dataSet) const {
	SatPointSet satPointSet = SatPointSet(axiom, dataSet, FuzzyDataSet::Reference, abType);
	
	// ���������� ��������� ������� ��������
	// � ������������ ��������� ������� �������� �� ����������
	int maxClusterSize = 0;
	int minMaxClusterSize = -1;
	for(int multiTS = 0; multiTS < satPointSet.size(); ++multiTS) {
		int currentPoint = 0;
		int currentMaxClusterSize = 0;
		IntInterval currentCluster;
		while(true) {
			currentCluster = getNextCluster(satPointSet, multiTS, currentPoint, m_minDistance);
			if(currentCluster == IntInterval()) {
				break;
			}
			int currentClusterSize = currentCluster.length();
			if(currentMaxClusterSize < currentClusterSize) {
				currentMaxClusterSize = currentClusterSize;
			}
		}
		if(maxClusterSize < currentMaxClusterSize) {
			maxClusterSize = currentMaxClusterSize;
		}
		if(minMaxClusterSize > currentMaxClusterSize || minMaxClusterSize == -1) {
			minMaxClusterSize = currentMaxClusterSize;
		}
	}
	
	// ���������� ��������
	return maxClusterSize >= m_maxClusterSize && minMaxClusterSize >= m_minMaxClusterSize;
}

};

};

#endif // OCCCLUSTERING_H
