#ifndef QNARYPERCSINGLE_H_
#define QNARYPERCSINGLE_H_

#include "NeuralDataHolder.h"
#include "QnaryPercWrapper.h"
#include "NeuralDistRejecter.h"
#include "NeuralCoderGeneric.h"
#include "NeuralCommonSettings.h"

namespace AxiomLib {

struct QnaryPercSingleSettings {
	// ����� ���������
	NeuralCommonSettings commonSettings;
	// ��������� �����������
	NeuralCoderSettings coderSettings;
	// ����� ��������� �������� ��������
	int qO;
	// ������������� �� ���������
	void initFromEnv(const Environment &env);
};

// ��� ������� q-������� ����������� 
// � ����� ����� ��� ������ ���� �����
// � ���������� �� ����������
// (����� ����������� ����������� ������� �������� Decorator)
class QnaryPercSingle: public
	// ���� ���������� ����� �� ����������
	NeuralDistRejecter<
		// ����, ��������������� q-������ ����
		QnaryPercWrapper<
			// ����, �������� ��������� ������
			NeuralDataHolder<
				// ���������� - ����� ����������� ���������� ����������, ����������� 
				// ��������� ������� � ����������� �� �������� ����������
				NeuralCoderGeneric
				> > > {
public:
// ��� �������� - �� ��������� ��� ��� ������������� � ������� RecognizerNeuralNet
	typedef QnaryPercSingleSettings Settings;
};

};

#endif
