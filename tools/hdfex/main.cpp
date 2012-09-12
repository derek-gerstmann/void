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

#include <boost/program_options.hpp>
#include <boost/functional/hash.hpp>
#include <boost/filesystem.hpp>

#include "std.h"
#include "hdf.h"
#include "dmpg.h"
#include "neo4j.h"

// ============================================================================================== //

namespace opt = boost::program_options;
namespace fs = boost::filesystem;

// ============================================================================================== //

struct Settings
{
    bool						Verbose;
    std::vector<std::string>    KeyList;
    std::vector<std::string>    MergeList;
    std::vector<std::string> 	DataSetList;
    std::vector<std::string> 	InputFileList;
    std::string				 	OutputFile;
    std::string                 Host;
    std::string                 Port;
};

// ============================================================================================== //

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    copy(v.begin(), v.end(), std::ostream_iterator<T>(std::cout, " "));
    return os;
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

// ============================================================================================== //

int
ExportToNeo4j(
    const Settings& settings)
{
    boost::asio::io_service service;

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

        OutputSeparator(std::cout);
        std::cout << " -- Exporting to " << settings.Host << ":" << settings.Port << " --" << std::endl;
        OutputSeparator(std::cout);

        Storage::Hdf::ToPropertyGraph extract;

        Storage::Hdf::ToPropertyGraph::GraphType graph;
        extract.Open(filename);
        extract.ExportMetaData(graph);

        Storage::Hdf::ToPropertyGraph::GraphType::NodeListType nodes;
        graph.GetNodeList(nodes);

        Neo4j::Client::Rest::Basic neo4j(service);

        OutputSeparator(std::cout);
        std::cout << " -- Nodes: " << nodes.size() << std::endl;
        OutputSeparator(std::cout);

        neo4j.Serialize(std::cout, nodes);
        nodes.clear();

        Storage::Hdf::ToPropertyGraph::GraphType::EdgeListType edges;
        graph.GetEdgeList(edges);

        OutputSeparator(std::cout);
        std::cout << " -- Edges: " << edges.size() << std::endl;
        OutputSeparator(std::cout);

        neo4j.Serialize(std::cout, edges);
        edges.clear();
    }

    return 0;
}

// ============================================================================================== //

bool ParseArgs(
    int ac, char* av[],
    Settings& settings)
{
    try
    {
        opt::options_description generic("Options", 160, 80);
        generic.add_options()
            ("version,v",   "Print version string")
            ("verbose,V",   "Use verbose output messages")
            ("help,h",      "Output this help message")
            ("key,k",       "Name(s) of datasets / attributes to use as keys for indexes")
            ("merge,m",     opt::value< std::vector<std::string> >(&(settings.MergeList)),
                            "Name(s) of same-size datasets to merge into unique objects")
            ("url,u",       opt::value< std::string >(&(settings.Host))->default_value("localhost"),
                            "Host URL for HTTP server to send export data")
            ("port,p",       opt::value< std::string >(&(settings.Port))->default_value("7474"),
                            "Port on the HTTP server to send export data")
            ("output,o",    opt::value< std::string >(&(settings.OutputFile))->default_value("exported.json"),
                            "Filename prefix to save export data into")
            ("dataset,d",   opt::value< std::vector<std::string> >(&(settings.DataSetList)),
                             "Fullpath of dataset to export.");

        opt::options_description hidden("Hidden Options");
        hidden.add_options()
            ("input,i", opt::value< std::vector<std::string> >(&(settings.InputFileList)), "Input file(s)");

        opt::options_description cmdline_options;
        cmdline_options.add(generic).add(hidden);

        opt::positional_options_description p;
        p.add("input", -1);

        opt::variables_map vm;
        store(opt::command_line_parser(ac, av).
              options(cmdline_options).positional(p).run(), vm);
        notify(vm);

        if (vm.count("help"))
        {
            std::cout << "Usage:" << "\n";
            std::cout << "  " << fs::basename(av[0]) << " [options] file ..." << "\n";
            std::cout << "\n";
            std::cout << generic << "\n";
            return false;
        }

        if (vm.count("version"))
        {
            std::cout << fs::basename(av[0]) << ": ";
            std::cout << "v1.0\n";
            return false;
        }

        if (vm.count("verbose"))
        {
            std::cout << "Verbose output enabled...\n";
            settings.Verbose = true;
        }
        else
        {
            settings.Verbose = false;
        }

        if (vm.count("merge"))
        {
            OutputSeparator(std::cout);

            std::cout << "Merging : \n";
            for(size_t n = 0; n < settings.MergeList.size(); n++)
                std::cout << "  " << settings.MergeList[n] << "\n";
        }

        if (vm.count("output"))
        {
            OutputSeparator(std::cout);

            std::cout << "Generating : "
                      << vm["output"].as< std::string >() << "\n";
        }
    }
    catch(std::exception& e)
    {
        std::cout << e.what() << "\n";
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------------------------------- //

int main(int ac, char* av[])
{
    Settings settings;

    if(ParseArgs(ac, av, settings) == false)
        return 1;

    ExportToNeo4j(settings);
    return 0;
}

// ----------------------------------------------------------------------------------------------- //
