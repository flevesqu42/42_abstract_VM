#include "AbstractVM.hpp"
#include <sstream>
#include <vector>
#include <unordered_map>
#include <regex>

/* PUBLIC */

/* constructors */

AbstractVM::AbstractVM(void) {}

AbstractVM::AbstractVM(AbstractVM const & e) {
	*this = e;
}

/* destructor */

AbstractVM::~AbstractVM(void) {}

/* operators */

AbstractVM & AbstractVM::operator=(AbstractVM const & e) {
	this->_stack = e._stack;
	this->_operand_factory = e._operand_factory;
	return (*this);	
}

/* Methods */

void AbstractVM::run(std::vector<std::string> lines) {
	size_t		line_counter = 1;
	std::string	last_valid_instruction("");

	for (std::string line : lines) {

		std::istringstream iss(line);
		std::getline(iss, line, ';');

		std::vector<std::string> words = AbstractVM::split(line, ' ');

		if (!words.size())
			continue ;
		try
		{
			this->parse_instruction(words);
			last_valid_instruction = words[0];
		}
		catch (VMException e) {
			error_handler(e, line_counter);
		}
		++line_counter;
	}
	if (last_valid_instruction != "exit")
		error_handler(VMException(NO_EXIT), line_counter);
}

/* PRIVATE */

void	AbstractVM::error_handler(VMException e, size_t line) {
	std::cerr
				<< "Error - line "
				<< line
				<< " : "
				<<  e.what()
				<< std::endl;
}

/* grammar */

void AbstractVM::parse_instruction(std::vector<std::string> & words) {
	void (AbstractVM::*instruction)(std::vector<std::string> &);
	static std::unordered_map<std::string, void (AbstractVM::*)(std::vector<std::string> &)> map = {
		{"push",	&AbstractVM::parse_push},
		{"pop",		&AbstractVM::parse_pop},
		{"dump",	&AbstractVM::parse_dump},
		{"assert",	&AbstractVM::parse_assert},
		{"add",		&AbstractVM::parse_add},
		{"sub",		&AbstractVM::parse_sub},
		{"mul",		&AbstractVM::parse_mul},
		{"div",		&AbstractVM::parse_div},
		{"mod",		&AbstractVM::parse_mod},
		{"print",	&AbstractVM::parse_print},
		{"exit",	&AbstractVM::parse_exit},
	};

	if (!(instruction = map[words[0]]))
		throw VMException(UNKNOWN_INSTRUCTION);

	((this->*instruction)(words));
}

IOperand const * AbstractVM::parse_value(std::vector<std::string> & value) {
	static std::regex	reg_int("^(int8|int16|int32)\\(([-]?[0-9]+)\\)$");
	static std::regex	reg_fp("^(float|double)\\(([-]?[0-9]+\\.[0-9]+)\\)$");
	static std::unordered_map<std::string, eOperandType> map_type = {
		{"int8",	Int8},
		{"int16",	Int16},
		{"int32",	Int32},
		{"float",	Float},
		{"double",	Double},
	};

	std::smatch	base_match;

	if (!std::regex_match(value[0], base_match, reg_int)
			&& !std::regex_match(value[0], base_match, reg_fp))
		throw VMException(LEXICAL_ERROR);

	eOperandType	type = map_type[base_match[1]];
	std::string		string = base_match[2];

	return (this->_operand_factory.createOperand(type, string));
}

/* instructions */

void	AbstractVM::parse_push(std::vector<std::string> & words) {
	if (words.size() < 2)
		throw VMException(NOT_ENOUGH_ARGUMENTS);
	if (words.size() > 2)
		throw VMException(TOO_MUCH_ARGUMENTS);

	std::vector<std::string> value(words.begin() + 1, words.end());
	IOperand const * operand = AbstractVM::parse_value(value);

	this->_stack.push(operand);
}
void	AbstractVM::parse_pop(std::vector<std::string> & words) {
	if (words.size() != 1)
		throw VMException(TOO_MUCH_ARGUMENTS);

	this->_stack.pop();
}
void	AbstractVM::parse_dump(std::vector<std::string> & words) {
	if (words.size() != 1)
		throw VMException(TOO_MUCH_ARGUMENTS);

	this->_stack.dump();
}
void	AbstractVM::parse_assert(std::vector<std::string> & words) {
	if (words.size() < 2)
		throw VMException(NOT_ENOUGH_ARGUMENTS);
	if (words.size() > 2)
		throw VMException(TOO_MUCH_ARGUMENTS);

	std::vector<std::string> value(words.begin() + 1, words.end());
	IOperand const * operand = AbstractVM::parse_value(value);

	this->_stack.assert(operand);
}
void	AbstractVM::parse_add(std::vector<std::string> & words) {
	if (words.size() != 1)
		throw VMException(TOO_MUCH_ARGUMENTS);

	this->_stack.add();
}
void	AbstractVM::parse_sub(std::vector<std::string> & words) {
	if (words.size() != 1)
		throw VMException(TOO_MUCH_ARGUMENTS);

	this->_stack.sub();
}
void	AbstractVM::parse_mul(std::vector<std::string> & words) {
	if (words.size() != 1)
		throw VMException(TOO_MUCH_ARGUMENTS);

	this->_stack.mul();
}
void	AbstractVM::parse_div(std::vector<std::string> & words) {
	if (words.size() != 1)
		throw VMException(TOO_MUCH_ARGUMENTS);

	this->_stack.div();
}
void	AbstractVM::parse_mod(std::vector<std::string> & words) {
	if (words.size() != 1)
		throw VMException(TOO_MUCH_ARGUMENTS);

	this->_stack.mod();
}
void	AbstractVM::parse_print(std::vector<std::string> & words) {
	if (words.size() != 1)
		throw VMException(TOO_MUCH_ARGUMENTS);

	this->_stack.print();
}
void	AbstractVM::parse_exit(std::vector<std::string> & words) {
	if (words.size() != 1)
		throw VMException(TOO_MUCH_ARGUMENTS);

	this->_stack.exit();
}

/* parser */

std::vector<std::string>	AbstractVM::split(const std::string & str, char sep) {
	std::vector<std::string>	new_vec;
	std::istringstream			iss(str);

	for (std::string buffer; std::getline(iss, buffer, sep); ) {
		if (buffer != "")
			new_vec.push_back(buffer);
	}
	return (new_vec);
}