/****************************************************************************
*						ElemCondPlusStruture
*
*	Description:	�������� ��������� ������������� �������
*	Author:			dk
*	History:     	-
*
****************************************************************************/

#ifndef __AXIOMS_LIB_ElemCondPlusStruture_HXX
#define __AXIOMS_LIB_ElemCondPlusStruture_HXX

#include <map>
#include <string>

namespace AxiomLib {

class ElemCondPlusStruture {

 public:

	// �������� ������������� �������
	std::string ecName;
	
	// ����, � ������� ������������ ������� ����������� � ���������, ������������ �������
	// ���� false, �� ����������� ������� ���� � ����������
	bool sign;

	// ����������� ������ ������, �� ������� ��������� ������ ������������ �������
	int dimension;

	// ��������� ������� ������������� �������
	std::map<std::string, std::string> ecParams;


	//   ��������� ������� ������:
	// ������� ������� ���������� ����� ������
	void clear() {
		ecName.clear();
		sign = true;
		dimension = -1;
		ecParams.clear();
	}
	
}; //end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_ElemCondPlusStruture_HXX */
