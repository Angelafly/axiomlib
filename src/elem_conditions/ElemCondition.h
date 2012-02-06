/****************************************************************************
*			ElemCondition
*
*	Description:	����� ������ ������� ������������ �������
*	Author:		Eugene A. Vasin
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_EL_SUPER_AXIOM_HXX
#define __AXIOMS_LIB_EL_SUPER_AXIOM_HXX

#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <map>
#include "boost/serialization/assume_abstract.hpp"
#include "boost/serialization/access.hpp"
#include "boost/serialization/nvp.hpp"


namespace AxiomLib {

//#pragma warning (disable:4996)

class ElemCondition {
  private:

	//����������, ����������� ��� ������������
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & /*archive*/, const unsigned int /*version*/)
	{
		//������ ����� ��� �� ���� �� ������� ������������ �����-���� ������
	}

  protected:
	// ���������� assign -- ��� ��� ���� ��������� � ���������
	// �� �������� ������, ������� ��� �������� � ��������� �������
	template<class Derived>
	static bool assign_impl(Derived* first, const ElemCondition* second);
  public:

      // ����������� �����������
    ElemCondition(void) {}
      // ����������� ����������
    virtual ~ElemCondition() {}

      // ���������� ��� ������ �������.
    virtual std::string name (void) const = 0;

      // �������� ���������� ������ �������. �� ����� - ����� ���������� ���� k
      // � ��� ��������� ��� x. �� ������ 1, ���� �����������, 0 ���� ��
      // ����������� � -1, ���� �� ��������� (����� ������� �� ������� ����
      // ����� ��� ������).
      // ����� ������� ���� �����������, ��� ��� ������� ���� � ��� ����� �����������,
      // � ���� ������������� ��������� ���� �� ����� ����������, �� �����
      // ������� ������.
    virtual signed int check (const unsigned long k, const std::vector<double>& x) const = 0;

      // ������� �������� - ����������� �� ������ ������������ ������� � ���������
	//virtual signed int enter (std::vector<int>& markup,const std::vector<double>& x, const unsigned long begin, const unsigned long end) const=0;
    
      // ��������� ������ ������� � �� ����������� ����������� � ������ �������� ���� �� ����.
      // ���������� 1, ���� ������ ������� �������� ������� ������� ������, 0 ���� �������
      // �� �������� ��� ������������, -1, ���� ������ �������� ������� ������� ������
    virtual double operator< (const ElemCondition& second) const = 0;
    
      // �������� �������: ��������� �����-�� ���������� ���������� ������� (��� ������ �
      // ��� �������� �� ����������� ������). p ���������� ������� ��������� (������ ����������
      // �� ������� [0,1]). ���������� 0 ���� ��������� �� ���������, -1, ���� ��������� �� ���������� (��������,
      // � ���� ������� ��� ����������), 1, ���� ��������� ���������
    virtual signed int transmute (const double p) = 0;
    
      // ������ ��������� ������������� �������
      // ���������� 0, ���� ��� ok, -1 �����
    virtual signed int setParams (const std::map<std::string,std::string>& newParams) = 0;
    
      // ������ ��������� ������������� �������
      // ���������� 0, ���� ��� ok, -1 �����
    virtual signed int getParams (std::map<std::string,std::string> &newParams) const = 0;
    
      // ������� ������������� �������� ������ ��������� ������������� �������,
      // ���� ����� �������� ������������ � ������ ������������ �������
   virtual signed int setParamValue (const double param, const std::string &paramName) = 0;
   
      // ������� ���������� �������� ������ ��������� ������������� �������,
      // ���� ����� �������� ������������ � ������ ������������ �������
   virtual signed int getParamValue (double &param, const std::string &paramName) const = 0;
   
      // ������� ��������� ������� ������ �������� � ���������� ���������� ������ ������������ �������
   virtual signed int getECParamNames (std::vector<std::string> &ecParamNames) const = 0;
     
      // �������� ������������� � ������ ������������ �������� �� ������ "�"
      // 0 - ���� �� ����������
      // 1 - ���� ����������
      // -1 - ���� undefined
    virtual signed int checkAndCompatibility (const ElemCondition& toCheck) const = 0;

      // ����������� ���� ������������ ������� (������ �� ���������)
    void crossEC(ElemCondition& second, const double crossProb, const double crossParam);

	  // ������� ������������� ������������ �������� ��� ������ left � right,
	  // ������� ���������� ����������� ������� ����� � ������� ����������� ������������ �������
	virtual signed int setLimits (const int leftLimit, const int rightLimit) = 0;

	  // ������� ������������� ������������ �������� ��� ���������� ������������� �������,
	  // �������� �� left � right
	virtual signed int setLimits (const std::vector<double>& row, const double reserve, const unsigned int numOfLevels) = 0;

	  // ������� �������� ��������� ������������� ������� � �������� �������� �� ����� �����,
	  // ��������� � ����������. ��������� �� ���� ��� - ��� ��������� �� ���� �������� delta.
	  // �������� delta ������������ ��� ��������� ������� ���������.
	virtual signed int changeParams (const int numOfSteps) = 0;

	// ������� ������������ - ���������� ������� "����������� ����������� �����������"
	virtual ElemCondition * clone() const = 0;

	// ������� ����������� ������� �� �� other � ���������� true, ���� � ��� ��������� ���
	// ����� ���������� false
	// � ���������� �� ����� �������������� ������� ��������,
	// �����, ��� new
	virtual bool assign(const ElemCondition* other) = 0;
	
}; // class ElemCondition

template<class Derived>
bool ElemCondition::assign_impl(Derived* first, const ElemCondition* second) {
	// ������� � ������ ����
	const Derived* secondDerived = dynamic_cast<const Derived*>(second);
	if(secondDerived == 0) {
		return false;
	}
	// ��������� ���������� �������� ��� ���� ������
	*first = *secondDerived;
	return true;	
}

}; // namespace AxiomLib

BOOST_SERIALIZATION_ASSUME_ABSTRACT(AxiomLib::ElemCondition);

#endif /* __AXIOMS_LIB_EL_SUPER_AXIOM_HXX */
