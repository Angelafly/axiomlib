/****************************************************************************
*			NeuralActFuncStr.cxx
*
*	Description: ���������� �������, ����������� � NeuralActFuncStr.h
*	Author:		wictor
*	History:	
*
****************************************************************************/
#include <boost/bimap.hpp>

#include "NeuralFANNHeader.h"

#include "NeuralActFuncStr.h"

namespace AxiomLib {

// �����, ���������� ����������� enum <-> string
class EnumStr {
	typedef boost::bimap<int, std::string> map_t;
	map_t name_map, train_name_map;
	
	std::string empty;
public:
	// ����������� ��������� - ��������� ������� ���������� bimap
	EnumStr():empty("") {

#define MY_ENUM_STR_INSERT(m, a) m.insert(map_t::value_type(a, #a))

		using namespace FANN;

		MY_ENUM_STR_INSERT(name_map, LINEAR);
		MY_ENUM_STR_INSERT(name_map, THRESHOLD);
		MY_ENUM_STR_INSERT(name_map, THRESHOLD_SYMMETRIC);
		MY_ENUM_STR_INSERT(name_map, SIGMOID);
		MY_ENUM_STR_INSERT(name_map, SIGMOID_STEPWISE);
		MY_ENUM_STR_INSERT(name_map, SIGMOID_SYMMETRIC);
		MY_ENUM_STR_INSERT(name_map, SIGMOID_SYMMETRIC_STEPWISE);
		MY_ENUM_STR_INSERT(name_map, GAUSSIAN);
		MY_ENUM_STR_INSERT(name_map, GAUSSIAN_SYMMETRIC);
		MY_ENUM_STR_INSERT(name_map, GAUSSIAN_STEPWISE);
		MY_ENUM_STR_INSERT(name_map, ELLIOT);
		MY_ENUM_STR_INSERT(name_map, ELLIOT_SYMMETRIC);
		MY_ENUM_STR_INSERT(name_map, LINEAR_PIECE);
		MY_ENUM_STR_INSERT(name_map, LINEAR_PIECE_SYMMETRIC);
		MY_ENUM_STR_INSERT(name_map, SIN_SYMMETRIC);
		MY_ENUM_STR_INSERT(name_map, COS_SYMMETRIC);
		MY_ENUM_STR_INSERT(train_name_map, TRAIN_INCREMENTAL);
		MY_ENUM_STR_INSERT(train_name_map, TRAIN_BATCH);
		MY_ENUM_STR_INSERT(train_name_map, TRAIN_QUICKPROP);
		MY_ENUM_STR_INSERT(train_name_map, TRAIN_RPROP);
	}

	// ������������� ������� �� ������
	int act_func_from_str (const std::string &val)
	{
		map_t::right_map::const_iterator it=name_map.right.find(val);
		return it==name_map.right.end() ? -1 : it->second;
	}

	// ������ �� ������������� �������
	const std::string &str_from_act_func (int val)
	{
		map_t::left_map::const_iterator it=name_map.left.find(val);
		return it==name_map.left.end() ? empty : it->second;
	}

	// ��������� �������� �� ������
	int train_from_str (const std::string &val)
	{
		map_t::right_map::const_iterator it=train_name_map.right.find(val);
		return it==train_name_map.right.end() ? -1 : it->second;
	}

	// ������ �� ���������� ���������
	const std::string &str_from_train (int val)
	{
		map_t::left_map::const_iterator it=train_name_map.left.find(val);
		return it==train_name_map.left.end() ? empty : it->second;
	}
};

// ��������� ������
EnumStr act_func_map;

// ������������� ������� �� ������ (���������� -1 � ������ ������������� ���������, ����� ����� ��������� � FANN:activation_function_enum)
int actFuncFromStr(const std::string &val)
{
	return act_func_map.act_func_from_str(val);
}

// ������ �� ������������� ������� (���������� "" � ������ ������������� ���������)
const std::string &strFromActFunc(int val)
{
	return act_func_map.str_from_act_func(val);
}

// ��������� �������� �� ������ (���������� -1 � ������ ������������� ���������, ����� ����� ��������� � FANN:training_algorithm_enum)
int trainFromStr(const std::string &val)
{
	return act_func_map.train_from_str(val);
}

// ������ �� ���������� ��������� (���������� "" � ������ ������������� ���������)
const std::string &strFromTrain(int val)
{
	return act_func_map.str_from_train(val);
}

};
