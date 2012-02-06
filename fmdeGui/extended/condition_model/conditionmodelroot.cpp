#include "conditionmodelroot.h"
#include "conditionmodelonecondition.h"

ConditionModelRoot::ConditionModelRoot() : ASModelTreeItemComposite(0)
{
}

int ConditionModelRoot::columnCount() const {
	return 3;
}

QVariant ConditionModelRoot::data(int column) const {
	switch(column) {
	case 0: return encode("���");
	case 1: return encode("��. �� ��.");
	case 2: return encode("����. �������");
	case 3: return encode("��1");
	case 4: return encode("��2");
	default: return QVariant();
	}
}

void ConditionModelRoot::addCondition(const PCondition &condition) {
	addChild(
			new ConditionModelOneCondition(this, condition)
			);
}

void ConditionModelRoot::clear() {
	clearChildren();
}
