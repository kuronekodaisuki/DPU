#include <assert.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cassert>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <string>
#include <vector>


class DPU
{
public :
    /// @brief Load model
    /// @param model_filename
    /// @return if succes return true
    bool Load(const char* model_filename);

    /// @brief read images from images_filepath
    /// @param images_filepath 
    void Run(const char* images_filepath);

protected :
    vart::Runner* _runner;
};