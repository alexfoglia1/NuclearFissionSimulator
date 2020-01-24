#include "fission_gen.h"

std::vector<int> generateFissionValues(std::vector<double> aYields, std::vector<double> zYields, std::vector<int> aNumbers, std::vector<int> zNumbers)
{
    std::vector<double> massNormalized, atomicNormalized;
    std::vector<double> unif;
    std::vector<int> nNumbers;

    for(double p_i : aYields)
    {
        massNormalized.push_back(p_i/2);
    }

    for(double q_i : zYields)
    {
        atomicNormalized.push_back(q_i/2);
    }

    unif.push_back(0.5);
    unif.push_back(0.5);
    nNumbers.push_back(2);
    nNumbers.push_back(3);

    IntegerRandomGenerator massGenerator(massNormalized, aNumbers);
    IntegerRandomGenerator atomGenerator(atomicNormalized, zNumbers);
    IntegerRandomGenerator freeGenerator(unif, nNumbers);

    int a_Product1 = massGenerator.generate();
    int z_Product1 = atomGenerator.generate();

    int a_Product2 = massGenerator.generate();
    int z_Product2 = atomGenerator.generate();

    int freeNeutr = freeGenerator.generate();

    std::vector<int> fissionProducts;

    fissionProducts.push_back(a_Product1);
    fissionProducts.push_back(z_Product1);

    fissionProducts.push_back(a_Product2);
    fissionProducts.push_back(z_Product2);

    fissionProducts.push_back(freeNeutr);


    int massDefect = 235 - (a_Product1 + a_Product2);
    double e = massDefect * 931.49469725613;

    fissionProducts.push_back(e);

    return fissionProducts;
}
