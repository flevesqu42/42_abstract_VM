#ifndef ABSTRACT_VM_HPP
# define ABSTRACT_VM_HPP

# include <list>
# include <iostream> 
# include <vector> 
# include "OperandFactory.hpp"
# include "VMExceptions.hpp"
# include "VMStack.hpp"

class AbstractVM
{
	public:
		AbstractVM(void);
		AbstractVM(AbstractVM const & e);
		~AbstractVM(void);

		AbstractVM & operator=(AbstractVM const & e);

		void	run(std::vector<std::string> lines);

	private:
		VMStack			_stack;
		OperandFactory	_operand_factory;

		void	error_handler(VMException e, size_t line);

		/* grammar */

		void				parse_instruction(std::vector<std::string> & words);
		IOperand const *	parse_value(std::vector<std::string> & words);

		/* instructions */

		void	parse_push(std::vector<std::string> & words);
		void	parse_pop(std::vector<std::string> & words);
		void	parse_dump(std::vector<std::string> & words);
		void	parse_assert(std::vector<std::string> & words);
		void	parse_add(std::vector<std::string> & words);
		void	parse_sub(std::vector<std::string> & words);
		void	parse_mul(std::vector<std::string> & words);
		void	parse_div(std::vector<std::string> & words);
		void	parse_mod(std::vector<std::string> & words);
		void	parse_print(std::vector<std::string> & words);
		void	parse_exit(std::vector<std::string> & words);

		/* parser */

		static std::vector<std::string>	split(const std::string & str, char sep);
};

#endif