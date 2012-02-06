/****************************************************************************
*					ElemCondPlusStat 
*
*	Description:	����� �������� ������������� ������� � �������� �����������
*					���������� �� �����������, ���������� ���������� ����������
*					���������, � �� ����������� ����������� ���������.
*	Author:			dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_ElemCondPlusStat_HXX
#define __AXIOMS_LIB_ElemCondPlusStat_HXX

#include "ElemCondPlus.h"

namespace AxiomLib {

class ElemCondPlusStat : public ElemCondPlus {
	
	friend class boost::serialization::access;
	
	template<class Archive>
	void serialize(Archive & archive, const unsigned int /*version*/)
	{
		archive & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ElemCondPlus);
		archive & BOOST_SERIALIZATION_NVP(goal);
		archive & BOOST_SERIALIZATION_NVP(statAbnorm);
		archive & BOOST_SERIALIZATION_NVP(statNormal);
		archive & BOOST_SERIALIZATION_NVP(statOccurence);
		archive & BOOST_SERIALIZATION_NVP(statOccVector);
	}	
	
	void setDefaultStatistics();
	
 public:

	//	���������� ��������� ������ ElemCondPlus �� ���� ��������� ������ �� ����������:

	// �������� ������� �������
    double goal;

	// ������� ���������� �� �����������, ���������� ���������� ���������� ���������
	double statAbnorm;

	// ������� ���������� �� ����������� ����������� ���������
	double statNormal;
	
	// �������������
	double statOccurence;
	
	// ����� ������������� ��� ������ ���������� ��������� �������
	std::vector<bool> statOccVector;


	//	������� ������:
 
    // ������ ����������� � �������� ���������� �� ���������
    ElemCondPlusStat (void);  
	
	// �����������, ��������� ������ �� ElemCondPlus
	explicit ElemCondPlusStat(const ElemCondPlus &ecPlus);
	
    // ������ ����������
    ~ElemCondPlusStat(void);

    // ������� �������� �������� ���� ����������
    void clear (void);
  
	// �������� ������������ ������������� ������� �� �����������
    //ElemCondPlusStat& operator= (const ElemCondPlusStat &second);
	
	// ������� ���������� ������� ������ � ����
	signed int saveECToFile (const std::string baseDir, const std::string ecName, const std::vector <std::string> &paramNames) const;
	
	// ������� ������� ������� �� ������ �� ��������� � ��������� ������� ������
	signed int initFromFile (const std::string baseDir, const std::string ecName, const std::vector <std::string> &paramNames);
}; // end of class

}; //  end of namespace

//BOOST_CLASS_EXPORT_GUID(AxiomLib::ElemCondPlusStat, "ElemCondPlusStat");

#endif /* __AXIOMS_LIB_ElemCondPlusStat_HXX */
