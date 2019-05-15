#ifndef EXCEPTIONS_HANDLER
# define EXCEPTIONS_HANDLER

# include <exception>

enum e_exception {
	LEXICAL_ERROR,
	NOT_ENOUGH_ARGUMENTS,
	TOO_MUCH_ARGUMENTS,
	UNKNOWN_INSTRUCTION,
	UNDERFLOW,
	OVERFLOW,
	EMPTY_STACK,
	DIVISION_BY_ZERO,
	NO_EXIT,
	BAD_ASSERT,
	NOT_ENOUGH_VALUES_ON_STACK,
	FLOATING_POINT_MODULUS,
	NO_INT8,
};

class VMException : public std::exception
{
	public:
		VMException(e_exception exception) {
			this->_exception = exception;
		}

		virtual const char * what() const throw() {
			static std::string	verbose[] = {
				"This instruction contain one or several lexical errors or syntactic errors.",
				"Not enough arguments for this instruction.",
				"Too much arguments for this instruction.",
				"Unknown instruction.",
				"Underflow on a value.",
				"Overflow on a value.",
				"Empty stack at this point.",
				"Division/modulo by 0.",
				"The program doesn't exit properly.",
				"Bad assertion.",
				"The stack is composed of strictly less that two values when an arithmetic instruction is executed.",
				"Floating point modulus is not a valid instruction.",
				"This instruction require top operand on stack to be int8"
			};

			return (verbose[_exception].c_str());
		}

	private:
		e_exception _exception;
};

#endif