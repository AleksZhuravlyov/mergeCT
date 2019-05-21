#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include <mergeNcFiles.h>

namespace {
    const size_t SUCCESS = 0;
    const size_t ERROR_IN_COMMAND_LINE = 1;
    const size_t ERROR_UNHANDLED_EXCEPTION = 2;
}

namespace po = boost::program_options;

const std::string VERSION = "1.0";


int main(int argc, char **argv) {

    try {

        po::options_description description("Command line options");
        description.add_options()

                ("help,h", "Display help message")

                ("version,v", "Display version")

                ("dimension,dim",
                 po::value<std::string>()->default_value("tomo_zdim"),
                 "Record dimension name")

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


        po::positional_options_description positionalDescription;
        positionalDescription.add("input", -1);
        po::variables_map variables;


        try {

            po::store(po::command_line_parser(argc, argv).options(
                    description).positional(positionalDescription).run(),
                      variables);

            if (variables.count("help")) {
                std::cout << "Utility name is MergeCT." << std::endl
                          << "Functionality is "
                          << "merging raw microCT UNSW nc files." << std::endl
                          << description;
                return SUCCESS;
            } else if (variables.count("version")) {
                std::cout << "MergeCT version is " << VERSION << std::endl;
                return SUCCESS;
            } else if (!variables.count("input")) {
                std::cout << "Input files list is not set" << std::endl;
                return SUCCESS;
            }

            po::notify(variables);

        }

        catch (po::error &e) {
            std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
            std::cerr << description << std::endl;
            return ERROR_IN_COMMAND_LINE;
        }

        mergeNcFiles(variables["dimension"].as<std::string>(),
                     variables["variable"].as<std::string>(),
                     variables["size"].as<float>(),
                     variables["output"].as<std::string>(),
                     variables["input"].as<std::vector<std::string>>());

    }

    catch (std::exception &e) {
        std::cerr << "Unhandled Exception reached the top of main: "
                  << e.what() << ", application will now exit" << std::endl;
        return ERROR_UNHANDLED_EXCEPTION;

    }

    return SUCCESS;

}
