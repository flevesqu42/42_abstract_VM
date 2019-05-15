#ifndef OPERAND_HPP
# define OPERAND_HPP

# include <sstream>
# include "IOperand.hpp"

template <typename T>
class Operand : public IOperand {
	public:
		Operand(std::string const & value);
		Operand(Operand const & e);
		Operand const & operator=(Operand const & e);
		~Operand();

		IOperand const * operator+(IOperand const & rhs) const;
		IOperand const * operator-(IOperand const & rhs) const;
		IOperand const * operator*(IOperand const & rhs) const;
		IOperand const * operator/(IOperand const & rhs) const;
		IOperand const * operator%(IOperand const & rhs) const;

		int					getPrecision( void ) const;
		eOperandType		getType( void ) const;
		std::string const & toString(void) const;


	private:
		T			_value;
		std::string	_string;

		Operand(T value);

		static Operand<T>	const * add(Operand<T> const & o1, Operand<T> const & o2);
		static Operand<T>	const * sub(Operand<T> const & o1, Operand<T> const & o2);
		static Operand<T>	const * mul(Operand<T> const & o1, Operand<T> const & o2);
		static Operand<T>	const * div(Operand<T> const & o1, Operand<T> const & o2);
		static Operand<T>	const * mod(Operand<T> const & o1, Operand<T> const & o2);

		T			convert(std::string const & value) const ;
		std::string	into_string(void) const ;
};

#endif