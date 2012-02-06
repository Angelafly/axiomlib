/****************************************************************************
*					TrajectorySampleDistance
*
*	Description:	��������� ��� �������� ���������� �� �������� ����������
*					�� ��������� ����������
*	Author:			wictor
*	History:	
*
****************************************************************************/
#ifndef TRAJECTORYDISTANCELABELING_H
#define TRAJECTORYDISTANCELABELING_H

#include <vector>

namespace AxiomLib {

class TrajectorySampleDistance
{
public:
	// �����������, � ������� �������� ����� ������� ���������� ���������
	// � ����� ����������
	explicit TrajectorySampleDistance(int numClasses, int length = 0);
	
	// ����� ����������
	int length() const;
	
	void setLength(int newValue);
	
	// ����� ������� ���������� ���������
	int numClasses() const;
	
	// ������ ���������� �� ������� ���������� � ������ � endPoint �� ��������� ���������� ������ classNo
	void setDistance(int classNo, int endPoint, double distance);
	
	void setDistancesForClass(int classNo, const std::vector<double>& distances);
	
	//void setDistancesForClass(int classNo, const std::vector<double>& distances);
	
	// ���������� �� ������� ���������� � ������ � endPoint �� ��������� ���������� ������ classNo
	double distance(int classNo, int endPoint) const;
	
private:
	std::vector<std::vector<double> > m_dist;
	int m_length;
	int m_nClasses;
};

}

#endif // TRAJECTORYDISTANCELABELING_H
