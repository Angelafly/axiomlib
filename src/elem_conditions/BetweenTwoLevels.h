/****************************************************************************
*				BetweenTwoLevels
*
*	Description:	������������ ������� ���������� ����� ����� ����� ��������
*	Author:		dk
*	History:
*
****************************************************************************/

#ifndef __AXIOMS_LIB_EL_BetweenTwoLevels_HXX
#define __AXIOMS_LIB_EL_BetweenTwoLevels_HXX

#include "ElemCondition.h"
#include <boost/serialization/nvp.hpp>

namespace AxiomLib { 

class BetweenTwoLevels : public ElemCondition {

  private:

	//����������, ����������� ��� ������������
	friend class boost::serialization::access;

	//������� ������������ - � ��� ������ ���� ������� �����-������,
	//��������������� ��������� �������
	template<class Archive>
	void serialize(Archive & archive, const unsigned int /*version*/)
	{
		archive & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ElemCondition);
		archive & BOOST_SERIALIZATION_NVP(minLimit);
		archive & BOOST_SERIALIZATION_NVP(maxLimit);
		archive & BOOST_SERIALIZATION_NVP(levelDelta);
		archive & BOOST_SERIALIZATION_NVP(leftLimit);
		archive & BOOST_SERIALIZATION_NVP(rightLimit);
		archive & BOOST_SERIALIZATION_NVP(Min);
		archive & BOOST_SERIALIZATION_NVP(Max);
		archive & BOOST_SERIALIZATION_NVP(Left);
		archive & BOOST_SERIALIZATION_NVP(Right);
	}

	// ����������� ������� ��������� ��� ��������� Min, ������� ����������� ��� ������� ���������� ���������
	double minLimit;
	
	// ����������� ������� ��������� ��� ��������� Max, ������� ����������� ��� ������� ���������� ���������
	double maxLimit;

	// ��� ��������� ���������� minLevel � maxLevel, ��� ������� ���������� ���������
	double levelDelta;
	
	// ������������ �������� ��������� left, ������� ����������� ��� ������� ���������� ���������
	int leftLimit;
	
	// ������������ �������� ��������� right, ������� ����������� ��� ������� ���������� ���������
	int rightLimit;

  protected:
	  
	// ����������� ����������� ���������� ������� ������������� �������
	double Min;
	
	// ������������� ����������� ��������� ������� ������������� �������
	double Max;
	
	// ����� ����� ��������� ��� ������������� ������� - �����
	int Left;
	
	// ����� ����� ��������� ��� ������������� ������� - ������
	int Right;
	
  public:
	
	// ������ �����������
	BetweenTwoLevels(void);
	
	// ���������� - �������� ��� ��������� ������������ �������
	~BetweenTwoLevels(void) {};
	
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
		
	// ������� ������������� ������������ �������� ���������� leftLimit � rightLimit
	signed int setLimits (const int newLeftLimit, const int newRightLimit);

	// ������� ������������� �������� ���������� minLevelLimit, maxLevelLimit � levelDelta
	signed int setLimits (const std::vector<double>& row, const double reserve, const unsigned int numOfLevels);

	// ������� �������� ��������� ������������� ������� �� ����� �����, ��������� � ����������
	signed int changeParams (const int numOfSteps);
	
	// ������� ������������ - ���������� ������� "����������� ����������� �����������"
	BetweenTwoLevels * clone() const { return new BetweenTwoLevels(*this); }
	
	// ������� ����������� ������� �� �� other � ���������� true, ���� � ��� ��������� ���
	// ����� ���������� false
	bool assign(const ElemCondition *other) { return assign_impl(this, other); }

}; // of class

}; // of namespace



#endif /* __AXIOMS_LIB_EL_BetweenTwoLevels_HXX */
