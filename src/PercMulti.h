/****************************************************************************
*						PercMulti.h
*
*       Description:	���������� ����� PercMulti � PercMultiSettings
*       Author:			wictor
*       History:
*
****************************************************************************/
#ifndef PERCMULTI_H_
#define PERCMULTI_H_

#include <vector>
#include <string>

#include "NeuralDataHolder.h"
#include "NeuralAuxSupplier.h"
#include "PercWrapper.h"
#include "NeuralDistRejecter.h"
#include "NeuralMultiplexer.h"
#include "NeuralCoderGeneric.h"
#include "NeuralCommonSettings.h"
#include "PercSingle.h"

namespace AxiomLib {

// ��� �������� ��� PercMulti - ���� �� ��������� � PercSingleSettings
typedef PercSingleSettings PercMultiSettings;

// ���, ��������������� ������ ������������ ������������,
// ��� ������� �� ������� �������� ��������� �� ����������,
// ��������� �������������� �����, �������� � ����������� ������
// (����� ����������� ����������� ������� �������� Decorator)
class PercMulti: public
	// ����, ��������������� ������ ����� � ��������������� ��������� ��� ��� ����� ����
	// (�.�. ������������ NetWrapperMinimal, ��. NeuralConcepts.h)
	NeuralMultiplexer<
		// ���� ���������� ����� �� ����������
		// (��� �������� ����������� ������������� �� ������ ���������� 
		// ���� ���� NeuralMultiplexer, � �� ����)
		NeuralDistRejecter<
			// ����, ��������������� ������������ ����������
			PercWrapper<
				// ����, ������������� ������������ �������������� ������ 
				// (���������� ��������� ������ ��� ��������������� ��������)
				NeuralAuxSupplier<
					// ����, �������� ��������� ������
					NeuralDataHolder<
						// ���������� - ����� ����������� ���������� ����������, ����������� 
						// ��������� ������� � ����������� �� �������� ����������
						NeuralCoderGeneric
						> > > > > {
public:
	// ����������� ���� �������� ��� ������������� � ������� RecognizerNeuralNet
	typedef PercMultiSettings Settings;
};

};

#endif
