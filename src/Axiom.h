/***************************************************************************
*			Axiom
*
*	Description:	���������� ������������ ������
*	Author:		Eugene A. Vasin
*	History:
*
****************************************************************************/

/*!
  @file Axiom.h
  @author Eugene A. Vasin
  �onjunction of elementary axioms
 */

#ifndef __AXIOMS_LIB_AXIOM_HXX
#define __AXIOMS_LIB_AXIOM_HXX

#include "./elem_conditions/ElemCondition.h"
#include "ElemConditionsFactory.h"
#include "AxiomLibException.h"
#include "AxiomStructure.h"
#include "Round.h"
#include "all_axioms.h"

#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace AxiomLib {

/*!
   @class     Axiom
   @brief     Class for axiom
   @author    Eugene A. Vasin
 */
class Axiom {

 private:

 // ������ ���� ��������� ��� �������� ������������� �������� ������������ ������� � ��������
    /*!
      @brief Friend is needed for checking compatibility of given elemetary
             conditions in axioms
      */
    friend class Check;

 protected:

  // ������, �������� ���������� ������������ ������, �������� � ������ �������
    /// Vector contains instances of elemetary axioms, which are part of given axiom
    std::vector<ElemCondition *> elemConditions;

  // �������� �������
    /// Name of axiom
    std::string nameOfAxiom;

  // ����������� ����� ������������ ������� � �������
    /// Minimal number of elemetary conditions in axiom
    unsigned int minNumOfEC;
/// Maximal number of elemetary conditions in axiom
  // ������������ ����� ������������ ������� � �������
    unsigned int maxNumOfEC;

 public:

  // ������ �����������: ������� ������� ������
        /*!
          @brief Empty constructor: creates null vector
          Empty constructor with determinition name of axiom as no_name
          */
    Axiom(void);

  // ������ �����������: ������� ������� ������ � ���������� ��� ������ �������
    /*!
      Empty constructor with definition name of axiom as input parameter
      @brief Empty constructor: creates null vector and writes name of given axiom
      @param sName - name of given axiom
      */
    Axiom(const std::string sName);

  // �����������, ������������ ��� ���������� ��������
    /*!
      @brief destructor - deletes each dynamic generated objects
      */
    ~Axiom(void);

  // �������� ������� (���������� ���������� �� ����������, ������������ ��� ������� ��������� ������)
    /*!
      Getting name of axiom. Name is independently of parameters.
      Name is using for axiom library determition.
      @return std::string - name of axiom
      */
    std::string name (void) const;

  // ������� ������������� �������� ��������� ������
    /*!
      Function sets values of class attributes
      @return 0 - if success
      */
    signed int setAttrValue (const unsigned int min, const unsigned int max);

  // ������� ���������� �������� ��������� ������
    /*!
      Function returns values of class attributes
      @return 0 - if success
      */
    signed int getAttrValue (unsigned int &min, unsigned int &max);

  // ������� ���������� �������� ����� ����������� ������� � ������ �������
    /*!
        Function returns current number of elementary conditions in given axiom
        @return int - number of elementary conditions in given axiom
      */
    inline int size(void) const {return (int) elemConditions.size();};
    inline int getNumOfEC (void) const {return (int) elemConditions.size();};

  // ������� ������� ��� ����������� ��������� ������������ �������
    /*!
      Fuction deletes each dynamic created elementary conditions
      @brief  Fuction deletes each dynamic created elementary conditions
      @author dk
      */
    signed int clear (void);

  // ���������, ����������� �� ������ ������� �������� ����� k ���� ts
  // ����������:  1  ���� �����������
  //              0  ���� �� �����������
  //		 -1  ���� ������ ������� ������� �� ���������
    /*!
      Checks for marking point k in row ts by given compound axiom
      @author dk
      @param
      @param     k -    point in checking row
      @param     ts -   time row
      @retval 1  point is marking
      @retval 0  point is not marking
      @retval -1 if given axiom does not apply
      */
    signed int check (const unsigned long k, const std::vector<double>& ts);

  // ������ ������ ������� ��� ���������� ������������ ������ � ��������, ��������� � ������� axiom_nums
  // ��� ����� ������������ ��� �������� "���������" ������
  // ��� �������� ������������ ������������ ������ ��� ����������
    /*!

      Sets given axiom as conjunction of elementary conditions with numbers
      from vector axiom_nums. Numbers correspond to position of elementary
      condition in array of each used elementary conditions.
      This is used for creation of "random" axioms. Constructors of elementary
      conditions without parameters are used in creation.
      @author dk
      @param axiomNums - vector of numbers correspond to elementary conditions
                         which will be added in axiom.
      @return 0 if success. Otherwise - exit and throw exception
      @throw AxiomLibException 1. If creation one of elementary conditions
      failed. 2. If listed axioms from input parameters are not compatibility
      */
    signed int initAxiomByNums (const std::vector<signed int>& axiomNums);

  // ������� ����������� ������� ��������� ������� ������������� ���������
  // �� ���� ����������� �������� - ����� ���� ������� � ����������� �������
  // � ������ - ���� �� ������� ���������� ���������� ������� ����� �������  - ������� ���������� -1
    /*!

/****************************************************************************
*				Axiom::initAxiomByRand
*
*	Description:	������� ������ ������ ������� ��� ���������� ������������ ������� ��������� ��������� �������
*				���������� ������������ ��������� � ����� ������� ������ - ������ �������� ������������ ������� -
*				� ��������� �������� ���������� ������ ����������� ������������ �������,
*				����� ������������ ������� ��� ���� �������� ��� ��������
*	Parameters:	numOfEC - ����� ������������ �������, ������� ����� ������� � ������ �������
*	Returns:		-1 - ����  ������� �� �������
*				 0 - ���� ��� ������ ���������
*	Throws:		AxiomLibExeption - ���� ���������� ��������� numOfEC ����������� ������������ �������
*	Author:		dk
*	History:
*
****************************************************************************/

      */
    signed int initAxiomByRand (const int numOfEC);

  // ������ ������ ������� ��� ���������� ������������ ������ � �������, ��������� � ������� axiom_names
  // � ��������, ���� � ��������� ����� ��������
  // ��� ����� ������������ ��� �������� ������������� ��������� ������
  // ��� �������� ������������ ������������ ������ ��� ����������
    /*!

/****************************************************************************
*				Axiom::initAxiomByNames
*
*	Description:	������ ������ ������� ��� ���������� ������������ ������ � �������, �������������� � ������� axiom_nums.
*				��� ����� ������������ ��� �������� ��������� ������. ��� �������� ������������ ������������ ������������ �������  ��� ����������
*	Parameters:	axiomNums - ������ ������� �������������� ������������ ��������, ������� ����� ��������� � �������.
*	Returns:		0 - ���� ��� ��������� �������, � ��������� ������ - ����� �� ������� �� throw
*	Throws:		AxiomLibException -  1)���� �� ������� ������� ��������� �� ������������ �������, ������������� � ������
*								2)���� ������������ �� ������� ��������� ������� �� ����������
*	Author:		dk
*	History:
*
****************************************************************************/

      */
    signed int initAxiomByNames (const std::vector<std::string>& axiomNames);

  // ������������� ���������� ������������ �������, �������� � �������
    /*!

/****************************************************************************
*				Axiom::setECParameters
*
*	Description:	������������� ���������� ������������ �������, �������� � �������
*	Parameters:	setECParameters - ������ ����������� �� ���� ������������ ������� � "������ �����������"
*				"������ �����������" - ��� ����������� �� ���� ���������� ������������ ������� � �������� ���� ����������
*				�������� ���������� �������� � ���� �����, ��� �������� ( ���� ��� ����� �� ��� ����������� �������� atof() )
*	Returns:		0  - ���� ���� �� ���� ������� ��� �������
*				-2 - � ecParams �� ������� �� ������ ���������� �����������
*	Throws:		AxiomLibExeption - ���� ����������� ecParams �����
*	Author:		dk
*	History:
*
****************************************************************************/

      */
    signed int setECParameters (const std::map <std::string, std::map <std::string, std::string> >& ecParams);

  // ������� ���������� ��������� ������
    /*!

/****************************************************************************
*				Axiom::getECParameters
*
*	Description:	������� ���������� ��������� ������
*	Parameters:	ecParams - ����������� - ������ ����������� �� ���� ������������ ������� � "������ �����������"
*				"������ �����������" - ��� ����������� �� ���� ���������� ������������ ������� � �������� ���� ����������
*				�������� ���������� �������� � ���� �����, ��� �������� ( ���� ��� ����� �� ��� ����������� �������� atof() )
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/


      */
    signed int getECParameters (std::map <std::string, std::map <std::string, std::string> > &ecParams) const;

  // ������� ������������� �������� ������������� ��������� ������������� ������������� �������
    /*!
      // ������� ������������� �������� ������������� ��������� ������������� ������������� �������
      */
    signed int setParamValue (const double param, const int ecNum, const std::string &paramName);

  // ������� ���������� �������� ������������� ��������� ������������� ������������� �������
    /*!
      // ������� ���������� �������� ������������� ��������� ������������� ������������� �������

      */
    signed int getParamValue (double &param, const int ecNum, const std::string &paramName) const;

  // ������� ��������� ������� ������ ������� ���������� ������������� ������� � �������� ������� � ������� ������������ �������
    /*!

/****************************************************************************
*				Axiom::getECParamNames
*
*	Description:	������� ��������� ������� ������ ������� ���������� �������������
*				������� � �������� ������� � ������� ������������ �������
*	Parameters:	ecNum - ����� ������������� �������, � ������� ������������� ����������
*				ecParamNames - ����������� ������ ����� � ���������� ����������
*	Returns:		0
*	Throws:		AxiomLibException - ���� ����� ���������� ������������� �������
*				������� �� ������� ����� ������������ ������� ������ �������
*	Author:		dk
*	History:
*
****************************************************************************/


      */
    signed int getECParamNames(int ecNum, std::vector<std::string> &ecParamNames) const;

  // ������� ���������� ����� ������������� ������� � ������� ������� �� ��� �����
    /*!
/****************************************************************************
*				Axiom::getECNumByName
*
*	Description:	������� ���������� ����� ������������� ������� � ������� ������� �� ��� �����
*	Parameters:	ecName - ��� ������������� �������
*	Returns:		>= 0 - ���� ������������� ������� � �������� ������ �������
*				< 0 - ���� ������������� ������� � �������� ������ �� �������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/


      */
    signed int getECNumByName (std::string &ecName) const;

  // ������� ��������� ������� ������ ������� ������������ ������� ������ �������
    /*!

/****************************************************************************
*				Axiom::getECNames
*
*	Description:	������� ��������� ������� ������ ������� ������������ ������� ������ �������
*	Parameters:	ecParamNames - ����������� ������ ����� � ���������� ������������ �������
*	Returns:		������������� ����� - ����� ��������� �������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/

      */
    signed int getECNames(std::vector<std::string> &ecNames) const;

  // ������� ��������� ��������� ������ ������� � AxiomStructure
    /*!

 /****************************************************************************
*					Axiom::createAxiomStructure
*
*	Description:	������� ��������� ��������� ������ ������� ������ � AxiomStructure
*	Parameters:		as - ���������, � ������� ���������� �������� �������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/

      */
    signed int createAxiomStructure (AxiomStructure &as) const;

  // ������� �� ������ ���������� ������������ �������, �������� � ������ ������� (ectp). � ��
  // ������ ��������� ����� ������� (�������� atp)
    /*!

 /****************************************************************************
*				Axiom::transmute
*
*	Description:	������� �� ������ ���������� ������������ �������, �������� � ������ ������� (ectp)
*				� �� ������ ��������� ����� ������� (�������� atp)
*				������� �� ������ ������������� ������� ���������� (��� �������) ��������� ��������� ����������
*				������� �� ������ ������� ������������ ������, �������� ��� ��������� ���������� ������� ���������� ������������� �������
*	Parameters:	ectp - �������� ���������� ������� ������� �� ������ ������������ ������� - �������� ��������� ������� ������������� �� 0 �� 1
*				atp  - �������� ���������� ������� ������� �� ������ ������� - �������� ��������� ������� ������������� �� 0 �� 1
*	Returns:		0 - ���� ������� ������ �������, � ��������� ������ - ���� ���-�� ���� �� ��� - ������� ������ �� ����������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/


      */
    signed int transmute (const double ectp, const double atp);

  // ��������� ������ ������� � �� ����������� ����������� � ������ �������� ���� �� ����.
  // �� �������, ������ ���������
    /*!
        Comparison of given axiom with its internal parameters and other axiom
        By default comparison is accurate
        @brief Assignment operator
        @param second - second axiom to be compared
        @author dk
        @return double - real number from -1 to 1
        As the number is near 1, it could be certainty said first axiom is
        embedded in second axiom
      */
    double operator< (const Axiom& second);

  // �������� ������������
    /*!
        Assignment operator
        @brief Assignment operator
        @param second - axiom to be assignmented
        @author dk
        @return Axiom
      */
    Axiom & operator= (const Axiom& second);

    Axiom(const Axiom& other);

  // �������� ������ ����������� ������������ ���� ������
    /*!
      Checking for weak structural identity of two axioms:
      sets of elemetary conditions are congruent. There is no checking for
      ligament, because AND ligament is not supposed for the time of being.
      @brief Checking for weak structural identity of two axioms
      @author gevor
      @param  second - axiom to be examined for comparison
      @retval 0 there is no structural identity
      @retval 1 there is structural identity
      */
    signed int checkWeakEquivalence(const Axiom& second) const;

};

}; //  end of namespace

#endif /* __AXIOMS_LIB_AXIOM_HXX */
