/****************************************************************************
*					AxiomExprPlus 
*
*	Description:	����� �������� ������� �� ������ ��������� �� 
*					������������ ������� � ��������� �����������.
*	Author:			dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_AXIOM_EXPR_PLUS_HXX
#define __AXIOMS_LIB_AXIOM_EXPR_PLUS_HXX

#include "AxiomExpr.h"
#include <vector>

#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/extended_type_info.hpp>

namespace AxiomLib {

class AxiomExprPlus : public AxiomExpr {
	friend class boost::serialization::access;
	
	template<class Archive>
	void serialize(Archive &archive, const unsigned int /*version*/) {
		archive & BOOST_SERIALIZATION_BASE_OBJECT_NVP(AxiomExpr);
		archive & BOOST_SERIALIZATION_NVP(statNormal);
		archive & BOOST_SERIALIZATION_NVP(statAbnorm);
		archive & BOOST_SERIALIZATION_NVP(statVariation);
		archive & BOOST_SERIALIZATION_NVP(goal);
		archive & BOOST_SERIALIZATION_NVP(statOccurence);
	}
	
	void setDefaultStatistics();
	
 public:

	//	���������� ��������� ������ AxiomExpr �� ���� ��������� ����������:
	// �������� ������� �������
    double goal;

	// ������� ���������� �� �����������, ���������� ���������� ���������� ���������
	double statAbnorm;

	// ������� ���������� �� ����������� ����������� ���������
	double statNormal;
	
	// ����������� ���������� ������ ������������� ������ �������
	// �� ��������� ����������� ���� ���������� ���������,
	// � �������� ����������� ������ �������
	double statVariation;
	
	// ��������� ����� ����������, ��������������� ���������� ���������, �� ������� 
	// ������� �����������, � ������ ����� ����������, ��������������� ����������
	// ���������
	double statOccurence;

		// ������� ������:
 
    // ������ ����������� � �������� ����� �� default'�
    AxiomExprPlus(void);  
  
    // ������ ����������� � �������� ����� � ���������
    AxiomExprPlus(const std::string sName);
	
	explicit AxiomExprPlus(const ElemCondPlus& ec);
	
	explicit AxiomExprPlus(const AxiomExpr& ax);
 
    // ���������� ��������� ��� ��������� � ������ ������ ������� ������ ������������ �������
    ~AxiomExprPlus(void);

    // ������� �������� ����������� ���������� ������� �������
    void clear (void);
  
	// �������� ������������ ������� ������
	// � ����� ���� ��� ������� ������ �� �� ���������
    // AxiomExprPlus& operator= (const AxiomExprPlus &second);
	
	// ������� ���������� ������� ������ � ���� (� ��������� ����������� ������ ������, �� ������� ����������� ������ ������������ ������� �� ������ �������)
	signed int saveAxiomToFile (const std::string& baseDir, const std::string& axiomName, const std::vector <std::string> &paramNames) const;
	
	// ������� ������� ������� �� ����� (� ��������� ����������� ������ ������, �� ������� ����������� ������ ������������ ������� �� ������ �������)
	signed int initAxiomFromFile (const std::string baseDir, const std::string axiomName, const std::vector <std::string> &paramNames);
	

}; // end of class

}; //  end of namespace

//BOOST_CLASS_EXPORT_GUID(AxiomLib::AxiomExprPlus, "AxiomExprPlus");

#endif /* __AXIOMS_LIB_AXIOM_EXPR_PLUS_HXX */
