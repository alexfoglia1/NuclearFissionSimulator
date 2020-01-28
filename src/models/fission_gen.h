#include "random_gen.h"

const uint32_t URANIUM_Z = 92;
const uint32_t URANIUM_A = 235;
const uint32_t URANIUM_N = URANIUM_A - URANIUM_Z;

const double URANIUM_BINDING_ENERGY = URANIUM_A * 9.1;
const double PROTON_MASS_UMA = 1.007276466621;
const double NEUTRON_MASS_UMA = 1.008664916;
const double CODATA_2018_UMA_EQUIV_MEV = 931.49410242;


class FissionGenerator
{

public:

    FissionGenerator()
    {
        std::vector<double> uniformDistribution;
        std::vector<int>    possibleFreeNeutron;

        uniformDistribution.push_back(0.5);
        uniformDistribution.push_back(0.5);

        possibleFreeNeutron.push_back(2);
        possibleFreeNeutron.push_back(3);

        mFreeGenerator = new IntegerRandomGenerator(uniformDistribution, possibleFreeNeutron);
    }

    std::vector<double> generateFissionValues(std::vector<double> aYields, std::vector<double> zYields, std::vector<int> aNumbers, std::vector<int> zNumbers);

private:

    IntegerRandomGenerator* mFreeGenerator;
};
