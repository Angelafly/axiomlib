/****************************************************************************
*						NeuralMultiplexer.h
*
*       Description:	���������� ������� ������ NeuralMultiplexer � ��������� IndexPair
*       Author:			wictor
*       History:
*
****************************************************************************/
#ifndef NEURALMULTIPLEXER_H_
#define NEURALMULTIPLEXER_H_

#include "NeuralCommon.h"
#include <map>
#include <boost/smart_ptr.hpp>
#include <boost/foreach.hpp>


namespace AxiomLib {

/****************************************************************************
*						IndexPair
*
*       Description:	���� (len, num), ��� ������� ���������� �������� <
*       Author:			wictor
*       History:
*
****************************************************************************/
struct IndexPair: public std::pair<int, int> {
	// ����� ��� first - ����� ������
	int &len;
	// ����� ��� second - ����� ��������� ������ ���� �����
	int &num;

	//����������� �������������� �� pair � ���������
	IndexPair(const std::pair<int, int> &p):std::pair<int, int>(p), len(first), num(second) {}
	
	// �������� < - �� ����� ��� �������������
	// ������� ������ � map � �������� key_type
	bool operator <(const IndexPair &p) {
		// ������������������ �������
		return len == p.len ? num < p.num : len < p.len;
	}
	
	// ����������� �����������
	IndexPair(const IndexPair &ip): std::pair<int, int>(ip), len(first), num(second) {}
	
	// �������� ������������
	IndexPair &operator=(const IndexPair &ip) {
		len = ip.len;
		num = ip.num;
		return *this;
	}
};

/****************************************************************************
*						NeuralMultiplexer
*
*       Description:	����� - ���������, �������� ������ ����������
*						(�� ����� �� ������ ����� ������)
*						���������� NetWrapperMinimalWithCentralStrings
*						(��. NeuralConcepts.h)
*						NetWrapper ������ ������������ NetWrapperMinimalWithCentralStrings
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class NetWrapper>
class NeuralMultiplexer {
	// ��� ������� �����
	typedef std::map<int, NetWrapper *> MapType;
	typedef std::pair<int, NetWrapper *> MapTypePair;
	// ������ �����
	MapType netArray;
	// �������������� ���������� ��������� (����� ������) � ��������� (�����, �����)
	std::vector<IndexPair> glNumToLocNum;
	// �������������� ��������� ��������� � ����������
	std::map<IndexPair, int> locNumToGlNum;
	
	// ����� ����� ��������� �����
	int globalTrainSize;
	// ��������� �������������� ��� ���������� ������ ������
	IndexPair lastIndex;
	
	// ������������ ���� ��������� netArray � �� �������
	void clearNetArray()
	{
		MapTypePair val;
		BOOST_FOREACH(val, netArray)
		{
			delete val.second;
		}
		netArray.clear();
	}
	
public:
	// ����������� ���������, ���������������� ��������� ��������
	NeuralMultiplexer():lastIndex(std::make_pair(-1, 0)), globalTrainSize(0) {}
	
	// ��������� ������ ��� ��������
	void supplyTrainingData(const Qstring &s);
	// ���������� ��������� ������ � (����������) ������� i
	const Qstring &getTrainString(int i) const;
	
	// ��������� �������������� ������ ��� ��������
	// (����������� � ��������� ����������� ������ ��� ��������)
	// ������ ������ ���� ��� �� ����� (����� AxiomLibException)
	void supplyAuxTrainingData(const Qstring &s);
	
	void supplyCentralString(const Qstring &str);
	
	// ������������� �� ��������
	template<class Settings>
	void construct(const Settings &settings);
	
	// ��������
	void train();
	
	// ������ ���������
	int run(const Qstring &inp, double &p) const;	
	
	// ������������ ����������� (����������������) ��������� ������
	int getMaxTrainDim() const { 
		return netArray.size() > 0 ? netArray.rbegin()->first : 0;
	}
	
	// ����������� ����������� (����������������) ��������� ������
	int getMinTrainDim() const { 
		return netArray.size() > 0 ? netArray.begin()->first : 0;
	}
	
	// ���������� ����� ����� ��� ��������
	int getNumTrain() const { return globalTrainSize; }
	
	// ������� ������ ��� ���������� �������������
	void clear()
	{
		clearNetArray();
		glNumToLocNum.clear();
		locNumToGlNum.clear();
		globalTrainSize = 0;
		lastIndex = std::make_pair(-1, 0);
	}
	
	~NeuralMultiplexer()
	{
		clearNetArray();
	}
};

};

#include "NeuralMultiplexer.cxx"

#endif
