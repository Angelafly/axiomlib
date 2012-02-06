/****************************************************************************
*					AxiomStatistics
*
*	Description:	����� ��� ��������/���������� ���������� �� �������
*	Author:			wictor
*	History:	
*
****************************************************************************/

#ifndef AXIOMSTATISTICS_H
#define AXIOMSTATISTICS_H

#include <fstream>

#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include "AxiomExprPlus.h"
#include "AxiomLibException.h"

namespace AxiomLib {
	class AxiomStatistics {
	private:
		
		// ������� ������������ �� ���������� ���������
		double m_statNormal;
		// ������� ������������ �� ��������� ���������
		double m_statAbnorm;
		// ������������ ����� ����������� �������� ������� �������������
		// �� ����������� ���������� ���������
		double m_statVariation;
		// ��������� ����� ����������, ��������������� ���������� ���������, �� ������� 
		// ������� �����������, � ������ ����� ����������, ��������������� ����������
		// ���������		
		double m_statOccurence;
		// �������� ������� �������
		double m_goal;


		
		friend class boost::serialization::access;
		
#define AXIOMLIB_MEMBER_NVP(mName) boost::serialization::make_nvp(#mName, m_##mName);
		
		// �������� ������������� ������ ������
		template<class Archive>
		void serialize(Archive & archive, const unsigned int version)
		{
			archive & AXIOMLIB_MEMBER_NVP(statNormal);
			archive & AXIOMLIB_MEMBER_NVP(statAbnorm);
			archive & AXIOMLIB_MEMBER_NVP(statVariation);
			archive & AXIOMLIB_MEMBER_NVP(statOccurence);
			archive & AXIOMLIB_MEMBER_NVP(goal);
		}
		
#undef AXIOMLIB_MEMBER_NVP
	public:
		
		AxiomStatistics() : m_statNormal(-1), m_statAbnorm(-1), m_statVariation(-1), 
		m_statOccurence(-1), m_goal(-1) { }
		
		// ����������� - ������� ������ �� AxiomExprPlus
		AxiomStatistics(const AxiomExprPlus &ax) {
			m_statNormal = ax.statNormal;
			m_statAbnorm = ax.statAbnorm;
			m_statVariation = ax.statVariation;
			m_statOccurence = ax.statOccurence;
			m_goal = ax.goal;
		}
		
		// ����������� � ������������ ����������� �� ��������� ���, ��� �����
		
		// �������� ������ � ������ AxiomExprPlus
		void copyTo(AxiomExprPlus &ax) const {
			ax.statNormal = m_statNormal;
			ax.statAbnorm = m_statAbnorm;
			ax.statVariation = m_statVariation;
			ax.statOccurence = m_statOccurence;
			ax.goal = m_goal;
		}
		
		double goal() const { return m_goal; }
		
		// ��������� ������ � ����
		void saveToFile(const std::string &fileName) const {
			std::ofstream ofstr(fileName.c_str());
			if(ofstr.good())
			{
				boost::archive::xml_oarchive archive(ofstr);
				archive << boost::serialization::make_nvp("AxiomStatistics", (*this));
			}
			else
			{
				throw AxiomLibException("AxiomStatistics::saveToFile(): Couldn't open file '"
										+ fileName + "' for writing");
			}
		}
		
		// ��������� ������ �� �����
		void initFromFile(const std::string &fileName) {
			std::ifstream ifstr(fileName.c_str());
			if(ifstr.good())
			{
				boost::archive::xml_iarchive archive(ifstr);
				archive >> boost::serialization::make_nvp("AxiomStatistics", (*this));
			}
			else
			{
				throw AxiomLibException("AxiomStatistics::initFromFile(): Couldn't open file '"
										+ fileName + "' for reading");				
			}
		}

		bool operator==(const AxiomLib::AxiomStatistics& other) const {
			return std::abs(this->m_goal - other.m_goal) < 1e-8
			        && std::abs(this->m_statAbnorm - other.m_statAbnorm) < 1e-8
			        && std::abs(this->m_statNormal - other.m_statNormal) < 1e-8
			        && std::abs(this->m_statOccurence - other.m_statOccurence) < 1e-8
			        && std::abs(this->m_statVariation - other.m_statVariation) < 1e-8;
			        
		}
		
	};
}

#endif // AXIOMSTATISTICS_H
