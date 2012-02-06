/****************************************************************************
*						NeuralMultiplexer.cxx
*
*       Description:	���������� ������ NeuralMultiplexer
*       Author:			wictor
*       History:
*
****************************************************************************/
#ifndef NEURALMULTIPLEXER_CXX_
#define NEURALMULTIPLEXER_CXX_

#include "NeuralMultiplexer.h"
#include <boost/foreach.hpp>

#include <iostream>

namespace AxiomLib {

/****************************************************************************
*						NeuralMultiplexer<NetWrapper>::supplyTrainingData
*
*       Description:	������� ��������� ���� ������ ��� ��������
*       Parameters:		const Qstring &s - ����������� ������
*       Returns:		-
*       Throws:			-
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class NetWrapper>
void NeuralMultiplexer<NetWrapper>::supplyTrainingData(const Qstring &s) {
	int len = s.size();
	
	// �������� ��������� �� ����, ��������������� ����� len 
	// (���� �� ���, �������)
	/*NetWrapper *pNetWrapper; // ���� ���, �� - �� ���� ���������� � ������� �� ���� ������� iterator ��� ����������� map � ������� ������������ �������� �� �������.
	MapType::const_iterator it = netArray.find(len);
	if(it!=netArray.end()) {
		// ���� ��� ������ ����� ��� ���� - �������� ��������� �� ���
		pNetWrapper = it->second;
	} else {
		// ���� ��� ������ ����� ��� ��� - ������� ��
		pNetWrapper = new NetWrapper();
		netArray.insert(std::make_pair(len, pNetWrapper));
	}*/
	NetWrapper *pNetWrapper;
	if(netArray.find(len) != netArray.end()) {
		// ���� ��� ������ ����� ��� ���� - �������� ��������� �� ���
		pNetWrapper = (netArray.find(len))->second;
	} else {
		// ���� ��� ������ ����� ��� ��� - ������� ��
		pNetWrapper = new NetWrapper();
		netArray.insert(std::make_pair(len, pNetWrapper));
	}
	// �������� ����� ��� ����������� � netWrapper �����
	// ��� ����� ������� ����������� ������ � ���� ����
	int num = pNetWrapper->getNumTrain();
	
	// ��������� ������ � ����
	pNetWrapper->supplyTrainingData(s);
	
	//��������� ������ ������ ������
	IndexPair ip = std::make_pair(len, num);
	glNumToLocNum.push_back(ip);
	
	// ���������� �����
	locNumToGlNum[ip] = globalTrainSize;
	
	// ����������� ������� �����
	globalTrainSize++;
	
	// ��������� lastIndex
	lastIndex.len = ip.len;
	lastIndex.num = ip.num;
}

/****************************************************************************
*						NeuralMultiplexer<NetWrapper>::supplyCentralString
*
*       Description:	������� ��������� ���� primary-������ ��� ��������
*       Parameters:		const Qstring &s - ����������� ������
*       Returns:		-
*       Throws:			-
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class NetWrapper>
void NeuralMultiplexer<NetWrapper>::supplyCentralString(const Qstring &s) {
	int len = s.size();
	
	// �������� ��������� �� ����, ��������������� ����� len 
	// (���� �� ���, �������)
	/*NetWrapper *pNetWrapper;	
	MapType::const_iterator it = netArray.find(len);
	if(it!=netArray.end()) {
		// ���� ��� ������ ����� ��� ���� - �������� ��������� �� ���
		pNetWrapper = it->second;
	} else {
		// ���� ��� ������ ����� ��� ��� - ������� ��
		pNetWrapper = new NetWrapper();
		netArray.insert(std::make_pair(len, pNetWrapper));
	}*/
	NetWrapper *pNetWrapper;
	if(netArray.find(len) != netArray.end()) {
		// ���� ��� ������ ����� ��� ���� - �������� ��������� �� ���
		pNetWrapper = (netArray.find(len))->second;
	} else {
		// ���� ��� ������ ����� ��� ��� - ������� ��
		pNetWrapper = new NetWrapper();
		netArray.insert(std::make_pair(len, pNetWrapper));
	}
	// �������� ����� ��� ����������� � netWrapper �����
	// ��� ����� ������� ����������� ������ � ���� ����
	int num = pNetWrapper->getNumTrain();
	
	// ��������� ����������� ������ � ����
	pNetWrapper->supplyCentralString(s);
	
	//��������� ������ ������ ������
	IndexPair ip = std::make_pair(len, num);
	glNumToLocNum.push_back(ip);
	
	// ���������� �����
	locNumToGlNum[ip] = globalTrainSize;
	
	// ����������� ������� �����
	globalTrainSize++;
	
	// ��������� lastIndex
	lastIndex.len = ip.len;
	lastIndex.num = ip.num;
}

/****************************************************************************
*						NeuralMultiplexer<NetWrapper>::getTrainString
*
*       Description:	���������� ��������� ������ � (����������) ������� i
*       Parameters:		int i - ����� ������
*       Returns:		const Qstring & - ������ ��� ��������
*       Throws:			AxiomLibException, ���� ����� ������ �����������
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class NetWrapper>
const Qstring &NeuralMultiplexer<NetWrapper>::getTrainString(int i) const {
	if (i < 0 || i >= globalTrainSize) {
		throw AxiomLibException("NeuralMultiplexer<NetWrapper>::getTrainString() : i out of bounds");
	}
	// ��������� ���������� ��������� � ���������
	int len = glNumToLocNum[i].len;
	int num = glNumToLocNum[i].num;
	// ���� ���� � ������ ������ � ��������� netArray
	/*MapType::const_iterator it = netArray.find(len);
	
	// ����� �� ������ ���������, �� �������� ��������� �� ������ ������
	if(it == netArray.end()) {
		throw AxiomLibException("NeuralMultiplexer<NetWrapper>::getTrainString() : len not found (this shouldn't happen)");
	}
	
	// ���������� ������ ������
	return it->second->getTrainString(num);*/	
	// ����� �� ������ ���������, �� �������� ��������� �� ������ ������
	if(netArray.find(len) == netArray.end()) {
		throw AxiomLibException("NeuralMultiplexer<NetWrapper>::getTrainString() : len not found (this shouldn't happen)");
	}
	
	// ���������� ������ ������
	return (netArray.find(len))->second->getTrainString(num);
}

/****************************************************************************
*						NeuralMultiplexer<NetWrapper>::supplyAuxTrainingData
*
*       Description:	������� ��������� ���� �������������� ������ ��� ��������,
*						������� ������������� ��������� ����������� ������ ��� ��������
*       Parameters:		const Qstring &s - ����������� ������
*       Returns:		-
*       Throws:			AxiomLibException, ���� �� ���� �� ���� ������� supplyTrainingData(),
*						���� ������ inp �� ��������� � �������� ��������� ����������� ������ ��� ��������
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class NetWrapper>
void NeuralMultiplexer<NetWrapper>::supplyAuxTrainingData(const Qstring &s) {
	int len = lastIndex.len;
	if(len <= 0) {
		throw AxiomLibException("NeuralMultiplexer<NetWrapper>::supplyAuxTrainingData : no train strings were supplied");
	}
	
	if(netArray.find(len) == netArray.end()) {
		throw AxiomLibException("NeuralMultiplexer<NetWrapper>::supplyAuxTrainingData : len not found (this shouldn't happen)");
	}
	
	// ���������� ���. ������ ��� �������� � ������ ����
	(netArray.find(len))->second->supplyAuxTrainingData(s);
}

/****************************************************************************
*						NeuralMultiplexer<NetWrapper>::construct
*
*       Description:	��������� ���������� � �������� �����
*       Parameters:		const Settings &settings - ���������
*						��� Settings ������ ������������� NetWrapper::construct()
*       Returns:		-
*       Throws:			-
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class NetWrapper>
template<class Settings>
void NeuralMultiplexer<NetWrapper>::construct(const Settings &settings) {
	// ������� ���������� �� �������� - ��� ������ ����
	MapTypePair val;
	BOOST_FOREACH(val, netArray) {
		val.second->construct(settings);
	}
}

/****************************************************************************
*						NeuralMultiplexer<NetWrapper>::train
*
*       Description:	������� ������� ���� �� ����� ��������������� �� ��������� �������
*       Parameters:		-
*       Returns:		-
*       Throws:			-
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class NetWrapper>
void NeuralMultiplexer<NetWrapper>::train() {
	// �������� �� ������� ����� � �������� ������ �� ���
	MapTypePair val;
	BOOST_FOREACH(val, netArray) {
		val.second->train();
	}
}

/****************************************************************************
*						NeuralMultiplexer<NetWrapper>::run
*
*       Description:	������� ��������� ������������ ��������
*						
*       Parameters:		const Qstring &inp - ���� ���� ()
*						double &p - �����, ������� ����� ���������� ���� ��� ������ (���� �� ������������)
*       Returns:		����� ��������� ������ (� ��� �������, � ������� ��� ���� ������ �������� supplyTrainingData()), 
*						���� -1, ���� ��� ������� NetWrapper � ������� ����� ������� -1
*       Throws:			-
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class NetWrapper>
int NeuralMultiplexer<NetWrapper>::run(const Qstring &inp, double &p) const {
	// ��� ������� �������� ������� �����
	MapTypePair val;
	BOOST_FOREACH(val, netArray) {
		
		Qstring actualInp;
		
		// ����� �����
		const int &len = val.first;
		// ��������� �� ������������ ������
		const NetWrapper *pNetWrapper = val.second;
		// �������� � actualInp ���������� inp �� 0 �� len
		copyVector(inp, actualInp, 0, len);
		
		// ��������� ������� ���� �� actualInp
		int res = pNetWrapper->run(actualInp, p);
		
		// ���� ��� ���-�� ���������� - ���������� ���������� ���������
		if (res >= 0) {
			// ��������� ��������� � ���������� ���������
			std::map<IndexPair, int>::const_iterator it = locNumToGlNum.find(IndexPair(std::make_pair(len, res)));
			// ����� �� ������ ���������, �� �������� ��������� �� ������ ������
			if(it == locNumToGlNum.end()) {
				throw AxiomLibException("NeuralMultiplexer<NetWrapper>::run(): pair not found (this shouldn't happen)");
			}
			return it->second;
		}
	}
	// ���� �� ���� �� ����� ������ �� �����, �� ���������� -1
	return -1;
}

};

#endif
