/****************************************************************************
*			NeuralActFuncStr.h
*
*	Description: ���������� �������, ������ ��� �������������� enum'�� ���
*				������� ��������� � ��������� ���������� � ���������� ��������-
*				����� � �������
*	Author:		wictor
*	History:	
*
****************************************************************************/
#ifndef NEURALACTFUNCSTR_H_
#define NEURALACTFUNCSTR_H_

#include <string>

namespace AxiomLib {

// ������������� ������� �� ������ (���������� -1 � ������ ������������� ���������,
//  ����� ����� ��������� � FANN:activation_function_enum)
int actFuncFromStr(const std::string &val);

// ������ �� ������������� ������� (���������� "" � ������ ������������� ���������)
const std::string &strFromActFunc(int val);

// ��������� �������� �� ������ (���������� -1 � ������ ������������� ���������, 
// ����� ����� ��������� � FANN:training_algorithm_enum)
int trainFromStr(const std::string &val);

// ������ �� ���������� ��������� (���������� "" � ������ ������������� ���������)
const std::string &strFromTrain(int val);

};

#endif /* ACTFUNCSTR_H_ */
