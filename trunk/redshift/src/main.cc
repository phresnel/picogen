//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copyright (C) 2010  Sebastian Mach (*1983)
// * mail: phresnel/at/gmail/dot/com
// * http://phresnel.org
// * http://picogen.org
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// TODO: check if boost reports on cerr or cout
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#include <string>

#include <boost/program_options.hpp>
#include <iostream>

#include "../include/auxiliary/currentdate.hh"
#include "../include/redshift.hh"
namespace {
        using redshift::optional;

        struct Options {
                bool printStats;
                std::string inputFile;
                std::string outputFile;
                std::string renderSetupName;
        };

        optional<Options> parseOptions (int argc, char *argv[]) {
                namespace po = boost::program_options;

                bool print_stats;
                std::string input_file="", output_file;
                std::string render_setup;

                // Declare the supported options.
                po::positional_options_description p;
                p.add ("input-file", -1);

                po::options_description desc("Allowed options");
                desc.add_options()
                        ("help", "print help message")

                        ("print-stats,p",
                        po::value<bool>(&print_stats)
                        ->default_value(false),
                        "print statistics after rendering")

                        ("input-file,i",
                        po::value<std::string>(&input_file),
                        "file that contains the job")

                        ("output-file,o",
                        po::value<std::string>(&output_file),
                        "image file to write to")

                        ("render-setup,r",
                        po::value<std::string>(&input_file),
                        "if there are multiple rendering-setups in the input-file, "
                        "use this parameter to describe the name of the setup you "
                        "want to render; if unset, you will be prompted"
                        )
                ;

                po::variables_map vm;
                try {
                        po::store(po::command_line_parser(argc, argv)
                                .options(desc).positional(p).run(), vm);
                        po::notify(vm);
                } catch (std::exception const &rhs) {
                        std::cout << rhs.what() << "\n";
                        std::cout << desc << std::endl;
                        return optional<Options>();
                }

                if (vm.count("help")) {
                        std::cout << desc << "\n";
                        return optional<Options>();
                }


                if (input_file == "") {
                        std::cout << "No input-file set.\n";
                        std::cout << desc << "\n";
                        return optional<Options>();
                }
                if (output_file == "") {
                        output_file = "redshift-"+CurrentDate::AsPartOfFilename()+".bmp";
                        std::cout << "No output-file set, will write to '" << output_file << "'.\n";
                }
                const std::string ext = filename_extension (output_file);
                if (ext == "bmp") {
                        // okay
                } else if (ext == "") {
                        std::cout << "Missing filename extension for output file, will use bmp.\n";
                } else {
                        std::cout << "Unsupported filename extension for output file: " << ext << "\n";
                        // TODO: --help extensions
                }

                Options ret;
                ret.printStats = print_stats;
                ret.inputFile = input_file;
                ret.outputFile = output_file;
                ret.renderSetupName = render_setup;
                return ret;
        }
}


int main (int argc, char *argv[]) {
        const optional<Options> oo = parseOptions(argc,argv);
        if (!oo)
                return 0;
        const Options options = *oo;
}
