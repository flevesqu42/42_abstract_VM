#include "OperandFactory.hpp"
#include "Operand.hpp"

/* PUBLIC */

/* methods */

IOperand const * OperandFactory::createOperand( eOperandType type, std::string const & value ) const {
	static IOperand const * (OperandFactory::*create[])(std::string const & value) const =
	{
		NULL,
		&OperandFactory::createInt8,
		&OperandFactory::createInt16,
		&OperandFactory::createInt32,
		&OperandFactory::createFloat,
		&OperandFactory::createDouble,
	};

	return ((this->*create[type])(value));
};

/* PRIVATE */

IOperand const * OperandFactory::createInt8(std::string const & value ) const {
	return (new Operand<int8_t>(value));
};
IOperand const * OperandFactory::createInt16(std::string const & value ) const {
	return (new Operand<int16_t>(value));
};
IOperand const * OperandFactory::createInt32(std::string const & value ) const {
	return (new Operand<int32_t>(value));
};
IOperand const * OperandFactory::createFloat(std::string const & value ) const {
	return (new Operand<float>(value));
};
IOperand const * OperandFactory::createDouble(std::string const & value ) const {
	return (new Operand<double>(value));
};
