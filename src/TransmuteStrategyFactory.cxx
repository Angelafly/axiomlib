/****************************************************************************
*
*			������� ������ TransmuteStrategyFactory
*
****************************************************************************/

#include "TransmuteStrategyFactory.h"
#include "AxiomLibException.h"

using namespace AxiomLib;

/****************************************************************************
*			TransmuteStrategyFactory::create
*
*	Description:	������� ����� ��������
*	Parameters:	name - ��� ������, ��������� �������� �������
*	Returns:		Transmute* - ��������� �� ��������� �����
*	Throws:		AxiomLibException - ���� ������ ���
*	Author:		dk
*	History:
*
****************************************************************************/
TransmuteStrategy* TransmuteStrategyFactory::create (const std::string name) {
	if (name == "TransmuteSimple") {
		SimpleTransmuteStrategy* toReturn;
		toReturn = new SimpleTransmuteStrategy;
		//std::cout << "\n	TransmuteSimple is created.";
		return toReturn;
	}
	if (name == "TransmutePrecision") {
		TransmutePrecision* toReturn;
		toReturn = new TransmutePrecision;
		return toReturn;
	}
	throw AxiomLibException ("TransmuteStrategyFactory::create : can not find the requested Transmute strategy.");
}
