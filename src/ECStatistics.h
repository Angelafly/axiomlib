/****************************************************************************
*					ECStatistics
*
*	Description:	����� ��� ��������/���������� ���������� �� ��
*	Author:			wictor
*	History:	
*
****************************************************************************/
#ifndef ECSTATISTICS_H
#define ECSTATISTICS_H

#include <fstream>

#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include "ElemCondPlusStat.h"
#include "AxiomLibException.h"

namespace AxiomLib {

	class ECStatistics {
		
	private:
		
		// ������� ������������ �� ���������� ���������
		double m_statNormal;
		// ������� ������������ �� ��������� ���������
		double m_statAbnorm;
		// �������� ������� �������
		double m_goal;
		// �������������
		double m_statOccurence;
		
		friend class boost::serialization::access;
		
#define AXIOMLIB_MEMBER_NVP(mName) boost::serialization::make_nvp(#mName, m_##mName);
		
		// �������� ������������� ������ ������
		template<class Archive>
		void serialize(Archive & archive, const unsigned int /*version*/)
		{
			archive & AXIOMLIB_MEMBER_NVP(statNormal);
			archive & AXIOMLIB_MEMBER_NVP(statAbnorm);
			archive & AXIOMLIB_MEMBER_NVP(goal);
			archive & AXIOMLIB_MEMBER_NVP(statOccurence);
		}
		
#undef AXIOMLIB_MEMBER_NVP
		
	public:
		
		// ����������� ���������
		ECStatistics() : m_statNormal(-1), m_statAbnorm(-1), m_goal(-1), m_statOccurence(-1) {}
		
		// ����������� - ������� ������ �� ElemCondPlusStat
		ECStatistics(const ElemCondPlusStat &ec) {
			m_statNormal = ec.statNormal;
			m_statAbnorm = ec.statAbnorm;
			m_goal = ec.goal;
			m_statOccurence = ec.statOccurence;
		}
		
		double goal() const { return m_goal; }
		
		double statOccurrence() const { return m_statOccurence; }
		
		// ����������� � ������������ ����������� �� ��������� ���, ��� �����
		
		// �������� ������ � ������ ElemCondPlusStat
		void copyTo(ElemCondPlusStat &ec) const {
			ec.statNormal = m_statNormal;
			ec.statAbnorm = m_statAbnorm;
			ec.goal = m_goal;
			ec.statOccurence = m_statOccurence;
		}
		
		// ��������� ������ � ����
		void saveToFile(const std::string &fileName) const {
			std::ofstream ofstr(fileName.c_str());
			if(ofstr.good())
			{
				boost::archive::xml_oarchive archive(ofstr);
				archive << boost::serialization::make_nvp("ECStatistics", *this);
			}
			else
			{
				throw AxiomLibException("ECStatistics:saveToFile(): Couldn't open file '"
										+ fileName + "' for writing");
			}
		}
		
		// ��������� ������ �� �����
		void initFromFile(const std::string &fileName) {
			std::ifstream ifstr(fileName.c_str());
			if(ifstr.good())
			{
				boost::archive::xml_iarchive archive(ifstr);
				archive >> boost::serialization::make_nvp("ECStatistics", *this);
			}
			else
			{
				throw AxiomLibException("ECStatistics:initFromFile(): Couldn't open file '"
										+ fileName + "' for reading");				
			}
		}
		
	};
	
}

#endif // ECSTATISTICS_H
