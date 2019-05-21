#ifndef MERGECT_H
#define MERGECT_H

#include <string>
#include <vector>


void mergeNcFiles(const std::string &dimension,
                  const std::string &variable,
                  const float &voxelSize,
                  const std::string &outputFileName,
                  const std::vector<std::string> &inputFileNames);

#endif //MERGECT_H
