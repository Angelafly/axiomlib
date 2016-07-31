/****************************************************************************
*				SecDerivativeDecreaseRight
*
*	Description:	Элементарное условие убывания производной справа
*	Author:		dk
*	History:
*
****************************************************************************/

#ifndef __AXIOMS_LIB_EL_SecDerivativeDecreaseRight_HXX
#define __AXIOMS_LIB_EL_SecDerivativeDecreaseRight_HXX

#include "ElemCondition.h"

namespace AxiomLib { 

class SecDerivativeDecreaseRight : public ElemCondition {

  private:
	//объявления, необходимые для сериализации
	friend class boost::serialization::access;

	//функция сериализации - в ней должны быть указаны члены-данные,
	//характеризующие состояние объекта
	template<class Archive>
	void serialize(Archive & archive, const unsigned int /*version*/)
	{
		archive & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ElemCondition);
		archive & BOOST_SERIALIZATION_NVP(levelDelta);
		archive & BOOST_SERIALIZATION_NVP(secDerivDecreaseLeft);
		archive & BOOST_SERIALIZATION_NVP(secDerivDecreaseLeftLimit);
		archive & BOOST_SERIALIZATION_NVP(secDerivDecreaseMaxLevel);
		archive & BOOST_SERIALIZATION_NVP(secDerivDecreaseMaxLevelLimit);
		archive & BOOST_SERIALIZATION_NVP(secDerivDecreaseMinLevel);
		archive & BOOST_SERIALIZATION_NVP(secDerivDecreaseMinLevelLimit);
		archive & BOOST_SERIALIZATION_NVP(secDerivDecreaseRight);
		archive & BOOST_SERIALIZATION_NVP(secDerivDecreaseRightLimit);
	}

	// Минимальная граница изменения для параметра secDerivDecreaseMinLevel, которая учитывается при подборе параметров перебором
	double secDerivDecreaseMinLevelLimit;

	// Максиомальная граница изменения для параметра secDerivDecreaseMaxLevel, которая учитывается при подборе параметров перебором
	double secDerivDecreaseMaxLevelLimit;

	// Шаг изменения параметров secDerivDecreaseMinLevel и secDerivDecreaseMaxLevel, при подборе параметров перебором
	double levelDelta;

	// Максимальное значение параметра secDerivDecreaseLeft, которое учитывается при подборе параметров перебором
	int secDerivDecreaseLeftLimit;

	// Максимальное значение параметра secDerivDecreaseRight, которое учитывается при подборе параметров перебором
	int secDerivDecreaseRightLimit;

  protected:
	  
	// Минимальное возрастание подходящее данному элементарному условию
	double secDerivDecreaseMinLevel;
	
	// Максимальное возрастание пододящее данному элементарному условию
	double secDerivDecreaseMaxLevel;
	
	// Число точек требуемых для элементарного условия - слева
	int secDerivDecreaseLeft;
	
	// Число точек требуемых для элементарного условия - справа
	int secDerivDecreaseRight;
	
  public:
	
	// Пустой конструктор
	SecDerivativeDecreaseRight(void);
	
	// Деструктор - удаляает все созданные динамические объекты
	~SecDerivativeDecreaseRight(void) {};
	
	// Функция возвращает имя данного элементарного условия
	std::string name(void) const;
	
	// Функция проверки - выполняется ли данное элементарное условие в точке ряда
	inline signed int check (const unsigned long k, const std::vector<double>& x) const;

	// Оператор сравнения для данного элементарного условия
	double operator< (const ElemCondition& second) const;
	
	// Оператор мутации элементарного условия
	signed int transmute (const double p);
	
	// Оператор задания параметров данного элементарного условия
	signed int setParams (const std::map<std::string,std::string>& newParams);
	
	// Функция возвращает текущие выставленные параметры
	signed int getParams (std::map<std::string,std::string> &params) const;
	
	// Функция устанавливает значение одного параметра элементарного условия, если такой параметр присутствует в данном элементарном условии
	signed int setParamValue (const double param, const std::string &paramName);
	
	// Функция записывает значение одного параметра элементарного условия, если такой параметр присутствует в данном элементарном условии
	signed int getParamValue (double &param, const std::string &paramName) const;
	
	// Функция заполняет входной вектор строками с названиями параметров данной элементарной аксиомы
	signed int getECParamNames (std::vector<std::string> &ecParamNames) const;
	
	// Оператор проверки совместимости элемнтарного условия с задаваемым в параметрах
	signed int checkAndCompatibility (const ElemCondition& toCheck) const;

	// Функция устанавливает максимальные значения параметров secDerivDecreaseLeftLimit и secDerivDecreaseRightLimit
	signed int setLimits (const int newLeftLimit, const int newRightLimit);

	// Функция устанавливает значения параметров secDerivDecreaseMinLevelLimit, secDerivDecreaseMaxLevelLimit и levelDelta
	signed int setLimits (const std::vector<double>& row, const double reserve, const unsigned int numOfLevels);

	// Функция изменяет параметры элементарного условия на число шагов, указанное в параметрах
	signed int changeParams (const int numOfSteps);
	
	// Функция клонирования - реализация шаблона "виртуальный конструктор копирования"
	SecDerivativeDecreaseRight* clone() const { return new SecDerivativeDecreaseRight(*this); }

	// Функция присваивает данному ЭУ ЭУ other и возвращает true, если у них совпадает тип
	// Иначе возвращает false
	bool assign(const ElemCondition *other) { return assign_impl(this, other); }
	
}; // of class

}; // of namespace



#endif /* __AXIOMS_LIB_EL_SecDerivativeDecreaseRight_HXX */
