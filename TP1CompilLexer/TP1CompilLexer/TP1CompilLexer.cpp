// TP1CompilLexer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>

using namespace std;

regex m_regexKeyword("^[\-\!\$\%\^\&\*\(\)\_\+\|\~\=\`\{\}\\\[\\\]\:\"\;\'\<\>\?\,\.\\\/]$"); //One of the folowing:
regex m_regexType("^(int|float|bool|char|string)$"); //One of the following: "int","float","bool","char","string"
regex m_regexInt("^[+-]?[1-9][0-9]*$|^0$"); //-(OPTIONNAL) 1-9 as first digit followed by any number of digit OR 0
regex m_regexFloat("^[+-]?[0-9]+(\.[0-9]+)?$"); //-(OPTIONNAL) 0-9 at least one time + (. 0-9 at least one time (OPTIONNAL))
regex m_regexBool("^(0|1|true|false)$"); //0 or 1 or true or flase
regex m_regexChar("^\'.\'$"); //One char between ''
regex m_regexString("^\".*\"$"); //Any number of char between ""

regex m_regexCondition("^if$"); //Starts with if( + contains anything + ends with ) + may be followed by { + contains anything + } //\(.*\)(\{.*\})?
regex m_regexLeftP("^\($"); //Left parenthesis (
regex m_regexRightP("^\)$"); //Right parenthesis )
regex m_regexLeftB("^\{$"); //Left bracket (
regex m_regexRightB("^\}$"); //Right braket )


regex m_regexIdentificator("^[a-zA-Z][a-zA-Z0-9_$]*[a-zA-Z]$|^[a-zA-Z]$"); //Starts with a letter + any char in between + ends with a letter OR is a letter


//regex m_regexOperator("^[==,!=,<,>]$"); //One of the following: "==","!=","<",">"

//regex m_regexLoop("^for\(_*;_*;_*\)?(\{_*\})$"); //Starts with for( + contains anything + ; + contains anything + ; + contains anything + ends with ) + may be followed by { + contains anything + }


//regex m_declarationType("int ([a-zA-Z][a-zA-Z0-9_]*[a-zA-Z]|[a-zA-Z]);");
//regex m_identificatorRegex("[a-zA-Z][a-zA-Z0-9_]*[a-zA-Z]|[a-zA-Z]"); //Commence par une lettre, nimportequoi dans le millieu et fini par une lettre OU est une lettre
//regex m_conditionRegex("");
//regex m_operatorType("[==,!=,<,>]");
//regex m_loopRegex("for(;;)");
//
//vector<string> m_declarationType = { "int", "float", "bool", "char", "string" };
//
//vector<string> m_conditionType = { "if" };
//vector<string> m_operateurType = { "==", "!=", "<", ">" };
//vector<string> m_loopType = { "for" };
//
vector<string> *m_tokenTable = new vector<string>();

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

//int lexer(string* fileContent)
//{
//	int i = 0;
//	string lex = "";
//
//	while (i < fileContent->length())
//	{
//		if (fileContent->at(i) == ' ' || fileContent->at(i) == ';' || fileContent->at(i) == '\n')//reset lexem
//		{
//			//check to see what lexem looks like
//			if (find(m_declarationType.begin(), m_declarationType.end(), lex) != m_declarationType.end())//Lexem is a declaration
//			{
//				string decType = lex;
//				lex = "";
//
//				//dec id;
//				//OR
//				//dec id = ...;
//				//OR
//				//dec id0, id1 =..., id2;
//				while (fileContent->at(i) != ';')
//				{
//					if (fileContent->at(i) == ',')//keep declaration and reset a new one
//					{
//						m_lexTable->push_back("DEC: " + decType + " " + lex);
//						lex = "";
//					}
//					else
//					{
//						if (fileContent->at(i) != ' ')
//						{
//							lex += fileContent->at(i);
//						}
//					}
//
//					i++;
//				}
//				m_lexTable->push_back("DEC: " + decType + " " + lex);
//			}
//			else
//			{
//				if (find(m_conditionType.begin(), m_conditionType.end(), lex) != m_conditionType.end())//Lexem is a condition
//				{
//					string condType = lex;
//					lex = "";
//					i++;//next char == (
//					if (fileContent->at(i) != '(')//read all inside if()
//					{
//						while (fileContent->at(i) != ')' && i < fileContent->length())
//						{
//							lex += fileContent->at(i);
//							i++;
//						}
//						if (i == fileContent->length())
//						{
//							m_lexTable->push_back("ERROR: " + lex);
//						}
//						else
//						{
//						}
//						
//					}
//					else
//					{
//
//					}
//
//					
//					//if (find(m_operateurType.begin(), m_operateurType.end(), lex) != m_operateurType.end())//Lexem is an operator
//					//{
//					//	m_lexTable->push_back("OP: " + lex);
//					//}
//				}
//				else
//				{
//					if (find(m_loopType.begin(), m_loopType.end(), lex) != m_loopType.end())//Lexem is a loop
//					{
//						m_lexTable->push_back("LOOP: " + lex);
//					}
//					else
//					{
//						smatch match;
//						if (regex_match(lex, match, m_identificatorRegex))//Lexem match identificator
//						{
//							m_lexTable->push_back("ID: " + lex);
//						}
//						else
//						{
//							m_lexTable->push_back("ERROR: " + lex);
//						}
//					}
//				}
//			}
//
//			//then reset lexem
//			lex = "";
//		}
//		else
//		{
//			lex += fileContent->at(i);
//		}
//		i++;
//	}
//	return 0;
//}


int lexer(string* fileContent)
{
	int i = 0;
	int state = 0;
	string lex = "";

	while (i < fileContent->length())
	{
		if (fileContent->at(i) != ' ' && fileContent->at(i) != '\n' && fileContent->at(i) != ';')//remove spaces and line return from content
		{
			lex += fileContent->at(i);
		}
		else//push token and reset lex
		{
			if (fileContent->at(i) == ';')//and that is not in for loop declaration
			{
				m_tokenTable->push_back("SEPARATOR: ;");
			}
			else
			{
				if (lex != "")
				{
					smatch match;
					if (regex_match(lex, match, m_regexType))
					{
						m_tokenTable->push_back("TYPE: " + lex);
					}
					else 
					{
						if (regex_match(lex, match, m_regexInt))
						{
							m_tokenTable->push_back("INT: " + lex);
						}
						else
						{
							if (regex_match(lex, match, m_regexFloat))
							{
								m_tokenTable->push_back("FLOAT: " + lex);
							}
							else
							{
								if (regex_match(lex, match, m_regexBool))
								{
									m_tokenTable->push_back("BOOL: " + lex);
								}
								else
								{
									if (regex_match(lex, match, m_regexChar))
									{
										m_tokenTable->push_back("CHAR: " + lex);
									}
									else
									{
										if (regex_match(lex, match, m_regexString))
										{
											m_tokenTable->push_back("STRING: " + lex);
										}
										else
										{
											if (regex_match(lex, match, m_regexIdentificator))
											{
												m_tokenTable->push_back("ID: " + lex);
											}
											else
											{
												m_tokenTable->push_back("ERROR: " + lex);
											}
										}
									}
								}
							}
						}
					}
					
					lex = "";
				}
			}
		}
		i++;
	}
	return 0;
}

/*
if (fileContent->at(i) != ';' && fileContent->at(i) != ' ')//keep everything in memory until you hit a semicolon ';' or a space ' '
		{
			//|| fileContent->at(i) != '\n' || fileContent->at(i) != ' '
			lex += fileContent->at(i);
		}
		else//analyse what we have in memory
		{
			smatch matchType;
			if (regex_match(lex, matchType, m_regexType))
			{
				string suite = "";
				while (fileContent->at(i) != ';')
				{
					i++;
					if (fileContent->at(i) == ' ')
					{
						smatch matchIdentificator;
						if()
					}
					else
					{
						if (fileContent->at(i) == ',')
						{
							//push + reset suite
							suite = "";
						}
						else
						{
							suite += fileContent->at(i);
						}
					}

				}
			}
			lex = "";
		}
*/

int main()
{
	string fileContent = "";

	readFile("example.cs", &fileContent);

	lexer(&fileContent);

	cout << fileContent << endl;

	for (int i = 0; i < m_tokenTable->size(); i++)
	{
		cout << m_tokenTable->at(i) << endl;
	}

	int i;
	cin >> i;
}
