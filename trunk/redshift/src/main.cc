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

#include "../include/auxiliary/currentdate.hh"
#include "../include/redshift.hh"

#include "../include/redshift.hh"
#include "../include/rendertargets/sdlrendertarget.hh"
#include "../include/rendertargets/colorrendertarget.hh"
#include "../include/interaction/sdlcommandprocessor.hh"
#include "../include/basictypes/height-function.hh"
#include "../../actuarius/actuarius.hh"

#include <string>
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <fstream>

#include <boost/program_options.hpp>

namespace {
        using redshift::optional;
        using redshift::tuple;
        using redshift::make_tuple;
        using redshift::get;

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

        tuple<int,std::string> toIntOrString (std::string const &str) {
                bool canBeNumber = true;
                for (unsigned int i=0; i<str.length(); ++i) {
                        switch (str[i]) {
                        case '0':case '1':case '2':case '3':case '4':
                        case '5':case '6':case '7':case '8':case '9':
                                break;
                        default:
                                canBeNumber = false;
                                goto afterLoop;
                        };
                }
                afterLoop:

                int num;
                if (!canBeNumber) {
                        num = -1;
                } else {
                        std::stringstream ss;
                        ss << str;
                        ss >> num;
                }
                return make_tuple(num, str);
        }
}



namespace redshift { namespace scenefile {

        // Object.
        struct Object {
                enum Type {
                        horizon_plane,
                        lazy_quadtree
                };
                static const actuarius::Enum<Type> Typenames;
                Type type;

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;

                }
        };
        const actuarius::Enum<Object::Type> Object::Typenames =
                ( actuarius::Nvp<Object::Type>(Object::horizon_plane, "horizon-plane")
                | actuarius::Nvp<Object::Type>(Object::lazy_quadtree, "lazy-quadtree")
                );



        // SurfaceIntegrator.
        struct SurfaceIntegrator {
                enum Type {
                        redshift
                };
                static const actuarius::Enum<Type> Typenames;
                Type type;

                SurfaceIntegrator ()
                : type(redshift)
                {}

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;

                        arch & pack("type", Typenames, type);
                }
        };
        const actuarius::Enum<SurfaceIntegrator::Type> SurfaceIntegrator::Typenames =
                ( actuarius::Nvp<SurfaceIntegrator::Type>(SurfaceIntegrator::redshift, "redshift")
                );



        // VolumeIntegrator.
        struct VolumeIntegrator {
                enum Type {
                        none,
                        emission,
                        single
                };
                static const actuarius::Enum<Type> Typenames;
                Type type;
                float stepSize;

                VolumeIntegrator ()
                : type(none), stepSize(100.f)
                {}

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;
                        arch & pack("type", Typenames, type);
                        arch & pack("step-size", stepSize);
                }
        };
        const actuarius::Enum<VolumeIntegrator::Type> VolumeIntegrator::Typenames =
                ( actuarius::Nvp<VolumeIntegrator::Type>(VolumeIntegrator::none, "none")
                | actuarius::Nvp<VolumeIntegrator::Type>(VolumeIntegrator::emission, "emission")
                | actuarius::Nvp<VolumeIntegrator::Type>(VolumeIntegrator::single, "single")
                );



        // RenderSettings.
        struct RenderSettings {
                unsigned int width, height;
                std::string title;
                SurfaceIntegrator surfaceIntegrator;
                VolumeIntegrator volumeIntegrator;

                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;

                        //arch & pack ("title", title);
                        arch & pack ("width", width);
                        arch & pack ("height", height);
                        arch & pack ("surface-integrator", surfaceIntegrator);
                        if (volumeIntegrator.type != VolumeIntegrator::none)
                                arch & pack ("volume-integrator", volumeIntegrator);
                }
        };



        // Scene = (RenderSettings+)(Objects*)
        class Scene {
                std::vector<Object> objects_;
                std::vector<RenderSettings> renderSettings_;
        public:
                void addRenderSettings (RenderSettings const &rs) {
                        renderSettings_.push_back (rs);
                }
                unsigned int renderSettingsCount() const {
                        return renderSettings_.size();
                }
                RenderSettings const & renderSettings(unsigned int index) const {
                        return renderSettings_[index];
                }

                void addObject (Object const &o) {
                        objects_.push_back (o);
                }
                unsigned int objectCount() const {
                        return objects_.size();
                }
                Object const & object(unsigned int index) const {
                        return objects_[index];
                }


                // Serialization.
                template<typename Arch>
                void serialize (Arch &arch) {
                        using actuarius::pack;

                        arch & pack ("render-settings", &RenderSettings::title, renderSettings_);
                        arch & pack ("objects", &Object::type, Object::Typenames, objects_);
                }
        };
} }

void actuarius_test () {
        // TODO: make render settings an advice-thing, have multiple skies, have if-render-is member in sky (so that e.g. in "preview" there could be no ckouds)
        using namespace redshift::scenefile;
        using namespace actuarius;
        Scene scene;

        Object o;
        o.type = Object::lazy_quadtree;
        scene.addObject (o);
        o.type = Object::horizon_plane;
        scene.addObject (o);

        RenderSettings rs;
        rs.width = 800;
        rs.height = 600;
        rs.title = "preview-easy";
        scene.addRenderSettings (rs);
        rs.width = 800;
        rs.height = 600;
        rs.title = "preview-tough";
        scene.addRenderSettings (rs);
        rs.width = 3200;
        rs.height = 1600;
        rs.title = "full";
        scene.addRenderSettings (rs);

        {std::ofstream ofs("test.red");
        OArchive (ofs) & pack("scene", scene);}

        {
                // Evolve read function from this
                Scene scene;
                std::ifstream ifs("test.red");
                IArchive (ifs) & pack("scene", scene);

                if (scene.renderSettingsCount()>1) {
                        std::cout << "There are multiple render settings present, \n";
                        for (unsigned int i=0; i<scene.renderSettingsCount(); ++i) {
                                std::cout << " [" << i << "] " << scene.renderSettings(i).title << "\n";
                        }
                        std::cout << ". Which one do you want to use (number or [partial] name)? "<< std::endl;

                        int bestMatch = 0;
                        int index = -1;
                        do {
                                std::string str;
                                std::getline (std::cin,str);
                                const tuple<int,std::string> ns = toIntOrString(str);

                                if (get<0>(ns)>=0 && (unsigned)get<0>(ns)<scene.renderSettingsCount()) {
                                        index = get<0>(ns);
                                } else {
                                        for (unsigned int i=0; i<scene.renderSettingsCount(); ++i) {
                                                if (scene.renderSettings(i).title == get<1>(ns)) {
                                                        index = i;
                                                        break;
                                                } else if (index < 0) {
                                                        if (std::string::npos !=
                                                                scene.renderSettings(i).title.find(get<1>(ns)))
                                                                index = i;
                                                }
                                        }
                                }
                                if (index < 0) {
                                        std::cout << "Number or name \"" << str << "\" "
                                                << "not found. Please type in a valid "
                                                << "number or [partial] name." << std::endl;
                                }
                        } while (index < 0);

                        std::cout << "You have chosen [" << index << "], \""
                                << scene.renderSettings(index).title << "\"." << std::endl;
                }
        }
}

int main (int argc, char *argv[]) {
        actuarius_test();
        return 0;

        const optional<Options> oo = parseOptions(argc,argv);
        if (!oo)
                return 0;
        const Options options = *oo;
}
