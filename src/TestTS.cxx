/****************************************************************************
*			Функции класса TestTS.h
*
*	Description:	Данный класс служит как еденица хранения тестовой информации
*				То есть в нем присутствуют поля для хранения временного ряда и для
*				хранения корректной разметки данного ряда. - Реализация класса.
*	Author:		dk
*	History:		
*
****************************************************************************/
#include "TestTS.h"

using namespace AxiomLib;

// Конструктор класса
TestTS::TestTS () {
	this->corrMarkUp.clear();
}

// Деструктор класса (пустой)
TestTS::~TestTS () {

}

// Функция возвращает число временных рядов в данной тестовой разметке
int TestTS::size(void) const {
	return data.size();
}

/****************************************************************************
*					TestTS::getTSByIndex
*
*	Description:	Функция выдает требуемый ряд по запросу, где 
*				элемент запроса - номер поля в каконической последовательности параметров
*	Parameters:	vec - заполняемое значение вектора чисел 
*				i - номер запрашиваемого ряда в канонической последовательности параметров
*	Returns:		true - если указанный ряд существует и он успешно записан в переменную vec
*				false - если запрашиваемый ряд не существует или не удалось его скопировать в vec
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
bool TestTS::getTSByIndex(std::vector<double> &vec, int i) const {
	// Если указан параметр -1 - то возвращаем корректную разметку
	if (i == -1) {
		return correct.getTSByIndex (vec, i);
	}
	// Если задан положительный параметр - то возвращаем ряд из мультиряда по его индексу во внутреннем представлении
	if (i >= 0) {
		return data.getTSByIndex (vec, i);
	}
	return false;
}


/****************************************************************************
*					TestTS::getTSByIndex
*
*	Description:	Функция выдает требуемый ряд по запросу, где 
*				элемент запроса - номер поля в каконической последовательности параметров.
*				Отличие от предыдущей функции - в формате параметров функции - в случае, 
*				если нужен вектор целых чисел, то выдается именно такой вектор.
*	Parameters:	vec - заполняемое значение вектора чисел 
*				i - номер запрашиваемого ряда в канонической последовательности параметров
*	Returns:		true - если указанный ряд существует и он успешно записан в переменную vec
*				false - если запрашиваемый ряд не существует или не удалось его скопировать в vec
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
bool TestTS::getTSByIndex(std::vector<int> &vec, int i) const {
	// Если указан параметр -1 - то возвращаем корректную разметку
	if (i == -1) {
		if (corrMarkUp.size() > 0) {
			vec = corrMarkUp;
			return true;
		} 
		return false;
	}
	// Если задан положительный параметр - то возвращаем ряд из мультиряда по его индексу во внутреннем представлении
	if (i >= 0) {
		// Внутренние переменный для послучения вектора рациональных чисел с последующим переводом в вектор целых чисел
		std::vector<double> vecTemp;
		bool boolTemp;
		// Запрос соответвтующего ряда рац. чисел
		boolTemp = data.getTSByIndex (vecTemp, i);
		if (boolTemp) {
			// Перевод ряда в целочисленный
			vec.resize(vecTemp.size());
			for (unsigned int i = 0; i < vecTemp.size(); i++)
				vec[i] = (int) vecTemp[i];
			return true;
		} 
		return false;
	}
	return false;
}


/****************************************************************************
*					TestTS::setTSByIndex
*
*	Description:	Функция устанвливает новое значение ряду по указанному параметру
*	Parameters:		vec - новый ряд
*					i - номер ряда в канонической последовательности параметров
*	Returns:		true - если успешно установлено новое значение
*					false - если новое значение не установлено
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
bool TestTS::setTSByIndex(std::vector<double> &vec, int i) {
	// Если указан параметр -1 - то устанавливаем новую корректную разметку
	if (i == -1) {
		if (correct.setTSByIndex (vec, i))
			return makeIntClone();
		return false;
	}
	// Если задан положительный параметр - то устанвливаем ряд из мультиряда по его индексу во внутреннем представлении
	if (i >= 0) {
		return data.setTSByIndex (vec, i);
	}
	return false;
}


/****************************************************************************
*					TestTS::setTSByIndex
*
*	Description:	Функция устанвливает новое значение ряду по указанному параметру
*					Отличие от предыдущей функции - в формате параметров функции - в случае, 
*					если нужен вектор целых чисел, то устанавливается именно такой вектор.
*	Parameters:		vec - новое значение вектора чисел 
*					i - номер ряда в канонической последовательности параметров
*	Returns:		true - если новое значение установлено успешно
*					false - если новое значение не установлено
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
bool TestTS::setTSByIndex(std::vector<int> &vec, int i) {
	// Создаем представление для данного ряда в виде ряда действительных чисел
	std::vector<double> vecTemp;
	vecTemp.resize(vec.size());
	for (unsigned int j = 0; j < vec.size(); j++)
		vecTemp[j] = (double) vec[j];
	// Если указан параметр -1 - то устанавливаем корректную разметку
	if (i == -1) {
		corrMarkUp.assign (vec.begin(), vec.end());
		return correct.setTSByIndex (vecTemp, i);
	}
	// Если задан положительный параметр - то устанавливаем ряд из мультиряда по его индексу во внутреннем представлении
	if (i >= 0) {		
		return data.setTSByIndex (vecTemp, i);
	}
	return false;
}


/****************************************************************************
*					TestTS::makeIntClone
*
*	Description:	Функция создает реплику корректной разметки в целоцисленном векторе corrMarkUp
*	Parameters:	-
*	Returns:		true - если удалось скопировать корректную разметку в целочисленный вектор corrMarkUp
*				false - если не удалось найти ряд корректной разметки
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/ 
bool TestTS::makeIntClone (void) {
	// Внутренние переменные для получения корректной разметки в виде ряда рац. чисел
	std::vector<double> vecTemp;
	bool boolTemp;
	// Запрос ряда корректной разметки
	boolTemp = correct.getTSByIndex (vecTemp, -1);
	if (boolTemp) {
		// Заполнение целочисленного ряда корректной разметки
		corrMarkUp.resize(vecTemp.size());
		for (unsigned int i = 0; i < vecTemp.size(); i++)
			corrMarkUp[i] = (int) vecTemp[i];
		return true;
	}
	// Если не удалось найти корректную разметку - возвращаем false
	return false;
}
