/****************************************************************************
 *			ManagedFuzzyDataStage2
 *
 *	Description:
 *	Author:		wictor
 *	History:	
 *
 ****************************************************************************/

#ifndef MANAGEDFUZZYDATASTAGE2_H_
#define MANAGEDFUZZYDATASTAGE2_H_

#include <vector>
#include <string>

#include <QtDebug>

#include "AxiomExprPlus.h"
#include "ManagedFuzzyDataStage.h"
#include "ManagedFuzzyDataUtility.h"
#include "../utilities.h"

class ManagedFuzzyDataStage2: public ManagedFuzzyDataStage {
	//����������, ����������� ��� ������������
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &archive, const unsigned int /*version*/)
	{
		archive & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ManagedFuzzyDataStage);
		archive & BOOST_SERIALIZATION_NVP(bestAxioms);
		archive & BOOST_SERIALIZATION_NVP(bestAxiomsSelected);
	}
	
	ManagedFuzzyDataStage2() {}
public:
	AxiomMultiVector bestAxioms;
	AxiomSelectedMultiVector bestAxiomsSelected;
	
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
	
	
	int getStageNumber() const { return 2; }
	
	void initImpl() {
		
		std::vector<int> sizeVector;
		m_algorithm->getAXSize(sizeVector);
		
		//TransformMultiVectorHelper<2>::makeSameSize(size)
		
		TransformMultiVectorHelper<1>::transform(sizeVector, bestAxioms,
											   [](int size) -> std::vector<AxiomLib::AxiomExprPlus>
											   { std::vector<AxiomLib::AxiomExprPlus> v; v.resize(size); return v; }
		);
		
		TransformMultiVectorHelper<1>::transform(sizeVector, bestAxiomsSelected,
											   [](int size) ->std::vector<bool>
											   { std::vector<bool> v; v.resize(size); return v; }
		);

		forEachAxiom(m_algorithm, 
					 [this](int i, int j, const AxiomLib::AxiomExprPlus& ax, bool selected)->void
					 {
			bestAxioms.at(i).at(j) = ax;
			bestAxiomsSelected.at(i).at(j) = selected;
		}
		);
		
		if(bestAxioms.empty()) {
			throw std::logic_error("ManagedFuzzyDataStage2::initImpl() : bestAxioms is empty");
		}
		
		if(bestAxiomsSelected.empty()) {
			throw std::logic_error("ManagedFuzzyDataStage2::initImpl() : bestAxiomsSelected is empty");
		}
	}
	
	void saveImpl() {
		
		if(bestAxioms.empty()) {
			throw std::logic_error("ManagedFuzzyDataStage2::saveImpl() : bestAxioms is empty");
		}
		
		if(bestAxiomsSelected.empty()) {
			throw std::logic_error("ManagedFuzzyDataStage2::saveImpl() : bestAxiomsSelected is empty");
		}
		
		m_algorithm->setAxioms(bestAxioms);
		forEachAxiom(m_algorithm,
					 [this](int i, int j, const AxiomLib::AxiomExprPlus&, bool)
					 {
			m_algorithm->setAXSelected(i, j, bestAxiomsSelected.at(i).at(j));
		}
		);
		
//		m_env->setParamValue(shiftErrIn, "shiftErrIn");
//		m_env->setParamValue(extendMarkUpLensLevel, "extendMarkUpLensLevel");
//		m_env->setParamValue(maxAxiomSetPopSize, "maxAxiomSetPopSize");
//		m_env->setParamValue(maxNumberOfSteps, "maxNumberOfSteps");
//		m_env->setParamValue(bestAxiomSetGoal, "bestAxiomSetGoal");
//		m_env->setParamValue(numberOfBestAxiomSets, "numberOfBestAxiomSets");
//		m_env->setParamValue(percentBestAxiomSets, "percentBestAxiomSets");
		
//		m_algorithm->initFromEnv(*m_env);
	}

	ManagedFuzzyDataStage2(AxiomLib::FuzzyMultiDataExtAlgorithm* algorithm, AxiomLib::Environment* env) : ManagedFuzzyDataStage(algorithm, env) {}
	~ManagedFuzzyDataStage2() {}
};


#endif /* MANAGEDFUZZYDATASTAGE2_H_ */
