#ifndef ELEMCONDMEAN_H
#define ELEMCONDMEAN_H

#include "../elem_conditions/ElemCondition.h"

namespace AxiomLib {

class ElemCondMean : public ElemCondition
{

	//����������, ����������� ��� ������������
	friend class boost::serialization::access;

	//������� ������������ - � ��� ������ ���� ������� �����-������,
	//��������������� ��������� �������
	template<class Archive>
	void serialize(Archive & archive, const unsigned int /*version*/)
	{
		archive & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ElemCondition);
		archive & BOOST_SERIALIZATION_NVP(left);
		archive & BOOST_SERIALIZATION_NVP(right);
		archive & BOOST_SERIALIZATION_NVP(minLevel);
		archive & BOOST_SERIALIZATION_NVP(maxLevel);
	}

public:
	ElemCondMean();

	ElemCondMean(int left, int right, double minLevel, double maxLevel);

	// ���������� ��� ������ �������.
  virtual std::string name (void) const;

	// �������� ���������� ������ �������. �� ����� - ����� ���������� ���� k
	// � ��� ��������� ��� x. �� ������ 1, ���� �����������, 0 ���� ��
	// ����������� � -1, ���� �� ��������� (����� ������� �� ������� ����
	// ����� ��� ������).
	// ����� ������� ���� �����������, ��� ��� ������� ���� � ��� ����� �����������,
	// � ���� ������������� ��������� ���� �� ����� ����������, �� �����
	// ������� ������.
  virtual signed int check (const unsigned long k, const std::vector<double>& x) const;

	// ������� �������� - ����������� �� ������ ������������ ������� � ���������
  //virtual signed int enter (std::vector<int>& markup,const std::vector<double>& x, const unsigned long begin, const unsigned long end) const=0;

	// ��������� ������ ������� � �� ����������� ����������� � ������ �������� ���� �� ����.
	// ���������� 1, ���� ������ ������� �������� ������� ������� ������, 0 ���� �������
	// �� �������� ��� ������������, -1, ���� ������ �������� ������� ������� ������
  virtual double operator< (const ElemCondition& second) const { return 0; }

	// �������� �������: ��������� �����-�� ���������� ���������� ������� (��� ������ �
	// ��� �������� �� ����������� ������). p ���������� ������� ��������� (������ ����������
	// �� ������� [0,1]). ���������� 0 ���� ��������� �� ���������, -1, ���� ��������� �� ���������� (��������,
	// � ���� ������� ��� ����������), 1, ���� ��������� ���������
  virtual signed int transmute (const double p) { return -1; }

	// ������ ��������� ������������� �������
	// ���������� 0, ���� ��� ok, -1 �����
  virtual signed int setParams (const std::map<std::string,std::string>& newParams) { return -1; /*TODO*/ }

	// ������ ��������� ������������� �������
	// ���������� 0, ���� ��� ok, -1 �����
  virtual signed int getParams (std::map<std::string,std::string> &newParams) const;

	// ������� ������������� �������� ������ ��������� ������������� �������,
	// ���� ����� �������� ������������ � ������ ������������ �������
 virtual signed int setParamValue (const double param, const std::string &paramName) { return -1; /*TODO*/ }

	// ������� ���������� �������� ������ ��������� ������������� �������,
	// ���� ����� �������� ������������ � ������ ������������ �������
 virtual signed int getParamValue (double &param, const std::string &paramName) const { return -1; /*TODO*/ }

	// ������� ��������� ������� ������ �������� � ���������� ���������� ������ ������������ �������
 virtual signed int getECParamNames (std::vector<std::string> &ecParamNames) const { return -1; /*TODO*/ }

	// �������� ������������� � ������ ������������ �������� �� ������ "�"
	// 0 - ���� �� ����������
	// 1 - ���� ����������
	// -1 - ���� undefined
  virtual signed int checkAndCompatibility (const ElemCondition& toCheck) const { return -1; }

	// ����������� ���� ������������ ������� (������ �� ���������)
  void crossEC(ElemCondition& second, const double crossProb, const double crossParam) {}

	// ������� ������������� ������������ �������� ��� ������ left � right,
	// ������� ���������� ����������� ������� ����� � ������� ����������� ������������ �������
  virtual signed int setLimits (const int leftLimit, const int rightLimit) { return -1; /*TODO*/ }

	// ������� ������������� ������������ �������� ��� ���������� ������������� �������,
	// �������� �� left � right
  virtual signed int setLimits (const std::vector<double>& row, const double reserve, const unsigned int numOfLevels) { return -1; /*TODO*/ }

	// ������� �������� ��������� ������������� ������� � �������� �������� �� ����� �����,
	// ��������� � ����������. ��������� �� ���� ��� - ��� ��������� �� ���� �������� delta.
	// �������� delta ������������ ��� ��������� ������� ���������.
  virtual signed int changeParams (const int numOfSteps) { return -1; /*TODO*/ }

  // ������� ������������ - ���������� ������� "����������� ����������� �����������"
  virtual ElemCondition * clone() const { return new ElemCondMean(*this); }

  // ������� ����������� ������� �� �� other � ���������� true, ���� � ��� ��������� ���
  // ����� ���������� false
  // � ���������� �� ����� �������������� ������� ��������,
  // �����, ��� new
  virtual bool assign(const ElemCondition* other) { return assign_impl(this, other); }

private:
	int left, right;
	double minLevel, maxLevel;
};

}

#endif // ELEMCONDMEAN_H
