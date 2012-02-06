/****************************************************************************
*						MultiTS
*
*	Description:	����� �������� ��������� �������� ������ ������������ ��������� ����
*	Author:		dk
*	History:     	-
*
****************************************************************************/
#ifndef __AXIOMS_LIB_MultiTS_HXX
#define __AXIOMS_LIB_MultiTS_HXX

#include <stdlib.h>
#include <vector>

#include "IntInterval.h"
    
namespace AxiomLib {

// ����������� ��������� ��� 
class MultiTS {

  public:
	
  	// ���� ��� �������� ������������ ���������� ����
	std::vector<std::vector<double> > data; // �� ����� paramNames
	
	// ���� �������� ���������� - ����� �� ���������� �� ������������ ������� ���������� ������������ � ������ ��������� ����
	std::vector<bool> validParams; // validParams[i]=0, ���� i-� �������� ���������� � ����, =1 �����
	
	// ������� ������ �� ������� ����������� ��������� ���� �� ������ ��������� � ������������ ������������������ ����������
	bool getTSByIndex(std::vector<double> &vec, int i, int left = -1, int right = -1) const { // =1 ���� ��� ok; =0 ���� ����� ��������� ���; i - ����� ���������
		// ��������� ������������ ����������
		if ((i < (int) validParams.size()) && (i >= 0)) {
		// ��������� ���� �� ������ ��������� ���
			if (validParams[i]) {
				return actualGetTsByIndex(vec, i, left, right);
			}
		}
		if ((i == -1) && (validParams.size() > 0)) {
			// ������ ��������� ���������� �������� - ������� �� ��������� �������� � ��������� ���� ������� data
			if (validParams[validParams.size() - 1]) {
				return actualGetTsByIndex(vec, validParams.size() - 1, left, right);
			}
		}
		return false;
	}

	// ������� ��������� ����������� ��������� ���� �� ������ ��������� � ������������ ������������������ ����������
	bool setTSByIndex(std::vector<double> &vec, int i) { // =1 ���� ��� ok; =0 ���� �� ������� ���������� ������
		// ��������� ������������ ����������
		if ((i < (int) validParams.size()) && (i >= 0)) {
			data[i].assign (vec.begin(), vec.end());
			validParams[i] = true;				
			return true;
		}
		if ((i == -1) && (validParams.size() > 0)) {
			// ������ ��������� ���������� �������� - ������� �� ��������� �������� � ��������� ���� ������� data
			data[validParams.size() - 1].assign (vec.begin(), vec.end());
			validParams[validParams.size() - 1] = true;
			return true;
		}
		return false;
	}
	
	// ������� ���������� ����� �������� ����� � ������ ����������
	int size(void) const {
		return (int) data.size();
	}
	
	// ������� ���������� ����� ����������
	int length() const {
		// ��������� � �������������, ��� ����� ���� ������������ ���������
		// ����� ������ �������� �����������
		for(unsigned int i = 0; i < validParams.size(); i++) {
			if(validParams[i]) {
				return (int) data[i].size();
			}
		}
		return 0;		
	}
private:
	bool actualGetTsByIndex(std::vector<double> &vec, int i, int left = -1, int right = -1) const {
		if(right < 0 || left < 0) {
			vec = data[i];
			return true;
		} else {
			vec.assign(data[i].begin() + left, data[i].begin() + right + 1);
		}
		return true;
	}
	
}; //end of class
    

// ����� ��������� ��������� ����� ��� ������ ������
typedef std::vector<MultiTS>  ClassTS;

// ����� ��������� ��������� �����
// ������ ������ - ����� ������ �� ���������� ������ (��. classNames)
typedef std::vector<ClassTS> ReferenceClassesTS;  //��� MalfunctionClassesTS

}; //  end of namespace

#endif /* __AXIOMS_LIB_MultiTS_HXX */
