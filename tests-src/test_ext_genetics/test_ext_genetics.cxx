#include "Environment.h"
#include "ExtGenetics.h"
#include "AxiomLibException.h"
#include "Check.h"
#include <iostream>
#include <time.h>
#include <string>

#undef SEEK_SET
#undef SEEK_END
#undef SEEK_CUR
#include "mpi.h"

using namespace AxiomLib;
using namespace std;

#define comments true
#define defaultConfigFile	"./axiomlib_ext.conf"

int main (int argc, char** argv) {
	try {
		srand( (unsigned int) time(0));

		MPI_Init(&argc, &argv);
		int size, rank;
		MPI_Comm_size(MPI_COMM_WORLD, &size);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
		if (comments) {
			std::cout << "\n	MPI process  " << rank << " size " <<  size << "\n";
			//std::cout << "	To Continue press Enter.";
			std::cout.flush();
			//std::cin.get();
		}

		if (comments) {
			std::cout << "\n" << "AxiomLib test ICC / ExtGenetics.\n" << "\n";
			std::cout.flush();
		}

		// ��������� ���� ������������ �� ��������� ���������
		Environment envStart;
		envStart.readConfigParams (argc, argv);

		std::string pathToConfigFile;
		if (envStart.getStringParamValue (pathToConfigFile, "ConfigFile") < 0)
			pathToConfigFile.assign (defaultConfigFile);

		Environment env;
		env.readConfigFile(pathToConfigFile.c_str());
		env += envStart;
		
		// �������������� ������������ ��������
		string gaToUse;
		// ���������, ����� �� ��
		if (env.getStringParamValue(gaToUse, "GA") < 0) {
			cerr << "No ExtGA set in the environment.\n";
			return -1;
		}
		// ���������, ����� �� ������ ��� ��
		if (gaToUse == "ExtGenetics") {
			// ������� � ����������� ��������� ������������� ��������� ������ ������� ������
			ExtGenetics ga;
			ga.initFromEnv(env);
			// ��������� ������������ ��������
			ga.run();
			// return 0;
		} else {
			// ���� ������ �� �� ����� - ������ ��������� �� ������
			cerr << "GA '" << gaToUse << "' not implemented.\n";
			//return -1;
		}

		// ����� ���������� ��������� ������ ����������� ������� ������ - ���������� ���������� ���� ����� ���������
		MPI_Barrier (MPI_COMM_WORLD);

		// ��������� ���������� �������������� ����� ����������.
		MPI_Finalize ();

		return 0;
	}
	catch ( AxiomLibException testex ) {
		std::cerr << "Fatal toplevel AxiomLibException:\n\t";
		std::cerr << testex.error() << "\n";
		MPI_Finalize ();
	}
}
