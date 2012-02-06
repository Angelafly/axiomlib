/****************************************************************************
*			RefMarkupSimple
*
*	Description:	���� ��������� ��������
*	Author:		dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_REF_MARKUP_Simple_HXX
#define __AXIOMS_LIB_REF_MARKUP_Simple_HXX

#include "./elem_conditions/ElemCondition.h"
#include "ElemConditionsFactory.h"
#include "AxiomLibException.h"
#include "Environment.h"
#include "all_axioms.h"
#include "Axiom.h"
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace AxiomLib {

// ���� ������������ ������ ���������� � -1, �� �� ��������������
#define MARKUPINF = -1
	
//#define min(a,b) (((a) < (b)) ? (a) : (b))
// Defines.h ������� � .cxx

class RefMarkupSimple {

 private:
 
    // ���������� ���������� ������ - ������������ ��� ��������������� ��� �������������� ��������� �� ������ DTW, � ��������� ������ - �� ������������
    float a1, a2, b1, b2;

 protected:

    // ���� ��������
    std::vector<int> axiomTypes;
    
    // �������� �������� ������� ������� ���������, �� �������� ������� ������������ ������� ������������� ������� ������� ����� ����������
    double maxDistance;
    
    // ���� �� ��������, �������, ������ ������� � ������ ��������� �������� � � ������ �������� ����.
    std::vector <double> delPrice;
    std::vector <double> insPrice;
    std::vector <double> chgPrice;
    
    // ���������� ��� ��������, ������� � ������ �������- ������������ � ��������� DTW � ���������� ������ � ������ 
    float averDel;
    float averIns;
    float averChg;
    
    // �������� ����� �������� ������������
    bool delUse;
    bool insUse;
    bool chgUse;
    
    // ������������ �� �������� ������� ������������ ����� � ������� ���������� � ��������� DTW
    float truncationAxioms;
    float truncationRow;
    // ��������� ������ � ���������� � �������� ������� ������ ������������ ��������� ��������
    float coef1; 
    float coef2;
    
    // ������� ���������� ���������� ����� ����� ��������� �������� ��� ��������� �� ������ DTW
    inline float distance (int symbA, int symbB);
    
    // ������� �������� ����������� �� ���� � ������� DTW
    inline bool checkInRange (const int i, const int j) const;
    
    // �������� ����� �������� �������� ���������� ������������ - ���� true - �� ���������� ��������� � �������������� ��������� DTW - ����� - �� ������ �������
    bool dtwUse;
    // ���������� ��� �������� ����� �� ���������� DTW ������������
    bool useDTWwideSearch;
    bool useDTWdeepSearch;
    
    // ������� ������� ���������� ���������� DTW � ����������� ��������� ����������
    int runDistDTW (std::vector <double> &preResult, std::vector < std::vector <float> > &distMatrix);
    
    // ������� ���������� ���������� ����� ����� ����������� �������� � ��������� DTW - ����� ������ � �������
    double distDTWdeepSearch (const double curDist, int &curDeep, std::vector < std::vector <float> > &distMatrix, const int i, const int j) const;
    
    // ������� ���������� ���������� ����� ����� ����������� �������� � ��������� DTW - ����� ������ � ������
    double distDTWwideSearch (std::vector <double> &preResult, std::vector < std::vector <float> > &distMatrix) const;

 public:
 
    // �������� ������� - ����� ����������� 
    int setOut ();
    
    // ����������� � �������� ��������� �������� �� ���������
    RefMarkupSimple(void);

    // ���������� ��������� ��� ��������� � ������ ������� ������ ������������ �������
    ~RefMarkupSimple(void);

	// ������ ������� ��������� ��������
	int getCopy (std::vector<int> &axTypes) const;
    
    // ��������� ���������� �� ����������������� �����
    int setParamsFromEnv (const Environment& env);
    
    // ��������� ������ �������� ���������� dtwUse
    void setDTWUse (bool newDTWUse);
    
    // ��������� ��� �� �������� ��������
    void setDelPrice (bool newUse, double delNull, double delNoNull);
    
    // ��������� ��� �� �������� �������
    void setInsPrice (bool newUse, double insNull, double insNoNull);
    
    // ��������� ��� �� �������� ������ 
    void setChgPrice (bool newUse, double chgNullToNoNull, double chgNoNullToNoNull, double chgNoNullToNull);

    // ���������� ����� ��������� �������� ������ �������� ������
    void create (std::vector<signed int> &newAxiomTypes);
    
    // �������� ������ �� ���� ����������� ���������
    int clear ();
    
    // ������� �������� - �������� �� �������� ������� ������ � ������� ������������ �������� �� ������ ���������
    int check (const double newMaxDistance, std::vector<int> &rowAxiomTypes, long int upTo);
    
    // ��������������� ������� ���������� � ��������� ��������� ��� check
    double subcheck (const double curDist, std::vector<int> &rowAxiomTypes, long int upTo, int curEnd);
    
    // ��������������� ������� ������� ��������� ����� ���������� �������������� ���� � ��������� ��������� �� ������ ��������� DTW
    double subcheckDTW (std::vector<int> &rowAxiomTypes, long int upTo);
    
    // ������� ���������� ����� ������� � ��������� ���������
    int size (void) const;

    // ������� ������� ��������� ���� ��������
    bool operator == (const RefMarkupSimple& second) const;
    
    // ������� ������� ��������� �������� � �������� ���� 
    bool operator == (std::vector <int>& second) const;
    
    // ��������������� ������� - ��� ����������� �������� �������� � ���� set'a, � �� � ���� vector'a
    bool operator < (const RefMarkupSimple second) const {return 0;};

	// �������� ������������
    RefMarkupSimple& operator= (const RefMarkupSimple& second);
        
}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_REF_MARKUP_Simple_HXX */
