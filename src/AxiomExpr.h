/***************************************************************************
*					AxiomExpr
*
*	Description:	������� � ���� ��������� �� ������������ �������
*					(� ������������, ������������ � �����������)
*	Author:			dk
*	History:
*
****************************************************************************/

#ifndef _Axiom_Expr_HXX
#define _Axiom_Expr_HXX

#include "./elem_conditions/ElemCondition.h"
#include "ElemCondPlus.h"
#include "AxiomBase.h"
#include "ElemConditionsFactory.h"
#include "AxiomLibException.h"
#include "AxiomExprStructure.h"
#include "Round.h"
#include "all_axioms.h"

#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

namespace AxiomLib {

/*! AxiomExpr
   @class     AxiomExpr
   @brief     Class for axiom
   @author    dk

   Class AxiomExpr store axiom as expression of elementary conditions
   ( alternation, conjunction, negation )
 */
class AxiomExpr {

 private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &archive, const unsigned int /*version*/) {
        archive & BOOST_SERIALIZATION_NVP(nameOfAxiomExpr);
        archive & BOOST_SERIALIZATION_NVP(nameOfECBank);
        archive & BOOST_SERIALIZATION_NVP(expression);
    }

        // ������ ���� ��������� ��� �������� ������������� �������� ������������ ������� � ��������
        /*! This friend is needed for checking compatibility input elementary
        conditions in axioms
        */
        friend class Check;

    // ������������� ��������� ��� ��� ���������� ������
        /// Determine default name for direct instance of a class
        /// @return signed int - return 0 if determinition is ok
    signed int setDefaultName (void);

 protected:

 public:

    // �������� �������
        /// name of axiom
    std::string nameOfAxiomExpr;

    // ��� ������������� ����� ������������ �������
        /// name of bank of elementary conditions
    std::string nameOfECBank;

    // ������� �� - ��������� ��� ��. ������� � �������� ������� ������������, ������� ����� ����� - �����������.
    // ��������� � ��������� ��������� �������.
        /*! Matrix of elementary conditions - the expression above elementary
            condition. Conditions in columns are associated with conjunction,
            Columns are associated with alternation.
            Negations are in matrix structure

        */
    std::vector< std::vector <ElemCondPlus> > expression;

        // ������� ������: //

    int index;

    // �����������: �� ������� ��������� ��� �������
        /*!
            Constructor. Constructor doesn't create expression for axiom
            Empty constructor with creation name by default.
         */
    AxiomExpr(void);

    // �����������: �� ������� ��������� ��� �������, ���������� ����� ��� �������
        /*!
            Constructor. Constructor doesn't create expression for axiom.
            Constructor record new axiom name.
            @param sName - new axiom name
         */
    AxiomExpr(const std::string sName);

    /*!
        Constructor. Constructor creates expression for axiom.
        @param ec - elementary conditions
     */
    AxiomExpr(const ElemCondPlus& ec);

    // �����������, ������������ ��� ���������� ��������
        /*!
          Class destructor
          destroys each direct instances,
          delete each dynamic generated objects
          */
    ~AxiomExpr(void);

    AxiomExpr *clone() const { return new AxiomExpr(*this); }

    // ���������� �������� ������� (���������� ���������� �� ����������, ������������ ��� ������� ��������� ������)
        /*!
          Name of axiom is irrespective of each parameters.
          Name of axiom is used for definition of axiom library.
          @return std::string - name of axiom
          */
    std::string name (void) const;

    // ������� ���������� �������� ����� ����������� ������� � ������ �������
        /*!
          Function returns current number of elementary conditions in given
          axiom.
          @return signed int - current number of elementary conditions
         */
    signed int size(void) const;
    /*!
      Function returns current number of elementary conditions in given
      axiom.
      @return signed int - current number of elementary conditions
     */
    signed int getNumOfEC (void) const;

    // ������� ������� ��� ������������ ������� � ������� ���������� �������
    /*!
      Function deletes each of elementary conditions in given
      axiom.
      @return signed int - current number of elementary conditions
     */
    signed int clear (void);

    // ���������, ����������� �� ������ ������� �������� ����� k ���� ts (��� ����������� � ������������ �����)
    // ����������:  1  ���� �����������
    //              0  ���� �� �����������
    //				-1  ���� ������ ������� ������� �� ���������
    /*!
      Function checks for marking point in row by given compound axiom.
      (for one-dimensional rows)
      @author    dk
      @param     k -    point in checking row
      @param     ts -   time row
      @return signed int - return 1 if marking; 0 - not marking;
                                -1 if given axiom does not apply

     */
    signed int check (const unsigned long k, const std::vector<double>& ts) const;


    /*!
      Function checks for marking point in row by given compound axiom.
      (for several-dimensional rows)
      @author    dk
      @param     k -    point in checking row
      @param     ts -   time row
      @return signed int - return 1 if marking; 0 - not marking;
                                -1 if given axiom does not apply

     */
    signed int check (const unsigned long k, const std::vector < std::vector<double> >& ts) const;

    // ������������� ���������� ������������ �������, �������� � �������
    //signed int setECParameters (const std::map <std::string, std::map <std::string, std::string> >& ecParams);

    // ������� ���������� ��������� ������
    //signed int getECParameters (std::map <std::string, std::map <std::string, std::string> > &ecParams) const;

    // ��������� ������ ������� � �� ����������� ����������� � ������ �������� ���� �� ����.
    // �� �������, ������ ���������


    /*!
      Comparison given axiom with its internal parameters with another axiom.
      Comparison is fair by default.
      @author    dk
      @param     second - second comparing axiom
      @return double - real number from -1 to 1
      As the number is near 1, it could be certainty said first axiom is
      embedded in second axiom
      (i.e. second axiom is greater than the first one).
      If number is -1 then first axiom is greater than the second one.
      If number is 0 then axioms are congruent or not congruent with each other
     */
    double operator< (const AxiomExpr& second);

    // �������� ������������
    // � ����� ���� ��� ������� ������ ��� �������� �� ���������
    // AxiomExpr & operator= (const AxiomExpr& second);

        // ������ ������ �� ��������� �

    /*!
      Merging axioms by AND operator
      @author    dk
      @param     second - second axiom in axiom merging
      @return AxiomExpr - new axiom as merging *this and second
     */
    AxiomExpr operator& (const AxiomExpr& second) const;

    // ������ ������ �� ��������� ���
        /*!
          Merging axioms by OR operator
          @author    dk
          @param     second - second axiom in axiom merging
          @return AxiomExpr - new axiom as merging *this and second
         */
    AxiomExpr operator| (const AxiomExpr& second) const;

    AxiomExpr& operator &=(const AxiomExpr& second);

    AxiomExpr& operator |=(const AxiomExpr& second);

    // ������ ������ �� ��������� � � ������ �������
        /*!
          Merging axioms by AND operator in given axiom
          @author    dk
          @param     first -  first axiom in axiom merging
          @param     second - second axiom in axiom merging
          @return    0
         */
    signed int andExpr (const AxiomExpr& first, const AxiomExpr& second);

    // ������ ������ �� ��������� ��� � ������ �������
        /*!
          Merging axioms by OR operator in given axiom
          @author    dk
          @param     first -  first axiom in axiom merging
          @param     second - second axiom in axiom merging
          @return    0
         */
    signed int orExpr (const AxiomExpr& first, const AxiomExpr& second);

    // ���������� ����������� ������������ ��������
        /*!
          Calculation of dataset dimensions, which are needed for current axiom
          @author    wictor
          @param     v      - True is writing on place of dimensions, which are
          using. If vector size is insufficient, than vector size doesn't
          increase(i.e it needs to set right vector size in advance).
         */
    void getDimensions(std::vector<bool> &v) const;

    // �������� ������ ����������� ������������ ���� ������
        /*!
          Checking for weak structure equivalence for two axioms.
          Sets of elementary conditions are equal.
          @author    dk
          @param     second - second comparing axiom
          @retval    0 no weak structure equivalence
          @retval    1 weak structure equivalence
         */
    signed int checkWeakEquivalence(const AxiomExpr& second) const;

    // ������� ��������� ��������� ��������� ������� � AxiomExprStructures
    /*!
      Saving structure of choosing axiom in AxiomExprStructures
      @author    dk
      @param     aes - filling axiom structure field
      @return    0
     */
    signed int createAxiomExprStructure (AxiomExprStructure &aes) const;

    // ������� ����������� ������� ��������� ������� ������������� ���������
    // �� ���� ����������� �������� - ����� ���� ������� � ����������� �������
    // � ������ - ���� �� ������� ���������� ���������� ������� ����� �������  - ������� ���������� -1
        /*!
          Function defines current axiom as expression above random
          elementary conditions. Realization assumes adress to lower layer -
          a layer of elementary conditions creation - and getting random
          selected sets of compatible elementary conditions. Number of
          elementary conditions define as parameter.
          @author    dk
          @param     numOfEC - number of elementary conditions, which will be
                     part of current axiom
          @throws    AxiomLibExeption - if it is impossible to select numOfEC
                     compatible elementary conditions
          @retval    0  creation ends with success
          @retval    -1  creation faults
         */
    signed int initAxiomByRand (const int numOfEC);

    // ��������� ����������� ������� � ��������� ����������� ������ ������ ��� ������� ������� �� ��������� ���������
    /*!
      Function defines current axiom as expression above random
      elementary conditions. For each condition dimension of data set
      (with which condition will be test) is defined in random from
      specified range. Realization assumes adress to lower layer -
      a layer of elementary conditions creation - and getting random
      selected sets of compatible elementary conditions. Number of
      elementary conditions define as parameter.
      @author    dk
      @param     numOfEC - number of elementary conditions, which will be
                 part of current axiom
      @throws    AxiomLibExeption - if it is impossible to select numOfEC
                 compatible elementary conditions
      @retval    0  creation ends with success
      @retval    -1  creation faults
     */
    signed int initAxiomByRand (const int numOfEC, const int dimensionMin, const int dimensionMax);

    // ������� ������� ������� �� ������ �� ��������� � ��������� ������� ������
    /*!
      Function for definition of axiom from dedicated structure with
      axiom description
      @author    dk
      @param     aes - structure with axiom description
      @throws    AxiomLibExeption - if name of one of the elementary conditions
                 is wrong
      @return    0
     */
    signed int initAxiomFromStructure (AxiomExprStructure &aes);

    // ������� ������� ������� �� �����
    /*!
      Function for definition of axiom from file
      @author    dk
      @param     baseDir - directory, where askable file of description
                 of axioms is
      @param     AxiomSetName - name for saving axiom
      @return    0
     */
    signed int initAxiomFromFile (const std::string baseDir, const std::string axiomName);

    // ������� ������� ������� �� ����� (� ��������� ����������� ������ ������, �� ������� ����������� ������ ������������ ������� �� ������ �������)
    /*!
      Function for definition of axiom from file.
      ( With indication of dimension of data set, on which each elementary
        conditions from current axiom is checking)
      @author    dk
      @param     baseDir - directory, where askable file of description
                 of axioms is
      @param     AxiomSetName - name for saving axiom
      @param     paramNames - vector of parameters of data set for indication
                 of dimension, on which each elementary conditions need to check
      @return    0
     */
    signed int initAxiomFromFile (const std::string baseDir, const std::string axiomName, const std::vector <std::string> &paramNames);

    // ������� ���������� ������� ������ � ����
    /*!
      Function for saving axioms in file
      @author    dk
      @param     baseDir - directory, where askable file of description
                 of axioms is
      @param     AxiomSetName - name for saving axiom
      @return    0
     */
    signed int saveAxiomToFile (const std::string baseDir, const std::string axiomName) const;

    // ������� ���������� ������� ������ � ���� (� ��������� ����������� ������ ������, �� ������� ����������� ������ ������������ ������� �� ������ �������)
    /*!
      Function for saving axioms in file
      @author    dk
      @param     baseDir - directory, where askable file of description
                 of axioms is
      @param     AxiomSetName - name for saving axiom
      @param     paramNames - vector of parameters of data set for indication
                 of dimension, on which each elementary conditions need to check
      @return    0
     */
    signed int saveAxiomToFile (const std::string baseDir, const std::string axiomName, std::vector <std::string> &paramNames) const;

};

}; //  end of namespace

#endif /* _Axiom_Expr_HXX */
