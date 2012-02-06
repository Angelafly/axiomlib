/****************************************************************************
*						NeuralFANNHeader.h
*
*       Description:	����, ������� ����� �������� ��� ������������� FANN
*       Author:			wictor
*       History:
*
****************************************************************************/

#ifndef NEURALFANNHEADER_H_
#define NEURALFANNHEADER_H_

extern "C" {
// ��� ������������� float ������ double ����� ����� 
// ������ doublefann.h �������� floatfann.h,
// � ����� ��������� �� ������ doublefann.c
// � �������� floatfann.c
#include "FANN/doublefann.h"
}

#define FANN_NO_DLL
#include "FANN/fann_cpp.h"

#endif
