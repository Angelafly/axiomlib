# ����, � ������� �������� ���� � ����� � ��������� ����� ��������� ������ �������� �� AxiomLib

### ���� include'�� ###

DEFINES += BOOST_NO_CXX11_SCOPED_ENUMS
DEFINES += BOOST_NO_SCOPED_ENUMS
# ��� windows
win32 {
        AXIOMLIB_INCLUDE = F:/Workspace/Axiomlib_working_copy2/src
        QWT_INCLUDE = D:/lib/qwt-5.2.1/src
        MPICH_INCLUDE = "C:/Program Files (x86)/MPICH2/include"
        BOOST_INCLUDE = "C:/Program Files (x86)/boost/boost_1_47"
	OTHER_INCLUDE =
}
# ��� unix
unix {
        AXIOMLIB_INCLUDE = /home/bilalov/study/axiomlib/axiomlib/src
        QWT_INCLUDE = "/usr/include/qwt-qt4"
        # MPICH_INCLUDE = "/usr/include/mpich"
        BOOST_INCLUDE = "/home/bilalov/libs/boost_1_55_0"
        OTHER_INCLUDE += "/home/bilalov/libs/Shark/include"
        OTHER_INCLUDE += "/usr/include/mpi"
}

### ������������ ��������� ###

# ��� unix
unix {
    linux-icc-64|linux-icc {
            AXIOMLIB_LIB = /home/bilalov/study/axiomlib/build-AxiomLib-Desktop-Release/release/libAxiomLib.a
            SHARK_LIB = /home/bilalov/libs/Shark/lib/libshark.a
    } else {
        release {
                AXIOMLIB_LIB = /home/bilalov/study/axiomlib/build-AxiomLib-Desktop-Release/release/libAxiomLib.a
                SHARK_LIB = /home/bilalov/libs/Shark/lib/libshark.a
        } debug {
                AXIOMLIB_LIB = /home/bilalov/study/axiomlib/build-AxiomLib-Desktop-Release/release/libAxiomLib.a
                SHARK_LIB = /home/bilalov/libs/Shark/lib/libshark.a
        }
    }

    BOOST_LIBDIR = /home/bilalov/libs/boost_1_55_0/stage/lib

    MPICH_LIB = -lmpi -lgomp -lmpich
    QWT_LIB = -lqwt-qt4
    BOOST_LIB = $${BOOST_LIBDIR}/libboost_filesystem.a \
    $${BOOST_LIBDIR}/libboost_system.a \
    $${BOOST_LIBDIR}/libboost_serialization.a \
    $${BOOST_LIBDIR}/libboost_thread.a
    OTHER_LIB = -lpthread -lblas -llapack
}


### �������������� ����� ���������� ###

# ��� windows
win32 {
	QMAKE_CXXFLAGS_RELEASE += -openmp
        QMAKE_CXXFLAGS_RELEASE += -O2 -DRELEASE
        QMAKE_CXXFLAGS_DEBUG += /ZI /Od
}
# ��� unix
linux-g++|linux-g++-64 {
        QMAKE_CXXFLAGS_RELEASE += -fopenmp -O2 -std=c++11
        QMAKE_CXXFLAGS_DEBUG += -std=c++11
}

linux-icc|linux-icc-64 {
        QMAKE_CXXFLAGS += -std=c++0x -O2 -no-multibyte-chars -wd913
        QMAKE_CXXFLAGS_RELEASE += -openmp -O2
        QMAKE_LFLAGS_RELEASE += -openmp -O2
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
