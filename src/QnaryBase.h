/****************************************************************************
*			QnaryBase.h
*
*	Description:	������� ����� ��� q-������� ����������� � q-������ �����. ����
*					� ��� �������� ��������� ����������� ����� � �������� Q
*	Author:		wictor
*	History:	
*
****************************************************************************/

#ifndef QNARY_BASE_H_
#define QNARY_BASE_H_

#include <boost/numeric/ublas/matrix.hpp>

#include "NeuralCommon.h"

namespace AxiomLib {

class QnaryBase {
public:
		// ��� �������� ������ �����, �������� ������� � ��������� �����
		typedef double Elem;

		// ��� ���� - �������
		typedef boost::numeric::ublas::matrix<Elem> Weight;
		// ��� ���� ����� ���� - ������� ������
		typedef boost::numeric::ublas::matrix<Weight> NetWeights;

		// ��� ������
		typedef boost::numeric::ublas::vector<Elem> Threshold;
		
		// ��� ���������� ���� - ��������� � ����� ������
		typedef Threshold LocalField;
		
		// ��� �������� ������
		typedef boost::numeric::ublas::zero_vector<Elem> ZeroThreshold;
		// ��� ���� ������� ����
		typedef boost::numeric::ublas::vector<Threshold> NetThresholds;

		// ����������� - � ��� �������� �������� q
		QnaryBase(int q = 0): q(q) {}
		
		void construct(int q_) { q = q_; }
		
		// ���������� �������� q
		int getQ() const { return q; }
		
protected:
		// �������� q
		int q;
};

};

#endif /* QNARY_BASE_H_ */
