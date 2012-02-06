/****************************************************************************
*				DerivativeDecreaseRight
*
*	Description:	������������ ������� �������� ������
*	Author:		dk
*	History:
*
****************************************************************************/

#ifndef __AXIOMS_LIB_EL_DerivativeDecreaseRight_HXX
#define __AXIOMS_LIB_EL_DerivativeDecreaseRight_HXX

#include "ElemCondition.h"

namespace AxiomLib { 

class DerivativeDecreaseRight : public ElemCondition {

  private:
	//����������, ����������� ��� ������������
	friend class boost::serialization::access;

	//������� ������������ - � ��� ������ ���� ������� �����-������,
	//��������������� ��������� �������
	template<class Archive>
	void serialize(Archive & archive, const unsigned int /*version*/)
	{
		archive & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ElemCondition);
		archive & BOOST_SERIALIZATION_NVP(derivDecreaseLeft);
		archive & BOOST_SERIALIZATION_NVP(derivDecreaseLeftLimit);
		archive & BOOST_SERIALIZATION_NVP(derivDecreaseMaxLevel);
		archive & BOOST_SERIALIZATION_NVP(derivDecreaseMaxLevelLimit);
		archive & BOOST_SERIALIZATION_NVP(derivDecreaseMinLevel);
		archive & BOOST_SERIALIZATION_NVP(derivDecreaseMinLevelLimit);
		archive & BOOST_SERIALIZATION_NVP(derivDecreaseRight);
		archive & BOOST_SERIALIZATION_NVP(derivDecreaseRightLimit);
		archive & BOOST_SERIALIZATION_NVP(levelDelta);
	}

	// ����������� ������� ��������� ��� ��������� derivDecreaseMinLevel, ������� ����������� ��� ������� ���������� ���������
	double derivDecreaseMinLevelLimit;
	
	// ������������� ������� ��������� ��� ��������� derivDecreaseMaxLevel, ������� ����������� ��� ������� ���������� ���������
	double derivDecreaseMaxLevelLimit;

	// ��� ��������� ���������� derivDecreaseMinLevel � derivDecreaseMaxLevel, ��� ������� ���������� ���������
	double levelDelta;
	
	// ������������ �������� ��������� derivDecreaseLeft, ������� ����������� ��� ������� ���������� ���������
	int derivDecreaseLeftLimit;
	
	// ������������ �������� ��������� derivDecreaseRight, ������� ����������� ��� ������� ���������� ���������
	int derivDecreaseRightLimit;

  protected:
	  
	// ����������� ����������� ���������� ������� ������������� �������
	double derivDecreaseMinLevel;
	
	// ������������� ����������� ��������� ������� ������������� �������
	double derivDecreaseMaxLevel;
	
	// ����� ����� ��������� ��� ������������� ������� - �����
	int derivDecreaseLeft;
	
	// ����� ����� ��������� ��� ������������� ������� - ������
	int derivDecreaseRight;
	
  public:
	
	// ������ �����������
	DerivativeDecreaseRight(void);
	
	// ���������� - �������� ��� ��������� ������������ �������
	~DerivativeDecreaseRight(void) {};
	
	// ������� ���������� ��� ������� ������������� �������
	std::string name(void) const;
	
	// ������� �������� - ����������� �� ������ ������������ ������� � ����� ����
	inline signed int check (const unsigned long k, const std::vector<double>& x) const;

	// �������� ��������� ��� ������� ������������� �������
	double operator< (const ElemCondition& second) const;
	
	// �������� ������� ������������� �������
	signed int transmute (const double p);
	
	// �������� ������� ���������� ������� ������������� �������
	signed int setParams (const std::map<std::string,std::string>& newParams);
	
	// ������� ���������� ������� ������������ ���������
	signed int getParams (std::map<std::string,std::string> &params) const;
	
	// ������� ������������� �������� ������ ��������� ������������� �������, ���� ����� �������� ������������ � ������ ������������ �������
	signed int setParamValue (const double param, const std::string &paramName);
	
	// ������� ���������� �������� ������ ��������� ������������� �������, ���� ����� �������� ������������ � ������ ������������ �������
	signed int getParamValue (double &param, const std::string &paramName) const;
	
	// ������� ��������� ������� ������ �������� � ���������� ���������� ������ ������������ �������
	signed int getECParamNames (std::vector<std::string> &ecParamNames) const;
	
	// �������� �������� ������������� ������������ ������� � ���������� � ����������
	signed int checkAndCompatibility (const ElemCondition& toCheck) const;

	// ������� ������������� ������������ �������� ���������� derivDecreaseLeftLimit � derivDecreaseRightLimit
	signed int setLimits (const int newLeftLimit, const int newRightLimit);

	// ������� ������������� �������� ���������� derivDecreaseMinLevelLimit, derivDecreaseMaxLevelLimit � levelDelta
	signed int setLimits (const std::vector<double>& row, const double reserve, const unsigned int numOfLevels);

	// ������� �������� ��������� ������������� ������� �� ����� �����, ��������� � ����������
	signed int changeParams (const int numOfSteps);
	
	// ������� ������������ - ���������� ������� "����������� ����������� �����������"
	DerivativeDecreaseRight* clone() const { return new DerivativeDecreaseRight(*this); }

	// ������� ����������� ������� �� �� other � ���������� true, ���� � ��� ��������� ���
	// ����� ���������� false
	bool assign(const ElemCondition *other) { return assign_impl(this, other); }
	
}; // of class

}; // of namespace



#endif /* __AXIOMS_LIB_EL_DerivativeDecreaseRight_HXX */
