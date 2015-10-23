//--------------------------------------------------------------------------
//                                INIPARSER.H
//--------------------------------------------------------------------------
//!
//! \file INIParser.h
//! \brief Header file for parsing INI file
//! \author Maxime Fontaine
//! \version 0.1
//! \date 2015-02-08
//!
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
//                            PRE-COMPILER DECLARATIONS
//--------------------------------------------------------------------------
#include <string.h>
#include <vector>

#ifndef INIParserH
#define INIParserH
#endif

// machine Linux
#if __linux__
#define __CALL
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
// machine windows (Borland C++)
#elif __WIN32__
#define __CALL __fastcall
#endif

//--------------------------------------------------------------------------
//                              STRUCTURES DECLARATIONS
//--------------------------------------------------------------------------

//! \brief structure for parameter
struct parameter
{
	//! \brief the name of the parameter
	std::string name;
	//! \brief the value of the parameter
	std::string value;
	//! \brief the comment of the parameter
	std::string comment;
	//! \brief the line of the parameter in the ini file
	int line;
};

//! \brief structure for section
struct section
{
	//! \brief the name of the section
	std::string key;
	//! \brief the comment of the section
	std::string comment;
	//! \brief a vector of structures parameter with the parameters of this section
	std::vector<parameter> parameters;
	//! \brief the line of the section in the ini file
	int line;
};

//! \brief structure for dictionnary
struct dictionnary
{
	//! \brief a vector of structures section with the sections of the INI file
	std::vector<section> sections;
	//! \brief a vector of string with the section names
	std::vector<std::string> Keys;
};

//! \brief structure for comment
struct comment
{
	//!  \brief the text of the comment
	std::string text;
	//! \brief the line of the comment in the ini file
	int line;
};

//--------------------------------------------------------------------------
//                              INIPARSER CLASS
//--------------------------------------------------------------------------

//-------------------------------------------------------------------------
//!
//! \class INIParser
//! \brief INIParser class.
//!
//! This class is the only class of the iniparser package.
//! It deals with INI file (read and write).
//!
//--------------------------------------------------------------------------
class INIParser
{
	private:
		std::vector<unsigned int> *KeysLines;
		std::vector<std::string> *Keys;
		std::vector<comment> *Comments;
		std::vector<int> *LinesType;
		dictionnary *dico;
		unsigned int FileEndLine;
		unsigned int EndLine;

		std::string __CALL StrTrim(std::string strinit, int mode=-1);
		std::string __CALL StrLower(std::string strinit);
		std::string __CALL NumToStr(int value);
		std::string __CALL NumToStr(double value, int precision=6);
		std::vector<std::string> __CALL GetLines();
		void __CALL GetLinesType();
		std::string __CALL DicoToString(const dictionnary *d);
		bool __CALL WriteValue(const char *section, const char *parameter, std::string value);
		void __CALL ChangeLine(unsigned int NewLine);

	public:
		const char *FileName;
		std::string TextFile;

		__CALL INIParser(const char* File=NULL);
		__CALL ~INIParser();
		bool __CALL Open(const char* File);

		std::vector<std::string> __CALL GetSectionsName();
		int __CALL GetSectionNumber();
		section __CALL GetSection(const char *SectionName);
		dictionnary __CALL GetDictionnary();
		bool __CALL GetBoolean(const char *section, const char *key);
		std::string __CALL GetString(const char *section, const char *key);
		int __CALL GetInteger(const char *section, const char *key);
		double __CALL GetDouble(const char *section, const char *key);

		bool __CALL WriteINI(const dictionnary *d, const char *File=NULL);
		bool __CALL SetValue(const char *section, const char *key, bool value);
		bool __CALL SetValue(const char *section, const char *key, int value);
		bool __CALL SetValue(const char *section, const char *key, double value, int precision=6);
		bool __CALL SetValue(const char *section, const char *key, std::string value);
};
