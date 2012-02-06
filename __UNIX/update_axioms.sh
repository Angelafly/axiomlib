#! /bin/sh
#############################################################################
#			File name: update_axioms.sh
#			File type: Unix shell script
#
#			�������� ���������� �� ��������, �������� ������� � �����
#
#	Full name:	$evde_root/.sh
#	Author:		Eugene A. Vasin
#	Version:	0.9
#
#	File history:	6 ������� 2005 - 0.9
#
#			� ��������� ����� � �������� ������� - ������ ������������
#			������, ������� ����� ���������������� ������� ���� ������
#			�����, el_axiom. ���, �������, ����������.
#			���� ��� ������ ���������, ��� ������ ������� ����� �������
#			� �������, ��� ������� ����� �������������� ������ ���
#			������ �� �������� ../src/axioms , �� ����� �������
#			������ �����������.
#
#############################################################################

# ���������� ������ (�� ������ el_axiom)
export N_AXIOMS=0
export AXIOM_HEADER_FILES
export AXIOM_CLASS_LIST

#############################################################################
#			get_axiom_name
#
#	Description:	�������� �� header-����� ��� �������, ���������� ���
#	Parameters:	$1 - ��� �����-����� � �������� ../src/axioms
#	Returns:	0 ���� ������ ��������������� �����, ��������� el_axiom
#			    ����� � ���� ������ �� ��������� ��� ������ � ���������� AXIOM_CLASS_LIST � ����������� �� 1 �������� N_AXIOMS
#			1 ���� � ����� ���� 0 ������
#	Author:		Eugene A. Vasin
#	History:
#
#############################################################################
function get_axiom_name ( ) {
  echo Processing file $FL
  
  # �������� ������, ������� ������������� �������� ������ � ������ (���� >1 - error)
  NUM_AXIOM_NAMES=`cat $1 | grep public | grep ElemCondition | wc -l`
  
  # ���������, ������� ������������� ������
  if [ $NUM_AXIOM_NAMES -eq 1 ]
  then
    AXIOM_NAME=`cat $1 | grep public | grep ElemCondition | sed 's/.*class[ ]\([a-zA-Z0-9_]*\).*/\1/'`
    echo "    Found elementary conditions: $AXIOM_NAME"
    AXIOM_CLASS_LIST="$AXIOM_CLASS_LIST $AXIOM_NAME"
    N_AXIOMS=`expr $N_AXIOMS + 1`

  elif [ $NUM_AXIOM_NAMES -eq 0 ]
  then
    echo "WARNING: No elementary conditions in file $FL."
  else
    echo "ERROR: ambiguos file $FL. More than one elementary condition in a single file. Exiting."
    exit -1
  fi

  return 0
}

# ����� �� ���� ������ � ��������
# ������������ ������� - ������� � ������ ������ �� �����������
for FL in `find ../src/elem_conditions -name \*h -type f`
do
  get_axiom_name $FL
  FL=`echo $FL|sed 's/.*\/\(.*\)/\1/'`
  AXIOM_HEADER_FILES="$AXIOM_HEADER_FILES $FL"
done

# ���, ������� �������, ����� ���������� ������������ ��������
echo Found $N_AXIOMS elementary conditions: $AXIOM_CLASS_LIST
echo "Headers in: $AXIOM_HEADER_FILES"

# ��� 1 - ��������� all_axioms.h
#cat ../src/all_axioms.h | ./all_axioms_h.awk
./all_axioms_h.awk ../src/all_axioms.h > ../src/all_axioms_new.h
mv ../src/all_axioms_new.h ../src/all_axioms.h

# ��� 2 - ��������� ������� ������
./el_axiom_factory_cxx.awk ../src/ElemConditionsFactory.cxx > ../src/ElemConditionsFactory_new.cxx
mv ../src/ElemConditionsFactory_new.cxx ../src/ElemConditionsFactory.cxx

# ��� 3 - ������ makefile
cat makefile | sed "s/EL_AXIOMS =.*/EL_AXIOMS = ElemCondition $AXIOM_CLASS_LIST/" > makefile.1
mv makefile.1 makefile