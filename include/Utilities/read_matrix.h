#ifndef READ_MATRIX_H
#define READ_MATRIX_H

#include <Eigen/Dense>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>


namespace utilities {
  template<typename M>
  M read_matrix (const std::string & path) {
    std::ifstream indata;
    indata.open(path);
    std::string header, line;
    std::vector<double> values;
    uint rows = 0;
    std::getline(indata, header);
    std::cout << "Config header: " << header << std::endl;
    while (std::getline(indata, line)) {
      std::stringstream lineStream(line);
      std::string cell;
      while (std::getline(lineStream, cell, ',')) {
	values.push_back(std::stod(cell));
      }
      ++rows;
    }
    indata.close();
    return Eigen::Map<const Eigen::Matrix<typename M::Scalar, M::RowsAtCompileTime, M::ColsAtCompileTime, Eigen::RowMajor>>(values.data(), rows, values.size()/rows);
  }
} // namespace utilities

#endif // READ_MATRIX_H
