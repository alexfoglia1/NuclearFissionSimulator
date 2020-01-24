#include "yields.h"
#include "periodictable.h"
#include "fission_gen.h"

#include <iostream>
#include <unistd.h>
#include <chrono>

#include "utils/python_generator.h"

std::vector<double> convertIntToDoubleVector(std::vector<int> intVector)
{
    std::vector<double> doubleVector;
    for(int x_i : intVector)
    {
        doubleVector.push_back(static_cast<double>(x_i));
    }
    return doubleVector;
}

int main()
{
    const int nSim = 100000;

    uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();

    std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
    mRng.seed(ss);

    std::vector<double> distMass(std::begin(MASS_YIELD), std::end(MASS_YIELD));
    std::vector<double> distAtom(std::begin(ATOMIC_YIELD), std::end(ATOMIC_YIELD));
    std::vector<int>   massNumbers;
    std::vector<int>   atomNumbers;

    for(int i = 0; i < MASS_YIELD_LENGTH; i++)
    {
        massNumbers.push_back(i + MIN_MASS_NUMBER);
    }
    for(int i = 0; i < ATOMIC_YIELD_LENGTH; i++)
    {
        atomNumbers.push_back(i + MIN_ATOMIC_NUMBER);
    }

    int countsPerZ[ATOMIC_YIELD_LENGTH]{};
    std::vector<double> relCountsPerZ;

    int countsPerA[MASS_YIELD_LENGTH]{};
    std::vector<double> relCountsPerA;

    std::vector<double> freeNeutrons;

    std::vector<double> energies;
    std::vector<double> actualEnergies;
    std::cout << "Start " << nSim << " atomic fission simulations . . ." << std::endl;
    for(int i = 0; i < nSim;)
    {
        std::vector<int> fissionProducts = generateFissionValues(distMass, distAtom, massNumbers, atomNumbers);

        int a1 = fissionProducts.at(0);
        countsPerA[a1 - MIN_MASS_NUMBER] += 1;

        int z1 = fissionProducts.at(1);
        countsPerZ[z1 - MIN_ATOMIC_NUMBER] += 1;

        int a2 = fissionProducts.at(2);
        countsPerA[a2 - MIN_MASS_NUMBER] += 1;

        int z2 = fissionProducts.at(3);
        countsPerZ[z2 - MIN_ATOMIC_NUMBER] += 1;

        double free = fissionProducts.at(4);
        freeNeutrons.push_back(free);

        double actualEnergy = fissionProducts.at(5);
        actualEnergies.push_back(actualEnergy);

        i++;

        if(i%(nSim/100) == 0)
        {
            energies.push_back(std::accumulate(std::begin(actualEnergies), std::end(actualEnergies), 0.0)/static_cast<double>(nSim/100));
        }

        double percentage = (100.0 * i)/nSim;
        printf("Progress: %d %%\r", static_cast<int>(percentage));
        fflush(stdout);
    }

    for(int i = 0; i < MASS_YIELD_LENGTH; i++)
    {
        relCountsPerA.push_back(countsPerA[i]/static_cast<double>(nSim));
    }
    for(int i = 0; i < ATOMIC_YIELD_LENGTH; i++)
    {
        relCountsPerZ.push_back(countsPerZ[i]/static_cast<double>(nSim));
    }

    std::cout << std::endl << std::endl << "Writing script . . ." << std::endl;
    PythonGenerator pyGen("plot.py");
    pyGen.writePythonScript(distMass, distAtom, relCountsPerA,
                            relCountsPerZ, convertIntToDoubleVector(atomNumbers), convertIntToDoubleVector(massNumbers),
                            freeNeutrons, energies, nSim);
    std::cout << std::endl << "Done" << std::endl;

    system("python plot.py");

    return 0;
}
