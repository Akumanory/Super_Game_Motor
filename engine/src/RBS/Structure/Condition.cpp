#include "Condition.h"

#include "../../StringOp.h"

const std::string Condition::ArbitraryString = "*"; 


bool Condition::IsConstTest(Field::Name fieldName) const {
	return !Field::isParam(this->get(fieldName));
}

Condition Condition::GetIndex() const {
	Condition ret = *this;
	if (!ret.IsConstTest(Field::id))
		ret.fields.at(0) = Condition::ArbitraryString;
	if (!ret.IsConstTest(Field::attr))
		ret.fields.at(1) = Condition::ArbitraryString;
	if (!ret.IsConstTest(Field::value))
		ret.fields.at(2) = Condition::ArbitraryString;
	return ret;
}

const Condition::Type & Condition::GetType() const {
	return ConditionType;
}

void Condition::Print(int level) const {
	std::string prefix = CStringOp::repeat("\t", level);
	std::cout << prefix << fields.at(0) << ","
		<< (ConditionType == negative ? "~" : "")
		<< fields.at(1)
		<< "," << fields.at(2) << std::endl;
}

bool Condition::operator==(const Condition & rhs) const {
	return (fields.at(0) == rhs.fields.at(0)
		&& fields.at(1) == rhs.fields.at(1)
		&& fields.at(2) == rhs.fields.at(2)
		&& ConditionType == rhs.ConditionType);
}
