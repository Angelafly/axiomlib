/****************************************************************************
*
*				������� ������ FuzzyMultiDataLearnAlgorithm
*
****************************************************************************/

#include <cmath>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/variance.hpp>

#include "FuzzyMultiDataLearnAlgorithm.h"
#include "Defines.h"

#define FUZZYMULTIDATA_AXIOMGOAL_EXPERIMENTAL

#undef SEEK_SET
#undef SEEK_END
#undef SEEK_CUR
#include "mpi.h"

using namespace AxiomLib;

#define str_AbnormalType_AxiomNaming	"_at_" // ����������� ���� ���������� ��������� ��� �������� ���� ������ � ���������� ������ ������
#define int_Axiom_Name_Max_Size			200 // ������������ ����� �������� � ����� ����� � ��������� ������� (����� ����� ���� � �������!)
#define eps						0.0000001 // ������������ � ��������� ��������, ����� ��������� ������ ��������� ����������
#define max_goal_val			10000 // ������������ �������� ������� ������� ��� ������� (������ ���������� ������� �����)

/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::formAxioms
*
*	Description:	������� ������������ ������ �� ������������ ������� ��� 
*					���� ����� ���������� ���������
*	Parameters:		-
*	Returns:		0
*	Throws:			AxiomLibException - ���� � ����� �� ���������� �������� ������
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::formAxioms (void) {
	//	����������� ����� �������� ��������
	int size, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// MPI-����������������� ���������� �� ������ ���� ����� ���������
	// ��� �����: ����������� ��� ������ ���� ���������� ��������� ������� �������� �������� ����������� �������� ������
	int from, upTo, rankFrom, rankUpTo;
	splitAbnormalTypeRange (rank, size, from, upTo, rankFrom, rankUpTo);
	// ����� ���������� � ����������� ��� ������� ������� ����� ���������� ��������� - ��� ��� �������� ����� ��������� ����������� ������� � ������� �������
	std::cout << "\n\tProcess: " << rank + 1 << " from " <<  size << " set for abnormal behaviour: " << from + 1 << " - " << upTo << " \n";
	std::cout.flush();

	if (bestECForAllAbnormalTypes.size() != (upTo - from))
		throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::matterAxiomFunc : incorrect response from internal function*.");

	if (rank == rankFrom) { // ��������� ������������ ������ ��� ������������ � ������ ���������
		std::vector < std::vector <AxiomExprPlus> > axioms;
		axioms.resize (upTo - from);
		bestAxiomsFileNames.resize(upTo - from);
		// ������ ���������� ������� ��� ����� ���������� ��������� � from �� upTo
		#pragma omp parallel for schedule(dynamic, 1)
		for (int abType = 0; abType < (upTo - from); abType++) {
			// ����� ���������� �������
			formAxioms (from + abType, bestECForAllAbnormalTypes[abType], axioms[abType], bestAxiomsFileNames[abType]);
			// �������� ����������� ������ 
			for (unsigned int u = 0; u < axioms[abType].size(); u++)
				axioms[abType][u].clear();
			axioms[abType].clear();
			// ����� �� ����� ������ ��������
			std::cout << "\n\tProcess: " << rank + 1 << " from " <<  size <<  ":\tAbnormal type\t" << abType+1 << " done.\n";
			std::cout.flush();
		}
	} else { 
		// ��� ������� �������� - ��� ������������� ����������� �������, �.�. ������ ������� ��� ��� ������. ��� ���, ������ ���:
		//  ������� splitAbnormalTypeRange �������� ����� �������, ��� ����� ������� ��������� ����� min (NumOfProcesses, NumOfClasses)
		bestAxiomsFileNames.clear();
	}

	// ����������� ������� � ������� ������, ���������� �������� ������, ���������� � ������ ������ ���������
	gatherBestAxioms ();

	// ������������� ���������
	MPI_Barrier (MPI_COMM_WORLD);

	// ���������� � ����� ������, ������������� � ���������������� �����
	updateBestAxiomsListFromEnv();

	return 0;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::gatherBestAxioms
*
*	Description:	������� ����� ������� � ���������� ������, � ������� ���� 
*					�������� �������� ������ ������, � ��������� 
*					���������, ������������ �� ������ ����� ������ ��������� ���������.
*	Parameters:		rank - ���� �������� ��������
*					size - ����� ����� ���������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::gatherBestAxioms (void) {
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (size > 1) { // ������ ��������� ������� ��� ������ ����� ���������� ��������� ����������� ���������� - ���������� ���������� ������
		// ��� ����� - �������� ������������ ���� MPI ��� �������� �������� ������ �� �� ������, � ������ ���������
		struct FileDescr {
			char name[int_Axiom_Name_Max_Size]; /* file name */
		};
		struct FileDescr fDescr[2];
		/* build datatype describing structure */
		MPI_Datatype FileDescrType;
		MPI_Aint base;
		MPI_Datatype type[2] = {MPI_CHAR, MPI_UB};
		int blocklen[2] = {int_Axiom_Name_Max_Size, 1};
		MPI_Aint disp[2];
		/* compute displacements of structure components */
		MPI_Address(fDescr, disp);
		MPI_Address(fDescr+1, disp+1);
		base = disp[0];
		for (int i = 0; i < 2; i++) disp[i] -= base;
		/* build datatype describing structure */
		MPI_Type_struct( 2, blocklen, disp, type, &FileDescrType);
		// ����������� ���� � MPI
		MPI_Type_commit(&FileDescrType);

		// ����������� ����������� ��� �������� ����������
		int vec[1];
		int *vecs;
		vecs = new int[size];
		int secSize, secMax, wholeLen;
		// ����������� ������������� ������� ������� ������������ ���������
		//  ��������� ����������� ������������� ������� ������������ ���������
		secMax = 0;
		for (unsigned int i = 0; i < bestAxiomsFileNames.size(); i++) {
			if (secMax < (int) bestAxiomsFileNames[i].size())
				secMax = (int) bestAxiomsFileNames[i].size();
		}
		//  ���������� ����������� ������������� ������� ������������ ���������
		vec[0] = secMax;
		MPI_Allgather(vec, 1, MPI_INT, vecs, 1, MPI_INT, MPI_COMM_WORLD);
		for (int u = 0; u < size; u++) {
			if (secMax < vecs[u])
				secMax = vecs[u];
		}		
		// ������� ������, ����� ������� ����� ������������ ������� ����������
		struct FileDescr *fDescrips;
		fDescrips = new struct FileDescr[secMax];
		// ����������� ������ ����������� ���� ������������ ���������� � �������������� ����� ����� ����������� ����������
		vec[0] = (int) bestAxiomsFileNames.size();
		MPI_Allgather(vec, 1, MPI_INT, vecs, 1, MPI_INT, MPI_COMM_WORLD);
		wholeLen = 0;
		for (int u = 0; u < size; u++) {
			wholeLen += vecs[u];
		}
		// ������ ������ ����������
		int allFirst = 0;
		std::vector <std::vector <std::string> > allNames;
		allNames.resize (wholeLen);
		// ���� �� ������ ����������� ������������� �������
		for (int proc = 0; proc < size; proc++) {
			for (int first = 0; first < vecs[proc]; first++) {
				// �������� ����� ������������ �� ������ ����������� ��������
				if (proc == rank) {
					for (int sec = 0; sec < (int) bestAxiomsFileNames[first].size(); sec++) {
						strncpy(fDescrips[sec].name, bestAxiomsFileNames[first][sec].c_str(), int_Axiom_Name_Max_Size - 1);
					}
					vec[0] = (int) bestAxiomsFileNames[first].size();
				}
				// �������� ����� ������������ ������
				MPI_Bcast(vec, 1, MPI_INT, proc, MPI_COMM_WORLD);
				secSize = vec[0];
				// �������� ���������� ������
				MPI_Bcast(fDescrips, secSize, FileDescrType, proc, MPI_COMM_WORLD);
				// ������������ ���������� ������
				allNames[allFirst].resize(secSize);
				for (int sec = 0; sec < secSize; sec++) {
					allNames[allFirst][sec].assign(fDescrips[sec].name);
				}
				allFirst++;
			}
		}
		// ��������� ��������������� ��� �������� ������������ ������
		delete vecs;
		// ������� �������� ������
		delete fDescrips;
		// �������� ��������� �� ���� ��������� ������ � ��������� ���������� ������
		bestAxiomsFileNames = allNames;

		/* // OLD VARIANT
		// �������� ��� ����� ������ � ���������� ������ ��������������
		unsigned int buffer[1];
		if (rank == 0) { // ���� ������� �������� ������
			struct FileDescr *fDescrips;
			unsigned int st, vecSize;
			MPI_Status status;
			for (int procNum = 1; procNum < size; procNum++) {
				// �������� ����� ��������, ���������� ������� �� �������� - ������ ����������� ���� ���������
				MPI_Recv( buffer, 1, MPI_UNSIGNED, procNum, 300 + procNum, MPI_COMM_WORLD, &status);
				vecSize = buffer[0];
				st = bestAxiomsFileNames.size();
				bestAxiomsFileNames.resize (st + vecSize);
				for (unsigned int i = 0; i < vecSize; i++, st++) {
					MPI_Recv( buffer, 1, MPI_UNSIGNED, procNum, 301 + procNum, MPI_COMM_WORLD, &status);
					fDescrips = new struct FileDescr[buffer[0]];
					// �������� ������ � ������� �������
					MPI_Recv( fDescrips, buffer[0], FileDescrType, procNum, 302 + procNum, MPI_COMM_WORLD, &status);
					// ��������� ���������� �������� - ������ ���� ������ � ��������� ������������ �������
					bestAxiomsFileNames[st].resize(buffer[0]);
					for (unsigned int t = 0; t < buffer[0]; t++) {
						bestAxiomsFileNames[st][t].assign (fDescrips[t].name);
					}
					delete fDescrips;
				}
			}
		} else { // ���� ������� �������� ������
			struct FileDescr *fDescrips;
			unsigned int curSize;
			// �������� ����� ������������, �� ������� � ���� �������� ����� ����� ���������� ������������ �������
			buffer[0] = bestAxiomsFileNames.size();
			MPI_Send( buffer, 1, MPI_UNSIGNED, 0, 300 + rank, MPI_COMM_WORLD);
			for (unsigned int i = 0; i < bestAxiomsFileNames.size(); i++) {
				curSize = bestAxiomsFileNames[i].size();
				// ���� ����� ����� � ������� ������ � ��������� ����������� - 0, �� ���������� ��
				if (curSize < 1)
					continue;
				// �������� ������ � ������� ������ � ��������� ��������� ��� �������� ���������� MPI
				fDescrips = new struct FileDescr[curSize];
				for (unsigned int t = 0; t < curSize; t++)
					strncpy(fDescrips[t].name, bestAxiomsFileNames[i][t].c_str(), int_Axiom_Name_Max_Size - 1);
				// �������� ����� ����� � �������
				buffer[0] = curSize;
				MPI_Send( buffer, 1, MPI_UNSIGNED, 0, 301 + rank, MPI_COMM_WORLD);
				// �������� ���������� ������ � ������� �������
				MPI_Send( fDescrips, curSize, FileDescrType, 0, 302 + rank, MPI_COMM_WORLD);
				delete fDescrips;
			}
		} // END OF OLD VARIANT
		*/ 
	}
	return 0;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::updateBestAxiomsListFromEnv
*
*	Description:	������� ��������� ������ ���� ������ � ���������� ������ 
*					������ - ������� �� ����������������� �����.
*	Parameters:		-
*	Returns:		0
*	Throws:			AxiomLibException - ���� 
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::updateBestAxiomsListFromEnv(void) {
	int at, index;
	std::set <std::string> :: const_iterator c_iter;
	for (c_iter = predefinedAxiomsFileName.begin(); c_iter != predefinedAxiomsFileName.end(); ++c_iter) {
		disintegrateAxiomFileName(*c_iter, at, index);
		if ((at < (int) bestAxiomsFileNames.size()) && (at >= 0)) {
			// ��������� ��� � ������
			bestAxiomsFileNames[at].push_back (*c_iter);
		} else {
			// ������ ������ - ����� ����� �������� warning - �.�. ���� �� ��������� �� � ������ ���� ���������� ���������, ���������� � ������������ ������ ������
			std::cout << "\n\tAxiom File '" << *c_iter << "' is incompatible with current dataSet properties: wrong number of abnormalType or no such number in file's name.\n";
		}
	}
	return 0;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::disintegrateAxiomFileName
*
*	Description:	������� ������������ ��� ����� �� ������������ � ������, 
*					���� �������, ��� �������� ���������.
*	Parameters:		fileName - ��� ����� ��� �������
*					at - ����� ����������� ���� ��������� 
*					index - ����������� ���������, �� ������� ���� ���� ��������.
*						���� �������� ����� ���� �� �����, � ����� ������
*						��� �������� ������������ ��� -1. 
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::disintegrateAxiomFileName(const std::string fileName, int &at, int &index) const {
	// ������������� �������� �� ���������: 
	at = -1;
	index = -1;
	// �������� �����
	std::string :: size_type posAT, first;
	// �������� ������� AbnormalType
	posAT = fileName.rfind (str_AbnormalType_AxiomNaming);
	if ( posAT != std::string::npos ) {
		at = getNearestNumber (fileName, posAT) - 1; // -1 - �.�. ��� ����������� ����� ����� �� ���� ������ ������������ 1
	}
	// �������� ������� ���������� ������
	// ���������� ����� ����������� � AT
	first = fileName.find_first_of ("0123456789", posAT);
	first = fileName.find_first_not_of ("0123456789", first);
	// ���� ������ ��������� ���������� �����
	first = fileName.find_first_of ("0123456789", first);
	// ���� ����� ����� ����� - �� ��������� ���
	if (first != std::string::npos) {
		index = getNearestNumber (fileName, first) - 1; // -1 - �.�. ��� ����������� ����� ����� �� ���� ������ ������������ 1
	}
	return 0;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::formAxioms
*
*	Description:	������� ������������ ������ �� ������������ ������� ��� 
*					��������� ���� ���������� ���������
*	Parameters:		abType - ��� ���������� ���������, ��� �������� ����������� �������
*					ecFileNames - �������� ������ � ���������� ������������ �������, 
*						������� ����� ������������ ��� ���������� ������
*					axioms - ����������� ������ ������ ������, ���������� � ������ �������
*					axiomFileNames - ����������� ������ � ������� ������ � ���������� ������ ������
*	Returns:		0
*	Throws:			AxiomLibException - ���� � ����� �� ���������� �������� ������
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::formAxioms (const int abType, std::vector <std::vector <std::vector <std::string> > > &ecFileNames, std::vector <AxiomExprPlus> &axioms, std::vector <std::string> &axiomFileNames) const {
	// ������� ����� ��������� ������������ �������
	unsigned int totSize = 0;
	for (unsigned int i = 0; i < ecFileNames.size(); i++)
		for (unsigned int j = 0; j < ecFileNames[i].size(); j++)
			totSize += ecFileNames[i][j].size();
	if (totSize < 1)
		throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::formAxioms : set of prepared elementary conditions is indeed empty.");

	// �������� ��������� ��������� ������, �� ������� ���� ��������� ������� ������ � ����������� ���������
	axioms.resize(totSize);
	unsigned int t = 0;
	std::vector <std::string> dataSetParamNames;
	dataSetParamNames = fuzzyDataSet.getParamNames();
	for (unsigned int i = 0; i < ecFileNames.size(); i++) {
		for (unsigned int j = 0; j < ecFileNames[i].size(); j++) {
			for (unsigned int k = 0; k < ecFileNames[i][j].size(); k++, t++) {
				axioms[t].expression.resize(1);
				axioms[t].expression[0].resize(1);
				// �������� ������������� �������, �� ��� �������� �� �����
				axioms[t].expression[0][0].initFromFile (axiomBaseDir, ecFileNames[i][j][k], dataSetParamNames);
			}
		}
	}
	// ��������� ��������� ���������� �� ���������
	if (this->comments)	std::cout << "\n\tCounting initial statistics.\n";
	#pragma omp parallel for schedule(dynamic, 1)
	for (int i = 0; i < (int) totSize; i++) {
		if (this->comments) {
			char buf[128];
			sprintf (buf, "\r\t\taxiomNum=%d\tby thread %d.\t", i, omp_get_thread_num());
			std::cout<<buf<<std::endl;
			//std::cout.flush();
		}
		matterAxiomFunc (axioms[i], abType);
	}
	// ���������� ���������� ��� ������ ������
	AxiomExprPlus axiomExprPlus;
	int iterNum = 0; // ������� �������� ��������� ���������� ������
	bool criteriaToContinue = true; // ���� ����������� ��������� ���������� ������
	int curSize, curBest;
	std::vector <AxiomExprPlus> bestAxiomsNew;
	// �������� ������������ �������� ������������� ������� ���������
	if (maxAxiomPopSize < 2) 
		throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::formAxioms : incorrect value of parameter 'maxAxiomPopValue'.");
	// ������ ������ ��������� ������, ������� ����� ���� �������� �� ������ ���� ��������� - ������ ������ ����������� ��� ������� ������
	bestAxiomsNew.resize(maxAxiomPopSize + round ((double)((maxAxiomPopSize - 1) * maxAxiomPopSize) / 2.0));
	// ����������� ��������� ��������� 
	while (criteriaToContinue) {
		iterNum++;
		// ���������� �� ����� ������ ��������
		if (this->comments) {
			char buf[128];
			sprintf (buf, "\r\tAbType: %d\tIteration: %d\tNumber of axioms: %d\tby thread %d.\t", abType, iterNum, axioms.size(), omp_get_thread_num());
			std::cout<<buf<<std::endl;
			//std::cout.flush();
		}
		// �������, ���� ����������, ������ ������ �� ����������� ���������� ��������
		cutDownBestAxioms (axioms);
		// ��������� - ����������� �� ����� ��������� �������
		curSize = axioms.size();
		if (curSize < 2) {
			criteriaToContinue = false;
			if(this->comments) {
				std::cout<<"Stopping because axioms vector size < 2"<<std::endl;
			}
			break;
		}
		// ��������� ������������ ������� � ��������� �� ������ ����		
		for (int i = 0; i < curSize; i++) {
			bestAxiomsNew[i] = axioms[i];
		}
		curBest = curSize;
		// ������� ����� �������� � ��������� �� �������� ������� �������
		for (unsigned int i = 0; i < (unsigned int) (curSize - 1); i++) {
			for (unsigned int j = i+1; j < (unsigned int) curSize; j++) {
				if (combineAxioms (axioms[i], axioms[j], axiomExprPlus, abType) > 0) {
					bestAxiomsNew[curBest] = axiomExprPlus;
					curBest++;
				}
			}
			axioms[i].clear();
		}
		axioms[curSize - 1].clear();
		axioms.clear();
		// ���������� ���������� �������� � ������ ��� ��������� ��������
		axioms.resize(curBest);
		for (int i = 0; i < curBest; i++) {
			axioms[i] = bestAxiomsNew[i];
			bestAxiomsNew[i].clear();
		}

		// ��������� �������� ��������
		if (curBest == curSize) {
			criteriaToContinue = false;
			if(comments) {
				std::cout<<"Stopping because no new axioms created"<<std::endl;
			}
		}
		if (iterNum > maxIterNum) {
			criteriaToContinue = false;
			if(comments) {
				std::cout<<"Stopping because maximum number of iterations exceeded"<<std::endl;
			}
			
		}
		for (int i = 0; i < curBest; i++) {
			if (axioms[i].goal > maxAxiomRes) {
				criteriaToContinue = false;
				if(comments) {
					std::cout<<"Stopping because goal value "<<
							axioms[i].goal<<" exceeds maximum goal value "<<
							maxAxiomRes<<std::endl;
				}
				break;
			}			
		}
	}

	// ������� �������������� ������������� �������
	bestAxiomsNew.clear();

	// ��������� ������ ��������� ������
	cutDownBestAxioms (axioms);

	// ��������� ������ ������� � ������
	saveBestAxiomsInFiles (abType, axioms, axiomFileNames);
	
	return 0;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::saveBestAxiomsInFiles
*
*	Description:	������� ��������� � ����� ������ �������������� �� ������ ����� �����
*	Parameters:		abnormalBehaviourType - ��� ���������� ��������� ��� �������� ����������� �������
*					axioms - ������ ������ ������
*	Returns:		0
*	Throws:			AxiomLibException - ���� ������� �������� ��������������� ��������� �� ������ ������
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::saveBestAxiomsInFiles (const int abnormalBehaviourType, std::vector <AxiomExprPlus> &axioms, std::vector <std::string> &axiomFileNames) const {
	// ��������� �������� ����� � ��������� �������
	std::string curAxiomName, tmpAxiomName;
	char tmss[10];
	curAxiomName = axiomNameTemplate;
	// ���������� � �������� ��� ���������� ���������, ��� �������� ����������� �������
	curAxiomName.append (str_AbnormalType_AxiomNaming);
	tmss[0]='\0';
	sprintf(tmss, "%d", abnormalBehaviourType + 1);
	curAxiomName.append (tmss);
	curAxiomName.append ("_");
	// �������� ������ �������� ��������� ������ ������
	std::vector <std::string> dataSetParams;
	dataSetParams = fuzzyDataSet.getParamNames();
	// �������������� ���������� ��� �������� ���� ���������� ������
	axiomFileNames.resize(axioms.size());
	// ��������� ��� �������
	for (unsigned int axNum = 0; axNum < axioms.size(); axNum++) {
		// ���������� � �������� ������ ������� �� ���������� ������������� - � ��������� ������
		tmpAxiomName = curAxiomName;
		tmss[0]='\0';
		sprintf(tmss, "%d", axNum + 1);
		tmpAxiomName.append (tmss);
		// ��������� ��������� ������� � ����
		axioms[axNum].saveAxiomToFile(axiomBaseDir, tmpAxiomName, dataSetParams);
		// ��������� ����� ������ � ��������� �������
		axiomFileNames[axNum].assign (tmpAxiomName);
	}
	return 0;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::matterAxiomFunc
*
*	Description:	������� ������� ������� ��� �������
*	Parameters:		ax - �������, ��� ������� ����������� �������� ������� �������
*					param - �������� ������ ������, �� ������� ����������� �������
*	Returns:		double - �������� ������� �������
*	Throws:			AxiomLibException - ���� ������� �������� ��������������� ��������� �� ������ ������
*	Author:			dk
*	History:
*
****************************************************************************/
double FuzzyMultiDataLearnAlgorithm::matterAxiomFunc (AxiomExprPlus &ax, const int abType) const {
	// ���������� ����������� ����������
	int numOfClasses, numOfNormalMultiTS;
	std::vector <int> numOfMultiTS, numOfNormalTS;
	std::vector < std::vector <int> > numOfTS;
	std::vector < std::vector <double> > curTS;
	int classCount, classLen, classLenCur;
	// ��������� ���������� � ������ ������
	fuzzyDataSet.getClassSize (numOfClasses, numOfMultiTS, numOfTS);
	/* // ��� �������� ������� ������, ����� ���� ��� ����������� � � ������ ��������, �.�. ��� ����� �� �����������
	if ((numOfClasses != numOfMultiTS.size()) || (numOfClasses != numOfTS.size()))
		throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::matterAxiomFunc : incorrect response from internal function.");
	if (numOfMultiTS[abType] != numOfTS[abType].size())
		throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::matterAxiomFunc : incorrect response from internal function*.");
	*/
	// ����� ������� ��������� �� ����������� ������� � ���������� ������ ����������� ������������ ����������
	std::vector <bool> dims;
	dims.resize (fuzzyDataSet.paramNamesSize(), false);
	for (unsigned int a = 0; a < ax.expression.size(); a++) {
		for (unsigned int b = 0; b < ax.expression[a].size(); b++) {
			if ((ax.expression[a][b].dimension < 0) || (ax.expression[a][b].dimension >= (int) dims.size()))
				throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::matterAxiomFunc : incomplete definition of elementary condition, it's data set dimension is out of range.");
			dims[ax.expression[a][b].dimension] = true;
		}
	}
	// ���� �� ���� ����������� �� ��������� ������� ��� ������� ������ ���������� ���������
	classCount = 0;
	classLen = 0;
	curTS.resize (fuzzyDataSet.paramNamesSize());
		
	boost::accumulators::accumulator_set<double, 
		boost::accumulators::features<boost::accumulators::tag::variance> > varianceAccumulator;
	
	// ����� ����������, ��������������� ���������� ���������, �� ������� ������� ����������
	int numOccured = 0;
	
	// ����� ����� ����������, ��������������� ���������� ���������
	int numTraj = (int) numOfTS[abType].size();
	
	for (int j = 0; j < numTraj; j++) {
		/* // ��� �������� ���������� �� ���� ��� - ����� �������� ���������
		if (numOfTS[abType][j] != fuzzyDataSet.paramNamesSize())
			throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::matterAxiomFunc : dataSet is not full - some time series contains not enought dimensions.");
		*/
		// ����������� ���������� �� ��������� ������� ��� ������� ������ ���������� ��������� (����������� ������ ������ �����������)
		classLenCur = 0;
		for (int t = 0; t < numOfTS[abType][j]; t++) {
			if (dims[t]) {
				if (!fuzzyDataSet.getTSByIndexFromClass (curTS[t], abType, j, t))
					throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::matterAxiomFunc : unable to get concrete time series from dataSet.");
				classLenCur = curTS[t].size();
			} else {
				curTS[t].clear();
			}
		}
		// ��������� ��� ����������� ������� �� ��������� ������������ ��������� ���������� � ������� ���������� ��� �������� �������� ������� �������
		int currNumOfCarriedOutItems = numOfCarriedOutItems (ax, curTS, classLenCur);
		
		if(currNumOfCarriedOutItems > 0) {
			++numOccured;
		}
		
		// ��������� ������� ������������ �� ������� ����������
		double currFreq = (double) currNumOfCarriedOutItems / (double) classLenCur;
		
		// ������������ �������� ������� ��� ����������� �������� ��������
		varianceAccumulator(currFreq);
		
		classCount += currNumOfCarriedOutItems;
		classLen += classLenCur;
	}
	// ��������� ���������� �� ���������� ���������
	
	if (classLen > 0) {
		ax.statAbnorm = (double) classCount/ (double) classLen;
		ax.statVariation = std::sqrt(
				boost::accumulators::variance(varianceAccumulator)
				);
		ax.statOccurence = (double) numOccured / (double) numTraj;
	} else {
		ax.statAbnorm = -1.0; // - ������ ������ �� ����������
		ax.statVariation = -1.0;
		ax.statOccurence = -1.0;
		std::cout << "\nWarning in  FuzzyMultiDataLearnAlgorithm::matterAxiomFunc : incorrect dstaSet request for abnormal type.\n";
	}	

	// ���� �� ����������� ����������� ���������
	fuzzyDataSet.getNormalClassSize (numOfNormalMultiTS, numOfNormalTS);
	/* // ��� ��� ����� ��� ����������� - � ����� ����������� ����� ��������� ��������
	if (numOfNormalMultiTS != numOfNormalTS.size())
		throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::matterAxiomFunc : incorrect response from internal function getNormalClassSize");
	*/
	classCount = 0;
	classLen = 0;
	for (int j = 0; j < (int) numOfNormalTS.size(); j++) {
		// ����������� ���������� �� ��������� ������� ��� ������� ������ ���������� ��������� (����������� ������ ������ �����������)
		classLenCur = 0;
		for (int t = 0; t < numOfNormalTS[j]; t++) {
			if (dims[t]) {
				if (!fuzzyDataSet.getNormalTSFromClass (curTS[t], j, t))
					throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::matterAxiomFunc : unable to get concrete normal time series from dataSet.");
				classLenCur = curTS[t].size();
			} else {
				curTS[t].clear();
			}
		}
		// �������� ���������� �� �������� ��������� ����������
		classCount += numOfCarriedOutItems (ax, curTS, classLenCur);
		classLen += classLenCur;
	}
	// ��������� ���������� �� ���������� ���������
	if (classLen > 0)
		ax.statNormal = (double) classCount/ (double) classLen;
	else {
		ax.statNormal = -1.0; // - ������ ������ �� ����������
		std::cout << "\nWarning in FuzzyMultiDataLearnAlgorithm::matterAxiomFunc: incorrect dstaSet request for normal data.\n";
	}

#ifndef FUZZYMULTIDATA_AXIOMGOAL_EXPERIMENTAL
	// ����������� �������� ������� �������
	if (ax.statAbnorm < eps)
		ax.goal = 0.0;
	else
		if (ax.statNormal < eps)
			ax.goal = max_goal_val;
		else
			ax.goal = min (ax.statAbnorm / ax.statNormal, max_goal_val);
#else
	const double delta = 0.000005;
	if (ax.statAbnorm < eps)
		ax.goal = 0.0;
	else
		ax.goal = min (ax.statAbnorm / (ax.statNormal + delta), max_goal_val);
#endif
	return ax.goal;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::numOfCarriedOutItems
*
*	Description:	������� ����� ����� � ����, �� ������� ������� �����������
*	Parameters:		ax - ����������� �������
*					row - ��� ��� ��������
*	Returns:		int - ����� �����, �� ������� ������� ���������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
inline int FuzzyMultiDataLearnAlgorithm::numOfCarriedOutItems (AxiomExpr &ax, std::vector < std::vector <double> > &row, int &rowLen) const {
	int count = 0;
	for (unsigned long i = 0; i < (unsigned long) rowLen; i++) {
		if (ax.check(i, row) > 0)
			count++;
	}
	return count;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::getPriority
*
*	Description:	��������������� ������� ���������� ������� �� �������� 
*					� ���������� ����������� � ��������� ������ ��������.
*					�������� ��������� ������������ ������� �� ���������� � �� 
*					������������.
*	Parameters:		vec - ����������� ������ ������ �� �������� ������� �������
*					pos - ������ �������� ��� �������� ����������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
inline int FuzzyMultiDataLearnAlgorithm::getPriority (std::vector <AxiomExprPlus> &vec, std::vector <unsigned int> &pos) const {
	pos.resize(vec.size()); 
	for (unsigned int i = 0; i < vec.size(); i++) 
		pos[i] = i;
	unsigned int temp;
	for (unsigned int i = vec.size() - 1; i > 0; i--) {
		for (unsigned int j = 0; j < i; j++) {
			if (vec[pos[j]].goal < vec[pos[j+1]].goal) {
				temp = pos[j+1];
				pos[j+1] = pos[j];
				pos[j] = temp;
			}
		}
	}
	return 0;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::cutDownBestAxioms
*
*	Description:	������� ��������� ��������� ������������ ������ 
*					�� ������������� � ���������� (�� ������-�����) ��������
*	Parameters:		axioms - ������ ������ ������
*	Returns:		0 - ���� �������� ��������� �� ����
*					1 - � ��������� ������
*	Throws:			AxiomLibException - ���� ���������� ���������� ������ �� �����������
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::cutDownBestAxioms (std::vector <AxiomExprPlus> &axioms) const {
	// �������� ������� ����������
	if ((maxAxiomPopSize < 1) || (axioms.size() <= (unsigned int) maxAxiomPopSize))
		return 0;
	
	// ���������� ������ �� �������� ������� �������
	std::vector <unsigned int> pos;
	getPriority (axioms, pos);
	
	// ����������� ����� ������ ������, ����������� � ���������
	unsigned int numBestToSave, numToChoose, numFrom;
	numBestToSave = (unsigned int) round (percentBestAxioms*maxAxiomPopSize);
	numToChoose = (unsigned int) maxAxiomPopSize - numBestToSave;
	
	numFrom = axioms.size() - numBestToSave;
	std::vector <bool> chosen;
	chosen.resize (numFrom, false);
	int curChoice;
	std::vector <unsigned int> toSave;
	toSave.resize (maxAxiomPopSize);
	// ������� � ������ ����������� ��������� - �������� ����� ������
	for (unsigned int i = 0; i < numBestToSave; i++) {
		toSave[i] = pos[i];
	}
	// ��������� ����� ����������
	for (unsigned int i = 0; i < numToChoose; i++) {
		curChoice = round (((double) rand() / (double) RAND_MAX) * (double) (numFrom - i - 1));
		for (unsigned int t = (unsigned int) curChoice; t < numFrom; t++)
			if (!chosen[t]) {
				chosen[t] = true;
				toSave[numBestToSave + i] = pos[numBestToSave + t];
				break;
			}
	}

	// ���������� ������� ��������� ��� ���������� ��������� �� ��������
	unsigned int tmpInt;
	for (unsigned int i = maxAxiomPopSize - 1; i > 0; i--) {
		for (unsigned int j = 0; j < i; j++) {
			if (toSave[j] < toSave[j+1]) {
				tmpInt = toSave[j+1];
				toSave[j+1] = toSave[j];
				toSave[j] = tmpInt;
			}
		}
	}
	
	// �������� ����������� ������
	// ������� ��� �������� ����� ���������� �� ������� � ������
	if ((toSave[0] + 1) < axioms.size()) {
		for (unsigned int j = toSave[0] + 1; j < axioms.size(); j++)
			axioms[j].clear();
		axioms.erase (axioms.begin() + toSave[0] + 1, axioms.end());
	}
	for (int i = 1; i < maxAxiomPopSize; i++) {
		if ((toSave[i] + 1) < toSave[i-1]) {
			// ������� ���������� ��������� ������� ������
			for (unsigned int j = toSave[i] + 1; j < toSave[i-1]; j++)
				axioms[j].clear();
			// ������� �������� ��������
			axioms.erase (axioms.begin() + toSave[i] + 1, axioms.begin() + toSave[i-1]);
		}
	}
	// ������� ��� �������� �� ������� �� ������� � ������������ ������
	if (toSave[maxAxiomPopSize-1] > 0) {
		for (unsigned int j = 0; j < toSave[maxAxiomPopSize-1]; j++) {
			axioms[j].clear();
		}
		axioms.erase (axioms.begin(), axioms.begin() + toSave[maxAxiomPopSize-1]);
	}

	return 0;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::combineAxioms
*
*	Description:	����������� ���� ������ � ���������� ���������� ��� ���������� ����� ������.
*	Parameters:		axiomFirst - ������ �� ������, �� ������ ������� ����� ��������� ����� �������� ������
*					axiomSec - ������ �� ������, �� ������ ������� ����� ��������� ����� �������� ������
*					axiomExpr - ����������� ���������� �������, � ������ ���� ������� ��������� ������ �������
*					abType - ��� ���������� ��������� ��� �������� ������������ ���������� �������
*	Returns:		0 - ���� ����� �������, ��������� �� ���� ��������� ��������� �� �����
*					>0 - ���� ��������� ������� ��������� ����� ����������������
*					-1 - ���� �� ������� �������� �������������� ��������� ������
*	Throws:			AxiomLibException - ���� ��������� ������� ��������� �� ����������� � ����������� ���������� ������
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::combineAxioms (AxiomExprPlus &axiomFirst, AxiomExprPlus &axiomSec, AxiomExprPlus &axiomExprPlus, const int abType) const {
	// �������� ����� ������
	AxiomExprPlus axiomExprAnd, axiomExprOr;
	axiomExprAnd.andExpr(axiomFirst, axiomSec);
	axiomExprOr.orExpr (axiomFirst, axiomSec);
	
	// ���������� ������� �������
	matterAxiomFunc (axiomExprAnd, abType);
	matterAxiomFunc (axiomExprOr, abType);

	// ����� ���������� ����������
	//std::cout << "\n\t andRes = " << axiomExprAnd.goal << "\torRes = " << axiomExprOr.goal << "\taxF = " << fi << "\taxS = " << se << "\tand.size() = " << axiomExprAnd.expression.size() << "\tand[0].size() = " << axiomExprAnd.expression[0].size() << "\tor.size() = " << axiomExprOr.expression.size() << "\tor[0].size() = " << axiomExprOr.expression[0].size();
	
	// ����������� ����� �� ���������� �������� ������� ������� � ����� ������
	if ((axiomExprAnd.goal >= axiomExprOr.goal) || (((axiomExprOr.goal - axiomExprAnd.goal) <= eps) && (axiomExprAnd.statAbnorm >= axiomExprOr.statAbnorm))) {
		// ������ axiomExprAnd - ����� axiomExprOr
		if ((axiomFirst.goal >= axiomSec.goal) || (((axiomSec.goal - axiomFirst.goal) <= eps) && (axiomFirst.statAbnorm >= axiomSec.statAbnorm))) {
			// ������ axiomFirst - ����� axiomSec
			if ((axiomFirst.goal >= axiomExprAnd.goal) || (((axiomExprAnd.goal - axiomFirst.goal) <= eps) && (axiomFirst.statAbnorm >= axiomExprAnd.statAbnorm))) {
				// ������ ������ �� axiomExprAnd, axiomExprOr - ����, ��� ���� �� axiomFirst, axiomSec
				return 0;
			} else {
				// ������ ������ �� axiomExprAnd, axiomExprOr - �����, ��� ������ �� axiomFirst, axiomSec
				axiomExprPlus = axiomExprAnd;
				return 1;
			}
		} else {
			// ������ axiomSec - ����� axiomFirst
			if ((axiomSec.goal >= axiomExprAnd.goal) || (((axiomExprAnd.goal - axiomSec.goal) <= eps) && (axiomSec.statAbnorm >= axiomExprAnd.statAbnorm))) {
				// ������ ������ �� axiomExprAnd, axiomExprOr - ����, ��� ���� �� axiomFirst, axiomSec
				return 0;
			} else {
				// ������ ������ �� axiomExprAnd, axiomExprOr - �����, ��� ������ �� axiomFirst, axiomSec
				axiomExprPlus = axiomExprAnd;
				return 1;
			}			
		}
	} else {
		// ������ axiomExprOr - ����� axiomExprAnd
		if ((axiomFirst.goal >= axiomSec.goal) || (((axiomSec.goal - axiomFirst.goal) <= eps) && (axiomFirst.statAbnorm >= axiomSec.statAbnorm))) {
			// ������ axiomFirst - ����� axiomSec
			if ((axiomFirst.goal >= axiomExprOr.goal) || (((axiomExprOr.goal - axiomFirst.goal) <= eps) && (axiomFirst.statAbnorm >= axiomExprOr.statAbnorm))) {
				// ������ ������ �� axiomExprAnd, axiomExprOr - ����, ��� ���� �� axiomFirst, axiomSec
				return 0;
			} else {
				// ������ ������ �� axiomExprAnd, axiomExprOr - �����, ��� ������ �� axiomFirst, axiomSec
				axiomExprPlus = axiomExprOr;
				return 2;
			}
		} else {
			// ������ axiomSec - ����� axiomFirst
			if ((axiomSec.goal >= axiomExprOr.goal) || (((axiomExprOr.goal - axiomSec.goal) <= eps) && (axiomSec.statAbnorm >= axiomExprOr.statAbnorm))) {
				// ������ ������ �� axiomExprAnd, axiomExprOr - ����, ��� ���� �� axiomFirst, axiomSec
				return 0;
			} else {
				// ������ ������ �� axiomExprAnd, axiomExprOr - �����, ��� ������ �� axiomFirst, axiomSec
				axiomExprPlus = axiomExprOr;
				return 2;
			}			
		}

	}
	
	/*if ((axiomExprAnd.goal > max (axiomFirst.goal, axiomSec.goal)) && (axiomExprAnd.goal > axiomExprOr.goal)) {
		axiomExprPlus = axiomExprAnd;
		return 1;
	}
	if (axiomExprOr.goal > max (axiomFirst.goal, axiomSec.goal)) {
		axiomExprPlus = axiomExprOr;
		return 2;
	}*/

	// ���� ������ ��� �� ������ 
	return -1;
}
