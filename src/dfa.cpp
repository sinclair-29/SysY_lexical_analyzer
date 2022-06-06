#include "dfa.h"

int current_line = 1;

bool isunderline(char _ch) {
	if(_ch == '_') return true;
	return false;
}

int get_symbol(char _ch) {
	static char operator_char[20] = {
		'+', '-', '*', '/', '%',
		'=', '!', '<', '>',
		'&', '|',
		',', ';',
		'[', ']', '(', ')', '{', '}',
		'.'
	};
	if(std::isalpha(_ch) || isunderline(_ch)) return symbol::ALPHA;
	if(std::isdigit(_ch)) {
		if(_ch == '0') return symbol::ZERO;
		else return symbol::NONZERO_DIGIT;
	}
	for(auto ch : operator_char) 
		if(ch == _ch) return symbol::OPERATOR;
	return symbol::ILLEGAL;
}

void output_token(int type, std::string value) {
	switch(type) {
		case token_type::IDENTIFIER:
			printf("%-30s", "identifier");
			break;
		case token_type::KEYWORD:
			printf("%-30s", "keyword");
			break;
		case token_type::DECIMAL_INT:
			printf("%-30s", "decimal_integer");
			break;
		case token_type::DECIMAL_FLOAT:
			printf("%-30s", "decimal_float");
			break;
		case token_type::OCTAL_INT:
			printf("%-30s", "octal_integer");
			break;
		case token_type::OCTAL_FLOAT:
			printf("%-30s", "octal_float");
			break;
		case token_type::HEXADECIMAL_INT:
			printf("%-30s", "hexadecimal_integer");
			break;
		case token_type::HEXADECIMAL_FLOAT:
			printf("%-30s", "hexadecimal_float");
			break;
		case token_type::ARITHMETIC_OPERATOR:
			printf("%-30s", "arithemetic_operator");
			break;
		case token_type::RELATION_OPERATOR:
			printf("%-30s", "relationship_operator");
			break;
		case token_type::LEFT_PARENTHESIS:
			printf("%-30s", "left_parenthesis");
			break;
		case token_type::LEFT_BRACE:
			printf("%-30s", "left_brace");
			break;
		case token_type::LEFT_BRACKET:
			printf("%-30s", "left_bracket");
			break;
		case token_type::RIGHT_PARENTHESIS:
			printf("%-30s", "right_parenthesis");
			break;
		case token_type::RIGHT_BRACE:
			printf("%-30s", "right_brace");
			break;
		case token_type::RIGHT_BRACKET:
			printf("%-30s", "right_bracket");
			break;
		case token_type::COMMA:
			printf("%-30s", "comma");
			break;
		case token_type::SEMICOLON:
			printf("%-30s", "semicolon");
			break;	
		case token_type::BOOLEAN_OPERATOR:
			printf("%-30s", "boolean_operator");
			break;
		case token_type::ASSIGNMENT:
			printf("%-30s", "assignment");
			break;
	}
	std::cout << value << std::endl;
}

void output_error_message(std::string err) {
	printf("\nError at line %-3d: ", current_line);
	std::cout << err << std::endl << std::endl;
}

std::map<std::string, int> DFA::keyword;
std::map<std::string, int> DFA::table;
int DFA::total_token;
DFA::DFA() {

}

void DFA::initialize_keywork(std::string _s, int _m) {
	keyword[_s] = _m;
}

void DFA::initialize() {
	total_token = 0;
	initialize_keywork("int", 1);
	initialize_keywork("float", 2);
	initialize_keywork("const", 3);
	initialize_keywork("if", 4);
	initialize_keywork("while", 5);
	initialize_keywork("true", 6);
	initialize_keywork("false", 7);
	initialize_keywork("else", 8);
}


DFA::DFA(int start) {
	switch (start) {
		case symbol::ALPHA:
			this->current_state = &DFA::state_identifier;
			break;
		case symbol::NONZERO_DIGIT:
			this->current_state = &DFA::state_decimal1;
			break;	
	}
}

void DFA::reset(char ch) {
	int type = get_symbol(ch);
	if(type == symbol::OPERATOR) {
		switch(ch) {
			case '/':
				this->current_state = &DFA::state_division;
				break;
			case '=':
				this->current_state = &DFA::state_eq1;
				break;
			case '!':
				this->current_state = &DFA::state_not1;
				break;
			case '<':
				this->current_state = &DFA::state_lt1;
				break;
			case '>':
				this->current_state = &DFA::state_gt1;
				break;
			case '&':
				this->current_state = &DFA::state_and1;
				break;
			case '|':
				this->current_state = &DFA::state_or1;
				break;
			default:
				this->current_state = &DFA::state_other;
		}
	} else {
		switch(type) {
			case symbol::ALPHA:
				this->current_state = &DFA::state_identifier;
				break;
			case symbol::NONZERO_DIGIT:
				this->current_state = &DFA::state_decimal1;
				break;
			case symbol::ZERO:
				this->current_state = &DFA::state_zero;
		} 
	}
	current_token = "";
	current_token += ch;
}

void DFA::terminate() {
	if(this->current_state == &DFA::state_identifier) {
		if(keyword.find(this->current_token) == keyword.end()) {
			output_token(token_type::IDENTIFIER, this->current_token);
			if(table.find(this->current_token) == keyword.end())
				table[ this->current_token ] = ++total_token;
		} else {
			output_token(token_type::KEYWORD, this->current_token);
		}
		return;
	}
	// for deciaml integer and floating-point number
	if( this->current_state == &DFA::state_decimal2 ||
		this->current_state == &DFA::state_decimal4 ||
		this->current_state == &DFA::state_decimal6
		) {
		output_error_message(error_message[2]);
		return;
	}
	if( this->current_state == &DFA::state_decimal1 ||
		this->current_state == &DFA::state_decimal3 
	 ) {
		output_token(token_type::DECIMAL_INT, this->current_token);
		return;
	}
	if( this->current_state == &DFA::state_decimal5 ||
		this->current_state == &DFA::state_decimal7 
	 ) {
		output_token(token_type::DECIMAL_FLOAT, this->current_token);
		return;
	}
	// for octal integer and floating-point number
	if( this->current_state == &DFA::state_octal2 ||
		this->current_state == &DFA::state_octal4 ||
		this->current_state == &DFA::state_octal6
		) {
		output_error_message(error_message[2]);
		return;
	}
	if( this->current_state == &DFA::state_octal1 ||
		this->current_state == &DFA::state_octal3 
	 ) {
		output_token(token_type::DECIMAL_INT, this->current_token);
		return;
	}
	if( this->current_state == &DFA::state_octal5 ||
		this->current_state == &DFA::state_octal7 
	 ) {
		output_token(token_type::DECIMAL_FLOAT, this->current_token);
		return;
	}
	// for hexadecimal integer and floating-point number
	if( this->current_state == &DFA::state_hexadecimal2 ||
		this->current_state == &DFA::state_hexadecimal4 ||
		this->current_state == &DFA::state_hexadecimal6
		) {
		output_error_message(error_message[2]);
		return;
	}
	if( this->current_state == &DFA::state_hexadecimal1 ||
		this->current_state == &DFA::state_hexadecimal3 
	 ) {
		output_token(token_type::HEXADECIMAL_INT, this->current_token);
		return;
	}
	if( this->current_state == &DFA::state_hexadecimal5 ||
		this->current_state == &DFA::state_hexadecimal7 
	 ) {
		output_token(token_type::HEXADECIMAL_FLOAT, this->current_token);
		return;
	}
	// for floating-point number that begins with zero
	if( this->current_state == &DFA::state_zero1 ||
		this->current_state == &DFA::state_zero3 
		) {
		output_error_message(error_message[2]);
		return;
	}
	if( this->current_state == &DFA::state_zero2 ||
		this->current_state == &DFA::state_zero4 
	 ) {
		output_token(token_type::DECIMAL_FLOAT, this->current_token);
		return;
	}
	// for operators 
	if(this->current_state == &DFA::state_division) {
		output_token(token_type::ARITHMETIC_OPERATOR, this->current_token);
		return;
	}
	// for unterminated comments
	if( this->current_state == &DFA::state_comment1 ||
		this->current_state == &DFA::state_comment2
	 ) {
	 	output_error_message(error_message[3]);
		return;
	}
	// for terminated comments
	// do nothing
	if(this->current_state == &DFA::state_comment3) {
		return;
	}
	// for relationship operator
	if( this->current_state == &DFA::state_lt1 ||
		this->current_state == &DFA::state_lt1 ||
		this->current_state == &DFA::state_eq2 ||
		this->current_state == &DFA::state_gt1 ||
		this->current_state == &DFA::state_gt2 ||
		this->current_state == &DFA::state_not2
		) {
		output_token(token_type::RELATION_OPERATOR, this->current_token);
		return;
	}
	// for boolean operators 
	if( this->current_state == &DFA::state_and2 || 
		this->current_state == &DFA::state_or2  ||
		this->current_state == &DFA::state_not1
		) {
		output_token(token_type::BOOLEAN_OPERATOR, this->current_token);
		return;
	}
	// for assignment 
	if(this->current_state == &DFA::state_eq1) {
		output_token(token_type::ASSIGNMENT, this->current_token);
		return;
	}
	// for other characters
	if(this->current_state == &DFA::state_other) {
		switch(current_token[0]) {
			case '*':
			case '+':
			case '-':
			case '%':
				output_token(token_type::ARITHMETIC_OPERATOR, this->current_token);
			break;
			case '(':
				output_token(token_type::LEFT_PARENTHESIS, this->current_token);
			break;
			case ')':
				output_token(token_type::RIGHT_PARENTHESIS, this->current_token);
			break;
			case '[':
				output_token(token_type::LEFT_BRACKET, this->current_token);
			break;
			case ']':
				output_token(token_type::RIGHT_BRACKET, this->current_token);
			break;
			case '{':
				output_token(token_type::LEFT_BRACE, this->current_token);
			break;
			case '}':
				output_token(token_type::RIGHT_BRACE, this->current_token);
			break;
			case ',':
				output_token(token_type::COMMA, this->current_token);
			break;
			case ';':
				output_token(token_type::SEMICOLON, this->current_token);
			break;
		} 
		return;
	}

}

bool DFA::transfer(char next) {
	if( (this->*current_state) (next) ) {
		current_token += next;
		return true;
	} else {
		return false;
	}
}

bool DFA::is_comment() {
	if( this->current_state == &DFA::state_comment1 ||
		this->current_state == &DFA::state_comment2
		) return true;
	return false;
}

bool DFA::is_end_comment() {
	if(this->current_state == &DFA::state_comment3) return true;
	return false;
}


bool DFA::state_identifier(char next) {	
	if(isdigit(next) || isalpha(next) || isunderline(next)) {
		return true;
	} return false;
}

bool DFA::state_decimal1(char next) {
	if(isdigit(next)) return true;
	if(next == '.') {
		this->current_state = &DFA::state_decimal4;
		return true;
	}
	if(next == 'e') {
		this->current_state = &DFA::state_decimal2;
		return true;
	}
	return false;
}

bool DFA::state_decimal2(char next) {
	if(isdigit(next)) {
		this->current_state = &DFA::state_decimal3;
		return true;
	}
	return false;
}

bool DFA::state_decimal3(char next) {
	if(isdigit(next)) return true;
	return false;	
}

bool DFA::state_decimal4(char next) {
	if(isdigit(next)) {
		this->current_state = &DFA::state_decimal5;
		return true;
	}
	return false;
}

bool DFA::state_decimal5(char next) {
	if(isdigit(next)) return true;
	if(next == 'e') {
		this->current_state = &DFA::state_decimal6;
		return true;
	}
	return false;
}

bool DFA::state_decimal6(char next) {
	if(isdigit(next)) {
		this->current_state = &DFA::state_decimal7;
		return true;
	}
	return false;
}

bool DFA::state_decimal7(char next) {
	if(isdigit(next)) return true;
	return false;
}

bool DFA::state_zero(char next) {
	if(next == 'x' || next == 'X') {
		this->current_state = &DFA::state_prefix;
		return true;
	}
	if(isdigit(next) && next != '0') {
		this->current_state = &DFA::state_octal1;
		return true;
	}
	if(next == '.') {
		this->current_state = &DFA::state_zero1;
	}
	return false;
}

bool DFA::state_zero1(char next) {
	if(isdigit(next)) {
		this->current_state = &DFA::state_zero2;
		return true;
	}
	return false;
}

bool DFA::state_zero2(char next) {
	if(isdigit(next)) return true;
	if(next == 'e') {
		this->current_state = &DFA::state_zero3;
		return true;
	}
	return false;
}

bool DFA::state_zero3(char next) {
	if(isdigit(next)) {
		this->current_state = &DFA::state_zero4;
		return true;
	}
	return false;
}

bool DFA::state_zero4(char next) {
	if(isdigit(next)) return true;
	return false;
}

bool DFA::state_octal1(char next) {
	if(isdigit(next)) return true;
	if(next == '.') {
		this->current_state = &DFA::state_octal4;
		return true;
	}
	if(next == 'e') {
		this->current_state = &DFA::state_octal2;
		return true;
	}
	return false;
}

bool DFA::state_octal2(char next) {
	if(isdigit(next)) {
		this->current_state = &DFA::state_octal3;
		return true;
	}
	return false;
}

bool DFA::state_octal3(char next) {
	if(isdigit(next)) return true;
	return false;	
}

bool DFA::state_octal4(char next) {
	if(isdigit(next)) {
		this->current_state = &DFA::state_octal5;
		return true;
	}
	return false;
}

bool DFA::state_octal5(char next) {
	if(isdigit(next)) return true;
	if(next == 'e') {
		this->current_state = &DFA::state_octal6;
		return true;
	}
	return false;
}

bool DFA::state_octal6(char next) {
	if(isdigit(next)) {
		this->current_state = &DFA::state_octal7;
		return true;
	}
	return false;
}

bool DFA::state_octal7(char next) {
	if(isdigit(next)) return true;
	return false;
}

bool DFA::state_prefix(char next) {
	if(isdigit(next) && next != '0') {
		this->current_state = &DFA::state_hexadecimal1;
		return true;
	}
	return false;
}

bool DFA::state_hexadecimal1(char next) {
	if(isdigit(next)) return true;
	if(next == '.') {
		this->current_state = &DFA::state_hexadecimal4;
		return true;
	}
	if(next == 'e') {
		this->current_state = &DFA::state_hexadecimal2;
		return true;
	}
	return false;
}

bool DFA::state_hexadecimal2(char next) {
	if(isdigit(next)) {
		this->current_state = &DFA::state_hexadecimal3;
		return true;
	}
	return false;
}

bool DFA::state_hexadecimal3(char next) {
	if(isdigit(next)) return true;
	return false;	
}

bool DFA::state_hexadecimal4(char next) {
	if(isdigit(next)) {
		this->current_state = &DFA::state_hexadecimal5;
		return true;
	}
	return false;
}

bool DFA::state_hexadecimal5(char next) {
	if(isdigit(next)) return true;
	if(next == 'e') {
		this->current_state = &DFA::state_hexadecimal6;
		return true;
	}
	return false;
}

bool DFA::state_hexadecimal6(char next) {
	if(isdigit(next)) {
		this->current_state = &DFA::state_hexadecimal7;
		return true;
	}
	return false;
}

bool DFA::state_hexadecimal7(char next) {
	if(isdigit(next)) return true;
	return false;
}

bool DFA::state_comment1(char next) {
	if(next == '*') {
		this->current_state = &DFA::state_comment2;
	}
	return true;
}

/**/

bool DFA::state_comment2(char next) {
	if(next == '/') {
		this->current_state = &DFA::state_comment3;
		return true;
	}
	if(next != '*') {
		this->current_state = &DFA::state_comment1;
		return true;
	}
	return true;
}	

bool DFA::state_comment3(char next) {
	return false;
}

bool DFA::state_division(char next) {
	if(next == '*') {
		this->current_state = &DFA::state_comment1;
		return true;
	}
	return false;
}

bool DFA::state_not1(char next) {
	if(next == '=') {
		this->current_state = &DFA::state_not2;
	}
	return false;
}

bool DFA::state_not2(char next) {
	return false;
}

bool DFA::state_eq1(char next) {
	if(next == '=') {
		this->current_state = &DFA::state_eq2;
		return true;
	}
	return false;
}

bool DFA::state_eq2(char next) {
	return false;
}

bool DFA::state_lt1(char next) {
	if(next == '=') {
		this->current_state = &DFA::state_lt2;
		return true;
	}
	return false;
}

bool DFA::state_gt1(char next) {
	if(next == '=') {
		this->current_state = &DFA::state_gt2;
		return true;
	}
	return false;
}

bool DFA::state_and1(char next) {
	if(next == '&') {
		this->current_state = &DFA::state_and2;
		return true;
	}
	return false;
}

bool DFA::state_or1(char next) {
	if(next == '|') {
		this->current_state = &DFA::state_or2;
		return true;
	}
	return false;
}

bool DFA::state_lt2(char next) {
	return false;
}


bool DFA::state_or2(char next) {
	return false;
}

bool DFA::state_gt2(char next) {
	return false;
}

bool DFA::state_and2(char next) {
	return false;
}

bool DFA::state_other(char next) {
	return false;
}

