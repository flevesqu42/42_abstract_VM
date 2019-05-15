#include "AbstractVM.hpp"
#include <fstream> 

static std::vector<std::string>	get_lines_from_stdin(void) {
	std::vector<std::string> lines;

	for (std::string line; std::getline(std::cin, line); ){
		size_t	end = line.find(";;");
		if (end != std::string::npos)
		{
			lines.push_back(line.substr(0, end));
			break ;
		}
		lines.push_back(line);
	}
	return (lines);
}

static std::vector<std::string>	get_lines_from_file(char *file) {
	std::vector<std::string>	lines;
	std::ifstream				is;

	is.open(std::string(file));
	if (is.fail())
	{
		std::cerr	<< "Error: cannot open file `"
					<< std::string(file)
					<< "` ("
					<< strerror(errno)
					<< ")"
					<< std::endl;
		exit(1);
	}
	for (std::string line; std::getline(is, line); ) {
		lines.push_back(line);
	}
	is.close();
	return (lines);
}


int	main(int ac, char **av)
{
	AbstractVM	vm;

	if (ac < 2)
		vm.run(get_lines_from_stdin());
	for (int i = 1; i < ac; i++)
		vm.run(get_lines_from_file(av[i]));
	return (0);
}