/**
	* Copyright (C) 2026 Indonesia Chip Design Collaborative Center (ICDEC)
	*
	* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	* NONINFRINGEMENT.
	*/

#include <stdio.h>

#include "String.hpp"

int main(void)
{
	String *init1 = new String;
	String *init2 = new String("test dynamic init2");
	String *init3 = new String(*init2);

	/* print sentence */
	printf("init1: %s\r\n", init1->c_str());
	printf("init2: %s\r\n", init2->c_str());
	printf("init3: %s\r\n", init3->c_str());

	delete init1;
	delete init2;
	delete init3;

	return 0;
}
