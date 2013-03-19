/****************************************************************************
*					AxiomExprSetPlus 
*
*	Description:	����� �������� ������� ������ �� ������ ��������� �� 
*					������������ ������� � ��������� �����������.
*	Author:			dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_AXIOM_EXPR_SET_PLUS_HXX
#define __AXIOMS_LIB_AXIOM_EXPR_SET_PLUS_HXX


#include "AxiomExprSet.h"
#include <vector>
#include <utility>

#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/utility.hpp>

namespace AxiomLib {

class AxiomExprSetPlus : public AxiomExprSet {
	friend class boost::serialization::access;
	
	template<class Archive>
	void serialize(Archive &archive, unsigned int /*version*/) {
		archive & BOOST_SERIALIZATION_BASE_OBJECT_NVP(AxiomExprSet);
		archive & BOOST_SERIALIZATION_NVP(goal);
		archive & BOOST_SERIALIZATION_NVP(errFirst);
		archive & BOOST_SERIALIZATION_NVP(errSecond);
		archive & BOOST_SERIALIZATION_NVP(axiomsIndex);
		archive & BOOST_SERIALIZATION_NVP(markUps);
		archive & BOOST_SERIALIZATION_NVP(multiMarkUps);
		archive & BOOST_SERIALIZATION_NVP(errors);
	}

 public:
	
	bool operator <(const AxiomExprSetPlus& other) const;

	//	���������� ��������� ������ AxiomExprSet �� ���� ��������� ����������:

	// �������� ������� �������
    double goal;

	// ����� ������ I ����
	int errFirst;

	// ����� ������ II ����
	int errSecond;

	// ������� �������� � ������� ������ � ��������� �������������
	std::vector <unsigned int> axiomsIndex;

	// ������� �������� ��� ��������� ����� ���������� ���������
	std::vector < std::vector <int> > markUps;
	
	// �������� �������������� ��� ������� ������ ���������� ���������
	std::vector < std::vector <std::vector<bool> > > multiMarkUps;
	
	// ������ ������� � ������� ���� ��� ������� ���� ���������� ���������
	std::vector<std::pair<unsigned int, unsigned int> > errors;


	//	������� ������:
 
    // ������ ����������� � �������� ����� �� default'�
    AxiomExprSetPlus(void);  
  
    // ������ ����������� � �������� ����� � ���������
    AxiomExprSetPlus(const std::string sName);
	
	AxiomExprSetPlus(const AxiomExprSet& as);
 
    // ���������� ��������� ��� ��������� � ������ ������ ������� ������ ������������ �������
    ~AxiomExprSetPlus(void);
 
    // ������� �������� ���� ������ �� ������� ������
    void clear (void);
  
	// �������� ������������ ������� ������
	// � ����� ���� ��� ��������� � ������ ������ ��� �������������
    //AxiomExprSetPlus& operator= (const AxiomExprSetPlus &second);
	
	// ������� ������� ������� ������ ������ ������������ ������� (�� ������������ �� �����)
    int initAxiomSetFromFile (std::string axiomSetBaseDir, std::string axiomSetName, const std::vector<std::string> &paramNames);
	
	// ������� ���������� ������� ������ � ���� (� ��������� ���������� �������� ������ ��� ������� ������������� �������)
	int saveAxiomSetToFile (std::string baseDir, std::string axiomSetName, 
							const std::vector <std::string> &dataSetParams, int first = -1, int second = -1) const;
        
}; // end of class

}; //  end of namespace

//BOOST_CLASS_EXPORT_GUID(AxiomLib::AxiomExprSetPlus, "AxiomExprSetPlus");

#endif /* __AXIOMS_LIB_AXIOM_EXPR_SET_PLUS_HXX */
