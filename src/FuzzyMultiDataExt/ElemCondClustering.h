#pragma once

#ifndef FUZZYMULTIDATAEXT_EC_CLUSTERING_ELEM_COND_H
#define FUZZYMULTIDATAEXT_EC_CLUSTERING_ELEM_COND_H

#include "../ElemCondPlus.h"
#include  "ClusteringFeatures\ClusteringFeature.h"
#include "ClusteringRealizations\ClusteringModel.h"

using namespace std;

namespace AxiomLib {
	namespace FuzzyMultiDataExt {
		class ElemCondClustering : public ElemCondition {
			
		public: 
			// ���������� ��� ������ �������.
			virtual std::string name (void) const {
				return 0;
			}

			virtual ElemCondition* clone() const {
				ElemCondClustering* clone = new ElemCondClustering(*this);
				return clone;
			}

			ElemCondClustering(int clusterNumber, int length, const std::list<ClusteringFeature*>& clusteringFeatures, FuzzyMultiDataExt::ClusteringModel* model);

			// �������� ���������� ������ �������. �� ����� - ����� ���������� ���� k
			// � ��� ��������� ��� x. �� ������ 1, ���� �����������, 0 ���� ��
			// ����������� � -1, ���� �� ��������� (����� ������� �� ������� ����
			// ����� ��� ������).
			// ����� ������� ���� �����������, ��� ��� ������� ���� � ��� ����� �����������,
			// � ���� ������������� ��������� ���� �� ����� ����������, �� �����
			// ������� ������.
			virtual signed int check (const unsigned long k, const std::vector<double>& x) const;

			// ��������� ������ ������� � �� ����������� ����������� � ������ �������� ���� �� ����.
			// ���������� 1, ���� ������ ������� �������� ������� ������� ������, 0 ���� �������
			// �� �������� ��� ������������, -1, ���� ������ �������� ������� ������� ������
			virtual double operator< (const ElemCondition& second) const {
				return -1;
			}
    
			// �������� �������: ��������� �����-�� ���������� ���������� ������� (��� ������ �
			// ��� �������� �� ����������� ������). p ���������� ������� ��������� (������ ����������
			// �� ������� [0,1]). ���������� 0 ���� ��������� �� ���������, -1, ���� ��������� �� ���������� (��������,
			// � ���� ������� ��� ����������), 1, ���� ��������� ���������
			virtual signed int transmute (const double p) {
				return -1;
			}
    
			// ������ ��������� ������������� �������
			// ���������� 0, ���� ��� ok, -1 �����
			virtual signed int setParams (const std::map<std::string,std::string>& newParams) {
				return 0;
			}
    
			// ������ ��������� ������������� �������
			// ���������� 0, ���� ��� ok, -1 �����
			virtual signed int getParams (std::map<std::string,std::string> &newParams) const {
				return 0;
			}
    
			// ������� ������������� �������� ������ ��������� ������������� �������,
			// ���� ����� �������� ������������ � ������ ������������ �������
			virtual signed int setParamValue (const double param, const std::string &paramName) {
				return 0;
			}
   
			// ������� ���������� �������� ������ ��������� ������������� �������,
			// ���� ����� �������� ������������ � ������ ������������ �������
			virtual signed int getParamValue (double &param, const std::string &paramName) const {
				return -1;
			}
   
			// ������� ��������� ������� ������ �������� � ���������� ���������� ������ ������������ �������
			virtual signed int getECParamNames (std::vector<std::string> &ecParamNames) const {
				return -1;
			}
     
			  // �������� ������������� � ������ ������������ �������� �� ������ "�"
			  // 0 - ���� �� ����������
			  // 1 - ���� ����������
			  // -1 - ���� undefined
			virtual signed int checkAndCompatibility (const ElemCondition& toCheck) const {
				return -1;
			}

			  // ����������� ���� ������������ ������� (������ �� ���������)
			void crossEC(ElemCondition& second, const double crossProb, const double crossParam) {
				
			}

			  // ������� ������������� ������������ �������� ��� ������ left � right,
			  // ������� ���������� ����������� ������� ����� � ������� ����������� ������������ �������
			virtual signed int setLimits (const int leftLimit, const int rightLimit) {
				return 0;
			}

			  // ������� ������������� ������������ �������� ��� ���������� ������������� �������,
			  // �������� �� left � right
			virtual signed int setLimits (const std::vector<double>& row, const double reserve, const unsigned int numOfLevels) {
				return 0;
			}

			  // ������� �������� ��������� ������������� ������� � �������� �������� �� ����� �����,
			  // ��������� � ����������. ��������� �� ���� ��� - ��� ��������� �� ���� �������� delta.
			  // �������� delta ������������ ��� ��������� ������� ���������.
			virtual signed int changeParams (const int numOfSteps) {
				return -1;
			}

			// ������� ����������� ������� �� �� other � ���������� true, ���� � ��� ��������� ���
			// ����� ���������� false
			// � ���������� �� ����� �������������� ������� ��������,
			// �����, ��� new
			virtual bool assign(const ElemCondition* other) {
				return false;
			}

			private: 			
				int clusterNumber;

				int length;

				std::list<ClusteringFeature*> clusteringFeatures;

				ClusteringModel* clusteringModel;
		};
	};
};

#endif //FUZZYMULTIDATAEXT_EC_CLUSTERING_ELEM_COND_H