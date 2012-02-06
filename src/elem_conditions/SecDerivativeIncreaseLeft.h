/****************************************************************************
*				SecDerivativeIncreaseLeft
*
*	Description:	������������ ������� ����������� ����������� �����
*	Author:		dk
*	History:
*
****************************************************************************/

#ifndef __AXIOMS_LIB_EL_SecDerivativeIncreaseLeft_HXX
#define __AXIOMS_LIB_EL_SecDerivativeIncreaseLeft_HXX

#include "ElemCondition.h"

namespace AxiomLib { 

class SecDerivativeIncreaseLeft : public ElemCondition {

  private:
	//����������, ����������� ��� ������������
	friend class boost::serialization::access;

	//������� ������������ - � ��� ������ ���� ������� �����-������,
	//��������������� ��������� �������
	template<class Archive>
	void serialize(Archive & archive, const unsigned int /*version*/)
	{
		archive & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ElemCondition);
		archive & BOOST_SERIALIZATION_NVP(levelDelta);
		archive & BOOST_SERIALIZATION_NVP(secDerivIncreaseLeft);
		archive & BOOST_SERIALIZATION_NVP(secDerivIncreaseLeftLimit);
		archive & BOOST_SERIALIZATION_NVP(secDerivIncreaseMaxLevel);
		archive & BOOST_SERIALIZATION_NVP(secDerivIncreaseMaxLevelLimit);
		archive & BOOST_SERIALIZATION_NVP(secDerivIncreaseMinLevel);
		archive & BOOST_SERIALIZATION_NVP(secDerivIncreaseMinLevelLimit);
		archive & BOOST_SERIALIZATION_NVP(secDerivIncreaseRight);
		archive & BOOST_SERIALIZATION_NVP(secDerivIncreaseRightLimit);
	}

	// ����������� ������� ��������� ��� ��������� secDerivIncreaseMinLevel, ������� ����������� ��� ������� ���������� ���������
	double secDerivIncreaseMinLevelLimit;

	// ������������� ������� ��������� ��� ��������� secDerivIncreaseMaxLevel, ������� ����������� ��� ������� ���������� ���������
	double secDerivIncreaseMaxLevelLimit;

	// ��� ��������� ���������� secDerivIncreaseMinLevel � secDerivIncreaseMaxLevel, ��� ������� ���������� ���������
	double levelDelta;

	// ������������ �������� ��������� secDerivIncreaseLeft, ������� ����������� ��� ������� ���������� ���������
	int secDerivIncreaseLeftLimit;

	// ������������ �������� ��������� secDerivIncreaseRight, ������� ����������� ��� ������� ���������� ���������
	int secDerivIncreaseRightLimit;

  protected:
	  
	// ����������� ����������� ���������� ������� ������������� �������
	double secDerivIncreaseMinLevel;
	
	// ������������ ����������� ��������� ������� ������������� �������
	double secDerivIncreaseMaxLevel;
	
	// ����� ����� ��������� ��� ������������� ������� - �����
	int secDerivIncreaseLeft;
	
	// ����� ����� ��������� ��� ������������� ������� - ������
	int secDerivIncreaseRight;
	
  public:
	
	// ������ �����������
	SecDerivativeIncreaseLeft(void);
	
	// ���������� - �������� ��� ��������� ������������ �������
	~SecDerivativeIncreaseLeft(void) {};
	
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

	// ������� ������������� ������������ �������� ���������� secDerivIncreaseLeftLimit � secDerivIncreaseRightLimit
	signed int setLimits (const int newLeftLimit, const int newRightLimit);

	// ������� ������������� �������� ���������� secDerivIncreaseMinLevelLimit, secDerivIncreaseMaxLevelLimit � levelDelta
	signed int setLimits (const std::vector<double>& row, const double reserve, const unsigned int numOfLevels);

	// ������� �������� ��������� ������������� ������� �� ����� �����, ��������� � ����������
	signed int changeParams (const int numOfSteps);
	
	// ������� ������������ - ���������� ������� "����������� ����������� �����������"
	SecDerivativeIncreaseLeft* clone() const { return new SecDerivativeIncreaseLeft(*this); }

	// ������� ����������� ������� �� �� other � ���������� true, ���� � ��� ��������� ���
	// ����� ���������� false
	bool assign(const ElemCondition *other) { return assign_impl(this, other); }
	
}; // of class

}; // of namespace



#endif /* __AXIOMS_LIB_EL_SecDerivativeIncreaseLeft_HXX */
