/****************************************************************************
*				Net.cxx
*
*	Description:	������� ������ Net - ����������
*	Author:		dk
*	History:		
*
****************************************************************************/
#include "Net.h"

using namespace AxiomLib;


// ����������� ����������� - ������� �������� �� ���������
Net::Net (unsigned int newQ) {
	status = false;
	q = newQ;
	inNum = 0;
	outNum = 5;
	maxOutNum = 0;
	for (int i = 1; i <= (int) outNum; i++)
		maxOutNum += (int) pow ((double) q, i);
	outCoeff.resize (outNum);
	int curQ = 1;
	for (unsigned int j = 0; j < outNum; j++) {
		outCoeff[j] = curQ;
		curQ = curQ * q;
	}
	matrices.clear();
	distValues.clear();
	usedDistValues.clear();
}


// ����������� ���������� - ������
Net::~Net () {

}


/****************************************************************************
*				Net::codeRes
*
*	Description:	������� ����������� ����� � q-������ ������. ��� �������� �����
*					������������ ��������� ���������� ������������, ������� 
*					���������� � ���� �������.
*	Parameters:		input - ���������� �����
*					res - ��������� �����������
*	Returns:		- 
*	Throws:			AxiomLibException - ���� ������� ����� ������������ �� ������
*	Author:		dk
*	History:
*
****************************************************************************/
inline void Net::codeRes (int input, std::vector <int> &res) {
	if ((input < 0) || (input > (maxOutNum - 2)))
		throw AxiomLibException("Error in Net::codeRes: incorrect input param value.");
	
	int toAdd;
	// 1. ������� ��� input ���������� ��������, ������������ ��������, 
	//	  ����� ������� ��������� input ��������������� ��������� �����, ���������� ���������
	std::map <int, int> :: const_iterator iter;
	iter = distValues.find(input);
	if (iter == distValues.end()) {
		// ������ ��� ������ ������� ����� ����������� ������� - ����������� ��� ���� ����� ��������� �����
		bool flag = true;
		std::set <int> :: const_iterator iterSet;
		while (flag) {
			toAdd = 1 + ((int) ((double) rand() / (double) RAND_MAX)) * (maxOutNum - 2);
			iterSet = usedDistValues.find (toAdd);
			if (iterSet == usedDistValues.end()) {
				flag = false;
				usedDistValues.insert(toAdd);
				distValues[input] = toAdd;
			}
		}
	} else {
		// ������ ��� ������� ����� ��� ������������ ���������� ��������� ��������
		toAdd = iter->second;
	}
	
	// ������ � toAdd - �������� ���������� ��������, ��������������� ����������� ����� input
	// 2. ��������� �������, ��������������� input � ��� q-������� �������
	res.resize(outNum);

	div_t div_result;

	for (unsigned int i = outNum - 1; i !=0 ; i--) {
		div_result = div( toAdd, outCoeff[i] );
		res [i] = div_result.quot;
		toAdd = div_result.rem;
	}
}


/****************************************************************************
*				Net::decodeRes
*
*	Description:	������� �������� ���������� �� q-������� ������� �����,
*					��� �������� ����� ���� ��������������� ��� �������������.
*	Parameters:		output - ������������ �����
*					res - q-������ ������, �� �������� ������� ������������ �����
*	Returns:		- 
*	Throws:			AxiomLibException - ���� ��� �������� ������� ��� ����������������
*					����� � ������ ��������������
*	Author:		dk
*	History:
*
****************************************************************************/
inline void Net::decodeRes (int &output, std::vector <int> &res) const {
	if ((res.size() != outNum) || (outNum < 1))
		throw AxiomLibException("Error in Net::decodeRes: incorrect input param value.");

	// 1. �������� ����� �� ��� ������������� � ���� q-������� �������
	int toRet = 0;
	for (unsigned int i = 0; i < outNum; i++)
		toRet += outCoeff[i]*res[i];
	
	// 2. ������� ��� ���������� ����� ��������������� �������� �����, �������������� ��������� �������
	std::set <int> :: const_iterator iterSet;
	iterSet = usedDistValues.find(toRet);
	if (iterSet != usedDistValues.end()) {
		// ������ ����� ���� � ����� ����� - ������� ���
		bool flag = false;
		std::map <int, int> :: const_iterator iter;
		for (iter = distValues.begin(); iter != distValues.end(); iter++) {
			if (iter->second == toRet) {
				flag = true;
				output = iter->first;
				break;
			}
		}
		if (!flag)
			throw AxiomLibException("Error in Net::decodeRes: incorrect input param value - strange difference between set and map.");
	} else
		// ���� ������������ �� ������� - ������ ������
		throw AxiomLibException("Error in Net::decodeRes: incorrect input param value - there is no decode number.");
}


/****************************************************************************
*				Net::code
*
*	Description:	������� �������� ����� �� 0 �� q-1 ������������� � ������ 
*					�� q ���������, � ��������� ����������
*	Parameters:		input - ���������� �����
*					res - ��������� �����������, ������ �� 0 � 1
*	Returns:		- 
*	Throws:			AxiomLibException - ���� ������� ����� ������� �� ������������ ���������
*	Author:		dk
*	History:
*
****************************************************************************/
inline void Net::code (int input, std::vector <char> &res) const {
	res.resize(q);
	if ((input < 0) || (input >= (int) q))
		throw AxiomLibException("Error in Net::code: incorrect param 'q' is set.");
	for (unsigned int i = 0; i < q; i++)
		res[i] = 0;
	res[input] = 1;
}


/****************************************************************************
*				Net::code
*
*	Description:	������� ����������� ������� �������� � q-������ ����
*					� ��� ������� � ��������� ����������
*	Parameters:		input - ������� ������ ���������� ��������
*					res - �������� �������, �������������� ���
*	Returns:		-
*	Throws:			-
*	Author:		dk
*	History:
*
****************************************************************************/
inline void Net::code (std::vector <int> &input, std::vector <std::vector <char> > &res) const {
	res.resize(input.size());
	for (unsigned int i = 0; i < input.size(); i++)
		code (input[i], res[i]);
}


/****************************************************************************
*				Net::mult
*
*	Description:	������� ���������� ������������ 2-� ��������
*	Parameters:		x, y - ������� �������
*					res - �������� �������, ��������� ������������
*	Returns:		-
*	Throws:			-
*	Author:		dk
*	History:
*
****************************************************************************/
inline void Net::mult (std::vector <char> &x, std::vector <char> &y, std::vector <std::vector <char> > &res) const {
	res.resize(x.size());
	for (unsigned int i = 0; i < x.size(); i++) {
		res[i].resize(y.size());
		for (unsigned int j = 0; j < y.size(); j++)
			res[i][j] = x[i]*y[j];
	}
}


/****************************************************************************
*				Net::prepare
*
*	Description:	������� ������� ����
*	Parameters:		in - ������� ������
*					out - �������������� �������� ��������
*	Returns:		0
*	Throws:		AxiomLibException - � ������ ������
*	Author:		dk
*	History:
*
****************************************************************************/
int Net::prepare (std::vector <int> &in, int out) {
	std::vector <std::vector <char> > inCode; 
	std::vector <char> outCode;
	code (in, inCode);
	if (!status){
		// ������ ������ ��������. ������� ��� ������������� ��� �� �������
		inNum = (unsigned int) in.size();
		matrices.resize (inNum);
		for (unsigned int i = 0; i < inNum; i++) {
			matrices[i].resize(outNum);
			for (unsigned int j = 0; j < outNum; j++){
				mult (outCode, inCode[i], matrices[i][j]);


			}

		}
	}

	status = true;
	return 0;
}

 
/****************************************************************************
*				Net::check
*
*	Description:	������� ��������, ���� �� � ������� i � ���� row ���� ��
*					��������, ����������� �� ��������. ��� ���� ������������ 
*					������������� �������� ���������� ��������.
*	Parameters:		resNum - ������ ������ ���������� ���������, �����������
*							� ������ �������
*					row - ������������� ��� ��������
*					i - ������, �� �������� ������ ��������� ��������
*	Returns:		double (�� 0 �� 1) - ������������� ��������, �������������
*					� �������� resNum
*	Throws:		AxiomLibException - � ������ ������
*	Author:		dk
*	History:
*
****************************************************************************/
double Net::check (int &resNum, std::vector <int> &row, long int &i) const {

	return 0.0;
}
