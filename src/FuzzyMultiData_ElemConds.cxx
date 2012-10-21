/****************************************************************************
*
*				������� ������ FuzzyMultiDataLearnAlgorithm
*
****************************************************************************/

#include "FuzzyMultiDataLearnAlgorithm.h"
#include "Defines.h"

#undef SEEK_SET
#undef SEEK_END
#undef SEEK_CUR
#include "mpi.h"

//#define DEBUG_FUZZYMULTYDATA

#ifdef DEBUG_FUZZYMULTYDATA
#include <iostream>
#endif

using namespace AxiomLib;

// ����� �����������, ����������� ��� ������� ������
#define str_Dimension			"_dim_"	// ����������� ��������� ��� ����������� ����� ������������ ������������� �������
#define str_AbnormalType		"_at_"	// ����������� ���� ���������� ��������� ��� ����������� ����� ����� � ��������� ������������� �������
#define name_max_size			200 // ������������ ����� �������� � ����� ����� � ��������� ������������� ������� (����� ����� ���� � �������!)
//��� �������� ������� ����������
//#define numBestECs				20	// ������ ���������� ����������� ������������ ������� �� ������� ���� ������� ����������� ����� ������� ����� ������ ���������
//#define eps						0.0001 // ������������ � ��������� ��������, ����� ��������� ������ ��������� ����������
#define eps						0.0000001 // ������������ � ��������� ��������, ����� ��������� ������ ��������� ����������
#define max_goal_val			10000 // ������������ �������� ������� ������� ��� ������������� ������� (������ ���������� ������� �����)

#define replacementProbability	0.5 // ����������� ��������� ������ �� ��������� ������� bestECs �����, ���� ���� ������ ��� ������ � �������� ��������� � ����� �� ��������� ������� �������
#define additionalECItems		10	// > 0 !!! ����� �������������� ������, �� ������� ����������� bestECs, ���� ���� ������ ��������

/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::selectElemCond
*
*	Description:	������� ������� � ��������� ������������ �������.
*	Parameters:		void
*	Returns:		0
*	Throws:			AxiomLibException - ���� � ����� �� ���������� �������� ������
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::selectElemCond (void) {
	//	����������� ����� �������� ��������
	int size, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// MPI-����������������� ���������� �� ������ ���� ����� ���������
	// ��� �����: ����������� ��� ������ ���� ���������� ��������� ������� �������� �������� ����������� �������� ������������ ������� � ������
	int from, upTo, rankFrom, rankUpTo;
	splitAbnormalTypeRange (rank, size, from, upTo, rankFrom, rankUpTo);
	// ����� ���������� � ����������� ��� ������� ������� ����� ���������� ��������� - ��� ��� �������� ����� ��������� ����������� ������� � ������� �������
	std::cout << "\n\tProcess: " << rank + 1 << " from " <<  size << " set for abnormal behaviour: " << from + 1 << " - " << upTo << " \n";
	std::cout.flush();

	// ������ ���������� ������� ��� ����� ���������� ��������� � from �� upTo
	bestECForAllAbnormalTypes.resize (upTo - from);
#ifdef DEBUG_FUZZYMULTYDATA
		std::cout<<"Starting loop..."<<std::endl;
#endif
	#pragma omp parallel for schedule(dynamic, 1)
	for (int i = from; i < upTo; i++) {
		// ����� ���������� �������
		bestECForAllAbnormalTypes[i - from].clear();
#ifdef DEBUG_FUZZYMULTYDATA
		std::cout<<"Selecting elem cond..."<<std::endl;
#endif
		selectElemCond (i, rank, rankFrom, rankUpTo, bestECForAllAbnormalTypes[i - from]);
		// ����� �� ����� ������ ��������
		std::cout << "\n\tProcess: " << rank + 1 << " from " <<  size <<  ":\tAbnormal type\t" << i+1 << " done.\n";
		std::cout.flush();
	}
#ifdef DEBUG_FUZZYMULTYDATA
		std::cout<<"Exiting loop..."<<std::endl;
#endif
	// ����������� ������� � ������� ������, ���������� �������� ������������ �������, ���������� � ������ ���� ���������
	//gatherBestECfromProcesses (rank, size);

	// ������������� ���������
	MPI_Barrier (MPI_COMM_WORLD);

	// ���������� � ������ ���� ������ � ��������� ������������ ������� - ���, ������� ���� ������� � ���������������� �����
	updateBestECListFromEnv();
	
	return 0;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::splitAbnormalTypeRange
*
*	Description:	������� ���������� ������� ��������� ������� ������ ��� ������� ��������
*	Parameters:		rank - ���� �������� ��������
*					size - ����� ����� ���������, ������������ � ������ ���������
*					from - ����������� ��������, ����� ������� ������������ ��������� ��� ������� ��������
*					upTo - ����������� ��������, ������ ������� ������������ ��������� ��� ������� ��������
*					rankFrom - ����������� ���� ��������, ������� ��������� � ������ ����� ��������� ��������� ��� ������ ���� ���������� ���������
*					rankUpTo - ������������ ���� ��������, ������� ��������� � ������ ����� ��������� ��������� ��� ������ ���� ���������� ���������
*	Returns:		0
*	Throws:			AxiomLibException - ���� ������������ ����� ������ ����
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::splitAbnormalTypeRange (int rank, int size, int &from, int &upTo, int &rankFrom, int &rankUpTo) const {
	int numOfClasses;
	std::vector <int> numOfMultiTS;
	std::vector < std::vector <int> > numOfTS;
	// ��������� ���������� � ������ ������ - ���� ����� ����, �� ������ ������
	if (!fuzzyDataSet.getClassSize (numOfClasses, numOfMultiTS, numOfTS))
		throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::splitAbnormalTypeRange : incorrect dataSet - it is probably empty");
	return splitRange (rank, size, 0, numOfClasses, from, upTo, rankFrom, rankUpTo);
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::gatherBestECfromProcesses
*
*	Description:	������� ����� ������� � ���������� ������, � ������� ���� 
*					�������� �������� ������ ������������ �������, � ��������� 
*					���������, ������������ � ������ ����� ������ ��������� ���������.
*	Parameters:		rank - ���� �������� ��������
*					size - ����� ����� ���������, ������������ � ������ ���������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
/*int FuzzyMultiDataLearnAlgorithm::gatherBestECfromProcesses (int rank, int size) {
	if (size > 1) { // ������ ��������� ������� ��� ������ ����� ���������� ��������� ����������� ���������� - ���������� ���������� ������
		// ��� ����� - �������� ������������ ���� MPI ��� �������� �������� ������ �� �� ������, � ������ ���������
		struct FileDescr {
			char name[name_max_size]; // file name
		};
		struct FileDescr fDescr[2];
		// build datatype describing structure 
		MPI_Datatype FileDescrType;
		MPI_Aint base;
		MPI_Datatype type[2] = {MPI_CHAR, MPI_UB};
		int blocklen[2] = {name_max_size, 1};
		MPI_Aint disp[2];
		// compute displacements of structure components
		MPI_Address(fDescr, disp);
		MPI_Address(fDescr+1, disp+1);
		base = disp[0];
		for (int i = 0; i < 2; i++) disp[i] -= base;
		// build datatype describing structure
		MPI_Type_struct( 2, blocklen, disp, type, &FileDescrType);
		// ����������� ���� � MPI
		MPI_Type_commit(&FileDescrType);

		// �������� ��� ����� ������ � ���������� ������ ��������������
		unsigned int buffer[1];
		if (rank == 0) { // ���� ������� �������� ������
			struct FileDescr *fDescrips;
			unsigned int st, level1, level2, level3;
			MPI_Status status;
			for (int procNum = 1; procNum < size; procNum++) {
				// �������� ����� ��������, ���������� ������� �� �������� - ������ ����������� ���� ���������
				MPI_Recv( buffer, 1, MPI_UNSIGNED, procNum, 200 + procNum, MPI_COMM_WORLD, &status);
				level1 = buffer[0];
				st = bestECForAllAbnormalTypes.size();
				bestECForAllAbnormalTypes.resize (st + level1);
				for (unsigned int i = 0; i < level1; i++, st++) {
					// �������� ����� �������� �� ��������
					MPI_Recv( buffer, 1, MPI_UNSIGNED, procNum, 201 + procNum, MPI_COMM_WORLD, &status);
					level2 = buffer[0];
					bestECForAllAbnormalTypes[st].resize (level2);
					for (unsigned int j = 0; j < level2; j++) {
						// �������� ����� �������� �� ��������
						MPI_Recv( buffer, 1, MPI_UNSIGNED, procNum, 202 + procNum, MPI_COMM_WORLD, &status);
						level3 = buffer[0];
						bestECForAllAbnormalTypes[st][j].resize (level3);
						for (unsigned int k = 0; k < level3; k++) {
							MPI_Recv( buffer, 1, MPI_UNSIGNED, procNum, 203 + procNum, MPI_COMM_WORLD, &status);
							fDescrips = new struct FileDescr[buffer[0]];
							// �������� ������ � ������� �������
							MPI_Recv( fDescrips, buffer[0], FileDescrType, procNum, 204 + procNum, MPI_COMM_WORLD, &status);
							// ��������� ���������� �������� - ������ ���� ������ � ��������� ������������ �������
							bestECForAllAbnormalTypes[st][j][k].resize(buffer[0]);
							for (unsigned int t = 0; t < buffer[0]; t++) {
								bestECForAllAbnormalTypes[st][j][k][t].assign (fDescrips[t].name);
							}
							delete fDescrips;
						}
					}
				}
			}
		} else { // ���� ������� �������� ������
			struct FileDescr *fDescrips;
			unsigned int curSize;
			// �������� ����� ������������, �� ������� � ���� �������� ����� ����� ���������� ������������ �������
			buffer[0] = bestECForAllAbnormalTypes.size();
			MPI_Send( buffer, 1, MPI_UNSIGNED, 0, 200 + rank, MPI_COMM_WORLD);
			for (unsigned int i = 0; i < bestECForAllAbnormalTypes.size(); i++) {
				// �������� ����� �������� - ������ �� ������� ����� �� ����������� ��������
				buffer[0] = bestECForAllAbnormalTypes[i].size();
				MPI_Send( buffer, 1, MPI_UNSIGNED, 0, 201 + rank, MPI_COMM_WORLD);
				for (unsigned int j = 0; j < bestECForAllAbnormalTypes[i].size(); j++) {
					// ��������� ����������� ������� � ������� ������ - ������������ ��
					buffer[0] = bestECForAllAbnormalTypes[i][j].size();
					MPI_Send( buffer, 1, MPI_UNSIGNED, 0, 202 + rank, MPI_COMM_WORLD);
					for (unsigned int k = 0; k < bestECForAllAbnormalTypes[i][j].size(); k++) {
						curSize = bestECForAllAbnormalTypes[i][j][k].size();
						// ���� ����� ����� � ������� ������ � ��������� ����������� - 0, �� ���������� ��
						if (curSize < 1)
							continue;
						// �������� ������ � ������� ������ � ��������� ��������� ��� �������� ���������� MPI
						fDescrips = new struct FileDescr[curSize];
						for (unsigned int t = 0; t < curSize; t++)
							strncpy(fDescrips[t].name, bestECForAllAbnormalTypes[i][j][k][t].c_str(), name_max_size - 1);
						// �������� ����� ����� � �������
						buffer[0] = curSize;
						MPI_Send( buffer, 1, MPI_UNSIGNED, 0, 203 + rank, MPI_COMM_WORLD);
						// �������� ���������� ������ � ������� �������
						MPI_Send( fDescrips, curSize, FileDescrType, 0, 204 + rank, MPI_COMM_WORLD);
						delete fDescrips;	
					}					
				}
			}
		}
	}
	return 0;
}
*/

/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::getNearestNumber
*
*	Description:	������� ������� ��������� ����� �� ������ str ������� � ������� index
*	Parameters:		str - ������ ��� �������
*					index - ������ � ������, ������� � ������� ������ �����
*	Returns:		int - ��������� �������� ��� -1, ���� ����� �� �������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::getNearestNumber (const std::string str, std::string::size_type &index) const {
	int toReturn = -1;
	std::string::size_type first, last;
	first = str.find_first_of ("0123456789", index);
	if (first != std::string::npos) {
		last = str.find_first_not_of ("0123456789", first);
		if (last == std::string::npos)
			last = str.size();
		toReturn = atoi (str.substr(first, last - first).c_str());
	}
	return toReturn;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::disintegrateECFileName
*
*	Description:	������� ����������� �� ����� ����� ������ �������� ���������, 
*					���� ��� �������. ����� �������� ���������� ������������ �� 
*					���������.
*	Parameters:		fileName - ��� ����� ��� �������
*					at - ����� ����������� ���� ��������� 
*					dim - ����� ����������� �������� ������
*					vecI, vecJ - ����������� ���������, �� ������� ����� ���� 
*						���������. ��� ��������� ����� ���� �� ������, � ����� ������
*						�� �������� ������������ ��� -1. 
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::disintegrateECFileName(const std::string fileName, int &at, int &dim, int &vecI, int &vecJ) {
	// ������������� �������� �� ���������: 
	at = -1;
	dim = -1;
	vecI = -1;
	vecJ = -1;
	// �������� �����
	std::string :: size_type indexAT, indexDIM, index, first;
	// �������� ������� AbnormalType
	indexAT = fileName.rfind (str_AbnormalType);
	if ( indexAT != std::string::npos ) {
		at = getNearestNumber (fileName, indexAT) - 1; // -1 - �.�. ��� ����������� ����� ����� �� ���� ������ ������������ 1
	}
	// �������� ������� Dimension
	indexDIM = fileName.rfind (str_Dimension);
	if ( indexDIM != std::string::npos ) {
		dim = getNearestNumber (fileName, indexDIM) - 1; // -1 - �.�. ��� ����������� ����� ����� �� ���� ������ ������������ 1
	}
	// �������� ������� ���������� ��� �������
	index = max (indexAT, indexDIM);
	// ���������� ����� ����������� � DIM ��� AT
	index = fileName.find_first_of ("0123456789", index);
	index = fileName.find_first_not_of ("0123456789", index);
	first = fileName.find_first_of ("0123456789", index);
	if (first != std::string::npos) {
		vecI = getNearestNumber (fileName, first) - 1; // -1 - �.�. ��� ����������� ����� ����� �� ���� ������ ������������ 1
		index = fileName.find_first_not_of ("0123456789", first);
		first = fileName.find_first_of ("0123456789", index);
		if (first != std::string::npos) {
			vecJ = getNearestNumber (fileName, first) - 1; // -1 - �.�. ��� ����������� ����� ����� �� ���� ������ ������������ 1
		}		
	}
	return 0;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::updateBestECListFromEnv
*
*	Description:	������� ��������� ������ ���� ������ � ��������� ���������� 
*					������������ ������� ������� ������ �� ����������������� �����
*	Parameters:		-
*	Returns:		0
*	Throws:			AxiomLibException - ���� ������ �� ����������������� ����� 
*						�� ����������� � �������, ����������� ��� ������ ���������
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::updateBestECListFromEnv(void) {
	int at, dim, inx, iny;
	std::set <std::string> :: const_iterator c_iter;
	for (c_iter = predefinedECFileName.begin(); c_iter != predefinedECFileName.end(); ++c_iter) {
		disintegrateECFileName(*c_iter, at, dim, inx, iny);
		if ((at < (int) bestECForAllAbnormalTypes.size()) && (at >= 0)) {
			if ((dim < (int) bestECForAllAbnormalTypes[at].size()) && (dim >= 0)) {
				// ��������� ��� � ������
				if ((inx < (int) bestECForAllAbnormalTypes[at][dim].size()) && (inx >= 0)) {
					bestECForAllAbnormalTypes[at][dim][inx].push_back (*c_iter);
				} else {
					if (bestECForAllAbnormalTypes[at][dim].size() < 1)
						bestECForAllAbnormalTypes[at][dim].resize(1);
					bestECForAllAbnormalTypes[at][dim][0].push_back (*c_iter);
				}
			} else {
				// ������ ������ - ����� ����� �������� warning - �� �� �����������
				std::cout << "\n\tElementary Condition File '" << *c_iter << "' is incompatible with current dataSet properties: wrong number of dataSet dimension.\n";
			}
		} else {
			// ������ ������ - ����� ����� �������� warning - �� �� �����������
			//std::cout << "\n\tElementary Condition File '" << *c_iter << "' is incompatible with current dataSet properties: wrong number of abnormalType.\n";
		}
	}
	return 0;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::storeBestECs
*
*	Description:	������� ���������� ������ ������������ ������� � ��������������� 
*					�������� ������� �������
*	Parameters:		ec - ����������� ������������ �������
*					bestECs - ������ ������ ������� ���� �� ����
*	Returns:		1 - ���� ������� ���������
*					0 - ���� ������� �� ������ � ������ ������ 
*	Throws:			AxiomLibException - ���� ������� ������� �� �����������
*	Author:			dk
*	History:
*
****************************************************************************/
inline int FuzzyMultiDataLearnAlgorithm::storeBestECs (std::vector <ElemCondPlusStat> &bestECs, ElemCondPlusStat &ec, double &goal, double &statNorm, double &statAbnorm) const {
	// �������� - ������ �� ������ �� � ������ ������ �� �������
	double minVal = goal;
	int whereToSave = -1;
	double bestGoal = -1;
	for (int i = 0; i < (int) bestECs.size(); i++) {
		// ���� ���� ��������� ����, �� ������ ���������� � ����
		if (bestECs[i].elemCondition == NULL) {
			bestECs[i] = ec;
			bestECs[i].goal = goal; 
			bestECs[i].statNormal = statNorm;
			bestECs[i].statAbnorm = statAbnorm;
			return 1;
		} else {
			// ���������� ������������ �������� ������� ������� ����� bestECs
			if (bestGoal < bestECs[i].goal) 
				bestGoal = bestECs[i].goal;
			// ���������� - ���� ���������� ������ ������� ec
			if (minVal > bestECs[i].goal) {
				minVal = bestECs[i].goal;
				whereToSave = i;
			}
		}
	}
	// ���� ������� ����� ������ - ��������� ���
	if (whereToSave >= 0) {
		// ��������� ������������ ������� ������� ����� ������ ������� � ������
		bestECs[whereToSave] = ec;
		bestECs[whereToSave].goal = goal; 
		bestECs[whereToSave].statNormal = statNorm;
		bestECs[whereToSave].statAbnorm = statAbnorm;
		return 1;
	}
	// ���� ������� �� ����� ����� ������, �� ����� ��� ���� ��� - �� ������� ��� �����
	if (abs(bestGoal - goal) < eps) {
		int curSize = (int) bestECs.size();
		if (curSize < numBestECsMax) {
			// ������ ���� ��� ����������� ��������� ������ bestECs
			std::vector <ElemCondPlusStat> tmpECs;
			tmpECs.resize (curSize);
			for (int i = 0; i < curSize; i++) {
				tmpECs[i] = bestECs[i];
				bestECs[i].clear();
			}
			bestECs.clear();
			bestECs.resize (curSize + additionalECItems);
			for (int i = 0; i < curSize; i++) {
				bestECs[i] = tmpECs[i];
				tmpECs[i].clear();
			}
			bestECs[curSize] = ec;
			bestECs[curSize].goal = goal; 
			bestECs[curSize].statNormal = statNorm;
			bestECs[curSize].statAbnorm = statAbnorm;
			return 1;
		} else {
			// ������ ������ bestECs ��� ������ �� ������� 

			// ��� ������� - ����������, ������� �������� �������� - ��������� �� ������ ������������ �������
			if (((double) rand() / (double) RAND_MAX) < replacementProbability) {
				// �������� �������, ������� ���������
				whereToSave = round ( (((double) rand() + 1.0)/((double) RAND_MAX + 2.0)) * ((double) curSize) - 0.5);
				bestECs[whereToSave] = ec;
				bestECs[whereToSave].goal = goal; 
				bestECs[whereToSave].statNormal = statNorm;
				bestECs[whereToSave].statAbnorm = statAbnorm;
				return 1;
			}
			return 0;

			/*
			// ��� ��� ���� ����� ��� ������� �������, �� ��� �� �������� statAbnorm
			minVal = statAbnorm;
			for (int i = 0; i < curSize; i++) {
				if (minVal > bestECs[i].statAbnorm) {
					minVal = bestECs[i].statAbnorm;
					whereToSave = i;
				}
			}
			if (whereToSave >= 0) {
				// ��������� ������������ ������� ������� ����� ������ ������� � ������
				bestECs[whereToSave] = ec;
				bestECs[whereToSave].goal = goal; 
				bestECs[whereToSave].statNormal = statNorm;
				bestECs[whereToSave].statAbnorm = statAbnorm;
				return 1;
			} else {
				// ��� ������� - ����������, ������� �������� �������� - ��������� �� ������ ������������ �������
				if ((abs(bestECs[0].statAbnorm - statAbnorm) < eps) && (((double) rand() / (double) RAND_MAX) < replacementProbability)) {
					// �������� �������, ������� ���������
					whereToSave = round ( (((double) rand() + 1.0)/((double) RAND_MAX + 2.0)) * ((double) curSize) - 0.5);
					bestECs[whereToSave] = ec;
					bestECs[whereToSave].goal = goal; 
					bestECs[whereToSave].statNormal = statNorm;
					bestECs[whereToSave].statAbnorm = statAbnorm;
					return 1;
				}
				return 0;
			}
			*/
		}
	}
	return 0;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::numOfCarriedOutItems
*
*	Description:	������� ����� ����� � ����, �� ������� ������������ ������� �����������
*	Parameters:		ec - ����������� ������������ �������
*					row - ��� ��� �������� ������������� �������
*	Returns:		int - ����� �����, �� ������� ������������ ������� ���������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
inline int FuzzyMultiDataLearnAlgorithm::numOfCarriedOutItems (ElemCondPlus &ec, std::vector <double> &row) const {
	int count = 0;
	for (unsigned long i = 0; i < row.size(); i++) {
		if (ec.check (i, row) > 0)
			count++;
	}
	return count;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::matterECFunc
*
*	Description:	������� ������� ������� ��� ������������� �������
*	Parameters:		ec - ������������ �������
*					param - �������� ������ ������, �� ������� ����������� ������������� �������
*					abnormalBehaviourType - ��� ���������� ���������, ��� �������� ������������ ������� ������� �������
*	Returns:		double - �������� ������� �������
*	Throws:			AxiomLibException - ���� ������� �������� ��������������� ��������� �� ������ ������
*	Author:			dk
*	History:
*
****************************************************************************/
double FuzzyMultiDataLearnAlgorithm::matterECFunc (ElemCondPlusStat &ec, const int param, const int abnormalBehaviourType) const {
	int numOfClasses, numOfNormalMultiTS;
	std::vector <int> numOfMultiTS, numOfNormalTS;
	std::vector < std::vector <int> > numOfTS;
	std::vector <double> curTS;
	int classCount, classLen;
	// ��������� ���������� � ������ ������
	fuzzyDataSet.getClassSize (numOfClasses, numOfMultiTS, numOfTS);
	// ���� �� ����������� �� ��������� �������, ���������� ���������� ���������� ��������� ���� i
	/* // ��� �������� �������� �� ���� ��� - ����� �������� ���������
	if (numOfMultiTS[abnormalBehaviourType] != numOfTS[abnormalBehaviourType].size())
		throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::matterECFunc : incorrect response from internal function.");
	*/
	classCount = 0;
	classLen = 0;
	int numTS = (int) numOfTS[abnormalBehaviourType].size();
	//std::cout<<"numTS = "<<numTS<<std::endl;
	int numOccurred = 0;
	for (int j = 0; j < numTS; j++) {
		curTS.clear();
		if (fuzzyDataSet.getTSByIndexFromClass (curTS, abnormalBehaviourType, j, param)) {
			int currentClassCount = numOfCarriedOutItems (ec, curTS);
			classCount += currentClassCount;
			classLen += curTS.size();
			if(currentClassCount > 0) {
				++numOccurred;
			}
		}
	}
	//std::cout<<"numOccurred = "<<numOccurred<<std::endl;
	// ��������� ���������� �� ���������� ���������
	if (classLen > 0) {
		ec.statAbnorm = (double) classCount/ (double) classLen;
		ec.statOccurence = (double) numOccurred / (double) numTS;
	}
	else {
		ec.statAbnorm = -1.0; // - ������ ������ �� ����������
		ec.statOccurence = -1.0;
		std::cout << "\nWarning in  FuzzyMultiDataLearnAlgorithm::matterECFunc : incorrect dstaSet request for abnormal type.\n";
	}

	// ���� �� ����������� ����������� ���������
	fuzzyDataSet.getNormalClassSize (numOfNormalMultiTS, numOfNormalTS);
	/* // ��� �������� �������� �� ���� ��� - ����� �������� ���������
	if (numOfNormalMultiTS != numOfNormalTS.size())
		throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::matterECFunc : incorrect response from internal function getNormalClassSize");
	*/
	classCount = 0;
	classLen = 0;
	for (int j = 0; j < numOfNormalMultiTS; j++) {
		curTS.clear();
		if (fuzzyDataSet.getNormalTSFromClass (curTS, j, param)) {
			classCount += numOfCarriedOutItems (ec, curTS);
			classLen += curTS.size();
		}
	}
	// ��������� ���������� �� ���������� ���������
	if (classLen > 0)
		ec.statNormal = (double) classCount/ (double) classLen;
	else {
		ec.statNormal = -1.0; // - ������ ������ �� ����������
		std::cout << "\nWarning in FuzzyMultiDatadLearnAlgorithm::matterECFunc: incorrect dstaSet request.\n";
	}

	// ����������� �������� ������� �������
	if (ec.statAbnorm < eps)
		ec.goal = 0.0;
	else
		if (ec.statNormal < eps)
			ec.goal = max_goal_val;
		else
			ec.goal = min (ec.statAbnorm / ec.statNormal, max_goal_val);

	return ec.goal;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::saveBestECinFile
*
*	Description:	������� ��������� �������� ������������ ������� 
*					�� �������� ������ ������ ������� � �����.
*	Parameters:		abnormalBehaviourType - ��� ���������� ���������, 
*						��� �������� ����������� ��������� ������������ �������
*					dimen - ����������� ��������� �����, �� ������� ������
*						����������� ���������� ���������
*	Returns:		0
*	Throws:			AxiomLibException - ���� � ����� �� ���������� �������� ������
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::saveBestECinFile (const int abnormalBehaviourType, const int  dimen, std::vector <std::vector <ElemCondPlusStat> > &bestECs, std::vector <std::vector <std::string> > &ecNames) const {
	// ��������� ����� ������ ����� ����� - � ������� ��������� ������ ���� ���������� ��������� � ����� ����������� �������� ������
	char tmss[10];
	std::string fileName (ecNameTemplate);
	fileName.append (str_AbnormalType);
	sprintf(tmss, "%d", abnormalBehaviourType + 1);
	fileName.append (tmss);
	fileName.append (str_Dimension);
	tmss[0]='\0';
	sprintf(tmss, "%d", dimen + 1);
	fileName.append (tmss);
	std::string curFileName, tmpFileName;
	int numReal;
	// �������� ������ �������� ���������� ������ ������
	std::vector <std::string> dataSetParamNames;
	dataSetParamNames = fuzzyDataSet.getParamNames ();
	// ��������� �� ���� ������������ ������ ������������ ��������
	ecNames.resize (bestECs.size());
	for (unsigned int i = 0; i < bestECs.size(); i++) {
		// ������� � ��� ����� ������ ������� � ������������� ���������
		tmpFileName = fileName;
		tmpFileName.append ("_");
		tmss[0]='\0';
		sprintf(tmss, "%d", i + 1);
		tmpFileName.append (tmss);
		// �������� ������, � ������� ����� �������� ����� ������ � ���������� ������������ �������
		numReal = 0; // ����� �������� ������������ ������� - �� ��������
		for (unsigned int j = 0; j < bestECs[i].size(); j++) {
			// ���� ������� �� ���� ��������� �� ����� ������ ��������� - �� �� ��������� ���, �.�. ��� ��������
			if (bestECs[i][j].goal < -eps)
				continue;
			numReal++;
		}
		ecNames[i].resize(numReal);
		numReal = 0;
		// ��������� ���� �� ���������� � �����
		for (unsigned int j = 0; j < bestECs[i].size(); j++) {
			// ���� ������� �� ���� ��������� �� ����� ������ ��������� - �� �� ��������� ���, �.�. ��� ��������
			if (bestECs[i][j].goal < -eps)
				continue;
			// ������� � ��� ����� ������ ������ ������������� �������
			curFileName = tmpFileName;
			curFileName.append ("_");
			tmss[0]='\0';
			sprintf(tmss, "%d", j + 1);
			curFileName.append (tmss);
			// ����������� �������� ������������� �������
			bestECs[i][j].saveECToFile (axiomBaseDir, curFileName, dataSetParamNames);
			ecNames[i][numReal] = curFileName;
			numReal++;
		}
	}
	return 0;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::selectElemCond
*
*	Description:	������� ������� � ��������� ������������ �������.
*	Parameters:		void
*	Returns:		0
*	Throws:			AxiomLibException - ���� � ����� �� ���������� �������� ������
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::selectElemCond (const int abnormalBehaviourType, const int rank, const int groupRankFrom, const int groupRankUpTo, std::vector <std::vector <std::vector <std::string> > > &bestECFileNames) {
	// ���������� ����������� ������� ������, �.�. ����������� ���������� ��������� �������
	int numOfDimens = dataSetParams.size();

	// ���������� ��������� ��� �����������������
	int forOne, from, upTo, grIndex;
	forOne = (int) ((double) numOfDimens / (double) (groupRankUpTo - groupRankFrom));
	grIndex = numOfDimens - forOne * (groupRankUpTo - groupRankFrom);
	if ((rank - groupRankFrom) < grIndex) {
		from = (rank - groupRankFrom) * (forOne + 1);
		upTo = (rank - groupRankFrom + 1) * (forOne + 1);
	} else {
		from = grIndex * (forOne + 1) + (rank - groupRankFrom - grIndex) * forOne;
		upTo = grIndex * (forOne + 1) + (rank - groupRankFrom - grIndex + 1) * forOne;
	}

	// ��������� ����� ���������� ������������ ������� �� �������� ������������ �������� ������
	// �������������� �������������� ����������� ���������
	std::vector <std::vector <std::vector <ElemCondPlusStat> > > bestECs;
	std::vector <std::vector <std::vector <std::string> > > ecFileNames;
	// ������ ��������� - �� ����� ������������ �������� ������, ������� ������� ��� ��������� � ������ ��������
	bestECs.resize (upTo - from);
	ecFileNames.resize (upTo - from);
	// ������-���������� ���������� �������� ������
	#pragma omp parallel for schedule(dynamic, 1)
	for (int dim = 0; dim < (upTo - from); dim++) {
		// ����� ���������� ������������ ������� ��� ��������� ���� ���������� ��������� �� �������� ����������� ������� ������
		selectElemCond(abnormalBehaviourType, dataSetParams[from + dim], bestECs[dim]);
#ifdef DEBUG_FUZZYMULTYDATA
		std::cout << "Saving selected ecs..."<<std::endl;
#endif
		// ���������� �������� ������ ������������ ������� � �����
		saveBestECinFile(abnormalBehaviourType, from + dim, bestECs[dim], ecFileNames[dim]);
#ifdef DEBUG_FUZZYMULTYDATA
		std::cout << "Selected ecs saved"<<std::endl;
#endif
		// �������� ��������� �������� � ������������� ��������� ��� ����� �������� �����
		for (unsigned int i = 0; i < bestECs[dim].size(); i++) {
			for (unsigned int j = 0; j < bestECs[dim][i].size(); j++) {
				bestECs[dim][i][j].clear();
			}
			bestECs[dim][i].clear();
		}
		bestECs[dim].clear();
	}

	MPI_Barrier (MPI_COMM_WORLD);

	// ����������� ������ � ���������� ������, � ������� ���� �������� �������� ������ ������������ �������
	gatherBestECNames (groupRankFrom, groupRankUpTo, ecFileNames, bestECFileNames);
	
	return 0;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::selectElemCond
*
*	Description:	������� ������� � ��������� ������������ �������.
*	Parameters:		abnormalBehaviourType - ��� ���������� ��������� ��� 
*						�������� ���������� ������ ���������� ������������ �������
*					dimension - ��������� �������� ������, ��� �������� ������������
*						������ ���������� ������������ �������
*	Returns:		0
*	Throws:			AxiomLibException - ���� � ����� �� ���������� �������� ������
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::selectElemCond (const int abnormalBehaviourType, const int dimension, std::vector <std::vector <ElemCondPlusStat> > &bestECs) const {
	ElemConditionsFactory ecf;
	// ������� �������� ������ elemConds ������������� ��������� �� initialECTypes
	std::vector <ElemCondPlusStat> elemConds (initialECTypes);
	for (unsigned int i = 0; i < elemConds.size(); i++) {
		elemConds[i].dimension = dimension;
	}
	
	int elemCondsSize = elemConds.size();
	
	// ������� ����������� ��� �������� ���
	std::vector<double> teachRow;
	int numOfMultiTS;
	std::vector <int> numOfTS;
	fuzzyDataSet.getNormalClassSize (numOfMultiTS, numOfTS);
	if ((numOfTS.size() != numOfMultiTS) || (numOfMultiTS < 1))
		throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::selectElemCond : not enough data for the algorithm.");
	for (int i = 0; i < numOfMultiTS; i++) {
		if (numOfTS[i] > dimension) {
			fuzzyDataSet.getNormalTSFromClass (teachRow, i, dimension);
			if (teachRow.size() > 0)
				break;
		}
	}
	if (teachRow.size() < 1)
		throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::selectElemCond : incorrect normal data set.");
	// ������ ������� ���� ��������� ����� ������� ��������� ���������� �� ������������ � ����������� ��������� ���
	// ���������� ������ ���������� � ������������ ��������. ������� ������� ������� ������ �� �����, ����� ���������� 1000.
	/*
	if (teachRow.size() > 1000) {
		teachRow.resize (1000);
	}*/
	
	// �������� ������ �� ����� ����� ������������ ������� - ��� ������� ���� ����� �������� ����� ������ ��������� �������
	bestECs.resize (elemCondsSize);
	// ������� �������� ��� ������� �� ����� ������� - � ������� ����������� ������ ������� ������� � ����� � ��� �� ��������� ������� �������
	std::vector <int> localExtremum;
	localExtremum.resize(elemCondsSize, 0);
	// ��������� ��� ������� ���� ������������� �������, ������� ����������, ��� �������� ������� ������� ������
	std::vector <bool> goalGrowing;
	goalGrowing.resize (elemCondsSize, true);
	// ������� ���������� ��� �������� ����������� �������� ������� ������� ��� ������� ���� ������������� �������
	std::vector <double> curGoal, curStatAbnorm, curStatNorm;
	curGoal.resize (elemCondsSize);
	curStatAbnorm.resize (elemCondsSize);
	curStatNorm.resize (elemCondsSize);

	// ������ � ����� ������� ������ ������������ �������
	#pragma omp parallel for schedule(dynamic, 1)
	for (int i = 0; i < elemCondsSize; i++) {
		if (this->comments) {
			int th_id = omp_get_thread_num();
			char buf[128];
			sprintf (buf, "\r\tAbType: %d\tDimension: %d\tElem condition: %d out of %d by thread %d.\t", abnormalBehaviourType, dimension, (i+1), elemConds.size(), th_id);
			std::cout<<buf<<std::endl;
		}
		// �������� ������ �� ����� ������ ������������ �������, ������� ��������� ��� ������� �� ����� �������
		bestECs[i].resize(numBestECs);
		// ������������� ������� �����
		elemConds[i].elemCondition->setLimits (leftLimit, rightLimit);
		elemConds[i].elemCondition->setLimits (teachRow, reserve, numOfLevels);
		// ������ ������ �������� � ����� ����������
		elemConds[i].elemCondition->changeParams (0);
		// ��������� �������� ������� ������� ��� ������� � ��������� �����������
		curGoal[i] = matterECFunc (elemConds[i], dimension, abnormalBehaviourType);
		curStatAbnorm[i] = elemConds[i].statAbnorm; 
		curStatNorm[i] = elemConds[i].statNormal; 
		// ���������� ��������� ���������� ������� �������� � ���������� ������ ��� ���� ��������� ���������� �������
		while (elemConds[i].elemCondition->changeParams(1) >= 0) {
			// ������� �������� ������� ������� ��� ���������� ����������
			matterECFunc (elemConds[i], dimension, abnormalBehaviourType);
			// ����������� ���������� �� ��������� ������������ ������� � ������ ������� ����������
			if (abs(elemConds[i].goal - curGoal[i]) < eps) {
				// ������������ ������� ������ �������� ������� �������
				localExtremum[i]++;
			} else {
				// �������� ������� ������� ������������ ����������� �������� ����������
				if (elemConds[i].goal < curGoal[i]) {
					// �������� ������� ������� ����� - ����� ��������� ���� ���������� ������������ ������� �� ������������� ������� 
					if (goalGrowing[i]) {
						// ������ �������� ������� ������� �� ���������� ������� ���������� ���� �� ��������� - ��������� ������� � ����� �� ��� ����������
						// ��� ����� ���������� �� ������� ����� �������� ��������� ������� "�����", ����� ��������� ����������� ������� �������
						localExtremum[i] = round (((double) localExtremum[i]) / 2.0) + 1;
						// ���������� ��������� �����
						elemConds[i].elemCondition->changeParams (- localExtremum[i]);
						// ��������� ������� ����� ������
						storeBestECs (bestECs[i], elemConds[i], curGoal[i], curStatNorm[i], curStatAbnorm[i]);
						// ���������� ��������� �������, ������� ���������� �����
						elemConds[i].elemCondition->changeParams (localExtremum[i]);
						// ������� ��������� � ��������������� ���������� ��������� �� ��, ��� �������� ������� ������� ������ ������
						goalGrowing[i] = false;
					}
				} else {
					// �������� ������� ������� ���������� - ��������� ���
					goalGrowing[i] = true;
					curStatNorm[i] = elemConds[i].statNormal; 
					curStatAbnorm[i] = elemConds[i].statAbnorm; 
				}
				// ������������� ������� �������� �������� ������� � �������� ������� ��������� � ��������� ������� � ����� ��������� ������� �������
				curGoal[i] = elemConds[i].goal;
				localExtremum[i] = 0;
			}
		}
	}
	
	return 0;
}

/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::gatherBestECNames
*
*	Description:	������� ����� ������� � ���������� ������, � ������� ���� 
*					�������� �������� ������ ������������ �������, � ��������� 
*					���������, ������������ � ������ ����� ������ ��������� ���������.
*	Parameters:		rank - ���� �������� ��������
*					size - ����� ����� ���������, ������������ � ������ ���������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::gatherBestECNames (const int rankFrom, const int rankUpTo, std::vector <std::vector <std::vector <std::string> > > &ecNames, std::vector < std::vector < std::vector <std::string> > > &allNames) const {
	// ���� ����� ��������� � ������ ������ 2-� - �� ������� ��������� ������ �� ���� - ��� ������ ����������� ��������
	if ((rankUpTo - rankFrom) < 2) {
		allNames = ecNames;
		return 0;
	}
	// ������ ��������� ������� ��� ������ ����� ���������� ��������� ����������� ���������� - ���������� ���������� ������
	// ��� ����� - �������� ������������ ���� MPI ��� �������� �������� ������ �� �� ������, � ������ ���������
	struct FileDescr {
		char name[name_max_size];
	};
	struct FileDescr fDescr[2];
	// build datatype describing structure
	MPI_Datatype FileDescrType;
	MPI_Aint base;
	MPI_Datatype type[2] = {MPI_CHAR, MPI_UB};
	int blocklen[2] = {name_max_size, 1};
	MPI_Aint disp[2];
	// compute displacements of structure components
	MPI_Address(fDescr, disp);
	MPI_Address(fDescr+1, disp+1);
	base = disp[0];
	for (int i = 0; i < 2; i++) disp[i] -= base;
	// build datatype describing structure
	MPI_Type_struct( 2, blocklen, disp, type, &FileDescrType);
	// ����������� ���� � MPI
	MPI_Type_commit(&FileDescrType);

	// �������� ������������� - ������ �������� �������� ����� ������������ �����������
	//  ��� ����� ������� ������� ������
	MPI_Group MPI_GROUP_WORLD, subgroup;
	// ������ ���������, ������� ������ � ������������
	int ranges[1][3];
	ranges[0][0] = rankFrom;
	ranges[0][1] = rankUpTo - 1;
	ranges[0][2] = 1;
	// �������� ������ �� ����� ����������
	MPI_Comm_group(MPI_COMM_WORLD, &MPI_GROUP_WORLD);
	// �������� ����� ������ ���������, ������� ����� ����� ����� ������������ �������
	MPI_Group_range_incl(MPI_GROUP_WORLD, 1, ranges, &subgroup);
	// ����������� ����� �������� �������� � ��������� ������
	int me;
	MPI_Group_rank(subgroup, &me);

	// �������� ������������� �� ������ ��������� ����� ������ ���������
	MPI_Comm local_comm;
	MPI_Comm_create(MPI_COMM_WORLD, subgroup, &local_comm);
	int commSize, commRank;
	MPI_Comm_size (local_comm, &commSize);
	MPI_Comm_rank (local_comm, &commRank);

	struct FileDescr *fDescrips;
	int vec[1];
	int *vecs;
	vecs = new int[commSize];
	int secSize, thirdSize, thirdMax, wholeLen;
	// �������� ��������� ������ ��������� ������
	if(me != MPI_UNDEFINED) {
		// ����������� ������������� ������� ������� ������������ ���������
		//  ��������� ����������� ������������� ������� ������������ ���������
		thirdMax = 0;
		for (unsigned int i = 0; i < ecNames.size(); i++) {
			for (unsigned int j = 0; j < ecNames[i].size(); j++) {
				if (thirdMax < (int) ecNames[i][j].size())
					thirdMax = (int) ecNames[i][j].size();
			}
		}
		//  ���������� ����������� ������������� ������� ������������ ���������
		vec[0] = thirdMax;
		MPI_Allgather(vec, 1, MPI_INT, vecs, 1, MPI_INT, local_comm);
		for (int u = 0; u < commSize; u++) {
			if (thirdMax < vecs[u])
				thirdMax = vecs[u];
		}		
		// ������� ������, ����� ������� ����� ������������ ������� ����������
		fDescrips = new struct FileDescr[thirdMax];
		
		// ����������� ������ ����������� ���� ������������ ���������� � �������������� ����� ����� ����������� ����������
		vec[0] = (int) ecNames.size();
		MPI_Allgather(vec, 1, MPI_INT, vecs, 1, MPI_INT, local_comm);
		wholeLen = 0;
		for (int u = 0; u < commSize; u++) {
			wholeLen += vecs[u];
		}
		// ������ ������ ����������
		int allFirst = 0;
		allNames.resize (wholeLen);
		// ���� �� ������ ����������� ������������� �������
		for (int proc = 0; proc < commSize; proc++) {
			for (int first = 0; first < vecs[proc]; first++) {
				// �������� ����� ������������ �� ������ ����������� ��������
				if (proc == commRank) {
					vec[0] = ecNames[first].size();
				}
				MPI_Bcast(vec, 1, MPI_INT, proc, local_comm);
				secSize = vec[0];
				allNames[allFirst].resize(secSize);
				// �������� ��� ������� ������ �����������
				for (int sec = 0; sec < secSize; sec++) {
					if (proc == commRank) {				
						for (int third = 0; third < (int) ecNames[first][sec].size(); third++) {
							strncpy(fDescrips[third].name, ecNames[first][sec][third].c_str(), name_max_size - 1);
						}
						vec[0] = (int) ecNames[first][sec].size();
					}
					// �������� ����� ������������ ������
					MPI_Bcast(vec, 1, MPI_INT, proc, local_comm);
					thirdSize = vec[0];
					// �������� ���������� ������
					MPI_Bcast(fDescrips, thirdSize, FileDescrType, proc, local_comm);
					// ������������ ���������� ������
					allNames[allFirst][sec].resize(thirdSize);
					for (int third = 0; third < thirdSize; third++) {
						allNames[allFirst][sec][third].assign(fDescrips[third].name);
					}
				}
				allFirst++;
			}
		}
		// ������� �������� ������
		delete fDescrips;
		// ������� �������������� ������������
		MPI_Comm_free(&local_comm);
	} else {
		// ����� �� throw, ��� ��� ������ MPI ���� ������� �������
		throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::gatherBestECNames : incorrect work of MPI - this code should be unreacheble.");
	}
	// ��������� ��������������� ��� �������� ������������ ������
	delete vecs;
	// �������� ��������� �����
	MPI_Group_free(&MPI_GROUP_WORLD);
	MPI_Group_free(&subgroup);

	return 0;
}
