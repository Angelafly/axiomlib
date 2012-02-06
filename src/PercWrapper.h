/****************************************************************************
*						PercWrapper.h
*
*       Description:	���������� ������� ������ PercWrapper
*       Author:			wictor
*       History:
*
****************************************************************************/
#ifndef PERCWRAPPER_H_
#define PERCWRAPPER_H_

#include "PercSettings.h"

#include "NeuralCommon.h"

namespace AxiomLib {

/****************************************************************************
*						PercWrapper<Holder>
*
*       Description:	�����, ��������������� ������������ ����������
*						���������� NetWrapper (��. NeuralConcepts.h)
*						Holder ������ ������������ ������� Holder
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class Holder>
class PercWrapper: public Holder {
	// ������ ����
	// �������� ������� ��� mutable, ����� ���������
	// ������������� ������ run()
	// �.�. ����� FANN::neural_net::run() �� �����������
	mutable FANN::neural_net net;
	// ������ ��� ��������
	FANN::training_data data;
	// ����� ������� � �������� ��������
	int numInp, numOutp;
	// ����. ����� ����
	int maxIters;
	// ����� ���� ����� ����������� � ��������� ��������
	int itersBetweenReports;
	// ������, ��� ������� �������� ������������
	double desiredError;
	
	// 0 - construct() �� �������, 1 - construct() �������, train() �� �������, 
	// 2 - construct() � train() �������
	int stage;
	
	// ���������� ��������� �������
	void constructTrainingData();
	
	// �������������� �������� ���������� ������
	// � ��������, ���������� �� ���� �������
	static double transform(const Qstring &str, int j) 
	{	
		if (j >= (int) str.size() || j < 0) {
			return 0;
		}
		return str[j] > 0.5 ? 1.0 : 0.0;
	}
	// ����������� ��������, ��������� � ������������ ��������� ������ ��
	// ��������������� �������� �������
	// (������� ��� ����, ����� ���. ��������� ������� ������ ��������,
	// ������� 1)
	static double determineOutput(const Qstring &templ, const Qstring &current) 
	{ return 1.0-difference(templ, current); }

	// �������� ��� ������� ��������� ������, ��� ��������, ���� ����������� �� �����
	static int defaultCallback(FANN::neural_net &net, FANN::training_data &train,
	        unsigned int max_epochs, unsigned int epochs_between_reports,
	        float desired_error, unsigned int epochs, void *user_data) { return 0; }
	
	// ������� "�����������" ����� �������� � ���� (� ����������� �� ������ ���� �
	// ����� ������� � �������� ��������)
	static int calcAveNumNeurons(int layer_no, int num_lay, int num_inp, int num_outp) {
		return (layer_no*num_outp + (num_lay-layer_no)*num_inp)/num_lay;
	}

public:
	// ����������� ��������� - �������������� ��������� ����
	PercWrapper():numInp(0), numOutp(0), stage(0) { /*net.set_callback(defaultCallback, 0);*/ }
	
	// ����������� ����������� - ����������� ��-�� ����, ��� 
	// FANN::training_data �� copy-constructible
	PercWrapper(const PercWrapper &);
	
	// ������������� ��������
	template<class Settings>
	void construct(const Settings &settings);
	
	// ��������
	void train();
	
	// ������ ��������� �� ������ str
	int run(const Qstring &str, double &p) const;
	
	//void clear();
	// TODO: �����������

};

};

#include "PercWrapper.cxx"

#endif
