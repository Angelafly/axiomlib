/****************************************************************************
*			NeuralCoderGeneric.h
*
*	Description:	�������� ����������� NeuralCoderGeneric, ��������� ����������� ���������� ����
*					� ����������� �� ����� ����������
*	Author:		wictor
*	History:	
*
****************************************************************************/

#ifndef NEURALCODERGENERIC_H_
#define NEURALCODERGENERIC_H_

#include <vector>
#include <ctime>

#include "NeuralCommon.h"
#include "Round.h"

namespace AxiomLib {

/****************************************************************************
*			NeuralCoderGeneric
*
*	Description:	����������, ������� ����������� ���������� ����
*					� ����������� �� ����� ����������
*					���������� Coder (��. NeuralConcepts.h)
*	Author:		wictor
*	History:	
*
****************************************************************************/
class NeuralCoderGeneric {
	// ��� ��������� ������, �������������� ����������� (��� 1-�� ���� �����������)
	typedef std::vector<Qstring> map_t;
	
	// ��������� ������, �������������� �����������
	map_t dict;
	
	// ���������
	int srcQ, dstQ, numVar, windowSize, 
	
	// ����� �������� ��������� ��������, ������� ���������� ���� ������ ���������
	codedSymbolSize;
	
	// ��� ������� �����������
	enum CodingWay {Way1, Way2, None};
	// ������ �����������
	CodingWay codingWay;
	
	// ����������� �� ������� �������
	void translateWay1(const Qstring &src, Qstring &dst) const;
	// ����������� �� ������� �������
	void translateWay2(const Qstring &src, Qstring &dst) const;
public:
	// ����������� ���������
	NeuralCoderGeneric():codingWay(None) {}
		
	// �������������
	// Settings ������ ����� ����� int srcQ, dstQ, numVar
	template<class Settings>
	void construct(const Settings &settings);
	
	// �������
	void translate(const Qstring &src, Qstring &dst) const;
	
	// ���������� srcQ
	int getSrcQ() const { return srcQ; }
	// ���������� dstQ
	int getDstQ() const { return dstQ; }
	
	// ����� ������ ��� ����� ����� srcLen	
	int getDstLen(int srcLen) const;
};

/****************************************************************************
*					NeuralCoderGeneric::construct
*
*	Description:	������������� �������, � �.�. ���������� ����������� �������� 
*					��������� �������� (�������) �� ������� ��� ������� 1.
*					��� Settings ������ ����� ����� int srcQ, dstQ, numVar
*					���� NumVar < 1, �� � ���������� ����� ����������� ������ 2
*					����� ����� ����������� ������ 1
*	Parameters:	const Settings &settings - ���������
*	Returns:	-
*	Throws:		AxiomLibException, ���� � settings srcQ <= 1 || dstQ <= 1
*	Author:		wictor
*	History:
*
****************************************************************************/
template<class Settings>
void NeuralCoderGeneric::construct(const Settings &settings)
{	
	srcQ = settings.commonSettings.srcQ;
	dstQ = settings.coderSettings.dstQ;
	numVar = settings.coderSettings.numVar;
	windowSize = settings.coderSettings.windowSize;
	
	// C������, ��� ���� numVar < 1, �� ���� ������������ ������ 2
	if(numVar < 1) {
		if(dstQ != 2) {
			throw AxiomLibException("NeuralCoderWayGeneric::construct() : bad settings value");
		}
		codingWay = Way2;
		codedSymbolSize = srcQ;
		return;
	} else {
		if(dstQ <= 1) {
			throw AxiomLibException("NeuralCoderWay1::construct() : bad settings value");
		}
		codingWay = Way1;
	}
	
	// ������, ����� ��� ���������
	if(srcQ == dstQ && numVar == 1) {
		codedSymbolSize = 1;
		return;
	}
	
	if(srcQ <= 1 || dstQ <= 1) {
		throw AxiomLibException("NeuralCoderWay1::construct() : bad settings value");
	}
	
	int numWords = round(std::pow((double)srcQ, windowSize));
	
	dict.resize(numWords);
	
	codedSymbolSize = numDigits(dstQ, numVar*numWords-1);
	for(int i=0; i<numWords; i++)
	{
		Qstring str(codedSymbolSize);
		makeZero(str);
		int number=i*numVar +(rand()%numVar);
		numToQnaryVector(dstQ, number, str);
		dict[i].resize(codedSymbolSize);
		dict[i]=str;
	}
}

};

#endif /* NEURALCODERGENERIC_H_ */
