// TP1CompilLexer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

bool readFile(string fileToRead, string* fileContent)
{
	ifstream f(fileToRead);
	string str;
	
	if (!f.good())//File doesn't exisit
		return false;
	
	while (getline(f, str))
	{
		*fileContent += str;
	}
	return true;
}

bool lexer(string* fileContent)
{
	int i = 0;
	bool error = false;

	while (i < fileContent->length() && !error)
	{
		if (true)
		{

		}
		i++;
	}
	return error;
}

int main()
{
	string fileContent = "";

	readFile("exemple.cs", &fileContent);

	lexer(&fileContent);

    cout << fileContent;
	int i;
	cin >> i;
}
