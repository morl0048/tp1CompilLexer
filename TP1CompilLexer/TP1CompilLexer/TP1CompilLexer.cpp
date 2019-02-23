// TP1CompilLexer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>

using namespace std;

vector<string> m_declarationType = { "int", "float", "bool", "char", "string" };
regex m_identificatorRegex("^[a-zA-Z][a-zA-Z0-9_]*[a-zA-Z]$|^[a-zA-Z]$"); //Commence par une lettre, nimportequoi dans le millieu et fini par une lettre OU est une lettre
vector<string> m_conditionType = { "if" };
vector<string> m_operateurType = { "==", "!=", "<", ">" };
vector<string> m_loopType = { "for" };

vector<string> *m_listTokens = new vector<string>();

vector<void> *m_listIdentificators = new vector<void>();

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

int lexer(string* fileContent)
{
	int i = 0;
	string lex = "";

	while (i < fileContent->length())
	{
		if (fileContent->at(i) == ' ' || fileContent->at(i) == ';' || fileContent->at(i) == '\n')//reset lexem
		{
			//check to see what lexem looks like
			if (find(m_declarationType.begin(), m_declarationType.end(), lex) != m_declarationType.end())//Lexem is a declaration
			{
				m_listTokens->push_back("DEC: " + lex);
				
				//dec id;
				//OR
				//dec id = ...;
				//OR
				//dec id0, id1 =..., id2;

				i++;//go to next char
				string id = "";
				while (fileContent->at(i) != ';' || i < fileContent->length())
				{
					if (fileContent->at(i) == '=')
					{

					}
					if (fileContent->at(i) == ',')//multiple declarations
					{

					}
					if (fileContent->at(i) != ' ' | fileContent->at(i) != '\n')//remove blank spaces
					{
						id += fileContent->at(i);
					}
					i++;
				}
			}
			else
			{
				if (find(m_conditionType.begin(), m_conditionType.end(), lex) != m_conditionType.end())//Lexem is a condition
				{
					m_listTokens->push_back("COND: " + lex);
					
					//if(...){...}
					//look if ids used in condition exist
				}
				else
				{
					if (find(m_operateurType.begin(), m_operateurType.end(), lex) != m_operateurType.end())//Lexem is an operator
					{
						m_listTokens->push_back("OP: " + lex);
					}
					else
					{
						if (find(m_loopType.begin(), m_loopType.end(), lex) != m_loopType.end())//Lexem is a loop
						{
							m_listTokens->push_back("LOOP: " + lex);
						}
						else
						{
							smatch match;
							if (regex_match(lex, match, m_identificatorRegex))//Lexem match identificator
							{
								m_listTokens->push_back("ID: " + lex);
							}
							else
							{
								m_listTokens->push_back("ERROR: " + lex);
							}
						}
					}
				}
			}

			//then reset lexem
			lex = "";
		}
		else
		{
			lex += fileContent->at(i);
		}
		i++;
	}
	return 0;
}

int main()
{
	string fileContent = "";

	readFile("example.cs", &fileContent);

	lexer(&fileContent);

	cout << fileContent << endl;

	for (int i = 0; i < m_listTokens->size(); i++)
	{
		cout << m_listTokens->at(i) << endl;
	}

	int i;
	cin >> i;
}
