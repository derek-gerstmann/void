// ============================================================================================== //
//
// License:		The Lesser GNU Public License (LGPL) v3.0.
// 
// Author(s): 	Derek Gerstmann. The University of Western Australia (UWA). 
//				As well as the shoulders of many giants...
//
// This file is part of the Void framework.
//
// The Void framework is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// The Void framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with the Void framework.  If not, see <http://www.gnu.org/licenses/>.
//
// ============================================================================================== //

#include "core/core.h"
#include "core/hashing.h"

#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
#include <istream>
#include <iostream>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <algorithm>
#include <string> 
#include <iterator>

// ============================================================================================== //

#include <boost/program_options.hpp>
#include <boost/functional/hash.hpp>
#include <boost/filesystem.hpp>

// ============================================================================================== //

namespace opt = boost::program_options;
namespace fs = boost::filesystem;

VD_USING_NAMESPACE();
VD_USING_CORE_NAMESPACE();

// ============================================================================================== //

struct Settings 
{
	bool						Verbose;
	std::string				 	HeaderDefineName;
	std::string				 	IndexEnumName;
	std::string				 	IndexPrefix;
	std::string				 	ValueTableName;
	std::string				 	ValuePrefix;
	std::string				 	StringTableName;
	std::string				 	StringPrefix;
	std::string				 	IdTypeName;
	std::string				 	IdTableName;
	std::string				 	IdPrefix;
	std::string				 	SymbolTableName;
	std::string				 	SymbolTypeName;
	std::vector<std::string> 	NameSpaceList;
	std::string 			 	NameSpaceMacro;
	std::string 			 	MacroName;
	std::string 			 	MacroBegin;
	std::string 			 	MacroEnd;
	std::vector<std::string> 	HeaderList;
	std::vector<std::string> 	InputFileList;
	std::string				 	OutputFile;
};

// ============================================================================================== //

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    copy(v.begin(), v.end(), std::ostream_iterator<T>(std::cout, " ")); 
    return os;
}

// ============================================================================================== //

bool CompareFiles(
	const std::string& a,
	const std::string& b)
{
	std::ifstream ina;
	std::ifstream inb;
	std::string astr;
	std::string bstr;
	std::string temp;
	boost::hash<std::string> strhash;

	ina.open(a.c_str());
	while( std::getline(ina, temp) )
	{
		astr += temp;
	}
	ina.close();
	
	std::size_t ah = strhash(astr);
	
	inb.open(b.c_str());
	while( std::getline(inb, temp) )
	{
		bstr += temp;
	}
	
	std::size_t bh = strhash(bstr);
	return ah == bh;
}

// ============================================================================================== //

void OutputSeparator(
	std::ostream& output,
	const char* v = "-",
	const char* prefix = "-",
	const char* suffix = "-",
	const size_t width = 96)
{
	output << std::string(prefix);
	output.width(width);
	output.fill(v[0]);
	output << std::string(v);
	output.width(0);
	output.fill(' ');
	output << std::string(suffix) << std::endl;
}


void 
ConvertStringToPascalCase(
	std::string &str, 
	std::string &enumerated)
{
	if(str.size() < 1)
		return;

	std::string v = str;
	v[0] = std::toupper(str[0]);
	enumerated = v;
}

void 
ConvertStringToEnumFormat(
	const std::string &str, 
	std::string &enumerated)
{
	if(str.size() < 1)
		return;

	std::string v;
	v += std::toupper(str[0]);
	char previous = str[0];
	for(int i = 1; i < (int)str.size(); i++)
	{
		char current = str[i];
		if(!std::isupper(previous) && std::isupper(current) && 
			std::isalpha(previous) && std::isalpha(current))
		{
			// v += "_";
		}
		v += str[i]; // std::toupper(str[i]);
		previous = current;
	}
	enumerated = v;
}

void 
ConvertStringToFormattedHex(
	const std::string &str, 
	std::string &formatted)
{
	std::string v = str;
	if(str.size() < 2)
		return;
		
	v[0] = str[0];	// 0
	v[1] = str[1];	// x
	
	for(size_t i = 2; i < str.size(); i++)
		v[i] = std::toupper(str[i]);

	formatted = v;
}

void
ExtractSymbolStringFromMacro(
	const std::string &str, 
	const std::string &macro,
	std::vector<std::string>& results) 
{
	std::string substr = macro;
	std::string endstr = std::string(")");
	substr += std::string("(");
	
	std::string::size_type cursor = 0;
	std::string::size_type end;
	do {
		if(cursor == 0)
			cursor = str.find(substr);
		else
			cursor = str.find(substr, cursor);
	
		if (cursor != std::string::npos)
		{
			end = str.find_first_of(endstr, cursor);
			if(end != std::string::npos)
			{
				std::string sym = str.substr(cursor + substr.size(), end-substr.size()-cursor);

				std::string::iterator rm;
				rm = std::remove(sym.begin(), sym.end(), ' ');
				sym.erase(rm, sym.end());

				rm = std::remove(sym.begin(), sym.end(), '"');
				sym.erase(rm, sym.end());

				results.push_back( sym );
				cursor += substr.size();
			}
		}
	} while (cursor != std::string::npos);
}

void ProcessFiles(
	Settings& settings,
	std::vector<std::string>& symbols)
{
	size_t count = 0;
	int progress = 0;
	std::vector<std::string>::const_iterator it;
	for(it = settings.InputFileList.begin(); it != settings.InputFileList.end(); ++it)
	{
		count++;
		std::string filename(*it);	

		if(settings.Verbose)
		{
			progress = int((double)count/(double)settings.InputFileList.size() * 100.0);
			std::cout << "[";
			std::cout.width(3);
			std::cout << progress;
			std::cout.width(0);
			std::cout << "%] ";
			std::cout << "Processing : '" << filename << "'" << std::endl;
		}
		
		std::ifstream file(filename.c_str());
		std::string data, temp;
 
		while( std::getline(file, temp) )
		{
			data += temp;
		}

		ExtractSymbolStringFromMacro(data, settings.MacroName, symbols); 
	}	
}

void
FindUniqueSymbols(
	Settings& settings,
	std::vector<std::string>& extracted,
	std::vector<std::string>& symbols)
{
	std::sort(extracted.begin(), extracted.end());
	extracted.insert(extracted.begin(), std::string("Invalid"));
	std::vector<std::string>::iterator new_end = std::unique(extracted.begin(), extracted.end());

	std::vector<std::string>::iterator it;
	for(it = extracted.begin(); it != new_end; ++it)
		symbols.push_back(*it);
	
	extracted.clear();
}

void
GenerateDigest(
	Settings& settings,
	const std::vector<std::string>& symbols,
	std::vector<std::string>& digests)
{
	std::vector<std::string>::const_iterator it;
	for(it = symbols.begin(); it != symbols.end(); ++it)
	{
		std::string hstr;
		std::stringstream stream;
		
		vd::uid id = Hashing::Murmur(it->c_str(), it->length());
    	if(id.high() == 0)
        	stream << std::hex << "0x" << id.low() << "ULL" << std::dec;
	    else
    	    stream << std::hex << "0x" << id.high() << "ULL, " << "0x" << id.low() << "ULL" << std::dec;
    	   
		digests.push_back( stream.str() );
	}
}

void
OutputKeyValue(
	std::ofstream& output,
	const std::string& key,
	const std::string& value,
	size_t pad)
{
	output << std::setiosflags(std::ios::left);
	output.width(pad);
	output.fill(' ');
	output << key;
	output << std::setiosflags(std::ios::left);
	output << value << std::endl;
	output << std::setiosflags(std::ios::left);
}

void
GenerateHeader(
	Settings& settings,
	std::vector<std::string> symbols,
	std::vector<std::string> digests )
{
	static const size_t pad = 36;
	std::string key;
	std::string value;
	std::string comma(",");
	std::string assign(" = ");

	std::ofstream output;
	fs::path tmpdir("/tmp");
	fs::path dst(settings.OutputFile);
	fs::path filename(dst.filename());
	
	fs::path tmpfile = tmpdir / filename;
	output.open(tmpfile.c_str());
	std::vector<std::string>::const_iterator it;

	std::cout << "Generating header '" << settings.OutputFile;
	std::cout << "' with " << symbols.size() << " entries ..." << std::endl;
	
	output << "#ifndef " << settings.HeaderDefineName << std::endl;
	output << "#define " << settings.HeaderDefineName << std::endl;

	output << std::endl;
	OutputSeparator(output, "=", "// ", " //");
	output << std::endl;

	if(settings.HeaderList.size())
	{
		for(it = settings.HeaderList.begin(); it != settings.HeaderList.end(); ++it)
			output << "#include \"" << *it << "\"" << std::endl;
	}

	output << std::endl;
	OutputSeparator(output, "=", "// ", " //");
	output << std::endl;

	if(settings.MacroBegin.size())
	{
		output << settings.MacroBegin << "();" << std::endl;

		output << std::endl;
		OutputSeparator(output, "=", "// ", " //");
		output << std::endl;
	}

	if(settings.NameSpaceList.size())
	{
		output << "#define " << settings.NameSpaceMacro << "	VD_CONSTANTS_NAMESPACE::";
		for(it = settings.NameSpaceList.begin(); it != settings.NameSpaceList.end(); ++it)
		{
			output << *it;
			std::vector<std::string>::const_iterator next = it;
			if(++next != settings.NameSpaceList.end())
			{
				output << "::";
			}
		}
		output << std::endl;

		output << std::endl;
		for(it = settings.NameSpaceList.begin(); it != settings.NameSpaceList.end(); ++it)
			output << "namespace " << *it << " {" << std::endl;
	}

	output << std::endl;
	OutputSeparator(output, "=", "// ", " //");
	output << std::endl;

	output << "static const size_t Count = " << boost::lexical_cast<std::string>(symbols.size()) << "UL;" << std::endl;

	output << std::endl;
	OutputSeparator(output, "=", "// ", " //");
	output << std::endl;

	output << "enum " << settings.IndexEnumName << std::endl;
	output << "{" << std::endl;
	{
		std::size_t count = 0;
		std::string prefix = settings.IndexPrefix;

		for(it = symbols.begin(); it != symbols.end(); ++it)
		{
			std::string sym;
			ConvertStringToEnumFormat(*it, sym);
			key = prefix + sym;
			value = assign + boost::lexical_cast<std::string>(count) + comma;

			output << "\t"; 
			OutputKeyValue(output, key, value, pad);
			count++;
		}
	}
	output << "};" << std::endl;

	output << std::endl;
	OutputSeparator(output, "=", "// ", " //");
	output << std::endl;

	output << "static const " << settings.IdTypeName << std::endl;
	output << settings.IdTableName << std::string("[] = ") << std::endl;
	output << "{" << std::endl;
	{
		std::size_t count = 0;
		std::size_t i = 0;
		std::string prefix = settings.IdPrefix; 
		for(it = symbols.begin(); it != symbols.end(); ++it)
		{
			value = digests[i++];
			output << "\t" << settings.IdTypeName << "(" << value << ")," << std::endl;
			count++;
		}
		output << "\t" << settings.IdTypeName << "(0,0)" << std::endl;
	}
	output << "};" << std::endl;

	output << std::endl;
	OutputSeparator(output, "=", "// ", " //");
	output << std::endl;
	
	output << "static const char*" << std::endl;
	output << settings.StringTableName << std::string("[] = ") << std::endl;
	output << "{" << std::endl;
	{
		for(it = symbols.begin(); it != symbols.end(); ++it)
		{
			output << "\t\"" << *it << "\"," << std::endl;
		}
		output << "\t0,0" << std::endl;
	}
	output << "};" << std::endl;
	output << std::endl;

	output << std::endl;
	OutputSeparator(output, "=", "// ", " //");
	output << std::endl;
	
	output << "static const " << settings.SymbolTypeName << std::endl;
	output << settings.SymbolTableName << std::string("[] = ") << std::endl;
	output << "{" << std::endl;
	{
		for(it = symbols.begin(); it != symbols.end(); ++it)
		{
			
			std::string sym;
			
			ConvertStringToEnumFormat(*it, sym);
			key = settings.IndexPrefix + sym;

			output << "\t" << settings.SymbolTypeName << "( ";
			output << settings.IdTableName << "[ " << key << " ], ";
			output << settings.StringTableName << "[ " << key << " ] )," << std::endl;
		}
		output << "\t"  << settings.SymbolTypeName << "( 0, 0 )" << std::endl;
	}
	output << "};" << std::endl;

	output << std::endl;
	OutputSeparator(output, "=", "// ", " //");
	output << std::endl;

	std::vector<std::string> reversed = settings.NameSpaceList;
	std::reverse(reversed.begin(), reversed.end());
	for(it = reversed.begin(); it != reversed.end(); it++)
		output << "} // end namespace: " << *it << std::endl;

	if(settings.MacroEnd.size())
	{
		output << std::endl;
		OutputSeparator(output, "=", "// ", " //");
		output << std::endl;
		output << settings.MacroEnd << "();" << std::endl;
	}

	output << std::endl;
	OutputSeparator(output, "=", "// ", " //");
	output << std::endl;

	output << "#endif // " << settings.HeaderDefineName << std::endl;
	output << std::endl;	
	output.close();
	
	if(fs::exists(dst))
	{
		if(CompareFiles(tmpfile.c_str(), dst.c_str()))
		{
			std::cout << "Header unchanged!  Leaving existing '" << settings.OutputFile << "' unmodified!" << std::endl;
		}
		else
		{	
			std::cout << "Replacing existing '" << settings.OutputFile << "' with new symbols!" << std::endl;
			fs::copy_file(tmpfile, dst, fs::copy_option::overwrite_if_exists);
		}
	}
	else
	{
		std::cout << "Creating new '" << settings.OutputFile << "' with extracted symbols!" << std::endl;	
		fs::copy_file(tmpfile, dst, fs::copy_option::overwrite_if_exists);
	}
}


int ParseArgs(
	int ac, char* av[],
	Settings& settings)
{
    try {
        opt::options_description generic("Options");
        generic.add_options()
            ("version,v", "print version string")
            ("verbose,V", "use verbose output messages")
            ("help,h", "output this help message")
            ("macro,m", opt::value< std::string >(&(settings.MacroName))->default_value("vd_sym"), 
                 "name of macro directive to locate for extracting symbol constants (should be of the form 'MyMacro(MySymbol)'.")
            ("output-file,o", opt::value< std::string >(&(settings.OutputFile))->default_value("symbols.h"), 
                 "filename of header file to generate")
            ("index-enum", opt::value< std::string >(&(settings.IndexEnumName))->default_value("Id"), 
                 "variable name of static enum for index values")
            ("index-prefix", opt::value< std::string >(&(settings.IndexPrefix))->default_value("Id"), 
                 "string prefix to prepend to index value names")
            ("str-table", opt::value< std::string >(&(settings.StringTableName))->default_value("Strings"), 
                 "variable name to use for static string table for storing literal strings (stored as const char*)")
            ("str-prefix", opt::value< std::string >(&(settings.StringPrefix))->default_value("Str"), 
                 "string prefix to prepend to string value names")
            ("id-table", opt::value< std::string >(&(settings.IdTableName))->default_value("Keys"), 
                 "variable name to use for static id table for storing the unique ids (stored as id-type)")
            ("id-type", opt::value< std::string >(&(settings.IdTypeName))->default_value("uid"), 
                 "type name used for declaring id values")
            ("id-prefix", opt::value< std::string >(&(settings.IdPrefix))->default_value("Key"), 
                 "string prefix to prepend to id value names")
            ("sym-table", opt::value< std::string >(&(settings.SymbolTableName))->default_value("Table"), 
                 "variable name to use for static symbol table for storing the unique symbols (stored as Symbol)")
            ("sym-type", opt::value< std::string >(&(settings.SymbolTypeName))->default_value("VD_CORE_NAMESPACE::Symbol"), 
                 "type name used for declaring symbol values")
            ("begin", opt::value< std::string >(&(settings.MacroBegin))->default_value("VD_CONSTANTS_NAMESPACE_BEGIN"), 
                 "name of begin function macro to use to wrap all declarations -- all other namespaces and variables are declared inside the wrapper.")
            ("end", opt::value< std::string >(&(settings.MacroEnd))->default_value("VD_CONSTANTS_NAMESPACE_END"), 
                 "name of end function macro to use to wrap all declarations -- all other namespaces and variables are declared inside the wrapper.")
            ("using-macro,u", opt::value< std::string >(&(settings.NameSpaceMacro))->default_value("VD_CONSTANTS_EXTRACTED_SYMBOLS_SCOPE"), 
                 "name of #define to use for importing extracted constants namespace from this generated header file")
            ("wrapper,w", opt::value< std::string >(&(settings.HeaderDefineName))->default_value("VD_CONSTANTS_EXTRACTED_SYMBOLS_INCLUDED"), 
                 "name of #define to use for wrapping header file")
            ("include,i", opt::value< std::vector<std::string> >(&(settings.HeaderList)), 
            	"header file names to use for generating #include statements in preamble of generated header (specify more than one for mutiple include statements)")
            ("namespace,n", opt::value< std::vector<std::string> >(&(settings.NameSpaceList)), 
            	"namespace to use for generated tables (specify more than one for nested namespace(s))")
                        ;
    
        opt::options_description hidden("Hidden Options");
        hidden.add_options()
            ("input-file,f", opt::value< std::vector<std::string> >(&(settings.InputFileList)), "input file(s)")
            ;
        
        opt::options_description cmdline_options;
        cmdline_options.add(generic).add(hidden);

        opt::options_description visible("Usage");
        visible.add(generic);
        
        opt::positional_options_description p;
        p.add("input-file", -1);
        
        opt::variables_map vm;
        store(opt::command_line_parser(ac, av).
              options(cmdline_options).positional(p).run(), vm);
        notify(vm);
        
        if (vm.count("help")) {
            std::cout << visible << "\n";
            return 0;
        }

        if (vm.count("version")) {
            std::cout << "Version 1.0\n";
            return 0;
        }

        if (vm.count("verbose")) {
            std::cout << "Verbose output enabled...\n";
            settings.Verbose = true;
        }
        else
        {
        	settings.Verbose = false;
        }

        if (vm.count("output-file"))
        {
        	OutputSeparator(std::cout);

            std::cout << "Generating : " 
                 << vm["output-file"].as< std::string >() << "\n";
        }
    }
    catch(std::exception& e)
    {
        std::cout << e.what() << "\n";
        return 1;
    }    
    return 0;
}

int main(int ac, char* av[])
{	
	Settings settings;
	std::vector<std::string> symbols;
	std::vector<std::string> digests;
	std::vector<std::string> extracted;
	
	if(ParseArgs(ac, av, settings))
		return 1;

	ProcessFiles(settings, extracted);
	if(extracted.size() < 1)
	{
		std::cerr << "Failed to locate any symbols to extract!\n" << std::endl;
		return 0;
	}

	FindUniqueSymbols(settings, extracted, symbols);

	OutputSeparator(std::cout);
	GenerateDigest(settings, symbols, digests);
	GenerateHeader(settings, symbols, digests);
	OutputSeparator(std::cout);

	return 0;
}