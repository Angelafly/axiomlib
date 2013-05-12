/***************************************************************************
*					ElemCondPlus
*
*	Description:	��������� ���������� ������������ ������� � ����������
*	Author:			dk
*	History:
*
****************************************************************************/

#ifndef _Elem_Cond_Plus_HXX
#define _Elem_Cond_Plus_HXX

#include "./elem_conditions/ElemCondition.h"
#include "all_axioms.h"
#include "AxiomBase.h"
#include "ElemCondPlusStruture.h"
#include <vector>
#include <string>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

BOOST_CLASS_TRACKING(AxiomLib::ElemCondition, boost::serialization::track_never);

namespace AxiomLib {

class ElemCondPlus {

	//���������� ��� ������������ (��. ������������ � boost::serialization)
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & archive, const unsigned int /*version*/)
	{
		archive & BOOST_SERIALIZATION_NVP(elemCondition);
		archive & BOOST_SERIALIZATION_NVP(sign);
		archive & BOOST_SERIALIZATION_NVP(dimension);
		archive & BOOST_SERIALIZATION_NVP(name);
	}
 
 public:

	 // ������ �� ������������ �������
	 ElemCondition* elemCondition;

	 // ���� ������� (���� false �� ��� ���� � ����������)
	 bool sign;

	 // ����������� ������ ������, �� ������� ��������� ������ ������������ �������
	 int dimension;
	 
	 // ��� ����������� ��
	 std::string name;

	 size_t index;
	 
		 // ������� ������ //

	 // �����������
	 ElemCondPlus(void);

	 // �����������
	 ~ElemCondPlus(void);

	 // ����������� �����������
	 ElemCondPlus(const ElemCondPlus &second);

	 ElemCondPlus operator!() const;

	 // ���������� ��� ���� ������� ��
	 std::string ecTypeName (void) const;

	 // ���������� ����������� ������������ ��������
	 void getDimensions(std::vector<bool> &v) const;

	 // ������� �������� ������������ �� � ������ ����� ���������, ���� �� ����
	 signed int check (const unsigned long k, const std::vector<double>& x) const;
	 
	 // ������� �������� ������������ �� � ������ ��������� � �����������
	 signed int check (unsigned long k, const std::vector<std::vector<double> > &x) const;

	 // ������� �������� ���� ��
	 unsigned long markUp(const std::vector<double>& x, std::vector<int> &markup) const;

	 // ������� �������� ���������� ������ ���������
	 signed int clear (void);

	 // ��������� ������ ������� � �� ����������� ����������� � ������ �������� ���� �� ����.
	 double operator< (const ElemCondPlus& second) const;

	 // �������� ������������
	 ElemCondPlus& operator= (const ElemCondPlus& second);

	 // ������� ������� ������� �� ������ �� ��������� � ��������� ������� ������
	 signed int initFromStructure (ElemCondPlusStruture &ecps);

	 // ������� ������� ������� �� ������ �� ��������� � ��������� ������� ������
	 signed int initFromFile (const std::string &fullName);

	 // ������� ������� ������� �� ������ �� ��������� � ��������� ������� ������
	 signed int initFromFile (const std::string baseDir, const std::string ecName);

	 // ������� ������� ������� �� ������ �� ��������� � ��������� ������� ������
	 signed int initFromFile (const std::string baseDir, const std::string ecName, const std::vector <std::string> &paramNames);

	 // ������� ������� ������� �� ������ �� ��������� � ��������� ������� ������
	 signed int initFromFile (const std::string &filePath, const std::vector <std::string> &paramNames);

	 // ������� ��������� ��������� ��������� ������� � AxiomExprStructures
	 signed int createStructure (ElemCondPlusStruture &ecps) const;

	 // ������� ���������� ������� ������ � ����
	 signed int saveECToFile (const std::string baseDir, const std::string ecName) const;

	 // ������� ���������� ������� ������ � ����
	 signed int saveECToFile (const std::string baseDir, const std::string ecName, const std::vector <std::string> &paramNames) const;

};

}; //  end of namespace

//BOOST_CLASS_EXPORT_GUID(AxiomLib::ElemCondPlus, "ElemCondPlus");

#endif /* _Elem_Cond_Plus_HXX */
