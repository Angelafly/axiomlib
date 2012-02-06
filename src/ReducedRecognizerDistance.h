/****************************************************************************
*					ReducedRecognizerDistance
*
*	Description:	������������� ����� � �������� ReducedRecognizer.
*                   ��������� ������� ������� run, ����������� ���������� �����
*                   �������, ����������� ���������� �� �������� �����������
*                   ���������� �� ��������� ����������
*                   (���������� ���� ������� ����� ��� �������� �������������).
*	Author:			wictor
*	History:
*
****************************************************************************/
#ifndef REDUCEDRECOGNIZERIMPLBASE_H
#define REDUCEDRECOGNIZERIMPLBASE_H

#include "ReducedRecognizer.h"

namespace AxiomLib {

class ReducedRecognizerDistance : public ReducedRecognizer
{
public:
	ReducedRecognizerDistance();
	
	// ������� ������� �������� ���������� ������
    virtual int setParamsFromEnv (const Environment& env);
    
    // ������� ������ �������� etalon � markUp � ������ ����������� ������ � result
    virtual int run (const std::vector <int> &markUp, const std::vector<int> &etalon, std::vector <int> &result);
	
	// ������� ������ �������� etalon � markUp -- � result ������� ������������ ����������
	virtual int run(const std::vector <int> &markUp, const std::vector<int> &etalon, std::vector <double> &result) = 0;
	
protected:
	
    //void setPrecision(double newValue);
	
	double precision() const;
	
private:
	
	double m_precision;	
};

}

#endif // REDUCEDRECOGNIZERIMPLBASE_H
