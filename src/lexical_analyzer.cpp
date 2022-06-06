#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdio>
#include "dfa.h"
#include "lexical_analyzer.h"

int main() {
	std::string filename;

	/**
	 * For debug
	 * */
	//filename = "test.txt";
	std::cin >> filename;
	std::ifstream infile(filename);

	std::stringstream buffer;	
	buffer << infile.rdbuf();
	contents = buffer.str();
	lexical_analyze();

	infile.close();

	return 0;
}



void lexical_analyze() {
	printf("%-30s%s\n", "TOKEN-TYPE", "TOKEN-VALUE");
	printf("-------------------------------------------------\n");
	DFA::initialize();	
	int length = contents.size();
	bool flag = false;
	for(int i = 0; i < length; ++i) {
		if(contents[i] == EOF) break;
		if(isws(contents[i]) && !dfa.is_comment()) {
			if(flag) {
				dfa.terminate();
				flag = false;
			}
			continue;
		}
		if(!flag) {
			int start_symbol = get_symbol(contents[i]);
			if(start_symbol == symbol::ILLEGAL) {
				output_error_message(error_message[0]);
			} else {
				dfa.reset(contents[i]);
				flag = true;
			}
		} else {
			if(dfa.is_comment()) {
				dfa.transfer(contents[i]);
				if(dfa.is_end_comment()) {
					dfa.terminate();
					flag = false;
				}
				continue;
			}
			int current_symbol = get_symbol(contents[i]);
			if(current_symbol != symbol::ILLEGAL && dfa.transfer(contents[i])) {
				continue;
			} else {
				if(current_symbol == symbol::ILLEGAL) {
					dfa.terminate();
					flag = false;
					output_error_message(error_message[0]);
				} else {
					dfa.terminate();
					i -= 1;
					flag = false;
				}
			}
		}
	}
	if(flag) dfa.terminate();
}

bool isws(char ch) {
	if(ch == ' ' || ch == '\t') return true;
	if(ch == '\n') {
		current_line += 1;
		return true;
	}
	return false;
}

