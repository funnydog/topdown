#include <iostream>

#include "application.hpp"

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	try
	{
		Application app;
		app.run();
		return 0;
	}
	catch (const std::exception &e)
	{
		std::cerr << "EXCEPTION: "
			  << e.what()
			  << std::endl;
		return 1;
	}
}
