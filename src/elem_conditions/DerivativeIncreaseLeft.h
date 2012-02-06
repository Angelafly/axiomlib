/****************************************************************************
*				DerivativeIncreaseLeft
*
*	Description:	������������ ������� ����������� �����
*	Author:		dk
*	History:
*
****************************************************************************/

#ifndef __AXIOMS_LIB_EL_DerivativeIncreaseLeft_HXX
#define __AXIOMS_LIB_EL_DerivativeIncreaseLeft_HXX

#include "ElemCondition.h"

namespace AxiomLib { 

class DerivativeIncreaseLeft : public ElemCondition {

  private:
	//����������, ����������� ��� ������������
	friend class boost::serialization::access;

	//������� ������������ - � ��� ������ ���� ������� �����-������,
	//��������������� ��������� �������
	template<class Archive>
	void serialize(Archive & archive, const unsigned int /*version*/)
	{
		archive & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ElemCondition);
		archive & BOOST_SERIALIZATION_NVP(derivIncreaseLeft);
		archive & BOOST_SERIALIZATION_NVP(derivIncreaseLeftLimit);
		archive & BOOST_SERIALIZATION_NVP(derivIncreaseMaxLevel);
		archive & BOOST_SERIALIZATION_NVP(derivIncreaseMaxLevelLimit);
		archive & BOOST_SERIALIZATION_NVP(derivIncreaseMinLevel);
		archive & BOOST_SERIALIZATION_NVP(derivIncreaseMinLevelLimit);
		archive & BOOST_SERIALIZATION_NVP(derivIncreaseRight);
		archive & BOOST_SERIALIZATION_NVP(derivIncreaseRightLimit);
		archive & BOOST_SERIALIZATION_NVP(levelDelta);
	}

	// ����������� ������� ��������� ��� ��������� derivIncreaseMinLevel, ������� ����������� ��� ������� ���������� ���������
	double derivIncreaseMinLevelLimit;
	
	// ������������� ������� ��������� ��� ��������� derivIncreaseMaxLevel, ������� ����������� ��� ������� ���������� ���������
	double derivIncreaseMaxLevelLimit;

	// ��� ��������� ���������� derivIncreaseMinLevel � derivIncreaseMaxLevel, ��� ������� ���������� ���������
	double levelDelta;
	
	// ������������ �������� ��������� derivIncreaseLeft, ������� ����������� ��� ������� ���������� ���������
	int derivIncreaseLeftLimit;
	
	// ������������ �������� ��������� derivIncreaseRight, ������� ����������� ��� ������� ���������� ���������
	int derivIncreaseRightLimit;

  protected:
	  
	// ����������� ����������� ���������� ������� ������������� �������
	double derivIncreaseMinLevel;
	
	// ������������� ����������� ��������� ������� ������������� �������
	double derivIncreaseMaxLevel;
	
	// ����� ����� ��������� ��� ������������� ������� - �����
	int derivIncreaseLeft;
	
	// ����� ����� ��������� ��� ������������� ������� - ������
	int derivIncreaseRight;
	
  public:
	
	// ������ �����������
	DerivativeIncreaseLeft(void);
	
	// ���������� - �������� ��� ��������� ������������ �������
	~DerivativeIncreaseLeft(void) {};
	
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

	// ������� ������������� ������������ �������� ���������� derivIncreaseLeftLimit � derivIncreaseRightLimit
	signed int setLimits (const int newLeftLimit, const int newRightLimit);

	// ������� ������������� �������� ���������� derivIncreaseMinLevelLimit, derivIncreaseMaxLevelLimit � levelDelta
	signed int setLimits (const std::vector<double>& row, const double reserve, const unsigned int numOfLevels);

	// ������� �������� ��������� ������������� ������� �� ����� �����, ��������� � ����������
	signed int changeParams (const int numOfSteps);
	
	// ������� ������������ - ���������� ������� "����������� ����������� �����������"
	DerivativeIncreaseLeft* clone() const { return new DerivativeIncreaseLeft(*this); }

	// ������� ����������� ������� �� �� other � ���������� true, ���� � ��� ��������� ���
	// ����� ���������� false
	bool assign(const ElemCondition *other) { return assign_impl(this, other); }
	
}; // of class

}; // of namespace



#endif /* __AXIOMS_LIB_EL_DerivativeIncreaseLeft_HXX */
