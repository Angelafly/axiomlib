/****************************************************************************
*				IntInterval.h
*
*	Description: �����, ����������� ������������� ��������
*	Author:		wictor
*	History:	
*
****************************************************************************/
#ifndef INTINTERVAL_H
#define INTINTERVAL_H

#include <vector>

#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

#include "AxiomLibException.h"

namespace AxiomLib {
	// ����� ��� �������������� ���������
	class IntInterval {
		int left_, right_;
		
		// ����������, ����������� ��� ������������
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & archive, const unsigned int /*version*/)
		{
			archive & BOOST_SERIALIZATION_NVP(left_);
			archive & BOOST_SERIALIZATION_NVP(right_);
		}
		
	public:
		// ����������� ���������
		IntInterval() : left_(-1), right_(0) { }
		
		// �����������, �������� ��������
		IntInterval(int left, int right) : left_(left), right_(right) {
			if(left < 0 || left > right) {
				throw AxiomLib::AxiomLibException("IntInterval::IntInterval(): invalid range");
			}
		}
		
		// �����������, �������� �������� � ������������ � ������ �������
		template<class T>
		explicit IntInterval(const std::vector<T> &v) : left_(0), right_(v.size() - 1) { }
		
		// ����� �������
		int left() const { return left_; }
		
		// ������ �������
		int right() const { return right_; }
		
		// ����� ��������� (��� ������� ����������)
		int length() const { return right() - left() + 1; }
		
		// ������� ������� � �.�., � ������� ����� ��������, �
		// ������ ������ ���������
		int translateIndex(int index) const { return index - left(); }
		
		// ������� ������� ������ ��������� � ������ � �.�., �
		// ������� ����� ��������
		int reverseIndex(int clippedIndex) const { return clippedIndex + left(); }
		
		// �������� ��������� �� ���������
		bool operator==(const IntInterval &other) const {
			return left() == other.left() && right() == other.right();
		}
	};
	
}

#endif // INTINTERVAL_H
