/****************************************************************************
*				AxiomBase
*
*	Description:	�����, ���������� �� ������ ���� ������
*	Author:		dk
*	History:
*
****************************************************************************/
/*!
  @file AxiomBase.h
  @author dk
  Class for reading base of axioms.
 */

#ifndef __AXIOMS_LIB_READAXIOMBASE_HXX
#define __AXIOMS_LIB_READAXIOMBASE_HXX

#include "./elem_conditions/ElemCondition.h"
#include "ElemConditionsFactory.h"
#include "AxiomLibException.h"
#include "AxiomExprStructure.h"
#include "Environment.h"
#include "all_axioms.h"
#include "Axiom.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
//includes for using boost
#include "boost/tokenizer.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/fstream.hpp"

/*!
  @brief
  Checking for weak structure equivalence for two axioms.
  Sets of elementary conditions are equal.
  @author    dk
  @param     second - second comparing axiom
  @retval    0 no weak structure equivalence
  @retval    1 weak structure equivalence
  @return
 */


namespace AxiomLib {

class AxiomBase {

 private:

    // ����� ���� ������ � ���� ������
    //!
    //! \brief axioms
    //!
    std::map <std::string, std::vector<std::string> > axioms;

    // ��� ���������� ����� ������
    //!
    //! \brief name
    //!
    std::string name;

 protected:

    // ���������� ��� ������� ���������
    //!
    //! \brief token_separator
    //!
    typedef boost::tokenizer<boost::char_separator<char> >      token_separator;

    // ������� ������ ������ �� ax-�����
    //!
    //! \brief readFromAX
    //! \param fileName
    //! \param vec
    //! \return
    //!

    /****************************************************************************
    *					AxiomBase::readFromAX
    *
    *	Description:	������� ��������� .ax ������
    *				(��������������� ������� � readAxioms)
    *				������� ��������� ���������� �� ����� �������� �������
    *				� ���������� ����������� ���������� � ����������
    *				������������ ��������� - ����������� �� ����� ������� �
    *				������ ������������ �������. ������������ ������ �������
    *	Parameters:	filePath - ���� � ������������� ����� �������� �������
    *				vec - ����������� ������ ������������ ������� ������������
    *					������ �������
    *	Returns:	0 - ���� ���� ��� ������ � ��������� ��������
    *				-1 - ���� ���� �� ��� ������
    *	Throws:		AxiomLibException  - ���� ���� � �������� ����� �� ������
    *								���� �� ����� ���� ��������� �������
    *								���� ������ ����� �� �������������
    *								�������� �������� (��. formats.x)
    *	Author:		dk
    *	History:
    *
    ****************************************************************************/
    signed int readFromAX (std::string fileName, std::vector<std::string> &vec);

    // ������� ������ ������ �� x-�����
    //!
    //! \brief readFromX
    //! \param fileName
    //! \param strName
    //! \return
    //!


    /****************************************************************************
    *					AxiomBase::readFromX
    *
    *	Description:	������� ��������� .x �����
    *				������� ��������� ��� ����� ������ � ���������� � ������ ������� �����
    *				(��������������� ������� � readAxioms)
    *	Parameters:	filePath - ���� � ������������� .x �����
    *				strName - ����������� �������� - ������ ����� ������ - ����������� �� �����
    *	Returns:		0
    *	Throws:		AxiomLibException  - ���� �������������� ����� ��� - �� ���� ���� �� ������
    *								���� ���� ����
    *								���� ��� ����� ������, ���������� � ����� �����������
    *								�� ����� ����� �����
    *	Author:		dk
    *	History:
    *
    ****************************************************************************/
    signed int readFromX (std::string fileName, std::string &strName);

    /****************************************************************************
    *					AxiomBase::checkName
    *
    *	Description:	������� ��������� �����.
    *				����� ���� �������� ����������� � ������ ������� � ������
    *				������ �������� ��������� � �� ������ ������� ��� �����.
    *				������������ ��� ��������� � �������� ���� ������.
    *	Parameters:	name1 - ������ ��� ���������
    *				from -����� ������� � ������ name1 � �������� �������� ���������
    *				upTo - ����� ������� � ������ name1 �� �������� ����� ���������
    *				name2 - ������ ��� ���������
    *	Returns:		true  - ���� ������ ���� �������� ��� ������ ��� ��� ������
    *				false - � ��������� ������
    *	Throws:		-
    *	Author:		dk
    *	History:
    *
    ****************************************************************************/

    // ������� ��������� ���� � ��������
    //!
    //! \brief checkName
    //! \param name1
    //! \param from
    //! \param upTo
    //! \param name2
    //! \return
    //!
    bool checkName (std::string name1, int from, int upTo, std::string name2) const;

    // ������� ��������� ������ � ������  ����� � ������ � ����� ����� - ��������� ���������� ������ �������� ������
    //!
    //! \brief compareStringAndPath
    //! \param a
    //! \param b
    //! \param n3
    //! \return
    //!

    /****************************************************************************
    *					AxiomBase::compareStringAndPath
    *
    *	Description:	������� ��������� ����� ����� � ����� � �����.
    *				��������� ���������� �� ������:
    *				- � ����� ����� ������������ ���������� - ����������� � ����������
    *				- � ������ ����� ������������ "/"
    *				- ���������� ��� ����� ������ � ����� ���� � �����
    *	Parameters:	fileName - ��� �����
    *				path - ���� � �����(�� ���� ��� � �����)
    *				n3 - ���������� �����, � ������� �������������� ����� ���� � ���� � �����
    *	Returns:		bool
    *	Throws:		-
    *	Author:		dk
    *	History:
    *
    ****************************************************************************/

    bool compareStringAndPath (std::string a, std::string b, const char* n3) const;

 public:


    // ������� ���������� �����  ������ � ��������� ���������
    //!
    //! \brief size
    //! \return
    //!
    int size (void) const;

    // �� ������ ��������� � private ! ! !
    // ��� ���������� ����� ������
    //std::string name;

    // ������ �����������
    //!
    //! \brief AxiomBase
    //!
    AxiomBase (void);

    // ������� ������ ��� ������� �� ��������, ���� ������� ������ � ����� ������������

    /****************************************************************************
    *					AxiomBase::readAxioms
    *
    *	Description:	������� ������ ���� ������.
    *				������� �������� �� �������� ��������� ����������
    *				� ����� (���� � ��������) �������� ���� ������. ����� �������
    *				� ����� ����� ������ �������� - � ���������� ��� ������
    *				�� ��������������� ��������. ����� ������ ������ - ����������
    *				�������, ��������� � ������.
    *	Parameters:	axiomBaseDir - ������ � ����� � �������� ���� ������
    *	Returns:		0
    *	Throws:		AxiomLiException  - 	���� �� ������� ��������, ���������� �
    *								���������������� ����� ��� ���� ������
    *								���� �������� ���� � ����� �������� - ���� � �����
    *								���� � �������� �� ���������� ����� .x � ���������
    *								����� ����� ������ (���� ����� ������ ����� ������)
    *	Author:		dk
    *	History:
    *
    ****************************************************************************/

    //!
    //! \brief readAxioms
    //! \param axiomBaseDir
    //! \return
    //!
    signed int readAxioms (const char* axiomBaseDir);

    // ������ ������ ������ ������������ ������� �������������� ����������� �������, ���� ������ ������ ���� �� ������� ������� � �������� ������

    /****************************************************************************
    *					AxiomBase::getVectorOfEC
    *
    *	Description:	������ ������ ������ ������������ ������� ��������������
    *				����������� �������, ���� ������ ������ ���� �� ������� �������
    *				� �������� ������
    *	Parameters:	toReturn - ������������ ������
    *			axiomName - ��� �������, ���������� � ������� ����
    *	Returns:		0
    *	Throws:		AxiomLiException  - 	���� �� ������� ��������, ���������� �
    *								���������������� ����� ��� ���� ������
    *								���� �������� ���� � ����� �������� - ���� � �����
    *								���� � �������� �� ���������� ����� .x � ���������
    *								����� ����� ������ (���� ����� ������ ����� ������)
    *	Author:		dk
    *	History:
    *
    ****************************************************************************/

    //!
    //! \brief getVectorOfEC
    //! \param toReturn
    //! \param axiomName
    //! \return
    //!
    signed int getVectorOfEC (std::vector<std::string>& toReturn, std::string axiomName);

    // ������� ���������� �������� ������� � �� ���������� � ��������� AxiomExprStructure

    /****************************************************************************
    *					AxiomBase::readFromAX
    *
    *	Description:	������� ������ ���������� �� ������ �� ������ .ax �����
    *					������ ����� ������ � .formats.x
    *					� ������ ������� ����������� � ������������ � ������� �����,
    *					��� ���������� � ������ ������ �����; ����������� � �����������
    *					��� ������������ ���� ������������ �������;
    *					(����� �� ������ ��������� ����������� �������� " ,';:[]" )
    *					����� � ������� ����������� �������� � ������ ������� � �� ���������.
    *					��� ��������� �������� ������������ � ���������� ������������
    *					���������, ��. AxiomExprStrucutre.h.
    *	Parameters:		filePath - ��� ����� ��� ��������� (���� � ����)
    *					aes - ����������� �� ����� �������� ���� AxiomExprStrucutre
    *	Returns:		0
    *	Throws:			AxiomLibException  - ���� ���� � �������� ����� �� ����������
    *							���� ������ ����� �� ���������� ��������� ������� (��. formats.x)
    *							���� ��� ����� �� ����������� � ������ � ������ ������ �����
    *	Author:			dk
    *	History:
    *
    ****************************************************************************/

    //!
    //! \brief readFromAX
    //! \param filePath
    //! \param aes
    //! \return
    //!
    signed int readFromAX (const std::string& filePath, AxiomExprStructure &aes) const;

    // ������� ���������� �������� ������� � �� ���������� � ��������� AxiomExprStructure

    /****************************************************************************
    *					AxiomBase::readFromAX
    *
    *	Description:	������� ������ ���������� �� ������ �� ������ .ax �����
    *					������ ����� ������ � .formats.x
    *					� ������ ������� ����������� � ������������ � ������� �����,
    *					��� ���������� � ������ ������ �����; ����������� � �����������
    *					��� ������������ ���� ������������ �������;
    *					(����� �� ������ ��������� ����������� �������� " ,';:[]" )
    *					����� � ������� ����������� �������� � ������ ������� � �� ���������.
    *					��� ��������� �������� ������������ � ���������� ������������
    *					���������, ��. AxiomExprStrucutre.h.
    *	Parameters:		filePath - ��� ����� ��� ��������� (���� � ����)
    *					aes - ����������� �� ����� �������� ���� AxiomExprStrucutre
    *					paramNames - ������ ���������� ������ ������, �.�. �������� ������������ ������ ������
    *	Returns:		0
    *	Throws:			AxiomLibException  - ���� ���� � �������� ����� �� ����������
    *							���� ������ ����� �� ���������� ��������� ������� (��. formats.x)
    *							���� ��� ����� �� ����������� � ������ � ������ ������ �����
    *	Author:			dk
    *	History:
    *
    ****************************************************************************/

    //!
    //! \brief readFromAX
    //! \param filePath
    //! \param aes
    //! \param paramNames
    //! \return
    //!
    signed int readFromAX (const std::string &filePath, AxiomExprStructure &aes, const std::vector<std::string> &paramNames) const;

    // ������ �� ��� ��������� ����� ������ �������� �������

    /****************************************************************************
    *					AxiomBase::readFromAX
    *
    *	Description:	������� ������ ���������� �� ������ �� ������ .ax �����
    *					������ ����� ������ � .formats.x
    *					� ������ ������� ����������� � ������������ � ������� �����,
    *					��� ���������� � ������ ������ �����; ����������� � �����������
    *					��� ������������ ���� ������������ �������;
    *					(����� �� ������ ��������� ����������� �������� " ,';:[]" )
    *					����� � ������� ����������� �������� � ������ ������� � �� ���������.
    *					��� ��������� �������� ������������ � ���������� ������������
    *					���������, ��. AxiomExprStrucutre.h.
    *	Parameters:		filePath - ��� ����� ��� ��������� (���� � ����)
    *					aes - ����������� �� ����� �������� ���� AxiomExprStrucutre
    *					paramNames - ������ ���������� ������ ������, �.�. �������� ������������ ������ ������
    *					file - �������� ���� �� �������� ����� ������������� ���������� �������
    *	Returns:		0
    *	Throws:			AxiomLibException  - ���� ���� � �������� ����� �� ����������
    *							���� ������ ����� �� ���������� ��������� ������� (��. formats.x)
    *							���� ��� ����� �� ����������� � ������ � ������ ������ �����
    *	Author:			dk
    *	History:
    *
    ****************************************************************************/

    //!
    //! \brief readFromAX
    //! \param file
    //! \param aes
    //! \param paramNames
    //! \return
    //!
    signed int readFromAX (boost::filesystem::ifstream &file, AxiomExprStructure &aes, const std::vector<std::string> &paramNames) const;

    // ������� ������ ������� � ����

    /****************************************************************************
    *					AxiomBase::saveToAX
    *
    *	Description:	������� ���������� ������� � ����
    *	Parameters:		baseDir - ����������, ��� ��������� ������������ ���� �������� �������
    *					AxiomName - ��� �������
    *	Returns:		0 - ���� ��� ���������, � ��������� ������ ����� �� throw
    *	Throws:			AxiomLibExeption - ���� ����� �� �������
    *	Author:			dk
    *	History:
    *
    ****************************************************************************/

    //!
    //! \brief saveToAX
    //! \param baseDir
    //! \param aes
    //! \return
    //!
    signed int saveToAX (const std::string& baseDir, AxiomExprStructure &aes) const;

    // ������� ������ ������� � ����

    /****************************************************************************
    *					AxiomBase::saveToAX
    *
    *	Description:	������� ���������� ������� � ����
    *	Parameters:		baseDir - ����������, ��� ��������� ������������ ���� �������� �������
    *					AxiomName - ��� �������
    *					paramNames - ������ ���������� ������ ������, �.�. �������� ������������ ������ ������
    *	Returns:		0 - ���� ��� ���������, � ��������� ������ ����� �� throw
    *	Throws:			AxiomLibExeption - ���� ����� �� �������
    *	Author:			dk
    *	History:
    *
    ****************************************************************************/

    //!
    //! \brief saveToAX
    //! \param baseDir
    //! \param aes
    //! \param paramNames
    //! \return
    //!
    signed int saveToAX (const std::string& baseDir, AxiomExprStructure &aes, const std::vector<std::string> &paramNames) const;

    // ������� ������ ������� � ��� �������� ����

    /****************************************************************************
    *					AxiomBase::saveToAX
    *
    *	Description:	������� ���������� ������� � ����
    *	Parameters:		baseDir - ����������, ��� ��������� ������������ ���� �������� �������
    *					AxiomName - ��� �������
    *					paramNames - ������ ���������� ������ ������, �.�. �������� ������������ ������ ������
    *					file - �������� ����, � ������� ������������ �������� �������
    *	Returns:		0 - ���� ��� ���������, � ��������� ������ ����� �� throw
    *	Throws:			AxiomLibExeption - ���� ����� �� �������
    *	Author:			dk
    *	History:
    *
    ****************************************************************************/

    //!
    //! \brief saveToAX
    //! \param file
    //! \param aes
    //! \param paramNames
    //! \return
    //!
    signed int saveToAX (std::ofstream &file, AxiomExprStructure &aes, const std::vector<std::string> &paramNames) const;

    /****************************************************************************
    *					AxiomBase::readFromEC
    *
    *	Description:	������� ������ ���������� ������������� ������� �� ������ .ec �����
    *					������ ����� ������ � .formats.x
    *					��� ��������� �������� ������������ � ���������� ������������
    *					���������, ��. ElemCondPlusStruture.h.
    *	Parameters:		filePath - ��� ����� ��� ��������� (���� � ����)
    *					ecps - ����������� �� ����� �������� ���� ElemCondPlusStruture
    *	Returns:		0
    *	Throws:			AxiomLibException  - ���� ���� � �������� ����� �� ����������
    *							���� ������ ����� �� ���������� ��������� ������� (��. formats.x)
    *	Author:			dk
    *	History:
    *
    ****************************************************************************/


    // ������� ���������� ���������� ������������� ������� �� �����
    //!
    //! \brief readFromEC
    //! \param filePath
    //! \param ecps
    //! \return
    //!
    signed int readFromEC (const std::string& filePath, ElemCondPlusStruture &ecps) const;

    // ������� ���������� ���������� ������������� ������� �� �����

    /****************************************************************************
    *					AxiomBase::readFromEC
    *
    *	Description:	������� ������ ���������� ������������� ������� �� ������ .ec �����
    *					������ ����� ������ � .formats.x
    *					��� ��������� �������� ������������ � ���������� ������������
    *					���������, ��. ElemCondPlusStruture.h.
    *	Parameters:		filePath - ��� ����� ��� ��������� (���� � ����)
    *					ecps - ����������� �� ����� �������� ���� ElemCondDataDimensionStruture
    *					paramNames - ������ ���������� ������ ������, �.�. �������� ������������ ������ ������
    *	Returns:		0
    *	Throws:			AxiomLibException  - ���� ���� � �������� ����� �� ����������
    *							���� ������ ����� �� ���������� ��������� ������� (��. formats.x)
    *	Author:			dk
    *	History:
    *
    ****************************************************************************/

    //!
    //! \brief readFromEC
    //! \param filePath
    //! \param ecps
    //! \param paramNames
    //! \return
    //!
    signed int readFromEC (const std::string &filePath, ElemCondPlusStruture &ecps, const std::vector<std::string> &paramNames) const;


    /****************************************************************************
    *					AxiomBase::readFromEC
    *
    *	Description:	������� ������ ���������� ������������� ������� �� ������ .ec �����
    *					������ ����� ������ � .formats.x
    *					��� ��������� �������� ������������ � ���������� ������������
    *					���������, ��. ElemCondPlusStruture.h.
    *	Parameters:		filePath - ��� ����� ��� ��������� (���� � ����)
    *					ecps - ����������� �� ����� �������� ���� ElemCondDataDimensionStruture
    *					paramNames - ������ ���������� ������ ������, �.�. �������� ������������ ������ ������
    *					file - �������� ���� �� �������� ����� ������������� ���������� �������� ������������� �������
    *	Returns:		0
    *	Throws:			AxiomLibException  - ���� ���� � �������� ����� �� ����������
    *							���� ������ ����� �� ���������� ��������� ������� (��. formats.x)
    *	Author:			dk
    *	History:
    *
    ****************************************************************************/

    // ������ �� ��� ��������� ����� ������ ������������� �������
    //!
    //! \brief readFromEC
    //! \param file
    //! \param ecps
    //! \param paramNames
    //! \return
    //!
    signed int readFromEC (boost::filesystem::ifstream &file, ElemCondPlusStruture &ecps, const std::vector<std::string> &paramNames) const;

    // ������� ������ ���������� ������������� ������� � ����

    /****************************************************************************
    *					AxiomBase::saveToEC
    *
    *	Description:	������� ���������� ���������� ������������� ������� � ����
    *	Parameters:		filePath - ������ ���� � ������������� ����� � �����������
    *					ecps - ��������� ���� ElemCondPlusStruture � �����������
    *						   ���������� ������������� �������
    *	Returns:		0 - ���� ��� ���������, � ��������� ������ ����� �� throw
    *	Throws:			AxiomLibExeption - ���� ���� �� ������� ��������
    *	Author:			dk
    *	History:
    *
    ****************************************************************************/

    //!
    //! \brief saveToEC
    //! \param baseDir
    //! \param fileName
    //! \param ecps
    //! \return
    //!
    signed int saveToEC (const std::string &baseDir, const std::string &fileName, ElemCondPlusStruture &ecps) const;

    // ������� ������ ���������� ������������� ������� � ����

    /****************************************************************************
    *					AxiomBase::saveToEC
    *
    *	Description:	������� ���������� ���������� ������������� ������� � ����
    *	Parameters:		filePath - ������ ���� � ������������� ����� � �����������
    *					ecps - ��������� ���� ElemCondPlusStruture � �����������
    *						   ���������� ������������� �������
    *	Returns:		0 - ���� ��� ���������, � ��������� ������ ����� �� throw
    *	Throws:			AxiomLibExeption - ���� ���� �� ������� ��������
    *	Author:			dk
    *	History:
    *
    ****************************************************************************/

    //!
    //! \brief saveToEC
    //! \param baseDir
    //! \param fileName
    //! \param ecps
    //! \param paramName
    //! \return
    //!
    signed int saveToEC (const std::string &baseDir, const std::string &fileName, ElemCondPlusStruture &ecps, const std::string &paramName) const;

    // ������� ������ ���������� ������������� ������� � ����

    /****************************************************************************
    *					AxiomBase::saveToEC
    *
    *	Description:	������� ���������� ���������� ������������� ������� � ����
    *	Parameters:		filePath - ������ ���� � ������������� ����� � �����������
    *					ecps - ��������� ���� ElemCondPlusStruture � �����������
    *						   ���������� ������������� �������
    *	Returns:		0 - ���� ��� ���������, � ��������� ������ ����� �� throw
    *	Throws:			AxiomLibExeption - ���� ���� �� ������� ��������
    *	Author:			dk
    *	History:
    *
    ****************************************************************************/

    //!
    //! \brief saveToEC
    //! \param baseDir
    //! \param fileName
    //! \param ecps
    //! \param paramNames
    //! \return
    //!
    signed int saveToEC (const std::string &baseDir, const std::string &fileName, ElemCondPlusStruture &ecps, const std::vector <std::string> &paramNames) const;

    // ������� ������ ���������� ������������� ������� � ��� �������� ����

    /****************************************************************************
    *					AxiomBase::saveToEC
    *
    *	Description:	������� ���������� ���������� ������������� ������� � ����
    *	Parameters:		filePath - ������ ���� � ������������� ����� � �����������
    *					ecps - ��������� ���� ElemCondPlusStruture � �����������
    *						   ���������� ������������� �������
    *					file - �������� ����, � ������� ������������ �������� ������������� �������
    *	Returns:		0 - ���� ��� ���������, � ��������� ������ ����� �� throw
    *	Throws:			AxiomLibExeption - ���� ���� �� ������� ��������
    *	Author:			dk
    *	History:
    *
    ****************************************************************************/

    //!
    //! \brief saveToEC
    //! \param file
    //! \param ecps
    //! \param paramName
    //! \return
    //!
    signed int saveToEC (std::ofstream &file, ElemCondPlusStruture &ecps, const std::string &paramName) const;

    // ������ �� ����� �� ������� �������� ������


    /****************************************************************************
    *					AxiomBase::readFromList
    *
    *	Description:	������ �� ����� �� ������� �������� ������
    *	Parameters:		baseDir - ����������, ��� ��������� ������������ ����
    *					fileName - ��� �����
    *					strList - ���������, � ������� ����������� ������
    *	Returns:		0 - ���� ��� ���������, � ��������� ������ ����� �� throw
    *	Throws:			-
    *	Author:			dk
    *	History:
    *

    ****************************************************************************/

    //!
    //! \brief readFromList
    //! \param filePath
    //! \param strList
    //! \return
    //!
    signed int readFromList (const std::string &filePath, std::vector <std::vector <std::vector <std::vector <std::string> > > > &strList) const;

    // ������ �� ����� �� ������� �������� ������
    //! Read from a file with a list of file names
    //! \brief Read from a file with a list of file names
    //! \param filePath - the file name
    //! \param StrList - a structure in which the data is readed
    //! \retval 0 - if all goes well
    //! \retval throw - otherwise
    //!
    signed int readFromList (const std::string &filePath, std::vector <std::vector <std::string> >  &strList) const;

    // ������� ������ � ���� ������ � ���������� ������

    //! Function for writing to file a list of file names
    //! \brief Function for writing to file a list of file names
    //! \param baseDir - the directory where the writable file is
    //! \param fileName - the file name
    //! \param strList - structure with saving data
    //! \retval 0 - if all goes well
    //! \retval throw - otherwise
    //!
    signed int saveToList (const std::string &baseDir, const std::string &fileName, const std::vector <std::vector <std::vector <std::vector <std::string> > > > &strList) const;

    // ������� ������ � ���� ������ � ���������� ������
    //! Function to write to a file a list of file names
    //! \brief Function to write to a file a list of file names
    //! \param baseDir - the directory where the writable file is
    //! \param fileName - the file name
    //! \param strList - structure with saving data
    //! \retval 0 - if all goes well
    //! \retval throw - otherwise
    //!
    signed int saveToList (const std::string& baseDir, const std::string& fileName, const std::vector <std::vector <std::string> > &strList) const;

    // ���������, ����� �� ��� ����� �������� ����������
    //!
    //! \brief checkExtension
    //! \param fileName
    //! \param extension
    //! \return
    //!
    static bool checkExtension(const std::string& fileName, const std::string& extension);

    // ��������� ����� ����� �������� ����������, ���� ��� ��� ��� �� �����
    //!
    //! \brief checkedAddExtension
    //! \param fileName
    //! \param extension
    //!
    static void checkedAddExtension(std::string& fileName, const std::string& extension);

}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_READAXIOMBASE_HXX */
