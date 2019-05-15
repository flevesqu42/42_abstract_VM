#include "VMStack.hpp"
#include "VMExceptions.hpp"
#include "Operand.hpp"

/* PUBLIC */

/* constructors */

VMStack::VMStack(void) {}

VMStack::VMStack(VMStack const & e) {
	*this = e;
}

/* destructor */

VMStack::~VMStack(void) {}

/* operators */

VMStack & VMStack::operator=(VMStack const & e) {
	this->_stack = e._stack;
	return (*this);	
}

/* Methods */

void VMStack::push(IOperand const * operand) {
	this->_stack.push_front(operand);
}

IOperand const * VMStack::pop(void) {
	if (!_stack.size())
		throw VMException(EMPTY_STACK);

	IOperand const * elem = _stack.front();
	_stack.pop_front();

	return (elem);
}

void VMStack::dump(void) const {
	for (IOperand const * operand : this->_stack) {
		std::cout << operand->toString() << std::endl;
	}
}

void VMStack::assert(IOperand const *comp) const {
	if (!_stack.size())
		throw VMException(EMPTY_STACK);

	IOperand const * front = _stack.front();
	if (front->getType() != comp->getType() || front->toString() != comp->toString())
		throw VMException(BAD_ASSERT);
}

void VMStack::add(void) {
	if (_stack.size() < 2)
		throw VMException(NOT_ENOUGH_VALUES_ON_STACK);
	IOperand const * o1 = this->pop();
	IOperand const * o2 = this->pop();
	this->push(*o2 + *o1);
	delete o1;
	delete o2;
}

void VMStack::sub(void) {
	if (_stack.size() < 2)
		throw VMException(NOT_ENOUGH_VALUES_ON_STACK);
	IOperand const * o1 = this->pop();
	IOperand const * o2 = this->pop();
	this->push(*o2 - *o1);
	delete o1;
	delete o2;
}

void VMStack::mul(void) {
	if (_stack.size() < 2)
		throw VMException(NOT_ENOUGH_VALUES_ON_STACK);
	IOperand const * o1 = this->pop();
	IOperand const * o2 = this->pop();
	this->push(*o2 * *o1);
	delete o1;
	delete o2;
}

void VMStack::div(void) {
	if (_stack.size() < 2)
		throw VMException(NOT_ENOUGH_VALUES_ON_STACK);
	IOperand const * o1 = this->pop();
	IOperand const * o2 = this->pop();
	this->push(*o2 / *o1);
	delete o1;
	delete o2;
}

void VMStack::mod(void) {
	if (_stack.size() < 2)
		throw VMException(NOT_ENOUGH_VALUES_ON_STACK);
	IOperand const * o1 = this->pop();
	IOperand const * o2 = this->pop();
	this->push(*o2 % *o1);
	delete o1;
	delete o2;
}

void VMStack::print(void) const {
	if (!_stack.size())
		throw VMException(EMPTY_STACK);
	if (_stack.front()->getType() != Int8)
		throw VMException(NO_INT8);

	std::istringstream	iss(_stack.front()->toString());
	int16_t				i;

	iss >> i;
	std::cout << static_cast<int8_t>(i);
}

void VMStack::exit(void) {
	while (_stack.size() > 0)
	{
		delete _stack.front();
		_stack.pop_front();
	}
	_stack.clear();
}
