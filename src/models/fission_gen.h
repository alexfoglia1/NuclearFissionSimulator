#include "random_gen.h"

const uint32_t URANIUM_Z = 92;
const uint32_t URANIUM_A = 235;
const uint32_t URANIUM_N = URANIUM_A - URANIUM_Z;

const double PROTON_MASS_UMA = 1.007276466621;
const double NEUTRON_MASS_UMA = 1.008664916;
const double CODATA_2018_UMA_EQUIV_MEV = 931.49410242;


class FissionGenerator
{
public:
    FissionGenerator()
    {

    }
    std::vector<int> generateFissionValues(std::vector<double> aYields, std::vector<double> zYields, std::vector<int> aNumbers, std::vector<int> zNumbers);
};
