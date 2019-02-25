// TP1CompilLexer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>

using namespace std;

//regex m_regexKeyword("^[\-\!\$\%\^\&\*\(\)\_\+\|\~\=\`\{\}\\\[\\\]\:\"\;\'\<\>\?\,\.\\\/]$"); //One of the folowing:
regex m_regexType("^(int|float|bool|char|string)$"); //One of the following: "int","float","bool","char","string"
regex m_regexInt("^[+-]?[1-9][0-9]*$|^0$"); //-(OPTIONNAL) 1-9 as first digit followed by any number of digit OR 0
regex m_regexFloat("^[+-]?[0-9]+(\.[0-9]+)?$"); //-(OPTIONNAL) 0-9 at least one time + (. 0-9 at least one time (OPTIONNAL))
regex m_regexBool("^(0|1|true|false)$"); //0 or 1 or true or flase
regex m_regexChar("^\'.\'$"); //One char between ''
regex m_regexString("^\".*\"$"); //Any number of char between ""

regex m_regexCondition("^if$"); //Starts with if
//regex m_regexLeftP("^\($"); //Left parenthesis (
//regex m_regexRightP("^\)$"); //Right parenthesis )
//regex m_regexLeftC("^\{$"); //Left curly brace {
//regex m_regexRightC("^\}$"); //Right curly brace }
//regex m_regexLeftB("^\[$"); //Left bracket [
//regex m_regexRightB("^\]$"); //Right bracket ]

regex m_regexOperator("^[==,!=,<,>,+,-]$"); //One of the following: "==","!=","<",">","+","-"

regex m_regexLoop("^for$"); //Starts with for

regex m_regexIdentificator("^[a-zA-Z][a-zA-Z0-9_$]*[a-zA-Z]$|^[a-zA-Z]$"); //Starts with a letter + any char in between + ends with a letter OR is a letter

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


int lexer(string* fileContent)
{
	int i = 0;
	int state = 0;
	string lex = "";

	while (i < fileContent->length())
	{
		if (fileContent->at(i) != ' ' && fileContent->at(i) != '\t' && fileContent->at(i) != '\n' && fileContent->at(i) != ';' && fileContent->at(i) != '(' && fileContent->at(i) != ')'
			&& fileContent->at(i) != '[' && fileContent->at(i) != ']' && fileContent->at(i) != '{' && fileContent->at(i) != '}')
		{
			lex += fileContent->at(i);
		}
		else//push token and reset lex
		{
			if (fileContent->at(i) == ';' || fileContent->at(i) == '(' || fileContent->at(i) == ')' || 
				fileContent->at(i) == '[' || fileContent->at(i) == ']' || fileContent->at(i) == '{' || fileContent->at(i) == '}')
			{
				switch (fileContent->at(i))
				{
				case ';':
					m_tokenTable->push_back("SEPARATOR: ;");
					break;
				case '(':
					m_tokenTable->push_back("LEFTP: (");
					break;
				case ')':
					m_tokenTable->push_back("RIGHTP: )");
					break;
				case '[':
					m_tokenTable->push_back("LEFTB: [");
					break;
				case ']':
					m_tokenTable->push_back("RIGHTB: ]");
					break;
				case '{':
					m_tokenTable->push_back("LEFTC: {");
					break;
				case '}':
					m_tokenTable->push_back("RIGHTC: }");
					break;
				}
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
											if(regex_match(lex, match, m_regexCondition))
											{
												m_tokenTable->push_back("COND: " + lex);
											}
											else
											{
												if (regex_match(lex, match, m_regexLoop))
												{
													m_tokenTable->push_back("LOOP: " + lex);
												}
												else
												{
													if (regex_match(lex, match, m_regexOperator))
													{
														m_tokenTable->push_back("OP: " + lex);
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
