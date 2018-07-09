//
// Created by Dominik Grybos on 31.05.2017.
//

#include <iostream>
#include "main.h"

void print_result(Result r)
{
	if(r == Result::Invalid)
		std::cout<< "Invalid\n";
	else if(r == Result::Failure)
		std::cout<< "Failure\n";
	else if(r == Result::Success)
		std::cout<< "Success\n";

}
