#ifndef VMSTACK_HPP
# define VMSTACK_HPP

# include <list>
# include <iostream> 
# include "IOperand.hpp"

class VMStack
{
	public:
		VMStack(void);
		VMStack(VMStack const & e);
		~VMStack(void);

		VMStack & operator=(VMStack const & e);

		void				push(IOperand const *);
		IOperand const *	pop(void);
		void				dump(void) const;
		void				assert(IOperand const *) const;
		void				add(void);
		void				sub(void);
		void				mul(void);
		void				div(void);
		void				mod(void);
		void				print(void) const;
		void				exit(void);

	private:
		std::list<IOperand const *>	_stack;
};

#endif