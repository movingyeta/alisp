#pragma once

#include "mpc.h"

enum {
	VALUE_NUMBER,
	VALUE_ERROR
};

enum {
	ERROR_UNKNOWN,
	ERROR_DIV_ZERO,
	ERROR_BAD_OP,
	ERROR_BAD_NUMBER,
	ERROR_LEXER
};

typedef struct {
	int type;
	int error;
	long number;
} Value;

typedef struct {
	mpc_parser_t* number;
	mpc_parser_t* operator;
	mpc_parser_t* expr;
	mpc_parser_t* lispy;
} Parser;

Parser*
parser_create();

Value
parser_run(Parser* parser, char* input);

void
parser_destroy(Parser* parser);


Value
value_number(long x);

Value
value_error(int error);

void
value_print(Value value);

void
value_print_line(Value value);