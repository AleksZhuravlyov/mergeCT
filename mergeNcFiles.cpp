#include <iostream>

#include <mergeNcFiles.h>

void mergeNcFiles(const std::string &dimension,
                  const float &voxelSize,
                  const std::string &outputFileName,
                  const std::vector<std::string> &inputFileNames) {

    std::cout << "dimension " << dimension << std::endl;
    std::cout << "voxelSize " << voxelSize << std::endl;
    std::cout << "outputFileName " << outputFileName << std::endl;
    std::cout << "Input fileNames";
    for (std::string fileName : inputFileNames)
        std::cout << " " << fileName;
    std::cout << std::endl;

}
