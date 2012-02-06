/****************************************************************************
*						QnaryPercWrapper.h
*
*       Description:	���������� ������� ������ QnaryPercWrapper
*       Author:			wictor
*       History:
*
****************************************************************************/
#ifndef QNARYPERCWRAPPER_H_
#define QNARYPERCWRAPPER_H_

#include "NeuralCommon.h"
#include "QnaryPerc.h"
#include <vector>
#include <boost/foreach.hpp>

namespace AxiomLib {

/****************************************************************************
*						QnaryPercWrapper<Holder>
*
*       Description:	�����, ��������������� ���������������� q-������� �����������
*						� ������������ NetWrapper (��. NeuralConcepts.h)
*						Holder ������ ������������ ������� Holder
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class Holder>
class QnaryPercWrapper : public Holder {
	// ������ ���������
	QnaryPerc qp;
	public:
	//����������� ���������
	//QnaryPercWrapper() {}
	
	// ��������� ���������� ����
	// Settings ������ ����� ���� int qO
	template<class Settings>
	void construct(const Settings &settings);
	
	// �������� ���� (�� ����� �������� �������)
	void train();
	
	// ������ �������� ���������
	int run(const Qstring &str, double &p) const;
	
	// ������� (����� ��������� ��������������)
	void clear();
};

};

#include "QnaryPercWrapper.cxx"

#endif
