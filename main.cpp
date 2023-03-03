#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <ctype.h>
#include "Lex.h"

using namespace std;

int main() {
	ifstream file;
	ofstream outFile("output.txt");
    string filename;
	string line;

	do {
		cout << "Enter a file name .txt (or exit to quit): ";
		cin >> filename;

        if (!(filename == "exit"))
        {
            file.open(filename.c_str());
            if (!file.is_open())
            {
                cerr << "Failed to open the file\n";
                exit(1);
            }
		
            Lex check;

			file.clear();
			file.seekg(0);
			//lexical analysis
            while (!file.eof())
            {
                check.lexer(file);

				//whitespace at eof, skip printing
				if (!(check.getLexeme() == "EOF"))
				{
					check.print();
					outFile << left << setw(20) << check.getToken() << setw(20) << check.getLexeme() << endl;
				}
            }

            file.close();
			cout << endl;
			outFile << endl;
        }
        
    } while (!(filename == "exit"));

	outFile.close();
    cout << endl;
    return 0;
}
