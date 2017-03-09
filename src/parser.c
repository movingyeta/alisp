#include "alisp.h"

#include <stdio.h>

enum {
	OP_INVALID,
	OP_PLUS,
	OP_MINUS,
	OP_MULT,
	OP_DIV
};

void
error_print(int error)
{
	if (error == ERROR_DIV_ZERO) {
		printf("Error: Division By Zero!");
	} else if (error == ERROR_BAD_OP)   {
		printf("Error: Invalid Operator!");
    } else if (error == ERROR_BAD_NUMBER)  {
        printf("Error: Invalid Number!");
	} else if (error == ERROR_LEXER) {
		printf("Error: Lexer");
    } else {
		printf("Error: Unknown");
	}
	
}

Value
value_number(long x)
{
	return (Value) {
		.type = VALUE_NUMBER,
		.number = x
	};
}

Value
value_error(int error)
{
	return (Value) {
		.type = VALUE_ERROR,
		.error = error
	};
}

void
value_print(Value value)
{
	if (value.type == VALUE_NUMBER) {
		printf("%ld", value.number);
	} else {
		error_print(value.error);
	}
}

void
value_print_line(Value value)
{
	value_print(value);
	putchar('\n');
}

int
eval_op(char* operator)
{
	if (strcmp(operator, "+") == 0) return OP_PLUS;
	if (strcmp(operator, "-") == 0) return OP_MINUS;
	if (strcmp(operator, "*") == 0) return OP_MULT;
	if (strcmp(operator, "/") == 0) return OP_DIV;

	// err!
	fprintf(stderr, "OPERATOR EXPECTED, not %s\n", operator);
	return OP_INVALID;
}

Value
eval_value(char* content)
{
	errno = 0;
	long x = strtol(content, NULL, 10);
	return (errno != ERANGE) ? value_number(x) : value_error(ERROR_BAD_NUMBER);
}

Value
eval_expr(mpc_ast_t* node)
{
	if (strstr(node->tag, "number")) {
		return eval_value(node->contents);
	} else {
		int operator = eval_op(node->children[1]->contents);
		Value x = eval_expr(node->children[2]);
		Value y = eval_expr(node->children[3]);
		
		if (x.type == VALUE_ERROR) return x;
		if (y.type == VALUE_ERROR) return y;
		
		if (operator == OP_PLUS) return value_number(x.number + y.number);
		if (operator == OP_MINUS) return value_number(x.number - y.number);
		if (operator == OP_MULT) return value_number(x.number * y.number);
		if (operator == OP_DIV) {
			if (y.number == 0) return value_error(ERROR_DIV_ZERO);
			else return value_number(x.number / y.number);
		}
	}
	return value_error(ERROR_UNKNOWN);
}

Parser*
parser_create()
{
	Parser* parser = (Parser*)malloc(sizeof(Parser));
	parser->number = mpc_new("number");
	parser->operator = mpc_new("operator");
	parser->expr = mpc_new("expr");
	parser->lispy = mpc_new("lispy");
	
	mpc_err_t* error = mpca_lang(MPCA_LANG_DEFAULT,
		"number		:	/-?[0-9]+/ ;"
		"operator	:	'+' | '-' | '*' | '/' ;"
		"expr		:	<number> | '(' <operator> <expr> <expr> ')' ;"
		"lispy		:	/^/ <operator> <expr> <expr> /$/ ;",
		parser->number, parser->operator, parser->expr, parser->lispy);
	if (error) {
		mpc_err_print(error);
		mpc_err_delete(error);
	}
	
	return parser;
}

Value
parser_run(Parser* parser, char* input)
{
	mpc_result_t result;
	if (mpc_parse("<stdin>", input, parser->lispy, &result)) {
		Value value = eval_expr(result.output);
		mpc_ast_delete(result.output);
		return value;
	} else {
		mpc_err_print(result.error);
		mpc_err_delete(result.error);
		return value_error(ERROR_LEXER);
	}
}

void
parser_destroy(Parser* parser)
{
	mpc_cleanup(4, parser->number, parser->operator, parser->expr, parser->lispy);
	free(parser);
}