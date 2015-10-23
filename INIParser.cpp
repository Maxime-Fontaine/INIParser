//--------------------------------------------------------------------------
//                                INIPARSER.CPP
//--------------------------------------------------------------------------
//!
//! \file INIParser.cpp
//! \brief Class for parsing INI file
//! \author Maxime Fontaine
//! \version 0.1
//! \date 2015-02-08
//!
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
//                            PRE-COMPILER DECLARATIONS
//--------------------------------------------------------------------------
//#include <stdio>
#include <iostream>
#include <fstream>

#pragma hdrstop

#include "INIParser.h"

#pragma package(smart_init)
//--------------------------------------------------------------------------

using namespace std;

//--------------------------------------------------------------------------
//                              INIPARSER METHODS
//--------------------------------------------------------------------------

//-------------------------------------------------------------------------
//!
//! \brief    Constructor of the INIParser class
//! \param    File   INI File to open (optionnal)
//! \return   an instance of the class
//! 
//! This function creates an instance of the INIParser class
//! If a file name is given in parameter, this method open the file
//! 
//!
//--------------------------------------------------------------------------
__CALL INIParser::INIParser(const char* File)
{
	this->dico = new dictionnary;
	this->Keys = new vector<string>;
	this->KeysLines = new vector<unsigned int>;
	this->Comments = new vector<comment>;
	this->LinesType = new std::vector<int>;

	this->FileEndLine = 0;
	this->EndLine = 0;
	
	if(File != NULL)
	{
		this->Open(File);
	}
}

//-------------------------------------------------------------------------
//!
//! \brief    Destructor of the INIParser class
//! 
//! This function destroys an instance of the INIParser class
//! 
//!
//--------------------------------------------------------------------------
__CALL INIParser::~INIParser()
{
	delete this->dico;
	delete this->Keys;
	delete this->KeysLines;
	delete this->Comments;
	delete this->LinesType;
}


//-------------------------------------------------------------------------
//!
//! \brief    Open an INI File
//! \param    File   INI File to open
//! \return   a boolean. true if file is opened, false otherwise
//! 
//! This function opens an ini file and reads it
//! 
//! This function returns true in case of success.
//!
//--------------------------------------------------------------------------
bool __CALL INIParser::Open(const char* File)
{
	this->KeysLines->clear();
	this->dico->sections.clear();
	this->dico->Keys.clear();
	this->Keys->clear();

	this->FileName = File;
	ifstream f;
	f.open(this->FileName, ios::in);

	if(!f.is_open())
		return false;

	string line = "";
	while (!f.eof())
	{
		char c = f.get();
		if((int)(c) == -1)
			c = '\0';
		line += (char)c;
		if(c == '\n')
		{
			this->TextFile += line;
			line = "";
		}
	}
	f.close();

	return true;
}


//-------------------------------------------------------------------------
//!
//! \brief    Trim spaces in a string at start, end or both
//! \param    strinit string to trim
//! \param    mode    integer describing what part to trim (optionnal)
//! \return   a string representing the trimmed string
//! 
//! This function trims spaces and tabulations which are at start and at end of a string
//! mode is an integer which could be :
//!    0 : trims the start of the string
//!    1 : trims the end of the string
//!   -1 : trims the start and the end of the string (default)
//! 
//! This function returns the trimmed string.
//!
//--------------------------------------------------------------------------
string __CALL INIParser::StrTrim(string strinit, int mode)
{
	if(mode == 0)
	{
		while(strinit.substr(0, 1) == " " || strinit.substr(0, 1) == "\t")
			strinit.erase(0, 1);
	}
	else if(mode == 1)
	{
		while(strinit.substr(strinit.size() - 1, 1) == " " || strinit.substr(strinit.size() - 1, 1) == "\t")
			strinit.erase(strinit.size() - 1, 1);
	}
	else
	{
		while(strinit.substr(0, 1) == " " || strinit.substr(0, 1) == "\t")
			strinit.erase(0, 1);
		while(strinit.substr(strinit.size() - 1, 1) == " " || strinit.substr(strinit.size() - 1, 1) == "\t")
			strinit.erase(strinit.size() - 1, 1);
	}
	return strinit;
}

//-------------------------------------------------------------------------
//!
//! \brief    Convert a string in lowercase
//! \param    strinit string to convert
//! \return   a string representing the converted string
//! 
//! This function converts all characters of a string into their lowercase.
//! 
//! This function returns the lowercase converted string.
//!
//--------------------------------------------------------------------------
string __CALL INIParser::StrLower(string strinit)
{
	// transforme une std::string en char*
	char *str = new char[strinit.length() + 1];
	copy(strinit.begin(), strinit.end(), str);
	str[strinit.length()] = '\0';

	for(unsigned int i = 0; i < strinit.length(); i++)
	{
		if(str[i] <= 'Z' && str[i] >= 'A')
			str[i] -= ('Z' - 'z');
	}

	return string(str);
}

//--------------------------------------------------------------------------
//!
//! \brief   Convert an integer in a string
//! \param   value integer to convert
//! \return  a string representing the converted value
//! 
//! this function converts an integer to a string.
//! 
//! this function returns the converted string.
//!
//--------------------------------------------------------------------------
string __CALL INIParser::NumToStr(int value)
{
	if (value == 0)
		return "0";

	string temp="";
	string returnvalue="";
	while (value > 0)
	{
		temp += value % 10 + 48;
		value /= 10;
	}
	for(int i = 0; i < temp.length(); i++)
		returnvalue += temp[temp.length() - i - 1];

	return returnvalue;
}

//--------------------------------------------------------------------------
//!
//! \brief   Convert a double in a string
//! \param   value double to convert
//! \param   precision integer representing the decimal number after point (default = 6)
//! \return  a string representing the converted value
//! 
//! this function converts a double to a string with a specified decimal number.
//! 
//! this function returns the converted string.
//!
//--------------------------------------------------------------------------
string __CALL INIParser::NumToStr(double value, int precision)
{
	if (value == 0.0)
		return "0.0";

	string temp="";
	string returnvalue="";

	int IntPart = int(value);
	while (IntPart > 0)
	{
		temp += IntPart % 10 + 48;
		IntPart /= 10;
	}
	for(int i = 0; i < temp.length(); i++)
		returnvalue += temp[temp.length() - i - 1];

	temp = "";
	value = double(value) - int(value);
	value *= pow(10.0, precision);
	int DecPart = int(value);
	for(int i = 0; i < precision; i++)
	{
		temp += DecPart % 10 + 48;
		DecPart /= 10;
	}
	returnvalue += ".";
	for(int i = 0; i < temp.length(); i++)
		returnvalue += temp[temp.length() - i - 1];

	return returnvalue;
}

//-------------------------------------------------------------------------
//!
//! \brief    Returns the lines in the INI File
//! \return   a vector of strings representing lines
//! 
//! This function returns a vector of strings for lines in the INI file.
//!
//--------------------------------------------------------------------------
vector<string> __CALL INIParser::GetLines()
{
	vector<string> v;
	v.clear();

	// transforme une std::string en char*
	char *str = new char[this->TextFile.length() + 1];
	copy(this->TextFile.begin(), this->TextFile.end(), str);
	str[this->TextFile.length()] = '\0';

	char *pch;
	pch = strtok (str,"\n\r");
	while (pch != NULL)
	{
		v.push_back(string(pch));
		pch = strtok (NULL, "\n\r");
	}

	this->FileEndLine = v.size();
	if(this->EndLine == 0)
		this->EndLine = this->FileEndLine;
	
	return v;
}

//--------------------------------------------------------------------------
//!
//! \brief Get the type of lines in the INI file
//! 
//! This function is for local using only. Types are follow :
//!    0 : comment line
//!    1 : key or parameter
//!    2 : key or parameter with comment
//!
//--------------------------------------------------------------------------
void __CALL INIParser::GetLinesType()
{
	vector<string> L = this->GetLines();
	this->LinesType->clear();
	this->Comments->clear();

	for(unsigned int i = 0; i < L.size(); i++)
	{

		L[i] = this->StrTrim(L[i], -1);

		int p = -1;
		int psharp = L[i].find_first_of("#", 0);
		unsigned int s = L[i].size();
		if(psharp >= 0 && psharp < L[i].size())
			p = psharp;
		int pcomma = L[i].find_first_of(";", 0);
		if(pcomma >= 0 && pcomma < L[i].size())
			p = pcomma;

		if(p == 0)
			this->LinesType->push_back(0);
		else if(p == -1)
			this->LinesType->push_back(1);
		else if(p > 0)
			this->LinesType->push_back(2);

		comment c;
		if(p >= 0)
			c.text = L[i].substr(p, L[i].size() - p);
		else
			c.text = "";
		c.line = i;
		this->Comments->push_back(c);
	}
	
}

//--------------------------------------------------------------------------
//!
//! \brief Get the sections name of the INI File
//! \return   a vector of strings with the sections name
//! 
//! This function read the lines of the INI file and peacks up sections.
//! 
//! This function returns a vector of strings with the sections names.
//!
//--------------------------------------------------------------------------
vector<string> __CALL INIParser::GetSectionsName()
{
	if(this->Keys->size() == 0)
	{
		this->KeysLines->clear();

		vector<string> p = this->GetLines();

		for(unsigned int i = 0; i < p.size(); i++)
		{
			p[i] = this->StrTrim(p[i], -1);

			// transforme une std::string en char*
			char *str = new char[p[i].length() + 1];
			copy(p[i].begin(), p[i].end(), str);
			str[p[i].length()] = '\0';

			char *pch;
			if(str[0] == '[' && str[p[i].length() - 1] == ']')
			{
				pch = strtok (str,"[]");
				while (pch != NULL)
				{
					this->Keys->push_back(string(pch));
					pch = strtok (NULL, "[]");
					this->KeysLines->push_back(i);
				}
			}
		}
	}

	return *(this->Keys);
}

//--------------------------------------------------------------------------
//!
//! \brief Get the sections number of the INI File
//! \return   an integer representing the sections number
//! 
//! This function returns an integer with the sections number.
//!
//--------------------------------------------------------------------------
int __CALL INIParser::GetSectionNumber()
{
	if(this->Keys->size() == 0)
		this->GetSectionsName();

	return this->Keys->size();
}

//--------------------------------------------------------------------------
//!
//! \brief Get a specified section of the INI File
//! \param    SectionName  a char pointer for the section name of the INI file to get
//! \return   a section structure
//! 
//! This function read the lines of the INI file and peacks up the specified section.
//! 
//! This function returns a section structure with the specified section.
//! If the specified section cannot be foud, this function returns an empty section.
//!
//--------------------------------------------------------------------------
section __CALL INIParser::GetSection(const char *SectionName)
{
	vector<string> p = this->GetLines();
	int KNumber = -1;

	section s;
	s.key = "";
	s.parameters.clear();

	for(int i = 0; i < this->GetSectionNumber(); i++)
	{
		if(string(SectionName) == this->Keys->at(i))
			KNumber = i;
	}

	if(KNumber < 0)
		return s;

	s.key = string(SectionName);
	s.line = this->KeysLines->at(KNumber);

	unsigned int EndLine;
	if(KNumber >= this->KeysLines->size() - 1)
		EndLine = p.size();
	else
		EndLine = this->KeysLines->at(KNumber + 1);

	for(unsigned int i = 0; i < p.size(); i++)
	{
		if(i >= EndLine)
			break;

		if(i >= this->KeysLines->at(KNumber) + 1)
		{
			parameter sp;

			p[i] = this->StrTrim(p[i], -1);

			// transforme une std::string en char*
			char *str = new char[p[i].length() + 1];
			copy(p[i].begin(), p[i].end(), str);
			str[p[i].length()] = '\0';

			char *pch;
			if(str[0] != '#' && str[0] != '\n' && str[0] != ';')
			{
				pch = strtok (str,"=");
				if(pch != NULL)
				{
					sp.name = this->StrTrim(string(pch), -1);
					pch = strtok (NULL, "=#;");
					sp.value = this->StrTrim(string(pch), -1);
					pch = strtok (NULL, "#;");
					if(pch != NULL)
						sp.comment = this->StrTrim(string(pch), -1);
					sp.line = i;
					s.parameters.push_back(sp);
				}
			}
		}
	}

	return s;
}

//--------------------------------------------------------------------------
//!
//! \brief Get the dictionnary of the INI File
//! \return   a dictionnary structure
//! 
//! This function returns a dictionnary structure for the entire INI file
//!
//--------------------------------------------------------------------------
dictionnary __CALL INIParser::GetDictionnary()
{
	if(this->dico->sections.size() == 0)
	{
		if(this->Keys->size() == 0)
			this->GetSectionsName();

		for(unsigned int i = 0; i < this->Keys->size(); i++)
		{
			this->dico->sections.push_back(this->GetSection(this->Keys->at(i).c_str()));
		}
	}

	return *(this->dico);
}

//--------------------------------------------------------------------------
//!
//! \brief Get a boolean value
//! \param    section    a char pointer to the section name
//! \param    key        a char pointer to the parameter name
//! \return   a boolean representing the value of the specified key
//! 
//! This function returns a boolean value for the specified key.
//! If the value is "true" or 1, the returned value is true, false otherwise
//!
//--------------------------------------------------------------------------
bool __CALL INIParser::GetBoolean(const char *section, const char *key)
{
	bool v = false;

	struct section s;
	s.key = "";
	s.parameters.clear();
	s = this->GetSection(section);

	for(unsigned int i = 0; i < s.parameters.size(); i++)
	{
		if(s.parameters[i].name == string(key))
		{
			if(this->StrLower(s.parameters[i].value) == "true" || s.parameters[i].value == "1")
				v = true;
			else
				v = false;
		}
	}

	return v;
}

//--------------------------------------------------------------------------
//!
//! \brief Get a string value
//! \param    section    a char pointer to the section name
//! \param    key        a char pointer to the parameter name
//! \return   a string representing the value of the specified key
//! 
//! This function returns a string value for the specified key.
//!
//--------------------------------------------------------------------------
string __CALL INIParser::GetString(const char *section, const char *key)
{
	struct section s;
	s.key = "";
	s.parameters.clear();
	s = this->GetSection(section);

	for(unsigned int i = 0; i < s.parameters.size(); i++)
	{
		if(s.parameters[i].name == string(key))
		{
			return s.parameters[i].value.c_str();
		}
	}

	return "";
}

//--------------------------------------------------------------------------
//!
//! \brief Get an integer value
//! \param    section    a char pointer to the section name
//! \param    key        a char pointer to the parameter name
//! \return   an integer representing the value of the specified key
//! 
//! This function returns an integer value for the specified key.
//! If the value cannot be converted into integer, 0 is returned
//!
//--------------------------------------------------------------------------
int __CALL INIParser::GetInteger(const char *section, const char *key)
{
	int v = 0;

	struct section s;
	s.key = "";
	s.parameters.clear();
	s = this->GetSection(section);

	for(unsigned int i = 0; i < s.parameters.size(); i++)
	{
		if(s.parameters[i].name == string(key))
		{
			try
			{
				v = int(strtod(s.parameters[i].value.c_str(), NULL));
			}
			catch(...)
			{}
		}
	}

	return v;
}

//--------------------------------------------------------------------------
//!
//! \brief Get a double value
//! \param    section    a char pointer to the section name
//! \param    key        a char pointer to the parameter name
//! \return   a double representing the value of the specified key
//! 
//! This function returns a double value for the specified key.
//! If the value cannot be converted into double, 0.0 is returned
//!
//--------------------------------------------------------------------------
double __CALL INIParser::GetDouble(const char *section, const char *key)
{
	double v = 0.0;

	struct section s;
	s.key = "";
	s.parameters.clear();
	s = this->GetSection(section);

	for(unsigned int i = 0; i < s.parameters.size(); i++)
	{
		if(s.parameters[i].name == string(key))
		{
			try
			{
				v = strtod(s.parameters[i].value.c_str(), NULL);
			}
			catch(...)
			{}
		}
	}

	return v;
}

//--------------------------------------------------------------------------
//!
//! \brief Converts a dictionnary into a string
//! \param    d    the dictionnary pointer to convert
//! \return   a string representing the converted dictionnary
//! 
//! This function returns a string value for the specified dictionnary.
//! Today, the dictionnary is linked to the INI file (this will change)
//!
//--------------------------------------------------------------------------
string __CALL INIParser::DicoToString(const dictionnary *d)
{
	string s = "";
	vector<string> p = this->GetLines();
	this->GetLinesType();

	if(d == NULL)
		return s;

	for(unsigned int l = 0; l < this->LinesType->size(); l++)
	{
		if(this->LinesType->at(l) == 0)
			s += this->Comments->at(l).text + "\n";
		else if(this->LinesType->at(l) == 1 || this->LinesType->at(l) == 2)
		{
			bool WriteDone = false;
			for(unsigned int i = 0; i < d->sections.size(); i++)
			{
				if(d->sections[i].line == l)
				{
					WriteDone = true;
					s += "\n[" + d->sections[i].key + "]" + this->Comments->at(l).text + "\n";
				}
			}
			if(!WriteDone)
			{
				for(unsigned int i = 0; i < d->sections.size(); i++)
				{
					for(unsigned int j = 0; j < d->sections[i].parameters.size(); j++)
					{
						if(d->sections[i].parameters[j].line == l)
						{
							s += d->sections[i].parameters[j].name + " = " + d->sections[i].parameters[j].value + \
								 this->Comments->at(l).text + "\n";
						}
					}
				}
			}
		}
	}

	return s;
}


//--------------------------------------------------------------------------
//!
//! \brief Writes a dictionnary into an INI file
//! \param    d    the dictionnary to write
//! \param    File a char pointer to the INI file name
//! \return   a boolean indicating if the writing operation is a success
//! 
//! This function needs a pointer to the specified dictionnary and a pointer 
//! to the file name. If the file name pointer is NULL, the dictionnary is
//! written into the previously opened file.
//! 
//! This function returns a boolean to indicate the success of the writing operation
//!
//--------------------------------------------------------------------------
bool __CALL INIParser::WriteINI(const dictionnary *d, const char *File)
{
	if(File == NULL)
		File = this->FileName;

	if(d == NULL)
		return false;

	try
	{
		ofstream f;
		f.open(File, ios::out);
		string s = this->DicoToString(d);
		f.write(s.c_str(), s.size());
		f.close();
		return true;
	}
	catch(...)
	{
		return false;
	}

}

//--------------------------------------------------------------------------
//!
//! \brief Set a boolean value
//! \param    section    a char pointer to the section name
//! \param    key        a char pointer to the parameter name
//! \param    value      the value to set for the specified key
//! \return   a boolean indicating if the writing operation is a success
//! 
//! This function needs a pointer to the specified dictionnary and a pointer 
//! to the specified key. if the section doesn't exists, it will be created.
//! If the key doesn't exist (but the section does), a new key is added to 
//! the specified section.
//! Then, the modified dictionnary is writted to the INI file if it exists
//! 
//! This function returns a boolean to indicate the success of the writing operation
//!
//--------------------------------------------------------------------------
bool __CALL INIParser::SetValue(const char *section, const char *key, bool value)
{
	if(value)
		return this->WriteValue(section, key, "true");
	else
		return this->WriteValue(section, key, "false");
}

//--------------------------------------------------------------------------
//!
//! \brief Set an integer value
//! \param    section    a char pointer to the section name
//! \param    key        a char pointer to the parameter name
//! \param    value      the value to set for the specified key
//! \return   a boolean indicating if the writing operation is a success
//! 
//! This function needs a pointer to the specified dictionnary and a pointer 
//! to the specified key. if the section doesn't exists, it will be created.
//! If the key doesn't exist (but the section does), a new key is added to 
//! the specified section.
//! Then, the modified dictionnary is writted to the INI file if it exists
//! 
//! This function returns a boolean to indicate the success of the writing operation
//!
//--------------------------------------------------------------------------
bool __CALL INIParser::SetValue(const char *section, const char *key, int value)
{
	return this->WriteValue(section, key, this->NumToStr(value));
}

//--------------------------------------------------------------------------
//!
//! \brief Set a double value
//! \param    section    a char pointer to the section name
//! \param    key        a char pointer to the parameter name
//! \param    value      the value to set for the specified key
//! \param    precision  the number of digits after point (default = 6)
//! \return   a boolean indicating if the writing operation is a success
//! 
//! This function needs a pointer to the specified dictionnary and a pointer 
//! to the specified key. if the section doesn't exists, it will be created.
//! If the key doesn't exist (but the section does), a new key is added to 
//! the specified section.
//! Then, the modified dictionnary is writted to the INI file if it exists
//! 
//! This function returns a boolean to indicate the success of the writing operation
//!
//--------------------------------------------------------------------------
bool __CALL INIParser::SetValue(const char *section, const char *key, double value, int precision)
{
	return this->WriteValue(section, key, this->NumToStr(value, precision));
}

//--------------------------------------------------------------------------
//!
//! \brief Set a string value
//! \param    section    a char pointer to the section name
//! \param    key        a char pointer to the parameter name
//! \param    value      the value to set for the specified key
//! \return   a boolean indicating if the writing operation is a success
//! 
//! This function needs a pointer to the specified dictionnary and a pointer 
//! to the specified key. if the section doesn't exists, it will be created.
//! If the key doesn't exist (but the section does), a new key is added to 
//! the specified section.
//! Then, the modified dictionnary is writted to the INI file if it exists
//! 
//! This function returns a boolean to indicate the success of the writing operation
//!
//--------------------------------------------------------------------------
bool __CALL INIParser::SetValue(const char *section, const char *key, string value)
{
	return this->WriteValue(section, key, value);
}

//--------------------------------------------------------------------------
//!
//! \brief Set a converted string value
//! \param    section    a char pointer to the section name
//! \param    key        a char pointer to the parameter name
//! \param    value      the value to set for the specified key
//! \return   a boolean indicating if the writing operation is a success
//! 
//! This function needs a pointer to the specified dictionnary and a pointer 
//! to the specified key. if the section doesn't exists, it will be created.
//! If the key doesn't exist (but the section does), a new key is added to 
//! the specified section.
//! Then, the modified dictionnary is writted to the INI file if it exists
//! 
//! This function returns a boolean to indicate the success of the writing operation
//!
//--------------------------------------------------------------------------
bool __CALL INIParser::WriteValue(const char *section, const char *parameter, string value)
{
	bool success = false;

	if(this->dico->sections.size() == 0)
		*(this->dico) = this->GetDictionnary();

	bool sExists = false;
	bool pExists = false;
	for(unsigned int i = 0; i < this->dico->sections.size(); i++)
	{
		if(this->dico->sections[i].key == string(section))
		{
			sExists = true;
			for(unsigned int j = 0; j < this->dico->sections[i].parameters.size(); j++)
			{
				cout << "Ancienne ligne : " << this->dico->sections[i].parameters[j].line << endl;
				if(this->dico->sections[i].parameters[j].name == string(parameter))
				{
					pExists = true;
					this->dico->sections[i].parameters[j].value = value;
				}
			}
			if(!pExists)
			{
				cout << endl;
				struct parameter p;
				struct comment c;
				vector<int>::iterator LIt = this->LinesType->begin();
				vector<comment>::iterator CIt = this->Comments->begin();
				c.text = "";
				p.name = string(parameter);
				p.value = value;
				p.line = this->dico->sections[i].parameters[this->dico->sections[i].parameters.size() - 1].line + 1;
				cout << "Nouvelle ligne : " << p.line << endl;
				c.line = p.line;
				this->ChangeLine(p.line);
				(this->EndLine)++;
				this->LinesType->insert(this->LinesType->begin() + p.line, 1);

				this->dico->sections[i].parameters.push_back(p);
			}
		}
	}

	if(!sExists)
	{
		struct section s;
		struct parameter p;
		struct comment c;
		c.text = "";
		s.key = string(section);
		s.line = this->EndLine;
		c.line = this->EndLine;
		(this->EndLine)++;
		this->LinesType->push_back(1);
		this->Comments->push_back(c);

		p.name = string(parameter);
		p.value = value;
		p.line = this->EndLine;
		c.line = this->EndLine;
		(this->EndLine)++;
		this->LinesType->push_back(1);
		this->Comments->push_back(c);

		s.parameters.push_back(p);
		this->dico->sections.push_back(s);
	}

	if(this->FileName != NULL)
		success = this->WriteINI(this->dico, NULL);

	return success;
}

// --------------------------------------------------------------------------
// Change the lines of sections, parameters and comments when a new parameter
// has been added to a dictionnary. Only line greater than the 'NewLine' are
// modified
void __CALL INIParser::ChangeLine(unsigned int NewLine)
{
	for(unsigned int i = 0; i < this->dico->sections.size(); i++)
	{
		if(this->dico->sections[i].line >= NewLine)
		{
			(this->dico->sections[i].line)++;
		}

		for(unsigned int j = 0; j < this->dico->sections[i].parameters.size(); j++)
		{
			if(this->dico->sections[i].parameters[j].line >= NewLine)
			{
				(this->dico->sections[i].parameters[j].line)++;
			}
		}
	}

	for(unsigned int i = 0; i < this->Comments->size(); i++)
	{
		if(this->Comments->at(i).line >= NewLine)
			(this->Comments->at(i).line)++;
	}

}
