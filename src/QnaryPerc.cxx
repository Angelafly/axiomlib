
#include "QnaryPerc.h"
#include "AxiomLibException.h"
using namespace AxiomLib;

#include <cmath>

/****************************************************************************
*					QnaryPerc::calcOutputNeuron
*
*	Description:	��������� ����� ������ ��������� ������� � ������� neuronNo
*					 � ���������� ��������� � outp[neuronNo]
*	Parameters:	int neuronNo - ����� �������
*				 double &p - ���� ������������ ������������ ���������� ���� �������
*				const Qstring &inp - ���� ����
*				Qstring &outp - ����� ����, ���������� �������� � ������� neuronNo ����������� � ������ �������
*	Returns:	-
*	Throws:		AxiomLibException, ���� neuronNo ����� ������������ �������� (������ ����. ������ ������� ��� ������ 0)
*	Author:		wictor
*	History:
*
****************************************************************************/
void QnaryPerc::calcOutputNeuron(int neuronNo, double &p, const Qstring &inp, Qstring &outp) const
{
	//cout<<"calcOutputNeuron()"<<endl;
	// �������� ����, ��� ����� ��������� ������� ���������
	if(neuronNo >= n || neuronNo < 0) throw AxiomLibException("QnaryPerc: bad parameters in calcOutputNeuron()");
	
	// h - ��������� ����, ����������� �� ������ ������. ���� �������������� ��� ��������� ������ ������� �������
	LocalField h(netThresholds(neuronNo));
	// ���������� ������ ����� - �� ����� ���� ������ ����� ������� ��������
	int actualN = inp.size();
	// �������� �� ����� ����
	for(int j=0; j<actualN; j++)
	{
		// netWeights(j, j) - ������� �������, ������� ���� �� ��������� ��������
		if(j==neuronNo) continue;
		
		// ���������� � h �������� ������������ netWeights(neuronNo, j) �� ���, ��������������� j-�� �������� �������
		// ����� ����������� (�� ����� ��������) ������: h+=prod(netWeights(neuronNo, j), input(j));
		// ����� ����������� ������:
		for (int i=0; i<qo; i++) {
			h(i)+=netWeights(neuronNo, j)(i, inp(j));
		}
	}
	
	int index_max=0;
	
	//������� ������ ������������� �������� h
	p = h(index_max);
	for(int i = 1; i < qo; i++)
	{
		double v = h(i);
		if(v > h(index_max))
		{
			index_max=i;
			p = v;
		}
	}
	
	// ����������� �������� ���������� ������� ��������� ������� �������
	outp(neuronNo) = index_max;

	//cout<<"calcOutputNeuron() end"<<endl;
}

/****************************************************************************
*					QnaryPerc::calcOutput
*
*	Description:	��������� ������ ���� �������� � ���������� ��������� � outp
*	Parameters:	double &p - ���� ������������ ������������ ���������� ���� ���������� ������� (TODO: ������� ���-������ ���)
*				const Qstring &inp - ���� ����
*				Qstring &outp - ����������� � ������ ������� ����� ����
*	Returns:	-
*	Throws:		-
*	Author:		wictor
*	History:
*
****************************************************************************/
void QnaryPerc::calcOutput(double &p, const Qstring &inp, Qstring &outp) const
{
	//�������� �� ���� �������� ��������
	for(int i=0; i<n; i++)
	{
		calcOutputNeuron(i, p, inp, outp);
	}
}

/****************************************************************************
*					QnaryPerc::construct
*
*	Description:	������ (��������������) ���� (� �.�. �������� ������ ��� ���� � ������) - ��� ������� ����� �������� ����� ������� � �����
*	Parameters:	int qi_ - ����� ��������� �������� �������
*				int qo_ - ����� ��������� ��������� �������
*				int N_ - ����� ������� ��������
*				int n_ - ����� �������� ��������
*	Returns:	-
*	Throws:		AxiomLibException, ���� qi_<2 || qo_<2 || n_<1 || N_<1
*	Author:		wictor
*	History:
*
****************************************************************************/
void QnaryPerc::construct(int qi_, int qo_, int N_, int n_) {
	// ������������� ����� ������
	q = qi_;
	qo = qo_;
	n = n_;
	N = N_;
	
	// ��������
	if(q<2 || qo<2 || n<1 || N<1) {
		throw AxiomLibException("QnaryPerc: bad parameters in construct()");
	}
	
	// ������������� ����� ������
	netWeights.resize(n, N);
	netThresholds.resize(n);
	valid = true;
	
	// ������������� ������ ����� � �������� �������
	for(int i=0; i<n; i++)
	{
		for(int j=0; j<N; j++)
		{
			// qi==q
			netWeights(i, j).resize(qo, q);
			netWeights(i, j)=boost::numeric::ublas::zero_matrix<Elem>(qo,q);
		}
		//netThresholds(i).resize(q);
		netThresholds(i)=ZeroThreshold(qo);
	}	
}

/****************************************************************************
*					QnaryPerc::train
*
*	Description:	������� ���� (�� ������� �����) ������������� �������� ������� ���������
*				����� �������� ������� ������ �� �������� ��� ������������ �������
*	Parameters:	const Qstring &inp - ������� ��������� ������
*				const Qstring &outp - �������� ��������� ������
*	Returns:	-
*	Throws:		AxiomLibException, ���� ������ �������� ������� ������ ������������� ������� �������� �������,
*				���� ������ ��������� ������� �� ����� ����������� � construct()
*	Author:		wictor
*	History:
*
****************************************************************************/
void QnaryPerc::train(const Qstring &inp, const Qstring &outp)
{
	
	NetThresholds thres(n);
	
	// ���������� ������ �����
	int actualN = inp.size();
	
	// ��������
	if(actualN > N) {
		throw AxiomLibException("QnaryPerc::train(): incorrect inp size");
	}
	if(outp.size() != n) {
		throw AxiomLibException("QnaryPerc::train(): incorrect outp size");
	}
	// ��������� ������� �����
	const double factor = double(N)/double(actualN);
	// ���� �� �������� ��������
	for(int i=0; i<n; i++)
	{
		// ���� �� ������� ��������
		for(int j=0; j < actualN; j++)
		{
			if(i!=j)
			// ���������� � ���� �������, ������ ������������ ���� �����,
			// ����������� �� ��������� faactor 
			// (�� ������ ���� ������ 1 �� ������ ������������ �����,
			// � ������ � �������� ����������������)
			netWeights(i,j)(outp(i), inp(j)) += factor;
		}
		thres(i).resize(qo);
		// ����� - ��� � 1 � ������� outp(i)
		thres(i)=boost::numeric::ublas::unit_vector<Elem>(qo, outp(i));
	}
	// ����� ����������� �� ���������
	thres*=(-double(actualN)/double(qo));
	// ����������� ���������� ����� � ����� ����� ����
	netThresholds+=thres;
}

/****************************************************************************
*					QnaryPerc::run
*
*	Description:	��������� ���� �� ������ inp
*	Parameters:	const Qstring &inp - ������� ������
*				double &p - ���� ������������ ��������� ��������, ���������� � ���������� �������
*	Returns:	Qstring - ��������� ������� (��, ��� ���������� �� �������� ��������)
*	Throws:		AxiomLibException, ���� ������ �������� ������� ������ ������������� ������� �������� �������
*	Author:		wictor
*	History:
*
****************************************************************************/
Qstring QnaryPerc::run(const Qstring &inp, double &p) const
{
	// �������� ������� �����
	if((int) inp.size() > N) {
		throw AxiomLibException("QnaryPerc::run() : inp size too large");
	}
	
	Qstring outp(n);
	
	calcOutput(p, inp, outp);

	return outp;
}
