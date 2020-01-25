#include "yields.h"
#include "periodictable.h"
#include "fission_gen.h"

#include <iostream>
#include <chrono>

#include "python_generator.h"

const uint32_t DEFAULT_NUMBER_OF_SIMULATIONS = 100000;
const uint32_t MAXIMUM_NUMBER_OF_SIMULATIONS = std::numeric_limits<uint32_t>::max()/2;

double mean(std::vector<double> v)
{
	double sum = 0;
	uint32_t vlen = static_cast<uint32_t>(v.size());

	for (uint32_t i = 0; i < vlen; i++)
	{
		sum += v.at(static_cast<int>(i));
	}

	return sum / vlen;
}

std::vector<double> convertIntToDoubleVector(std::vector<int> intVector)
{
    std::vector<double> doubleVector;
    for(int x_i : intVector)
    {
        doubleVector.push_back(static_cast<double>(x_i));
    }
    return doubleVector;
}

int main(int argc, char** argv)
{
    uint32_t nSim = DEFAULT_NUMBER_OF_SIMULATIONS;

    bool parsingErrors = false;
	if (argc == 1)
	{
		std::cout << "Usage: " << argv[0] << " [number of simulations]" << std::endl;
        parsingErrors = true;
	}
	else
	{
        int parsed_nSim = atoi(argv[1]);
        if (parsed_nSim <= 0)
        {
            std::cout << "Unvalid number of simulations " << argv[1] << ": shall be > 1" << std::endl;
            parsingErrors = true;
        }
        else if(parsed_nSim > MAXIMUM_NUMBER_OF_SIMULATIONS)
        {
            std::cout << "Unvalid number of simulations " << argv[1] << ": shall be <= " << MAXIMUM_NUMBER_OF_SIMULATIONS << std::endl;
            parsingErrors = true;
        }
        else
        {
            nSim = static_cast<uint32_t>(parsed_nSim);
		}
	}

    if(parsingErrors)
    {
        std::cout << "Using default value: " << nSim << std::endl << std::endl;
    }

    FissionGenerator fGen;

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
    uint32_t nSim_100 = nSim / 100;
	bool canAverage = nSim_100 > 0;

    std::cout << "Start " << nSim << " atomic fission simulations . . ." << std::endl;
    uint64_t t0 =
            std::chrono::duration_cast<std::chrono::microseconds>
            (std::chrono::system_clock::now().time_since_epoch()).count();
    for(int i = 0; i < nSim;)
    {
        std::vector<int> fissionProducts = fGen.generateFissionValues(distMass, distAtom, massNumbers, atomNumbers);

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

		if (canAverage)
		{
			if (i % nSim_100 == 0)
			{
				energies.push_back(mean(actualEnergies));
			}
		}
		else
		{
			energies.push_back(actualEnergy);
		}

        double percentage = (100.0 * i)/nSim;

        printf("Progress: %.2f %%\r", percentage);
        fflush(stdout);
    }
    uint64_t tf =
            std::chrono::duration_cast<std::chrono::microseconds>
            (std::chrono::system_clock::now().time_since_epoch()).count();

    std::cout << std::endl << "Elapsed: " << ((tf-t0)/pow(10,6)) << " s" << std::endl;

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
