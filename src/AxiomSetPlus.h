/****************************************************************************
*					AxiomSetPlus
*
*	Description:	Система аксиом + статистика по ней + параметры по ней
*	Author:			dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_AXIOM_SET_PLUS_HXX
#define __AXIOMS_LIB_AXIOM_SET_PLUS_HXX

#include "AxiomSet.h"

namespace AxiomLib {

struct AxiomSetPlus {

 private:

 protected: 
  
 public:

	// Система аксиом
	AxiomSet* axiomSet; 

	// Статистика по системе аксиом
	double goal; // значение целевой функции
	int nFirst;  // кол-во ошибок первого рода
	int nSecond; // кол-во ошибок второго рода
	std::vector <double> axiomProfit; // Полезность аксиом системы

	// Производная статистики - параметры системы аксиом
	double goalRatio;

	// Параметры скрещивания
	double asCrossProb; // средний уровень (+-0.5)
	double axCrossProb; // должен быть повыше (+-0.8) так как от системы аксиом и так в среднем 1-2 аксиомы могут быть выбраны, а потом еще и данная вероятность учитывается
	double ecCrossProb; // должен быть повыше (+-0.8) так как и так в среднем 1-2 ЭУ могут быть выбраны, а потом еще и данная вероятность учитывается

	// Параметры мутации
	// Мутация на уровне приоритета аксиом в системе 
	double prTransProb; // ниже среднего (0.2) так как применяется для всех аксиом в системе
	double asTransProb; // очень низкий (0.05) так как применяется для каждой аксиомы и очень дистабелизирует
	double axTransProb; // низкий (+-0.1) так как с данной вероятностью происходит замена ЭУ в аксиоме
	double ecTransProb; // низкий (+-0.1) так как применяется к каждому ЭУ в аксиоме

	// ********************* Функции структуры ***************************

	// Конструктор структуры
	AxiomSetPlus (void);

	// Деструктор структуры
	~AxiomSetPlus (void);

	// Функция очистки содержимого объекта структуры
	int clear (void);

	// Функция очистки статистики по системе аксиом
	int clearStat (void);

	AxiomSetPlus(const AxiomSetPlus& second);
	
	// Оператор присваивания системы аксиом
    AxiomSetPlus & operator= (const AxiomSetPlus &second);
    
}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_AXIOM_SET_PLUS_HXX */
