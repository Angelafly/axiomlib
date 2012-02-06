/****************************************************************************
*						StringPlus
*
*	Description:	�������� ���������, ���������� ������ � ����� ����������,
*					��������� ������� � ��� ���������
*	Author:			dk
*	History:     	-
*
****************************************************************************/

#ifndef __AXIOMS_LIB_StringPlus_HXX
#define __AXIOMS_LIB_StringPlus_HXX

#include <vector>
#include <string>

#include <boost/serialization/nvp.hpp>

namespace AxiomLib {

struct StringPlus {
private:
	//����������, ����������� ��� ������������
	friend class boost::serialization::access;
	//������� ������������ - � ��� ������ ���� ������� �����-������,
	//��������������� ��������� �������
	template<class Archive>
	void serialize(Archive & archive, const unsigned int version)
	{
		archive & BOOST_SERIALIZATION_NVP(str);
		archive & BOOST_SERIALIZATION_NVP(doubleVal);
	}
public:
	
	// ������
	std::string str;
	// �������� ����������
	double doubleVal;
	
}; // end of struct

}; // end of namespace

#endif /* __AXIOMS_LIB_StringPlus_HXX */
