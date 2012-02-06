# ����, � ������� �������� ��������� ������ AxiomLib
# ���� ���� �� ���� ��������� � �����������, �� � ������� ���� (� ����������� -- �������)

# � ������ ����� ������ ���� ������ �������� ���������� 
# MPICH_INCLUDE, MPICH_LIB, BOOST_INCLUDE, BOOST_LIB, AXIOMLIB_INCLUDE, 
# AXIOMLIB_LIBDIR, AXIOMLIB_LIBNAME

### ������������ ###

CONFIG += static
#CONFIG += release
#CONFIG -= debug
#CONFIG += debug
#CONFIG -= release

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
	MPICH_INCLUDE = -I/usr/include/mpich2/
	MPICH_LIB = -lmpi -lgomp -lmpich
	BOOST_INCLUDE = "/home/wictor/boost_1_46_1/include"

	BOOST_LIBDIR = "/home/wictor/boost_1_46_1/include"
	BOOST_LIB = -lboost_filesystem -lboost_serialization -lboost_system -lboost_thread


	OTHER_INCLUDE = 
	OTHER_LIB = -lpthread
}

AXIOMLIB_INCLUDE = ./src

debug{
	AXIOMLIB_LIBDIR = ./lib/debug/
}
release {
	AXIOMLIB_LIBDIR = ./lib/release/
}

AXIOMLIB_LIBNAME = AxiomLib

AXIOMLIB_LIB = $$AXIOMLIB_LIBDIR$$AXIOMLIB_LIBNAME
