/****************************************************************************
*				NeuralCommon.h
*
*	Description:	����, ���������� ��������� ����� ��� ���������� ����������
*	Author:		wictor
*	History:	
*
****************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

#undef min
#undef max

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <vector>
#include <sstream>
#include <cctype>
#include <algorithm>

#include "AxiomLibException.h"
#include "Environment.h"

namespace AxiomLib {

typedef int Symbol;
// q-������ ������ (q-������ ������)
typedef boost::numeric::ublas::vector<Symbol> Qstring;
typedef boost::numeric::ublas::zero_vector<Symbol> ZeroQstring;

// ���������� ���������� �������� ����� ����� �������� ������� � ��������� ����� ������� ������
int hammingDistance(const Qstring &str1, const Qstring &str2);
double difference(const Qstring &str1, const Qstring &str2);

// �������� � V2 windowSize (�� ��������� 0) ��������� V1 � ������� startPos (�� ��������� 0)
// ���� windowSize = 0, �� ���������� ���� V1.
// ������� ������� ��������, �.�. �������������� ������ ��� � std::vector, ��� � � Qstring
template<class Vector1, class Vector2>
inline void copyVector(const Vector1 &v1, Vector2 &v2, unsigned int startPos = 0, unsigned int windowSize = 0) {
	int s1;
	
	if(windowSize + startPos > v1.size()) {
		windowSize = v1.size() - startPos;
	}
	
	s1 = windowSize ? windowSize : v1.size();
	
	if(s1 != v2.size()) {
		v2.resize(s1);
	}
	
	// �������� � v2 ����� v1 �� ����
	for (int i = (int) startPos; i < (int) startPos + s1; i++) {
		v2[i - (int) startPos] = /*(Vector2::value_type)*/ v1[i];
	}
}

// ���������� ������� min, �.�. ����� ����� ������������ ������� std::min
//#undef min

inline void getWindow(const Qstring &v1, Qstring &v2, int pos, int size) {
	v2.resize(size);
	int m1 = std::min(size, ((int)v1.size()) - pos);
	for(int i = 0; i < m1; i++) {
		v2[i] = v1[pos + i];
	}
	for(int i = m1; i < size; i++) {
		v2[i] = 0;
	}	
}

// ���������� ����� ���� � ������ num � q-����� ������� ���������
int numDigits(int q, int num);

// ���������� � str ����� num � q-����� ������� ��������� ��� q-������ ������
void numToQnaryVector(int q, int num, Qstring &str);

// ����������� q-������ ������ � �����, ������������ ��� ��� ����� � q-����� ��
int qnaryVectorToNum(int q, const Qstring &str);

// �������� q-������ ������ v ������
void makeZero(Qstring &v);

// ���� ��������� toFind � ������� v � delta-����������� pos
bool scanDelta(const std::vector<int> &v, int pos, int delta, int toFind);

// ���������� ������� �������� �� ������
template<class T>
void getVectorFromString(std::vector<T> &v, const std::string &s, const std::string &sep = "|") {
	std::string s1(s);
	std::size_t pos = s1.find_first_of(sep);
	while(pos != std::string::npos) {
		s1[pos] = '\t';
		pos = s1.find_first_of(sep);
	}
	
	std::istringstream istr(s1);
	T t;
	v.clear();
	istr.exceptions(std::ios::badbit | std::ios::failbit);
	try {
		istr>>std::skipws;
		while(!istr.eof()) {
			istr>>t;
			v.push_back(t);
		}
	} catch (...) {
		throw AxiomLibException("getVectorFromString(): syntax error");
	}
}

// ��������� �� ��������� ��������� � ������ name ��� ���������� ��������
int getBoolParamValueFromEnv(const Environment &env, bool &value, 
	const std::string &name);

};

#endif
