/****************************************************************************
*			QnaryPerc.h
*
*	Description: �������� ������ QnaryPerc, ������������ q-������ ����������
*	Author:		wictor
*	History:	
*
****************************************************************************/

#ifndef QNARYPERC_H_
#define QNARYPERC_H_

#include "QnaryBase.h"


namespace AxiomLib {

class QnaryPerc: public QnaryBase
{
	// ����� ��������� ��������� ������� (����� ��������� �������� ������� �������� � ������� ������)
	int qo;
	
	// ����� ������� ��������
	int N;
	
	// ����� �������� ��������
	int n;
	
	// ��������� ���� ����� ���� (������� ������)
	NetWeights netWeights;
	
	// ��������� ���� ������� ���� (������ ��������)
	NetThresholds netThresholds;

	// ���������� ������ ������ ��������� �������
	void calcOutputNeuron(int neuronNo, double &p, const Qstring &inp, Qstring &outp) const;
	
	// ���������� ������ ���������
	void calcOutput(double &p, const Qstring &inp, Qstring &outp) const;
	
	// ���� ����, ��� ���� ����������������
	bool valid;

public:

	// ����������� ��������� - �������������� ��������� ��������
	QnaryPerc(): valid(false) {}

	// ������������� ����
	void construct(int qi_, int qo_, int N_, int n_);
	
	// �������� ����
	void train(const Qstring &inp, const Qstring &outp);
	
	// ������ ����
	Qstring run(const Qstring &inp, double &p) const;
	
	// ���������� ����� ��������� ��������� �������
	int getQo() const { return qo; }

};

};

#endif /* QNARYPERC_H_ */
