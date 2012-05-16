CONFIG += console
CONFIG += static release
CONFIG -= debug


include(../../AxiomLibConfig.pri)

INCLUDEPATH += $$BOOST_INCLUDE $$MPICH_INCLUDE $$OTHER_INCLUDE

INCLUDEPATH += $$AXIOMLIB_INCLUDE

LIBS += $$BOOST_LIB $$MPICH_LIB $$OTHER_LIB

LIBS += $$AXIOMLIB_LIB

debug {
OBJECTS_DIR = ./obj/debug
DESTDIR = ../bin/debug
}
release {
OBJECTS_DIR = ./obj/release
DESTDIR = ../bin/release
}
