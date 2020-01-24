#include "random_gen.h"

const auto URANIUM_Z = 92;
const auto URANIUM_A = 235;

class FissionGenerator
{
public:
    FissionGenerator()
    {

    }
    std::vector<int> generateFissionValues(std::vector<double> aYields, std::vector<double> zYields, std::vector<int> aNumbers, std::vector<int> zNumbers);
};
