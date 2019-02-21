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

int main()
{
	string fileContent = "";
	string h;
	ifstream f("exemple.cs");

	while (getline(f, h))
	{
		fileContent += h;
	}

	readFile("exemple.cs", &fileContent);
    cout << fileContent;
}
