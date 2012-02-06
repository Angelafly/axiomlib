/****************************************************************************
*
*			������� ������ PopElemExt
*
****************************************************************************/

#include "PopElemExt.h"
#include "AxiomLibException.h"

using namespace AxiomLib;

/****************************************************************************
*			PopElemExt::initFromEnv
*
*	Description:	�������������� ��������� ��������� ������ ������,
*				��������� ������ �� ����������������� �����.
*				� ������:
*				    - ���������� ���������� ������ � ���������
*				    - ����������, ��� ��������� ������� ������� ���
*				      ��������� ���������
*				    - ��� �������������, ������ ����� ������ ������ ��
*				      ������
*				    - ��� �������������, �������������� ����� ������
*				      ������ ��������� �������
*	Parameters:	const Environment& env - ���������, ���������:
*				    * popSize - ������ ���������
*				    * popPreset - ������� ������, ������� ���� �������
*				      �� �������� ��������� ���������
*	Returns:		0
*	Throws:		AxiomLibException
*	Author:		gevor
*	History:
*
****************************************************************************/
int PopElemExt::initFromEnv (const Environment& env) {
	std::string axiomBaseDir;
	if (env.getStringParamValue(axiomBaseDir, "AxiomBaseDir") < 0)
		throw AxiomLibException("PopElemExt::initFromEnv : popPreset > 0 but axiom base dir is not set.");
	AxiomBase axiomBase;
	axiomBase.readAxioms(axiomBaseDir.c_str());

	// 2. ���� ������� ���������� � �������� ������ ������
	std::string axiomSetDir;
	if (env.getStringParamValue(axiomSetDir, "AxiomSetBaseDir") < 0)
		throw AxiomLibException("PopElemExt::initFromEnv : popPreset > 0 but axiom set base dir is not set.");

	// 3. �������� ��������� ������� ������
	std::set<std::string> axiomSetNames;
	if (env.getStringSetParamValue(axiomSetNames, "AxiomSet") < 0)
		throw AxiomLibException("PopElemExt::initFromEnv : popPreset > 0 but no axiom set is given.");
	int nGivenAxiomSets = (int) axiomSetNames.size();
	if (nGivenAxiomSets > 1) std::cout << "\n	PopElemExt::initFromEnv: Too many diclarations of AxiomSet.\n" ;
	if (nGivenAxiomSets < 1) throw AxiomLibException("PopElemExt::initFromEnv : no axiom set is given.");
	// �� �����, �������� ��� axiomSet
	axSet.initAxiomSetFromFile(axiomSetDir, *(axiomSetNames.begin()), axiomBase);

	double tempDB;
	if (env.getDoubleParamValue(tempDB, "precision") < 0)
		throw AxiomLibException("PopElemExt::initFromEnv : cannot find value of precision.");
	// �������� �������� prec
	if ( ((double) rand() / (double) RAND_MAX) > tempDB ) {
		prec = tempDB + (1.0 - tempDB) * ((double) rand() / (double) RAND_MAX) / 3.0;
	} else {
		prec = tempDB -  tempDB * ((double) rand() / (double) RAND_MAX) / 3.0;
	}

	return 0;
}


/****************************************************************************
*			PopElemExt::enter
*
*	Description:	��������� ��� row ������� � ������� begin ���������� 
*				�������� end �������� ������ ����� asNum �� ������� asPop 
*				� ���������� ���������� �������� � ��� markup
*	Parameters:	markup - ������ ��� ������ ����������� ��������
*				row - �������� �������� ��� ��� �������� �������� ������
*				begin - ����� �������� ���� row ������ ����� ��������
*				end - ����� ������� ���� row ������ ����� ��������
*				asNum - ����� ������� ����� � asPop ������� ��������� �������� ����
*	Returns:		0
*	Throws:		AxiomLibException - ���� asNum - out of range
*	Author:		dk
*	History:
*
****************************************************************************/
int PopElemExt::enter (std::vector<int>& markup, const std::vector<double>& row, const unsigned long begin, const unsigned long end) {
	axSet.enter (markup, row, begin, end);
	return 0;
}


/****************************************************************************
*				PopElemExt::voidStats
*	
*	Description:	����� ���������� ������� ������ ��� ������� asNum � ������� asPop
*	Parameters:	asNum - ����� ������� ������ � ������� asPop � ������� ���� ���������
*	Returns:		-
*	Throws:		AxiomLibException - ���� asNum ��������� �� ���������� ���������
*	Author:		dk
*	History:
*	
****************************************************************************/
void PopElemExt::voidStats () {
	axSet.voidStats();
}
 

/****************************************************************************
*				PopElemExt::setStats
*	
*	Description:	��������� ���������� ������� ������ ��� ������� asNum � ������� asPop
*	Parameters:	asNum - ����� ������� ������ � ������� asPop � ������� ���� ���������
*				nf - ��������������� �������� ����� ������ ������� ����
*				ns - ��������������� �������� ����� ������ ������� ����
*				g - ��������������� �������� ������� �������
*	Returns:		-
*	Throws:		AxiomLibException - ���� asNum ��������� �� ���������� ���������
*	Author:		dk
*	History:
*	
****************************************************************************/
void PopElemExt::setStats (signed int nf, signed int ns, double g) {
	axSet.setStats (nf, ns, g);
}


/****************************************************************************
*				PopElemExt::getStats
*	
*	Description:	��������� ����������, ��� ��������, ��� ������� ������ ��� ������� asNum � ������� asPop
*	Parameters:	asNum - ����� ������� ������ � ������� asPop � ������� ���� ���������
*				nf - ����������� �������� ����� ������ ������� ����
*				ns - ����������� �������� ����� ������ ������� ����
*				g - ����������� �������� ������� �������
*	Returns:		-
*	Throws:		AxiomLibException - ���� asNum ��������� �� ���������� ���������
*	Author:		dk
*	History:
*	
****************************************************************************/
void PopElemExt::getStats (signed int& nf, signed int& ns, double& g) const {
	axSet.unsafeGetStats (nf, ns, g);
}
