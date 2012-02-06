/****************************************************************************
*			NeuralConcepts.h
*
*	Description: �������� ��������� NetWrapper, Holder � Coder.
*				������ ���� �� ������� � ������, �� ����� ������ ��� ��������� ����. 
*				� ���, ����������, �����������
*				����������, ������� ������ ������������� ��������� ������
*	Author:		wictor
*	History:	
*
****************************************************************************/
#ifndef NEURALCONCEPTS_H_
#define NEURALCONCEPTS_H_

#include "NeuralCommon.h"
#include "Environment.h"
#include <string>

namespace AxiomLib {

// NetWrapperMinimal + ����������� ��������� ���. ������ ��� ��������
class NetWrapper: public NetWrapperMinimal {
public:
	NetWrapper();
	
	// ��������� �������������� ������ ��� �������� 
	// (����������� � ��������� ����������� ������ ��� ��������)
	void supplyAuxTrainingData(const Qstring &str);
	
};

class NetWrapperMinimalWithCentralStrings: public NetWrapperMinimal {
public:
	NetWrapperMinimalWithCentralStrings();
	
	// ��������� ����������� ������.
	// ��� ������ ��� ��������, �������� ����� ������ ���� �������,
	// ��������� � ������ ����������� ������.
	void supplyCentralString(const Qstring &str);
	// ��� ���������������� ��������� �� ��������� �������� ������
	// ��� ������ ����� ��������� ��������� ������.
	// ����� ��� ��������� �� ����������
	// ����������� ���������� ����� �������� ������� �
	// ����������� �������, ��������������� ����������
	// ��������� ������
};

// ��� ������� ���������, ������� ����� ��������� � ���������������
class NetWrapperMinimal {
public:
	NetWrapperMinimal();
	
	// ��������� ������ ��� ��������
	void supplyTrainingData(const Qstring &str);

	// ���������� ������ ��� �������� � ������� i
	const Qstring &getTrainString(int i) const;
			
	// ���������� ����� ����� ��� ��������
	int getNumTrain() const;
	
	// ������������ ����������� (����������������) ��������� ������
	int getMaxTrainDim() const;
	// ����������� ����������� (����������������) ��������� ������
	int getMinTrainDim() const;
	
	// �������������� ������ ������������ �� settings
	// (���������� ����� ���������� ���� ��������� �����)
	template<class Settings>
	void construct(const Settings &settings);
	
	// ������� ���� �� �������������� �� ����� ��������� �������
	void train();
	
	// ��������� ���� �� ������ inp, ���������� ����� ��������� ������ ���� -1
	int run(const Qstring &inp, double &p) const;
	
	// ������� ������
	void clear();
};

// ��� ������� ��������, �������������� �������� � ����������� ��������� �������
class Holder {
public:
	Holder();
	
	// ��������� ������ ��� ��������
	void supplyTrainingData(const Qstring &str);
	
	// ��������� �������������� ������ ��� �������� 
	// (����������� � ��������� ����������� ������ ��� ��������)
	void supplyAuxTrainingData(const Qstring &str);
	
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
	int getCoderSrcQ() const;
	
	// ���������� dstQ �����������
	int getCoderDstQ() const;
	
	// ���������� ����� �������������� ������ � �������� ������ srcLen
	int getCoderDstLen(int srcLen) const;
	
	// ������������ ����������� (����������������) ��������� ������
	int getMaxTrainDim() const;
	// ����������� ����������� (����������������) ��������� ������
	int getMinTrainDim() const;
	
	// ���������� ����� ����� ��� ��������
	int getNumTrain() const;
	
	// ���������� ����� ������� �������������� ����� ��� ��������
	// ����������, ���������� �� ��, ��� getNumTrain()
	int getNumAuxTrain() const;
	
	// ���������� ����� �������������� ����� ��� ��������, 
	// ��������������� ������ ��� �������� � ������� i
	int getNumAuxTrain(int i) const;
	
	// �������������� ������ ������������ �� settings
	template<class Settings>
	void construct(const Settings &settings);
	
	// ������� ������
	void clear();
};

// ��� ������� ��������, �������������� ����������� ������
class Coder {
public:
	Coder();
	
	// �������������� ������ ������������ �� settings
	template<class Settings>
	void construct(const Settings &settings);
	
	// �������� src, ��������� �������� � dst
	void translate(const Qstring &src, Qstring &dst) const;
	// ���������� srcQ
	int getSrcQ() const;
	// ���������� dstQ
	int getDstQ() const;
	
	// ����� ������ ��� ����� ����� srcLen	
	int getDstLen(int srcLen) const;
};

class Settings {
public:
	// �������������� �������� �������� �� ���������
	void initFromEnv(const Environment &env);
};

};

#endif