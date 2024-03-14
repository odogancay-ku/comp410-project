//
// Created by ofaru on 12.03.2024.
//

#include "utility.h"

#include <iostream>
#include <fstream>
#include <sstream>

std::string readShaderSource(const std::string &filename) {

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();

    return ss.str();
}
