// ============================================================================================== //
//
// License:     The Lesser GNU Public License (LGPL) v3.0.
// 
// Author(s):   Derek Gerstmann. The University of Western Australia (UWA). 
//              As well as the shoulders of many giants...
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

#include "core/text.h"

// ============================================================================================== //

VD_CORE_NAMESPACE_BEGIN();

// ============================================================================================== //

namespace Text 
{

// ============================================================================================== //

std::vector<vd::string> 
Tokenize(
    const vd::string& string, 
    const vd::string& delim)
{
    vd::string::size_type lastPos = string.find_first_not_of(delim, 0);
    vd::string::size_type pos = string.find_first_of(delim, lastPos);
    std::vector<vd::string> tokens;

    while(vd::string::npos != pos || vd::string::npos != lastPos)
    {
        tokens.push_back(string.substr(lastPos, pos - lastPos));
        lastPos = string.find_first_not_of(delim, pos);
        pos = string.find_first_of(delim, lastPos);
    }

    return tokens;
}

vd::string 
Trim(const vd::string& str)
{
    vd::string::size_type
    start = str.find_first_not_of(" \t\r\n"),
    end = str.find_last_not_of(" \t\r\n");

    return str.substr(start == vd::string::npos ? 0 : start,
                      end == vd::string::npos ? str.length() - 1 : end - start + 1);
}

vd::string 
Indent(
    const vd::string& string, int amount)
{
    std::istringstream iss(string);
    std::ostringstream oss;
    vd::string str;
    bool firstLine = true;

    while(!iss.eof())
    {
        std::getline(iss, str);

        if(!firstLine)
        {
            for(int i = 0; i < amount; ++i)
                oss << "  ";
        }

        oss << str;

        if(!iss.eof())
            oss << std::endl;

        firstLine = false;
    }

    return oss.str();
}

vd::string 
FormatByteSize(size_t size)
{
    vd::real value = (vd::real) size;
    const char* prefixes[] =
    {
        "B", "KB", "MB", "GB", "TB", "PB", "EB"
    };

    int prefix = 0;
    while(prefix < 5 && value > 1024.0f)
    {
        value /= 1024.0f; ++prefix;
    }

    return Text::Format(prefix == 0 ? "%.0f %s" : "%.2f %s", value, prefixes[prefix]);
}

// ============================================================================================== //

}

// ============================================================================================== //

VD_CORE_NAMESPACE_END();

// ============================================================================================== //
