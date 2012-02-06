/****************************************************************************
*			NeuralDataHolder.h
*
*	Description: ����� ��� �������� � ����������� ��������� �������
*	Author:		wictor
*	History:	
*
****************************************************************************/

#ifndef NEURALDATAHOLDER_H_
#define NEURALDATAHOLDER_H_

#include "NeuralCommon.h"
#include "QnaryPerc.h"
#include <vector>
#include <boost/foreach.hpp>

namespace AxiomLib {

/****************************************************************************
*			NeuralDataHolder<Coder>
*
*	Description: ������ ������, ��������������� �������� �
*				����������� ������. ���������� Holder
*				�������� �������: Coder - ������ ���� ������� Coder (��. NeuralConcepts.h)
*	Author:		wictor
*	History:	
*
****************************************************************************/
template<class Coder>
class NeuralDataHolder {

	// ������ ��� ��������
	std::vector<Qstring> trainData;
	// �������������� ������ ��� ��������
	std::vector<std::vector<Qstring> > auxTrainData;
	// ����������	
	Coder coder;
	
	// ����������� � ������������ ����� �����
	int maxDim, minDim;
public:

	NeuralDataHolder():maxDim(0), minDim(0) {}
	
	// ������������ ����������� (����������������) ��������� ������
	int getMaxTrainDim() const { return maxDim; }
	
	// ����������� ����������� (����������������) ��������� ������
	int getMinTrainDim() const { return minDim; }
	
	// ���������� ����� ����� ��� ��������
	int getNumTrain() const { return trainData.size(); }
	
	// ���������� ����� ������� �������������� ����� ��� ��������
	// ����������, ���������� �� ��, ��� getNumTrain()
	int getNumAuxTrain() const { return auxTrainData.size(); }
	
	// ���������� ����� �������������� ����� ��� ��������, 
	// ��������������� ������ ��� �������� � ������� i
	int getNumAuxTrain(int i) const;
	
	// ���������� ������ ��� �������� � ������� i
	const Qstring &getTrainString(int i) const;
	
	// ���������� �������������� ������ ��� �������� � ������� j, 
	// ��������������� ������ ��� �������� � ������� i
	const Qstring &getAuxTrainString(int i, int j) const;
	
	// ���������� ������ ��� �������� � ������� i, �������������� ������� ��
	const Qstring getTranslated(int i) const;
	// ���������� �������������� ������ ��� �������� � ������� j, 
	// ��������������� ������ ��� �������� � ������� i, �������������� ������� ��	
	const Qstring getAuxTranslated(int i, int j) const;
	
	// �������� ������ str
	const Qstring coderTranslate(const Qstring &str) const;
	
	// ���������� srcQ �����������	
	int getCoderSrcQ() const { return coder.getSrcQ(); }
	
	// ���������� dstQ �����������
	int getCoderDstQ() const { return coder.getDstQ(); }
	
	// ���������� ����� �������������� ������ � �������� ������ srcLen
	int getCoderDstLen(int srcLen) const { return coder.getDstLen(srcLen); }
	
	// ��������� ������ ��� ��������
	void supplyTrainingData(const Qstring &s);
	
	// ��������� �������������� ������ ��� �������� 
	// (����������� � ��������� ����������� ������ ��� ��������)
	// ���� i=-1, �� ����������� ������ ������������� � ���������
	// ����������� ������� ��� ��������
	// ����� - �� ������� � ������� i
	void supplyAuxTrainingData(const Qstring &s, int i = -1);
	
	// �������������� ������
	template<class Settings>
	void construct(const Settings &settings);

	// ������� ������ (������� ��� ��������� ������)
	void clear();	
};

};

#include "NeuralDataHolder.cxx"

#endif
