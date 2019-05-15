#include "Operand.hpp"
#include "VMExceptions.hpp"
#include "OperandFactory.hpp"

/* Template specializations */

template class Operand<int8_t>;
template class Operand<int16_t>;
template class Operand<int32_t>;
template class Operand<float>;
template class Operand<double>;

/* static generic function(s) for all specializations */

static IOperand const * cast_into(IOperand const * operand, eOperandType type) {
	static OperandFactory factory;

	return (factory.createOperand(type, operand->toString()));
}

/* PUBLIC */

/* Constructors */

template<typename T>
Operand<T>::Operand(std::string const & value) {
	try
	{
		this->_value = convert(value);
		this->_string = into_string();
	}
	catch (std::out_of_range const & e)
	{
		if (value[0] == '-')
			throw VMException(UNDERFLOW);
		throw VMException(OVERFLOW);
	}
}
template<typename T>
Operand<T>::Operand(Operand const & e) {
	*this = e;
}

/* Destructor */

template<typename T>
Operand<T>::~Operand() {}

/* Operators */

template<typename T>
Operand<T> const & Operand<T>::operator=(Operand const & e) {
	this->_value = e._value;
	this->_string = e._string;
	return (this);
}

template<typename T>
IOperand const * Operand<T>::operator+(IOperand const & o2) const {
	if (this->getPrecision() < o2.getPrecision())
		return (o2 + *this);

	return (Operand<T>::add(*this, Operand<T>(o2.toString())));
}

template<typename T>
IOperand const * Operand<T>::operator-(IOperand const & o2) const {
	if (this->getPrecision() < o2.getPrecision()){
		IOperand const * o1 = cast_into(this, o2.getType());
		IOperand const * result = *o1 - o2;
		delete o1;
		return (result);
	}
	return (Operand<T>::sub(*this, Operand<T>(o2.toString())));
}

template<typename T>
IOperand const * Operand<T>::operator*(IOperand const & o2) const {
	if (this->getPrecision() < o2.getPrecision())
		return (o2 * *this);

	return (Operand<T>::mul(*this, Operand<T>(o2.toString())));
}

template<typename T>
IOperand const * Operand<T>::operator/(IOperand const & o2) const {
	if (this->getPrecision() < o2.getPrecision()) {
		IOperand const * o1 = cast_into(this, o2.getType());
		IOperand const * result = *o1 / o2;
		delete o1;
		return (result);
	}

	return (Operand<T>::div(*this, Operand<T>(o2.toString())));
}

template<typename T>
IOperand const * Operand<T>::operator%(IOperand const & o2) const {
	if (this->getPrecision() < o2.getPrecision()) {
		IOperand const * o1 = cast_into(this, o2.getType());
		IOperand const * result = *o1 % o2;
		delete o1;
		return (result);
	}

	return (Operand<T>::mod(*this, Operand<T>(o2.toString())));
}
template<>
IOperand const * Operand<float>::operator%(IOperand const & __attribute__((unused))) const {
	throw VMException(FLOATING_POINT_MODULUS);
}
template<>
IOperand const * Operand<double>::operator%(IOperand const & __attribute__((unused))) const {
	throw VMException(FLOATING_POINT_MODULUS);
}

/* Methods */

template<typename T>
std::string const & Operand<T>::toString(void) const
{
	return (this->_string);
}

template<>
int	Operand<int8_t>::getPrecision(void)		const { return (0); }
template<>
int	Operand<int16_t>::getPrecision(void)	const { return (1); }
template<>
int	Operand<int32_t>::getPrecision(void)	const { return (2); }
template<>
int	Operand<float>::getPrecision(void)		const { return (3); }
template<>
int	Operand<double>::getPrecision(void)		const { return (4); }

template<>
eOperandType	Operand<int8_t>::getType(void)	const { return (Int8); }
template<>
eOperandType	Operand<int16_t>::getType(void)	const { return (Int16); }
template<>
eOperandType	Operand<int32_t>::getType(void)	const { return (Int32); }
template<>
eOperandType	Operand<float>::getType(void)	const { return (Float); }
template<>
eOperandType	Operand<double>::getType(void)	const { return (Double); }

/* PRIVATE */

template<typename T>
Operand<T>::Operand(const T value) {
	this->_value = value;
	this->_string = into_string();
}

template<typename T>
Operand<T> const *	Operand<T>::add(Operand<T> const & o1, Operand<T> const & o2) {
	T result = o1._value + o2._value;
	if (o1._value < 0 && o2._value < 0 && result > 0)
		throw VMException(UNDERFLOW);
	if (o1._value > 0 && o2._value > 0 && result < 0)
		throw VMException(OVERFLOW);
	return (new Operand<T>(result));
}
template<typename T>
Operand<T> const *	Operand<T>::sub(Operand<T> const & o1, Operand<T> const & o2) {
	T result = o1._value - o2._value;
	if (o1._value < 0 && o2._value > 0 && result > 0)
		throw VMException(UNDERFLOW);
	if (o1._value > 0 && o2._value < 0 && result < 0)
		throw VMException(OVERFLOW);
	return (new Operand<T>(result));
}
template<typename T>
Operand<T> const *	Operand<T>::mul(Operand<T> const & o1, Operand<T> const & o2) {
	T result = o1._value * o2._value;
	if ((o1._value < 0 && o2._value < 0 && result < 0)
			|| (o1._value > 0 && o2._value > 0 && result < 0))
		throw VMException(OVERFLOW);
	if ((o1._value < 0 && o2._value > 0 && result > 0)
			|| (o1._value > 0 && o2._value < 0 && result > 0))
		throw VMException(UNDERFLOW);
	return (new Operand<T>(result));
}
template<typename T>
Operand<T> const *	Operand<T>::div(Operand<T> const & o1, Operand<T> const & o2) {
	if (o2._value == 0)
		throw VMException(DIVISION_BY_ZERO);
	T result = o1._value / o2._value;
	if ((o1._value < 0 && o2._value < 0 && result < 0)
			|| (o1._value > 0 && o2._value > 0 && result < 0))
		throw VMException(OVERFLOW);
	if ((o1._value < 0 && o2._value > 0 && result > 0)
			|| (o1._value > 0 && o2._value < 0 && result > 0))
		throw VMException(UNDERFLOW);
	return (new Operand<T>(result));
}
template<typename T>
Operand<T> const *	Operand<T>::mod(Operand<T> const & o1, Operand<T> const & o2) {
	if (o2._value == 0)
		throw VMException(DIVISION_BY_ZERO);
	return (new Operand<T>(o1._value % o2._value));
}

template<>
int8_t	Operand<int8_t>::convert(std::string const & value) const {
	int32_t i = stoi(value);

	if (i > INT8_MAX)
		throw VMException(OVERFLOW);
	else if (i < INT8_MIN)
		throw VMException(UNDERFLOW);
	return (i);
}
template<>
int16_t	Operand<int16_t>::convert(std::string const & value) const {
	int32_t i = stoi(value);

	if (i > INT16_MAX)
		throw VMException(OVERFLOW);
	else if (i < INT16_MIN)
		throw VMException(UNDERFLOW);
	return (i);
}
template<>
int32_t	Operand<int32_t>::convert(std::string const & value) const {
	return (stoi(value));
}
template<>
float	Operand<float>::convert(std::string const & value) const {
	return (stof(value));
}
template<>
double	Operand<double>::convert(std::string const & value) const {
	return (stod(value));
}

template<typename T>
std::string	Operand<T>::into_string(void) const {
	std::ostringstream	oss;
	oss << this->_value;
	return (std::string(oss.str()));
}
template<>
std::string	Operand<int8_t>::into_string(void) const {
	int16_t i = static_cast<int16_t>(this->_value);
	std::ostringstream	oss;
	oss << i;
	return (std::string(oss.str()));
}