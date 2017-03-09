#include "lispy.h"

#include <stdio.h>

static char input[2048];

void
printLn(char* string)
{
	puts(string);
}

void
print(char* string)
{
	fputs(string, stdout);
}

void
read(size_t lenght, char* buffer)
{
	fgets(buffer, lenght, stdin);
}

int
main()
{
    printLn("Lispy version 0.0.1");
	printLn("Press Ctrl+C to exit\n");

	Parser* parser = parser_create();
	
	while (1) {
		print("lispy> ");
		read(2048, input);
		Value value = parser_run(parser, input);
		value_print_line(value);
	}
	
	parser_destroy(parser);
}