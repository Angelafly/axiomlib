#ifndef QNARYPERCMULTI_H_
#define QNARYPERCMULTI_H_

#include "NeuralDataHolder.h"
#include "QnaryPercWrapper.h"
#include "NeuralDistRejecter.h"
#include "NeuralMultiplexer.h"
#include "NeuralCoderGeneric.h"
#include "NeuralCommonSettings.h"
#include "QnaryPercSingle.h"

namespace AxiomLib {

// ��� �������� ��� QnaryPercMulti - ���� �� ��������� � QnaryPercSingleSettings
typedef QnaryPercSingleSettings QnaryPercMultiSettings;

// ��� ������� q-������� ����������� 
// �� ����� ����� ��� ������� �������� ����� �����
// � ���������� �� ����������
// (����� ����������� ����������� ������� �������� Decorator)
class QnaryPercMulti: public
	// ����, ��������������� ������ ����� � ��������������� ��������� ��� ��� ����� ����
	// (�.�. ������������ NetWrapperMinimal, ��. NeuralConcepts.h)
	NeuralMultiplexer<
		// ���� ���������� ����� �� ����������
		// (��� ����������� ������������� �� ������ ���������� ���� ���� NeuralMultiplexer, � �� ����)
		NeuralDistRejecter<
			// ����, ��������������� q-������ ����
			QnaryPercWrapper<
				// ����, �������� ��������� ������
				NeuralDataHolder<
					// ���������� - ����� ����������� ���������� ����������, ����������� 
					// ��������� ������� � ����������� �� �������� ����������
					NeuralCoderGeneric
					> > > > {
public:
	// ��� �������� - �� ��������� ��� ��� ������������� � ������� RecognizerNeuralNet
	typedef QnaryPercMultiSettings Settings;
};

};

#endif
