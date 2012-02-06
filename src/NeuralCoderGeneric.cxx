/****************************************************************************
*			NeuralCoderGeneric.cxx
*
*	Description:	���������� ����, ��� ������� � NeuralCoderGeneric.h
*	Author:		wictor
*	History:	
*
****************************************************************************/
#include "NeuralCoderGeneric.h"
#include "NeuralCommonSettings.h"
#include "NeuralCommon.h"

namespace AxiomLib {

/****************************************************************************
*					NeuralCoderGeneric::translateWay1
*
*	Description:	����������� �� ������� 1
*	Parameters:	const Qstring &src - �������� ������
*				Qstring &dst - �������� ������
*	Returns:	-
*	Throws:		AxiomLibException, ���� �������� ������ ������
*	Author:		wictor
*	History:
*
****************************************************************************/
void NeuralCoderGeneric::translateWay1(const Qstring &src, Qstring &dst) const
{
	int dst_pos=0;
	int sz=src.size();
	dst.resize(this->getDstLen(src.size()));
	for(int src_pos=0; src_pos<sz; src_pos+=windowSize)
	{
		Qstring window;
		getWindow(src, window, src_pos, windowSize);
		int index=qnaryVectorToNum(srcQ, window);
		for(int j=0; j<codedSymbolSize; j++)
		{
			dst(dst_pos+j)=dict[index](j);
		}
		dst_pos+=codedSymbolSize;
	}
}

int NeuralCoderGeneric::getDstLen(int srcLen) const { 
	int n;
	switch(codingWay) {
		case Way2:
			return srcLen*codedSymbolSize; break;
		case Way1: n = (srcLen / windowSize) + (srcLen % windowSize > 0 ? 1 : 0);
			return n*codedSymbolSize; break;
		default:
			throw AxiomLibException("NeuralCoderGeneric::getDstLen(): unexpected CodingWayEnum value");	
	}
}

/****************************************************************************
*					NeuralCoderGeneric::translateWay2
*
*	Description:	����������� �� ������� 2
*	Parameters:	const Qstring &src - �������� ������
*				Qstring &dst - �������� ������
*	Returns:	-
*	Throws:		AxiomLibException, ���� �������� ������ ������
*	Author:		wictor
*	History:
*
****************************************************************************/
void NeuralCoderGeneric::translateWay2(const Qstring &src, Qstring &dst) const
{
	for(unsigned int i=0; i<src.size(); i++)
	{
		dst[i*srcQ + (src[i]%srcQ)]=1;
	}
}

/****************************************************************************
*					NeuralCoderGeneric::translate
*
*	Description:	�����������
*	Parameters:	const Qstring &src - �������� ������
*				Qstring &dst - �������� ������
*	Returns:	-
*	Throws:		AxiomLibException, ���� �������� ������ ������, ���� �� ���� ����������� construct()
*	Author:		wictor
*	History:
*
****************************************************************************/
void NeuralCoderGeneric::translate(const Qstring &src, Qstring &dst) const
{
	if(src.size() == 0) {
		throw AxiomLibException("NeuralCoderWay2::translate() : source string is empty");
	}
	
	// ������, ����� ��� ���������
	if(dstQ == srcQ && numVar == 1) {
		dst.resize(src.size());
		dst = src;
		return;
	}
	
	dst.resize(getDstLen(src.size()));
	makeZero(dst);
	switch(codingWay) {
	case Way1: translateWay1(src, dst); break;
	case Way2: translateWay2(src, dst); break;
	default: throw AxiomLibException("NeuralCoderGeneric::translate() : unexpected CodingWay value");
	}
}

};
