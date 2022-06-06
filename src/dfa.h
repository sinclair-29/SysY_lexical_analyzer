#ifndef DFA_H
#define DFA_H

#include <cctype>
#include <string>
#include <cstdio>
#include <map>
#include <iostream>

extern int current_line;
enum symbol {
	ALPHA = 0, NONZERO_DIGIT, ZERO, OPERATOR, ILLEGAL
};

const std::string error_message[5] {
	"Unrecognized character", 
	"illegal identifier",
	"illegal number",
	"unterminated comment",
	"unmatched string"
};

enum token_type {
	IDENTIFIER = 0, KEYWORD, 
	DECIMAL_INT, DECIMAL_FLOAT, 
	OCTAL_INT, OCTAL_FLOAT, 
	HEXADECIMAL_INT, HEXADECIMAL_FLOAT,
	ARITHMETIC_OPERATOR, RELATION_OPERATOR, 
	LEFT_PARENTHESIS, LEFT_BRACKET,
	LEFT_BRACE, RIGHT_PARENTHESIS, 
	RIGHT_BRACKET, RIGHT_BRACE, 
	COMMA, SEMICOLON, 
	BOOLEAN_OPERATOR, 
	ASSIGNMENT
};

int get_symbol(char);
bool isunderline(char);

void output_error_message(std::string);
void output_token(int type, std::string value);

class DFA {
	typedef bool (DFA::*state) (char next_character);
private:
	state current_state;
	std::string current_token;
	static std::map<std::string, int> keyword;
	static std::map<std::string, int> table;
	static int total_token;
	static void initialize_keywork(std::string _s, int _m);

public:
	DFA();
	DFA(int);
	static void initialize();

	void reset(char);
	bool transfer(char);
	void terminate();
	bool is_comment();
	bool is_end_comment();

	bool state_identifier (char);
	// for decimal integer
	bool state_decimal1(char);
	bool state_decimal2(char);
	bool state_decimal3(char);
	// for decimal floating-point
	bool state_decimal4(char);
	bool state_decimal5(char);
	bool state_decimal6(char);
	bool state_decimal7(char);
	bool state_zero(char);
	bool state_prefix(char);
	bool state_octal1(char);
	bool state_octal2(char);
	bool state_octal3(char);
	bool state_octal4(char);
	bool state_octal5(char);
	bool state_octal6(char);
	bool state_octal7(char);
	bool state_hexadecimal1(char);
	bool state_hexadecimal2(char);
	bool state_hexadecimal3(char);
	bool state_hexadecimal4(char);
	bool state_hexadecimal5(char);
	bool state_hexadecimal6(char);
	bool state_hexadecimal7(char);
	bool state_zero1(char);
	bool state_zero2(char);
	bool state_zero3(char);
	bool state_zero4(char);
	bool state_comment1(char);
	bool state_comment2(char);
	bool state_comment3(char);
	bool state_division(char);
	bool state_not1(char);
	bool state_not2(char);
	bool state_eq1(char);
	bool state_eq2(char);
	bool state_lt1(char);
	bool state_lt2(char);
	bool state_gt1(char);
	bool state_and1(char);
	bool state_or1(char);
	bool state_or2(char);
	bool state_gt2(char);
	bool state_and2(char);

	bool state_other(char);
};

#endif