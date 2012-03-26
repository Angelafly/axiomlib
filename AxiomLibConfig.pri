# ����, � ������� �������� ��������� ������ AxiomLib

# � ������ ����� ������ ���� ������ �������� ���������� 
# MPICH_INCLUDE, MPICH_LIB, BOOST_INCLUDE, BOOST_LIB, AXIOMLIB_INCLUDE, AXIOMLIB_LIB

### ���� include'�� � ��������� ###

# ��� windows
win32 {
	MPICH_INCLUDE = "C:\Program Files (x86)\MPICH2\include"
	MPICH_LIB = "C:\Program Files (x86)\MPICH2\lib\mpi.lib"

    BOOST_INCLUDE = D:\lib\boost_1_44_0
	BOOST_LIB = /LIBPATH:"D:\lib\boost_1_44_0\stage\lib"	

	OTHER_INCLUDE = 
	OTHER_LIB = 
}

# ��� unix
unix {
        MPICH_INCLUDE = /usr/include/mpich2/
	MPICH_LIB = -lmpi -lgomp -lmpich
	BOOST_INCLUDE = "/home/wictor/boost_1_46_1/include"

	BOOST_LIBDIR = "/home/wictor/boost_1_46_1/include"
	BOOST_LIB = -lboost_filesystem -lboost_serialization -lboost_system -lboost_thread


	OTHER_INCLUDE = 
	OTHER_LIB = -lpthread
}

#����� ����� ������� ��������� ���� � ���������� src
AXIOMLIB_INCLUDE = /home/mrmrmr/study/axiomlib/branches/armkor/src

unix {
#����� ����� ������� ���������� ���� � ����� libAxiomLib.a
debug{
        AXIOMLIB_LIB = /home/mrmrmr/study/axiomlib/branches/armkor/release/libAxiomLib.a
}
release {
        AXIOMLIB_LIB = /home/mrmrmr/study/axiomlib/branches/armkor/release/libAxiomLib.a
}

}

AXIOMLIB_LIBNAME = AxiomLib

#AXIOMLIB_LIB = $$AXIOMLIB_LIBDIR$$AXIOMLIB_LIBNAME

### �������������� ����� ���������� ###

# ��� windows
win32 {
	QMAKE_CXXFLAGS_RELEASE += -openmp
	QMAKE_CXXFLAGS_RELEASE += -o2 -DRELEASE
	QMAKE_CXXFLAGS_DEBUG += /ZI /Od
}
# ��� unix
linux-g++|linux-g++-64 {
        QMAKE_CXXFLAGS_RELEASE += -fopenmp -o2 -std=c++0x
}
linux-icc|linux-icc-64 {
        QMAKE_CXXFLAGS += -std=c++0x -no-multibyte-chars -wd913
        QMAKE_CXXFLAGS_RELEASE += -openmp
        QMAKE_LFLAGS_RELEASE += -openmp
}

### �������������� ����� �������� ###

# ��� windows
win32 {
	QMAKE_LFLAGS +=
	QMAKE_LFLAGS_DEBUG += /debug /incremental:yes
}

### ������� ������ ���������� warning'�� (�������� ������ ���� warn_off ���� � CONFIG)

# ��� unix
unix {
	#QMAKE_CXXFLAGS_WARN_OFF = -Wno-sign-compare -Wno-sign-conversion -Wno-unused-parameter
	QMAKE_CXXFLAGS_WARN_OFF = -Wno-unused-parameter
}

win32 {
        #���������� ���������, ��������� �� ������������ ���������,
        #������� cl ������� �������������
        QMAKE_CXXFLAGS_WARN_OFF += -D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS
}
