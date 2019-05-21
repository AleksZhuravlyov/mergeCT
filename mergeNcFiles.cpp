#include <iostream>

#include <netcdf>
#include <boost/progress.hpp>

#include <mergeNcFiles.h>


void mergeNcFiles(const std::string &variable, const float &voxelSize,
                  const std::string &outputFileName,
                  const std::vector<std::string> &inputFileNames) {


    int nIn = inputFileNames.size();

    std::vector<netCDF::NcFile> ncsIn(nIn);
    std::vector<std::vector<netCDF::NcDim>> ncInDims(nIn);
    std::vector<size_t> nLvlsIn(nIn);
    for (int i = 0; i < nIn; i++) {
        ncsIn[i].open(inputFileNames[i], netCDF::NcFile::read);
        ncInDims[i] = ncsIn[i].getVar(variable).getDims();
        nLvlsIn[i] = ncInDims[i][0].getSize();
    }


    netCDF::NcFile nc(outputFileName, netCDF::NcFile::replace);

    std::vector<size_t> dimSizes(3, 0);
    for (int i = 0; i < nIn; i++)
        dimSizes[0] += nLvlsIn[i];
    dimSizes[1] += ncInDims[0][1].getSize();
    dimSizes[2] += ncInDims[0][2].getSize();


    std::vector<std::vector<float>> dimArrays(3);
    for (int i = 0; i < dimArrays.size(); i++)
        for (int j = 0; j < dimSizes[i]; j++)
            dimArrays[i].push_back(voxelSize * j);

    std::string lvlName = "lvl";
    std::string ltdName = "ltd";
    std::string lngName = "lng";

    std::string units = "units";
    std::string lvlUnit = "m";
    std::string ltdUnit = "m";
    std::string lngUnit = "m";
    std::string valUnit = "magnitude";

    std::string sizes = "sizes";
    std::string lvlSize = std::to_string(voxelSize);
    std::string ltdSize = std::to_string(voxelSize);
    std::string lngSize = std::to_string(voxelSize);

    netCDF::NcDim lvlDim = nc.addDim(lvlName, dimSizes[0]);
    netCDF::NcDim ltdDim = nc.addDim(ltdName, dimSizes[1]);
    netCDF::NcDim lngDim = nc.addDim(lngName, dimSizes[2]);

    netCDF::NcVar lvlVar = nc.addVar(lvlName, netCDF::ncFloat, lvlDim);
    netCDF::NcVar ltdVar = nc.addVar(ltdName, netCDF::ncFloat, ltdDim);
    netCDF::NcVar lngVar = nc.addVar(lngName, netCDF::ncFloat, lngDim);

    lvlVar.putAtt(units, lvlUnit);
    ltdVar.putAtt(units, ltdUnit);
    lngVar.putAtt(units, lngUnit);

    lvlVar.putAtt(sizes, lvlSize);
    ltdVar.putAtt(sizes, ltdSize);
    lngVar.putAtt(sizes, lngSize);

    std::vector<netCDF::NcDim> dimVector;
    dimVector.push_back(lvlDim);
    dimVector.push_back(ltdDim);
    dimVector.push_back(lngDim);
    netCDF::NcVar valVar = nc.addVar(variable, netCDF::ncShort, dimVector);

    valVar.putAtt(units, valUnit);

    lvlVar.putVar(dimArrays[0].data());
    ltdVar.putVar(dimArrays[1].data());
    lngVar.putVar(dimArrays[2].data());


    std::vector<size_t> startPIn{0, 0, 0};
    std::vector<size_t> startP{0, 0, 0};
    std::vector<size_t> countP{1, dimSizes[1], dimSizes[2]};
    std::vector<short> valBlock(dimSizes[1] * dimSizes[2], 0);

    std::cout << std::endl;
    std::cout << "CT merging progress:" << std::endl;

    boost::progress_display progressBar(dimSizes[0]);
    for (int i = 0; i < nIn; i++)
        for (startPIn[0] = 0; startPIn[0] < nLvlsIn[i]; startPIn[0]++) {
            ncsIn[i].getVar(variable).getVar(startPIn, countP, valBlock.data());
            valVar.putVar(startP, countP, valBlock.data());
            startP[0]++;
            ++progressBar;
        }

    std::cout << std::endl;
    std::cout << "CT merging is completed." << std::endl;

    std::cout << "Result file is \'" << outputFileName << "\'." << std::endl;
    std::cout << "Result CT dimensions are " <<
              dimSizes[0] << ", " <<
              dimSizes[1] << ", " <<
              dimSizes[2] << ": lvl(z), ltd(y), lng(x)." << std::endl;
    std::cout << "Voxel size is " << voxelSize << " m." << std::endl;

}
