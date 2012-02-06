/****************************************************************************
*				IncreaseRightElemCondition
*
*	Description:	������������ ������� ����������� ������ � �������� ����
*	Author:		dk
*	History:
*
****************************************************************************/

#ifndef __AXIOMS_LIB_EL_IncreaseRightElemCondition_HXX
#define __AXIOMS_LIB_EL_IncreaseRightElemCondition_HXX

#include "ElemCondition.h"

namespace AxiomLib { 

class IncreaseRightElemCondition : public ElemCondition {

  private:
	//����������, ����������� ��� ������������
	friend class boost::serialization::access;

	//������� ������������ - � ��� ������ ���� ������� �����-������,
	//��������������� ��������� �������
	template<class Archive>
	void serialize(Archive & archive, const unsigned int /*version*/)
	{
		archive & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ElemCondition);
		archive & BOOST_SERIALIZATION_NVP(increaseLeft);
		archive & BOOST_SERIALIZATION_NVP(increaseLeftLimit);
		archive & BOOST_SERIALIZATION_NVP(increaseMaxLevel);
		archive & BOOST_SERIALIZATION_NVP(increaseMaxLevelLimit);
		archive & BOOST_SERIALIZATION_NVP(increaseMinLevel);
		archive & BOOST_SERIALIZATION_NVP(increaseMinLevelLimit);
		archive & BOOST_SERIALIZATION_NVP(increaseRight);
		archive & BOOST_SERIALIZATION_NVP(increaseRightLimit);
		archive & BOOST_SERIALIZATION_NVP(levelDelta);
	}

	// ����������� ������� ��������� ��� ��������� increaseMinLevel, ������� ����������� ��� ������� ���������� ���������
	double increaseMinLevelLimit;
	
	// ������������� ������� ��������� ��� ��������� increaseMaxLevel, ������� ����������� ��� ������� ���������� ���������
	double increaseMaxLevelLimit;

	// ��� ��������� ���������� increaseMinLevel � increaseMaxLevel, ��� ������� ���������� ���������
	double levelDelta;
	
	// ������������ �������� ��������� increaseLeft, ������� ����������� ��� ������� ���������� ���������
	int increaseLeftLimit;
	
	// ������������ �������� ��������� increaseRight, ������� ����������� ��� ������� ���������� ���������
	int increaseRightLimit;

  protected:
	  
	// ����������� ����������� ���������� ������� ������������� �������
	double increaseMinLevel;
	
	// ������������� ����������� ��������� ������� ������������� �������
	double increaseMaxLevel;
	
	// ����� ����� ��������� ��� ������������� ������� - �����
	int increaseLeft;
	
	// ����� ����� ��������� ��� ������������� ������� - ������
	int increaseRight;
	
  public:
	
	// ������ �����������
	IncreaseRightElemCondition(void);
	
	// ���������� - �������� ��� ��������� ������������ �������
	~IncreaseRightElemCondition(void) {};
	
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

	// ������� ������������� ������������ �������� ���������� increaseLeftLimit � increaseRightLimit
	signed int setLimits (const int newLeftLimit, const int newRightLimit);

	// ������� ������������� �������� ���������� increaseMinLevelLimit, increaseMaxLevelLimit � levelDelta
	signed int setLimits (const std::vector<double>& row, const double reserve, const unsigned int numOfLevels);

	// ������� �������� ��������� ������������� ������� �� ����� �����, ��������� � ����������
	signed int changeParams (const int numOfSteps);
	
	// ������� ������������ - ���������� ������� "����������� ����������� �����������"
	IncreaseRightElemCondition* clone() const { return new IncreaseRightElemCondition(*this); }

	// ������� ����������� ������� �� �� other � ���������� true, ���� � ��� ��������� ���
	// ����� ���������� false
	bool assign(const ElemCondition *other) { return assign_impl(this, other); }
	
}; // of class

}; // of namespace



#endif /* __AXIOMS_LIB_EL_IncreaseRightElemCondition_HXX */
