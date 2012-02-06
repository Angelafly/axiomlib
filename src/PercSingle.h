/****************************************************************************
*						PercSingle.h
*
*       Description:	���������� ����� PercSingle � PercSingleSettings
*       Author:			wictor
*       History:
*
****************************************************************************/
#ifndef PERCSINGLE_H_
#define PERCSINGLE_H_

#include <vector>
#include <string>

#include "NeuralDataHolder.h"
#include "NeuralAuxSupplier.h"
#include "PercWrapper.h"
#include "NeuralDistRejecter.h"
#include "Environment.h"
#include "NeuralCoderGeneric.h"
#include "NeuralCommonSettings.h"

namespace AxiomLib {

/****************************************************************************
*						PercSingleSettings
*
*       Description:	���������, ���������� ��������� ��� PercSingle
*       Author:			wictor
*       History:
*
****************************************************************************/
struct PercSingleSettings {
	// ����� ���������
	NeuralCommonSettings commonSettings;
	// ��������� �����������
	NeuralCoderSettings coderSettings;
	// ��������� �����������
	PercSettings percSettings;
	// ����� ���. ����� ��� ��������
	int numAux;
	// ���������� �� ���. ����� ��� �������� �� �������� ��������� �����
	double auxDiff;
	// ������������� �� ���������
	void initFromEnv(const Environment &env);
};

// ���, ��������������� ������������ ����������,
// ��� �������� �������� ��������� �� ����������,
// ��������� �������������� �����, �������� � ����������� ������
// (����� ����������� ����������� ������� �������� Decorator)
class PercSingle: public
	// ���� ���������� ����� �� ����������
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
					> > > > {
public:
	// ��� ��������
	typedef PercSingleSettings Settings;
};

};

#endif
