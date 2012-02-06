/****************************************************************************
*						NeuralDataHolder.cxx
*
*       Description:	���������� �������� �������-������ ������� ������ NeuralDataHolder
*						�� ������������ �� Coder ��. NeuralDataHolder.h
*       Author:			wictor
*       History:
*
****************************************************************************/

#ifndef NEURALDATAHOLDER_CXX_
#define NEURALDATAHOLDER_CXX_

#include "NeuralDataHolder.h"
#include "QnaryPercWrapper.h"

namespace AxiomLib {

/****************************************************************************
*						NeuralDataHolder<Coder>::supplyTrainingData
*
*       Description:	������� ��������� ���� ������ � ������ ����� ��� ��������
*       Parameters:		const Qstring &s - ����������� ������
*       Returns:		-
*       Throws:			-
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class Coder>
inline void NeuralDataHolder<Coder>::supplyTrainingData(const Qstring &s) {
	trainData.push_back(s);
	auxTrainData.push_back(std::vector<Qstring>());
	if((unsigned) maxDim < s.size()) {
		if(maxDim == 0) {
			minDim = (int) s.size();
		}
		maxDim = (int) s.size();
	} else if((unsigned) minDim > s.size()) {
		minDim = (int) s.size();
	}
}

/****************************************************************************
*						NeuralDataHolder<Coder>::supplyAuxTrainingData
*
*       Description:	������� ��������� ���� �������������� ������ ��� ��������,
*						������� ������������� ��������� ����������� ������ ��� ��������
*       Parameters:		const Qstring &s - ����������� ������
*						i - ������ �������� ������, � ������� ���� ������������� 
*						����������� ������ ��� -1, ���� ���� ������������� � ���������
*						����������� �������� �������
*       Returns:		-
*       Throws:			AxiomLibException, ���� �� ���� �� ���� ������� supplyTrainingData(),
*						���� ������ inp �� ��������� � �������� ��������� ����������� ������ ��� ��������
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class Coder>
void NeuralDataHolder<Coder>::supplyAuxTrainingData(const Qstring &inp, int i)
{
	if(auxTrainData.empty() || trainData.empty()) {
		throw AxiomLibException("NeuralDataHolder<Coder>::supplyAuxTrainingData():"
		" no train data supplied");	
	}
	if(i >= (int) auxTrainData.size()) {
		throw AxiomLibException("NeuralDataHolder<Coder>::supplyAuxTrainingData():"
		" index out of range");	
	}
	if(i <= -1) {
		i = auxTrainData.size() - 1;
	}
	if(trainData[i].size() != inp.size()) {
		throw AxiomLibException("NeuralDataHolder<Coder>::supplyAuxTrainingData():"
		" aux data does not match train data in size");	
	}
	
	auxTrainData[i].push_back(inp);
}

/****************************************************************************
*						NeuralDataHolder<Coder>::getNumAuxTrain
*
*       Description:	���������� ����� �������������� ����� ��� ��������, ���������������
*						������ ��� �������� � ������� i
*       Parameters:		const Qstring &s - ����������� ������
*       Returns:		-
*       Throws:			AxiomLibException, ���� ����� i �����������
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class Coder>
int NeuralDataHolder<Coder>::getNumAuxTrain(int i) const {
	// �������� �� ����� �� ������� �������
	if(i < 0 || i >= (int) trainData.size()) {
		throw AxiomLibException("NeuralDataHolder<Coder>::getNumAuxTrain() : i out of range");
	}
	
	return auxTrainData[i].size();
}

/****************************************************************************
*						NeuralDataHolder<Coder>::construct
*
*       Description:	������� ������ ����, ����������� �� ���������� ������.
*       Parameters:		��� Settings - ������ ������������� ������������ Coder::construct()
*						const Settings &settings - ���������
*       Returns:		-
*       Throws:			-
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class Coder>
template<class Settings>
void NeuralDataHolder<Coder>::construct(const Settings &settings) {
	coder.construct(settings);
}

/****************************************************************************
*						NeuralDataHolder<Coder>::clear
*
*       Description:	������� ������� ������ ��� ���������� �������������
*       Parameters:		-
*       Returns:		-
*       Throws:			-
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class Coder>
inline void NeuralDataHolder<Coder>::clear()
{
	trainData.clear();
	auxTrainData.clear();
	minDim = 0;
	maxDim = 0;
}

/****************************************************************************
*						NeuralDataHolder<Coder>::getTrainString
*
*       Description:	���������� ��������� ������ � ������� i
*       Parameters:		int i - ����� ������
*       Returns:		const Qstring & - ������ ��� ��������
*       Throws:			AxiomLibException, ���� ����� ������ �����������
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class Coder>
const Qstring & NeuralDataHolder<Coder>::getTrainString(int i) const { 
	// �������� �� ����� �� ������� �������
	if(i < 0 || i >= (int) trainData.size()) {
		throw AxiomLibException("NeuralDataHolder<Coder>::getTrainString() : i out of range");
	}
	return trainData.at(i); 
}

/****************************************************************************
*						NeuralDataHolder<Coder>::getAuxTrainString
*
*       Description:	���������� ������ ��� ���. �������� � ������� j,
*						��������������� ��������� ������ � ������� i
*       Parameters:		int i - ����� ��������� ������
*						int j - ����� ������ ��� ���. ��������
*       Returns:		const Qstring & - ������ ��� ���. ��������
*       Throws:			AxiomLibException, ���� ����� i ��� j �����������
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class Coder>
const Qstring & NeuralDataHolder<Coder>::getAuxTrainString(int i, int j) const { 
	// �������� �� ����� �� ������� ��������
	if(i < 0 || i >= (int) auxTrainData.size()) {
		throw AxiomLibException("NeuralDataHolder<Coder>::getAuxTrainString() : i out of range");
	}
	if(j < 0 || j >= (int) auxTrainData[i].size()) {
		throw AxiomLibException("NeuralDataHolder<Coder>::getAuxTrainString() : j out of range");
	}

	return auxTrainData[i][j]; 
}

/****************************************************************************
*						NeuralDataHolder<Coder>::getTranslated
*
*       Description:	���������� ��������� ������ � ������� i � �������������� ����
*       Parameters:		int i - ����� ������
*       Returns:		const Qstring & - �������������� ������ ��� �������� 
*       Throws:			-
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class Coder>
const Qstring NeuralDataHolder<Coder>::getTranslated(int i) const {
	Qstring translated;
	coder.translate(getTrainString(i), translated);
	return translated;
}

/****************************************************************************
*						NeuralDataHolder<Coder>::getAuxTranslated
*
*       Description:	���������� (� �������������� ����) ������ ��� ���. 
*						�������� � ������� j,
*						��������������� ��������� ������ � ������� i
*       Parameters:		int i - ����� ��������� ������
*						int j - ����� ������ ��� ���. ��������
*       Returns:		const Qstring & - �������������� ������ ��� ���. ��������
*       Throws:			-
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class Coder>
const Qstring NeuralDataHolder<Coder>::getAuxTranslated(int i, int j) const {
	Qstring translated;
	coder.translate(getAuxTrainString(i, j), translated);
	return translated;
}

/****************************************************************************
*						NeuralDataHolder<Coder>::coderTranslate
*
*       Description:	�������� ������ str, ���������� ���������
*       Parameters:		const Qstring &str
*       Returns:		const Qstring & - �������������� str
*       Throws:			-
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class Coder>
const Qstring NeuralDataHolder<Coder>::coderTranslate(const Qstring &str) const {
	Qstring res;
	coder.translate(str, res);
	return res;
}

};

#endif
