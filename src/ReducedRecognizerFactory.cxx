/****************************************************************************
*
*					������� ������ ReducedRecognizerFactory
*
****************************************************************************/

#include "ReducedRecognizerFactory.h"

#include "undefs.h"
#include <boost/algorithm/string/case_conv.hpp>
#include "Defines.h"

using namespace AxiomLib;

/****************************************************************************
*					ReducedRecognizerFactory::create
*
*	Description:	������� ����� �������������
*	Parameters:		name - ��� ������, ��������� �������� �������
*	Returns:		Recognizer* - ��������� �� ��������� �����
*	Throws:			AxiomLibException - ���� ������ ���
*	Author:			dk
*	History:
*
****************************************************************************/
ReducedRecognizer* ReducedRecognizerFactory::create (const std::string &name) const {
	std::string nameCopy = boost::algorithm::to_lower_copy(name);
	if (nameCopy == "dtw" || nameCopy == "reduceddtw") {
		ReducedRecognizerDTW* toReturn;
		toReturn = new ReducedRecognizerDTW;
		return toReturn;
	}
	if (nameCopy == "metric" || nameCopy == "reducedmetric") {
		ReducedRecognizerMetric* toReturn;
		toReturn = new ReducedRecognizerMetric;
		return toReturn;
	}
	throw AxiomLibException ("ReducedRecognizerFactory::create : unknown recognizer name '" + name + "'");
}
