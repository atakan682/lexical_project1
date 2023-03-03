#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctype.h>

using namespace std;

class Lex
{
public:
	//constructor
	Lex();

	//FSM for INTEGER
	int int_DFSM(const string str);

	//FSM for REAL
	int real_DFSM(const string str);

	//FSM for IDENTIFIER
	int identifier_DFSM(const string str);

	//Function to find column # of the character in the table
	int char_to_col(const char input) const;

	bool isSeparator(const char input) const;
	bool isOperator(const char input) const;
	bool checkKeyword(string identifier) const;

	//Function classify to determine tokens
	int Classify(string);

	//Function returns a Token Lexeme
	void lexer(ifstream& file);

	//Punction prints them
	void print() const;

	//SETTERS
	void setToken(const string newToken);
	void setLexeme(const string newLexeme);

	//GETTERS
	string getToken() const;
	string getLexeme() const;

	//Destructor
	~Lex();
private:
	char input;
	string lexeme;
	string token;
};

#endif
