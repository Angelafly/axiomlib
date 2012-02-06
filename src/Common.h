#ifndef	AXIOMLIBCOMMON_H
#define AXIOMLIBCOMMON_H

#include <algorithm>
#include <iterator>
#include <vector>

namespace AxiomLib {
// ������� ���������� ������ ������� ��������� �������� whatToFind ��� -1, ���� ������� �� ������
template<class T> 
int isIn(const T& whatToFind, const std::vector<T>& whereToFind)
{
	auto iter = std::find(whereToFind.begin(), whereToFind.end(), whatToFind);
	return iter != whereToFind.end() ? static_cast<int>(std::distance(whereToFind.begin(), iter)) : -1;
}

}

#endif // AXIOMLIBCOMMON_H
