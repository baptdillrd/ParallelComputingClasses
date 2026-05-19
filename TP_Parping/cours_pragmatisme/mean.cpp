#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <numeric>

int main(int argc, char **argv) {
  using data_type = double;
  std::ifstream fd(argv[1]);
  std::string buffer;
  std::vector<data_type> data;

  // data extraction part
  std::cout << "> reading " << argv[1] << std::endl;

  while(fd) {
    std::getline(fd, buffer);
    if(fd)
      data.push_back(std::stod(buffer));
  }

  // computation part
  std::cout << "> processing data " << std::endl;
  data_type mean = std::accumulate(data.begin(), data.end(), data_type(0)) / data.size();
  data_type absdiff = std::accumulate(data.begin(), data.end(), data_type(0),
                                      [mean](data_type acc, data_type elt) {
                                        return acc + std::abs(elt - mean);
                                      });
  data_type mean_absdiff = absdiff / data.size();

  std::cout << "> mean of absolute diff to the mean" << std::endl << mean_absdiff << std::endl;
  return 0;
}
