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
	String init1;
	/*String init2();*/
	String init3(nullptr);
	String init4("");
	String init5("test init 5");
	String init6(init4);
	String init7 = init5;
	String init8 = "test init 8";

	/* print sentence */
	printf("init1: %s\r\n", init1.c_str());
	/*printf("init2: %s\r\n", init2.c_str());*/
	printf("init3: %s\r\n", init3.c_str());
	printf("init4: %s\r\n", init4.c_str());
	printf("init5: %s\r\n", init5.c_str());
	printf("init6: %s\r\n", init6.c_str());
	printf("init7: %s\r\n", init7.c_str());
	printf("init8: %s\r\n", init8.c_str());

	return 0;
}
