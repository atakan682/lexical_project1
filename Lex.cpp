/*
  Author information
  Author names: Mustafa Atakan Tan (mtan17@csu.fullerton.edu)
	  	Joshua Germing (Joshuagerming@gmail.com)
		Valentyna Shyyan (ValentynaShyyan@gmail.com)  

Program information

  Program name: Compiler Project 1
  Programming languages: C++, RAT23S
  Date program began: 2023 Feb 28
  Date of last update: 2023 Mar 3

  Status: Finished.

This file
   File name: Lex.cpp
   Language: C++
   Max page width: 132 columns
   Assemble using provided bash (.sh) file

===== Begin code area =============================================================================================
*/

#include "Lex.h"

//Constructor
Lex::Lex()
{
	input = 'c';
}

//Function to check the separator
bool Lex::isSeparator(const char input) const
{
	int separator[8] = { '(',')', '{', '}', ',', ';' , '#',':' };
	for (int i = 0; i < 8; i++) {
		if (separator[i] == input) {
			return 1;
		}
	}
	return 0;
}

//Function to check  the operator
bool Lex::isOperator(const char input) const
{
	int operators[11] = { '+', '-', '/', '*', '<','>', '=' , '==', '!=', '<=', '>=' };
	for (int i = 0; i < 11; i++) {
		if (operators[i] == input) {
			return 1;
		}
	}
	return 0;
}

//Function to check the keyword
bool Lex::checkKeyword(string identifier) const
{
	string keywords[14] = { "while", "if", "fi", "else", "return","put", "get", "int", "true", "false", "bool", "real", "function", "endwhile" };
	for (int i = 0; i < 14; i++) {
		if (keywords[i] == identifier) {
			return 1;
		}
	}
	return 0;
}

// Check string input to see if there is a separator, a letter, a digit, '_', '.' and return the category number for that type
int Lex::Classify(string s) {
	int len = s.length();

	//detect is operator or not
	for (int i = 0; i < len; i++)
	{
		if (isOperator(s[i]))
			return 1;
	}

    //loop the array to check if the string has separator assign to #2 for separator
    for (int i = 0; i < len; i++)
	{
		if (isSeparator(s[i]))
			return 2;
	}

	//get first char of the string
	char classify_ch = s[0];
	if (isalpha(classify_ch) || classify_ch == '_')
	{
		for (int i = 0; i < len; i++)
		{
			//if char is # or letter 
			if (s[i] == '_' || isalpha(s[i]));
			else
				return 6; //if not or else return 6 which is invalid input
		}
		return 3; 
	}
    
	else if (isdigit(classify_ch))
	{
		//if valid, accept only string has integer or dot .
		for (int i = 0; i < len; i++)
		{
			if (s[i] == '.' || isdigit(s[i]));
			else
				return 6;
		}

		//check if there is a dot
		for (int i = 0; i < len; i++)
		{
			if (s[i] == '.')
				return 4;
		}

		//assign to #5 for integer
		return 5;
	}
	else
		return 6; //invalid

	//random # unless error
	return 7;
}

//Function char_to_column
int Lex::char_to_col(const char input) const 
{
	//Convert char to column using Finite State Machine
	if (isdigit(input))
		return 1; //#1 if a digit
	else if (isalpha(input))
		return 2; //#2 if a letter
	else if (input == '_')
		return 3; //#3 a _
	else if (input == '.')
		return 4; //#4 a .
	else
		return 5; //none of them
}

//Finite State Machine for integer
int Lex::int_DFSM(const string str)
{
	int state = 1;

	int a[3][2] = { 0, 'd', 1, 2, 2, 2 };

	//accepting state
	int f[1] = { 2 };

	//update to new transition
	int size = str.size();
	for (int i = 0; i < size; i++)
	{
		//convert char to column number
		int col = char_to_col(str[i]);

		//update current state
		state = a[state][col];
	}
    
    //If not equal to accepting state, return 1 else return 0
	if (state == f[0])
		return 1;
	else
		return 0;
}

//Finite State Machine for Real
int Lex::real_DFSM(string str)
{
	//starting state
	int state = 1;

	int a[5][3] = { 0, 'd', '.', 1, 2, 0, 2, 2, 3, 3, 4, 0, 4, 4, 0 };

	int f[1] = { 4 };

	//convert char to column number
	int size = str.size();
	for (int i = 0; i < size; i++)
	{
		int col = char_to_col(str[i]);
		state = a[state][col];

		//if state = 0 then reject
		if (state == 0) 
			return 0;
	}
    
    
  //If state equal to accepting state return 1 if not return 0
	if (state == f[0])
		return 1;
	else
		return 0;
}

//Finite State Machine for Identifier
int Lex::identifier_DFSM(string str)
{
	//starting state
	int state = 1;
	
	int a[6][4] = { 0, 'd','l', '_', 1, 0, 2, 0, 2, 3, 4, 5, 3, 3, 4, 5,
		4, 3, 4, 5, 5, 3, 4, 5};

	int f[4] = { 2, 3, 4, 5 };

	//convert char to column number
	int size = str.size();
	for (int i = 0; i < size; i++)
	{
		int col = char_to_col(str[i]);
        
        //update the current state
		state = a[state][col];
		if (state == 0)
			return 0;
	}

    //Check state if is equal to accepting states then return 1 else return 0
	for (int i = 0; i < 4; i++)
	{
		if (state == f[i])
			return 1;
	}
	return 0;
}

void Lex::lexer(ifstream& file)
{
	string str; 
	int state_status = 0;
	bool found = false;
	char ch = 'c';

	//loop and get char's until see a space, separator, or operator
	while (!found)
	{
		ch = file.get();

		//if comments skip until end of comment chars
        if (ch == '[' && file.peek() == '*') {
            ch = file.get();
            ch = file.get();
            while (ch != '*' && file.peek() != ']') {
                ch = file.get();
            }
            ch = file.get();
            ch = file.get();
        }
		//checking if current character is a separator, operator, whitespace if yes exit

		if (this->isSeparator(ch) || this->isOperator(ch) || isspace(ch) || ch == -1)
		{
			found = true;
		}

		//if string not empty, current character is either operator or separator decrement current by one char
		if (!str.empty() && (this->isOperator(ch) || this->isSeparator(ch)))
			file.unget();
		else if (!isspace(ch) && !(ch == -1))
			str += ch;

	
		if (str.empty() && !(ch == -1))
			found = false;
	}

	//text file with extra whitespaces
	if (str.empty() && ch == -1)
	{
		this->setLexeme("EOF");
		this->setToken("EOF");
		return;
	}

	int classify = Classify(str);

	//check token for identifier
	if (classify == 3) {

		//check if token is accepted or not
		state_status = identifier_DFSM(str);

		this->setLexeme(str);


        if (state_status == 1)
		{
			if (checkKeyword(str))
				this->setToken("keyword");
			else
				this->setToken("identifier");
		}
		else
		{
			this->setToken("invalid identifier");
		}
	}
	//for operator
	else if (classify == 1)
	{
		str = ch;

		//return the next char
		ch = file.peek();

		//check for valid operators: ==, !=, <=, >=
		if ((str[0] == '=' && ch == '=') || (str[0] == '!' && ch == '=')
			|| (str[0] == '=' && ch == '>') || (str[0] == '<' && ch == '='))
		{
			str += ch;
			file.get();
		}

		//reject invalid operators
		if (isOperator(str[0]) || str == "==" || str == "!=" || str == "<=" || str == ">=")
		{
			this->setToken("operator");
			this->setLexeme(str);
		}
		else
		{
			this->setToken("invalid operator");
			this->setLexeme(str);
		}
	}
	//check separator
	else if (classify == 2) {
		str = ch;
		ch = file.peek();
		if (str[0] == '#' )
		{
			str += ch;
			file.get();
		}

		if (!(str[0] == '#'))
		{
			this->setLexeme(str);
			this->setToken("separator");
		}
		else
		{
			this->setLexeme(str);
			this->setToken("invalid separator");
		}
	}

    
    else if (classify == 4) {
		state_status = real_DFSM(str);
		this->setLexeme(str);
		if (state_status == 1)
		{
			this->setToken("real");
		}
		else {
			this->setToken("invalid real");
		}
	}
    
    else if (classify == 5)
	{
		state_status = int_DFSM(str);
		this->setLexeme(str);

		if (state_status == 1)
		{
			this->setToken("integer");
		}
		else
			this->setToken("invalid integer");
	}
	else
	{
		this->setLexeme(str);
		this->setToken("invalid input");
	}
}


void Lex::print() const
{
	cout << left << setw(20) << this->token << setw(20) << this->lexeme << endl;
}

// Set Token
void Lex::setToken(const string newToken)
{
	token = newToken;
}
 //Set Lexeme
void Lex::setLexeme(const string newLexeme)
{
	lexeme = newLexeme;
}

 //Return a token
string Lex::getToken() const
{
	return token;
}

 //Return a lexeme
string Lex::getLexeme() const
{
	return lexeme;
}

//destructor
Lex::~Lex() {}
