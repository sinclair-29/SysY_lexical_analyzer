#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <cstring>

std::string contents;
int current_pos = 0;
DFA dfa;

void lexical_analyze();
bool isws(char ch);

#endif
