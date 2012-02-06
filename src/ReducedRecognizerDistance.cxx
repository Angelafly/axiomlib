#include "Logger.h"

#include "ReducedRecognizerDistance.h"

using namespace AxiomLib;

ReducedRecognizerDistance::ReducedRecognizerDistance()
{
	m_precision = 0.3;
}

/****************************************************************************
*                       ReducedRecognizerDistance::setParamsFromEnv
*
*       Description:    ���������������� ��������� ������� �� env
*       Parameters:     const Environment& env - �����
*       Returns:        0
*       Throws:         AxiomLibException - ���� ��� ������-�� �� ����������
*       Author:         wictor
*       History:
*
****************************************************************************/
int ReducedRecognizerDistance::setParamsFromEnv(const Environment &env) {
	if(env.getDoubleParamValue(m_precision, "precision") < 0)
	{
		Logger::getInstance()->writeDebug("ReducedRecognizerDistance::setParamsFromEnv : warning : precision"
										  " not specified in config, using default value "
										  + boost::lexical_cast<std::string>(m_precision));
	}
	
	if (m_precision < 0) {
		throw AxiomLibException("ReducedRecognizerImplBase::setParamsFromEnv : invalid precision value");
	}
}

/****************************************************************************
*                       ReducedRecognizerDistance::run
*
*       Description:    ������� ������ �������� ethalon � markUp � ������ ������������ ���������� � result
*       Parameters:     markUp - ������ ��� ������� ��� ��������
*						etalon - �������� ���������� ���������� ���������, �.�. ������� ������� ����
*						result - ������ � ������������ �������������
*       Returns:        0
*       Throws:         -
*       Author:         wictor
*       History:
*
****************************************************************************/
int ReducedRecognizerDistance::run(const std::vector<int> &markup, 
								   const std::vector<int> &templ, 
								   std::vector<int> &result) {
	if (m_precision < 0) {
		throw AxiomLibException("ReducedRecognizerImplBase::run : precision not set");
	}
	
	std::vector<double> prelimResult;
	
	run(markup, templ, prelimResult);
	
	for(int i = 0; i < prelimResult.size(); ++i) {
		if (prelimResult[i] >= 0 && prelimResult[i] <= m_precision) {
			result[i] = 1;
		} else {
			result[i] = 0;
		}
	}
	
	return 0;
}

//void ReducedRecognizerDistance::setPrecision(double newValue)
//{
//	m_precision = newValue;
//}

double AxiomLib::ReducedRecognizerDistance::precision() const
{
	return m_precision;
}
