/****************************************************************************
*				SecDerivativeDecreaseLeft
*
*	Description:	������������ ������� �������� ����������� ������
*	Author:		dk
*	History:
*
****************************************************************************/

#ifndef __AXIOMS_LIB_EL_SecDerivativeDecreaseLeft_HXX
#define __AXIOMS_LIB_EL_SecDerivativeDecreaseLeft_HXX

#include "ElemCondition.h"

namespace AxiomLib { 

class SecDerivativeDecreaseLeft : public ElemCondition {

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
		archive & BOOST_SERIALIZATION_NVP(secDerivDecreaseLeft);
		archive & BOOST_SERIALIZATION_NVP(secDerivDecreaseLeftLimit);
		archive & BOOST_SERIALIZATION_NVP(secDerivDecreaseMaxLevel);
		archive & BOOST_SERIALIZATION_NVP(secDerivDecreaseMaxLevelLimit);
		archive & BOOST_SERIALIZATION_NVP(secDerivDecreaseMinLevel);
		archive & BOOST_SERIALIZATION_NVP(secDerivDecreaseMinLevelLimit);
		archive & BOOST_SERIALIZATION_NVP(secDerivDecreaseRight);
		archive & BOOST_SERIALIZATION_NVP(secDerivDecreaseRightLimit);
	}

	// ����������� ������� ��������� ��� ��������� secDerivDecreaseMinLevel, ������� ����������� ��� ������� ���������� ���������
	double secDerivDecreaseMinLevelLimit;

	// ������������� ������� ��������� ��� ��������� secDerivDecreaseMaxLevel, ������� ����������� ��� ������� ���������� ���������
	double secDerivDecreaseMaxLevelLimit;

	// ��� ��������� ���������� secDerivDecreaseMinLevel � secDerivDecreaseMaxLevel, ��� ������� ���������� ���������
	double levelDelta;

	// ������������ �������� ��������� secDerivDecreaseLeft, ������� ����������� ��� ������� ���������� ���������
	int secDerivDecreaseLeftLimit;

	// ������������ �������� ��������� secDerivDecreaseRight, ������� ����������� ��� ������� ���������� ���������
	int secDerivDecreaseRightLimit;

  protected:
	  
	// ����������� ����������� ���������� ������� ������������� �������
	double secDerivDecreaseMinLevel;
	
	// ������������ ����������� ��������� ������� ������������� �������
	double secDerivDecreaseMaxLevel;
	
	// ����� ����� ��������� ��� ������������� ������� - �����
	int secDerivDecreaseLeft;
	
	// ����� ����� ��������� ��� ������������� ������� - ������
	int secDerivDecreaseRight;
	
  public:
	
	// ������ �����������
	SecDerivativeDecreaseLeft(void);
	
	// ���������� - �������� ��� ��������� ������������ �������
	~SecDerivativeDecreaseLeft(void) {};
	
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

	// ������� ������������� ������������ �������� ���������� secDerivDecreaseLeftLimit � secDerivDecreaseRightLimit
	signed int setLimits (const int newLeftLimit, const int newRightLimit);

	// ������� ������������� �������� ���������� secDerivDecreaseMinLevelLimit, secDerivDecreaseMaxLevelLimit � levelDelta
	signed int setLimits (const std::vector<double>& row, const double reserve, const unsigned int numOfLevels);

	// ������� �������� ��������� ������������� ������� �� ����� �����, ��������� � ����������
	signed int changeParams (const int numOfSteps);
	
	// ������� ������������ - ���������� ������� "����������� ����������� �����������"
	SecDerivativeDecreaseLeft* clone() const { return new SecDerivativeDecreaseLeft(*this); }

	// ������� ����������� ������� �� �� other � ���������� true, ���� � ��� ��������� ���
	// ����� ���������� false
	bool assign(const ElemCondition *other) { return assign_impl(this, other); }
	
}; // of class

}; // of namespace



#endif /* __AXIOMS_LIB_EL_SecDerivativeDecreaseLeft_HXX */
