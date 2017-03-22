INIParser documentation
========================

***
**Introduction**<br>
iniParser is a simple C++ library offering ini file parsing services.<br>
The library is pretty small (only one class) and robust, and
does not depend on any other external library to compile. It is written
in ANSI C++ and should compile on most platforms without difficulty.
***
**What is an ini file?**<br>
An ini file is an ASCII file describing simple parameters
(character strings, integers, floating-point values or booleans)
in an explicit format, easy to use and modify for users.<br>
An ini file is segmented into Sections, declared by the following
syntax:<br>
`[Section Name]`<br>
i.e. the section name enclosed in square brackets, alone on a
line. Sections names are allowed to contain any character but
square brackets or linefeeds.<br>

In any section are zero or more variables, declared with the
following syntax:<br>
`Key = value # comment`<br>
The key is any string (possibly containing blanks). The value is
any character on the right side of the equal sign. Values can be
given enclosed with quotes. If no quotes are present, the value is
understood as containing all characters between the first and the
last non-blank characters before the comment. The following
declarations are identical:<br>
`Hello = "this is a long string value" # comment`<br>
`Hello = this is a long string value # comment`<br>
The hash (or semicolon) and comment at the end of the line are optional. If
there is a comment, it starts from the first character after the hash up to
the end of the line.<br>

Comments in an ini file are:<br>
- Lines starting with a hash sign<br>
- Blank lines (only blanks or tabs)<br>
- Comments given on value lines after the the hash or the semicolon (if present)<br>
***
**Compiling/installing the library**<br>
Edit the Makefile to indicate the C++ compiler you want to use, the
options to provide to compile ANSI C, and possibly the options to pass
to the ar program on your machine to build a library (.a) from a set
of object (.o) files.<br>
Defaults are set for the gcc compiler and the standard ar library
builder.<br>
Type 'make', that should do it.<br>
To use the library in your programs, add the following line on top
of your module:<br>
`#include "INIParser.h"`<br>

See the file test/initest.cpp for an example.<br>
***
**Using the parser**<br>
To open an INI file, you can pass the file name in the constructor of the INIParser 
class or the INIParser::Open() method.<br>
`parser = new INIParser(myIniFile);
//or
parser = new INIParser();
parser->Open(myIniFile);`<br>
Then sections are identified, and in each section a new entry is 
created for every keyword found. The keywords are stored with 
the following syntax:<br>
`[Section]`<br>
`Keyword = value # comment`<br>

is converted to the following dictionnary structure:<br>
`dictionnary.section[i].key = "Section"`<br>
`dictionnary.section[i].parameter[j].name = "Keyword"`<br>
`dictionnary.section[i].parameter[j] = value`<br>

This means that if you want to retrieve the value that was stored
in the section called `water`, in the keyword `density`,
you would make a request to the dictionary for `section = Water` and `parameter = density`. All section and keyword names are converted to lowercase before storage in the structure. The value side is conserved as it has been parsed, though.<br>

To get the section names and section number present in the INI file, you can use the following methods<br>
`INIParser::GetSectionsName()`<br>
`INIParser::GetSectionNumber()`<br>
You can also get and/or set some specific values <br>
`INIParser::GetBoolean()`<br>
`INIParser::GetInteger()`<br>
`INIParser::GetDouble()`<br>
`INIParser::GetString()`<br>
`INIParser::SetValue()`<br>
All values parsed from the ini file are stored as strings. The
accessors are just converting these strings to the requested type on
the fly, but you could basically perform this conversion by yourself
after having called the string accessor.<br>

Notice that iniparser_getboolean() will return an integer (0 or 1),
trying to make sense of what was found in the file. Strings starting
with "y", "Y", "t", "T" or "1" are considered true values (return 1),
strings starting with "n", "N", "f", "F", "0" are considered false
(return 0). This allows some flexibility in handling of boolean
answers.<br>

If you want to add extra information into the structure that was not
present in the ini file, you can use iniparser_set() to insert a
string.<br>

If you want to add a section to the structure, add a key
with a NULL value. Example:<br>
`iniparser_set(ini, "section", NULL);`<br>
`iniparser_set(ini, "section:key1", NULL);`<br>
`iniparser_set(ini, "section:key2", NULL);`<br>
***
**A word about the implementation**<br>

The dictionary structure is a pretty simple dictionary
implementation which might find some uses in other applications.
If you are curious, look into the source.<br>
