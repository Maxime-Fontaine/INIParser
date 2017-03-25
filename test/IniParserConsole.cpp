//---------------------------------------------------------------------------
#include <iostream>

// GNU/Linux system
#if __linux__
#include <unistd.h>
// DEBUG definition of Makefile file
#ifndef DEBUG
#define DEBUG false
#endif
// DEBUG_FILE definition of Makefile file
#ifndef DEBUG_FILE
#define DEBUG_FILE "../ini_files/test_old.ini"
#endif
// Windows system (Borland C++)
#elif __WIN32__
#include <conio.h>
#define DEBUG false
#define DEBUG_FILE "..\\ini_files\\test_old.ini"
#endif

#pragma hdrstop

#include "INIParser.h"

using namespace std;

//---------------------------------------------------------------------------

char DisplayMenu()
{
	cout << "\nMenu of Ini Parser Console" << endl;
	cout <<   "--------------------------" << endl;
	cout << "  o. Open and read a file" << endl;
	cout << "  k. Display keys" << endl;
	cout << "  s. Display section" << endl;
	cout << "  d. Display dictionnary" << endl;
	cout << "  a. Add/Modify a parameter" << endl;
	if(DEBUG)
	{
		cout << "  v. Display some values" << endl;
		cout << "  w. Write in the INI file" << endl;
		cout << "  m. Modify some values" << endl;
		cout << "  t. Display file text" << endl;
	}

	cout << "  q. Quit" << endl;

	cout << "\nYour choice : ";
	char Choix;
	cin >> Choix;
	cout << endl;

	return Choix;
}

//---------------------------------------------------------------------------
bool OpenIniFile(INIParser *Parser)
{
	if(DEBUG)
		return Parser->Open(DEBUG_FILE);
	else
	{
		cout << "Enter the file name : ";
		string FileName;
		cin >> FileName;
		return Parser->Open(FileName.c_str());
	}
}

bool AddParameter(INIParser *Parser)
{
	bool success;
	string s, p;
	char type;
	cout << "Enter a section name : ";
	cin >> s;
	cout << "Enter a parameter name : ";
	cin >> p;
	cout << "Enter a type for this data" << endl;
	cout << "  1. Integer" << endl;
	cout << "  2. Double" << endl;
	cout << "  3. Boolean" << endl;
	cout << "  4. String" << endl;
	cout << "\nYour choice : ";
	cin >> type;
	cout << "\nEnter the value for this parameter : ";
	switch(type)
	{
		case '1':
		{
			int value;
			cin >> value;
			success = Parser->SetValue(s.c_str(), p.c_str(), value);
			break;
		}

		case '2':
		{
			double value;
			cin >> value;
			success = Parser->SetValue(s.c_str(), p.c_str(), value);
			break;
		}

		case '3':
		{
			bool value;
			cin >> value;
			success = Parser->SetValue(s.c_str(), p.c_str(), value);
			break;
		}

		case '4':
		{
			string value;
			cin >> value;
			success = Parser->SetValue(s.c_str(), p.c_str(), value);
			break;
		}

		default:
		{
			cout << "Wrong choice !" << endl;
			break;
		}
	}
	cout << endl;
}

//---------------------------------------------------------------------------
void DisplaySection(section s)
{
	if(s.key != "")
	{
		cout << "Section \"" << s.key << "\" :" << endl;
		for(unsigned int i = 0; i < s.parameters.size(); i++)
			cout << "-> " << s.parameters[i].name << ":" << s.parameters[i].value \
				 << " # " << s.parameters[i].comment << endl;
	}
	else
	{
		cout << "Error : This section doesn't exist" << endl;
	}
}

//---------------------------------------------------------------------------
void DisplayDictionnary(INIParser *Parser)
{
	dictionnary d = Parser->GetDictionnary();
	for(unsigned int i = 0; i < d.sections.size(); i++)
		DisplaySection(d.sections[i]);
}

//---------------------------------------------------------------------------
#pragma argsused
int main(int argc, char* argv[])
{
	if(DEBUG)
	{
		cout << "\t**** Running Debug Mode ****" << endl;
		cout << "\t----------------------------" << endl;
		cout << "Debug File = " << DEBUG_FILE << endl;
		/*char Buffer[200];
		string cwd = string(getcwd(&Buffer[0], sizeof(Buffer)));
		string s = argv[0];
		int fpos = s.find('/');
		int lpos = s.rfind('/');
		s = s.substr(0, fpos + 1);
		string fpath = s.substr(0,fpos);
		if(fpath == "." || fpath == "..")
		{
			if(fpath == "..")
			{
				int ppos = cwd.rfind('/');
				cwd = cwd.substr(0, ppos);
			}
			cwd += s.substr(fpos);
		}
		
		cout << "Path: " << cwd << endl;
		*/
	}
	INIParser *Parser;
	Parser = new INIParser();

	char choix;
	while(choix != 'q')
	{
		choix = DisplayMenu();
		switch(choix)
		{
			case 'o':
			{
				bool success = OpenIniFile(Parser);
				if(success)
				{
					cout << "File opened successfully" << endl;
				}
				else
					cout << "Can't open this file" << endl;
				break;
			}

			case 't':
			{
				cout << Parser->TextFile << endl;
				break;
			}

			case 'k':
			{
				vector<string> Names = Parser->GetSectionsName();
				cout << "Sections in this file :" << endl;
				for(unsigned int i = 0; i < Names.size(); i++)
					cout << "-> " << Names[i] << endl;
				break;
			}

			case 's':
			{
				cout << "Enter a section name : ";
				string c;
				cin >> c;
				cout << endl;

				section s = Parser->GetSection(c.c_str());
				DisplaySection(s);
				break;
			}

			case 'a':
			{
				bool success = AddParameter(Parser);
				if(success)
					cout << "Parameter added / modified successfully" << endl;
				else
					cout << "Error : Parameter not added / modified" << endl;
				break;
			}

			case 'd':
			{
				dictionnary d = Parser->GetDictionnary();
				cout << "Dictionnary of this file :\n" << endl;
				for(unsigned int i = 0; i < d.sections.size(); i++)
					DisplaySection(d.sections[i]);
				break;
			}

			case 'v':
			{
				bool bv = Parser->GetBoolean("cle2", "param4");
				cout << "Boolean : " << bv << endl;
				string sv = Parser->GetString("cle2", "param6");
				cout << "String : " << sv << endl;
				int iv = Parser->GetInteger("cle2", "param5");
				cout << "Integer : " << iv << endl;
				double dv = Parser->GetDouble("cle2", "param7");
				cout << "Double : " << dv << endl;
				break;
			}

			case 'w':
			{
				dictionnary d = Parser->GetDictionnary();
				d.sections[1].parameters[6].value = "6.18";
				bool Success = Parser->WriteINI(&d);
				break;
			}

			case 'm':
			{
				bool Success = Parser->SetValue("cle2", "param4", false);
				Success = Parser->SetValue("cle2", "param5", 14);
				Success = Parser->SetValue("cle2", "param6", "coucou");
				Success = Parser->SetValue("cle2", "param7", 1258.512489736);
				break;
			}

			case 'q':
			{
				delete Parser;
				cout << "Quit program" << endl;
				break;
			}

			default:
				break;
		}
	}

	return 0;
}
//---------------------------------------------------------------------------
