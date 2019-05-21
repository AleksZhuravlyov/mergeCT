#include <iostream>
#include <string>
#include <fstream>

#include <boost/program_options.hpp>

#include <mergeNcFiles.h>

namespace po = boost::program_options;

const std::string VERSION = "1.1";


int main(int argc, char **argv) {

    try {

        po::options_description description("Command line options");
        description.add_options()

                ("help,h", "Display help message")

                ("version,v", "Display version")

                ("config", po::value<std::string>(), "Config file")

                ("variable,var",
                 po::value<std::string>()->default_value("tomo"),
                 "Variable name")

                ("size,s",
                 po::value<float>()->default_value(3.600846E-5),
                 "Voxel size (m)")

                ("output,o",
                 po::value<std::string>()->default_value("result.nc"),
                 "Output file name")

                ("input,i",
                 po::value<std::vector<std::string>>(),
                 "Input files list (positional option)");


        po::positional_options_description posDescription;
        posDescription.add("input", -1);

        po::variables_map varMap;


        try {

            po::command_line_parser clParser(argc, argv);
            clParser.options(description).positional(posDescription);
            po::parsed_options parsedOptions = clParser.run();
            po::store(parsedOptions, varMap);

            if (varMap.count("help")) {

                std::cout << "The utility name is MergeCT." << std::endl
                          << "Functionality is "
                          << "merging raw microCT UNSW nc files." << std::endl
                          << "Utility renames dimensions as"
                          << " lvl(z), ltd(y), lng(x)." << std::endl
                          << "Record (merge) dimension is lvl(z)." << std::endl
                          << description;
                return EXIT_SUCCESS;

            } else if (varMap.count("version")) {

                std::cout << "MergeCT version is " << VERSION << std::endl;
                return EXIT_SUCCESS;

            } else if (varMap.count("config")) {

                std::ifstream ifs{varMap["config"].as<std::string>().c_str()};
                if (ifs)
                    po::store(po::parse_config_file(ifs, description), varMap);

            }

            if (!varMap.count("input")) {
                std::cout << "Input files list is not set" << std::endl;
                return EXIT_SUCCESS;
            }

            po::notify(varMap);

        }

        catch (po::error &e) {
            std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
            std::cerr << description << std::endl;
            return EXIT_FAILURE;
        }

        mergeNcFiles(
                varMap["variable"].as<std::string>(),
                varMap["size"].as<float>(),
                varMap["output"].as<std::string>(),
                varMap["input"].as<std::vector<std::string>>());

    }

    catch (std::exception &e) {
        std::cerr << "Unhandled Exception reached the top of main: "
                  << e.what() << ", application will now exit" << std::endl;
        return EXIT_FAILURE;

    }

    return EXIT_SUCCESS;

}
