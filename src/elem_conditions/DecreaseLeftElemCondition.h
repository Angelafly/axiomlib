/****************************************************************************
*				DecreaseLeftElemCondition
*
*	Description:	������������ ������� �������� ����� � �������� ����
*	Author:		dk
*	History:
*
****************************************************************************/

#ifndef __AXIOMS_LIB_EL_DecreaseLeftElemCondition_HXX
#define __AXIOMS_LIB_EL_DecreaseLeftElemCondition_HXX

#include "ElemCondition.h"

namespace AxiomLib { 

class DecreaseLeftElemCondition : public ElemCondition {

  private:

	//����������, ����������� ��� ������������
	friend class boost::serialization::access;

	//������� ������������ - � ��� ������ ���� ������� �����-������,
	//��������������� ��������� �������
	template<class Archive>
	void serialize(Archive & archive, const unsigned int /*version*/)
	{
		archive & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ElemCondition);
		archive & BOOST_SERIALIZATION_NVP(decreaseLeft);
		archive & BOOST_SERIALIZATION_NVP(decreaseLeftLimit);
		archive & BOOST_SERIALIZATION_NVP(decreaseMaxLevel);
		archive & BOOST_SERIALIZATION_NVP(decreaseMaxLevelLimit);
		archive & BOOST_SERIALIZATION_NVP(decreaseMinLevel);
		archive & BOOST_SERIALIZATION_NVP(decreaseMinLevelLimit);
		archive & BOOST_SERIALIZATION_NVP(decreaseRight);
		archive & BOOST_SERIALIZATION_NVP(decreaseRightLimit);
		archive & BOOST_SERIALIZATION_NVP(levelDelta);
	}

	// ����������� ������� ��������� ��� ��������� decreaseMinLevel, ������� ����������� ��� ������� ���������� ���������
	double decreaseMinLevelLimit;
	
	// ������������� ������� ��������� ��� ��������� decreaseMaxLevel, ������� ����������� ��� ������� ���������� ���������
	double decreaseMaxLevelLimit;

	// ��� ��������� ���������� decreaseMinLevel � decreaseMaxLevel, ��� ������� ���������� ���������
	double levelDelta;
	
	// ������������ �������� ��������� decreaseLeft, ������� ����������� ��� ������� ���������� ���������
	int decreaseLeftLimit;
	
	// ������������ �������� ��������� decreaseRight, ������� ����������� ��� ������� ���������� ���������
	int decreaseRightLimit;

  protected:
	  
	// ����������� ����������� ���������� ������� ������������� �������
	double decreaseMinLevel;
	
	// ������������� ����������� ��������� ������� ������������� �������
	double decreaseMaxLevel;
	
	// ����� ����� ��������� ��� ������������� ������� - �����
	int decreaseLeft;
	
	// ����� ����� ��������� ��� ������������� ������� - ������
	int decreaseRight;
	
  public:
	
	// ������ �����������
	DecreaseLeftElemCondition(void);
	
	// ���������� - �������� ��� ��������� ������������ �������
	~DecreaseLeftElemCondition(void) {};
	
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

	// ������� ������������� ������������ �������� ���������� decreaseLeftLimit � decreaseRightLimit
	signed int setLimits (const int newLeftLimit, const int newRightLimit);

	// ������� ������������� �������� ���������� decreaseMinLevelLimit, decreaseMaxLevelLimit � levelDelta
	signed int setLimits (const std::vector<double>& row, const double reserve, const unsigned int numOfLevels);

	// ������� �������� ��������� ������������� ������� �� ����� �����, ��������� � ����������
	signed int changeParams (const int numOfSteps);

	// ������� ������������ - ���������� ������� "����������� ����������� �����������"
	DecreaseLeftElemCondition * clone() const { return new DecreaseLeftElemCondition(*this); }

	// ������� ����������� ������� �� �� other � ���������� true, ���� � ��� ��������� ���
	// ����� ���������� false
	bool assign(const ElemCondition *other) { return assign_impl(this, other); }
	
}; // of class

}; // of namespace



#endif /* __AXIOMS_LIB_EL_DecreaseLeftElemCondition_HXX */
