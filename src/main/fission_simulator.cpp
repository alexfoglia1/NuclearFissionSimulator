#include "yields.h"
#include "periodictable.h"
#include "fission_gen.h"

#include <iostream>
#include <chrono>

#include "python_generator.h"

const uint32_t DEFAULT_NUMBER_OF_SIMULATIONS = 100000;
const uint32_t MAXIMUM_NUMBER_OF_SIMULATIONS = std::numeric_limits<uint32_t>::max()/2;

const bool DEFAULT_SAVEFIG_OPTION = false;

double mean(std::vector<double> v)
{
	double sum = 0;
	uint32_t vlen = static_cast<uint32_t>(v.size());

	for (uint32_t i = 0; i < vlen; i++)
	{
        sum += v.at(i);
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
    bool saveFig = DEFAULT_SAVEFIG_OPTION;

    bool parsingErrorsN = false;
    bool parsingErrorsSF = false;
    if (argc != 3)
	{
        std::cout << "Usage: " << std::endl << argv[0] << "\tn [number of simulations]\t"
                                                       << "s/p [save/plot report]"      << std::endl << std::endl;
        parsingErrorsN = true;
        parsingErrorsSF = true;
	}
	else
	{
        int parsed_nSim = atoi(argv[1]);
        std::string parsed_saveFig = argv[2];

        if (parsed_nSim <= 0)
        {
            std::cout << "Unvalid number of simulations " << argv[1] << ": shall be > 1" << std::endl;
            parsingErrorsN = true;
        }
        else if(parsed_nSim > MAXIMUM_NUMBER_OF_SIMULATIONS)
        {
            std::cout << "Unvalid number of simulations " << argv[1] << ": shall be <= " << MAXIMUM_NUMBER_OF_SIMULATIONS << std::endl;
            parsingErrorsN = true;
        }
        else
        {
            nSim = static_cast<uint32_t>(parsed_nSim);
        }

        if(parsed_saveFig.compare("s") != 0 && parsed_saveFig.compare("p") != 0)
        {
            std::cout << "Unvalid save or plot report option " << argv[2] << ": shall be 's' or 'p'" << std::endl;
            parsingErrorsSF = true;
        }
        else
        {
            saveFig = parsed_saveFig.compare("s") == 0 ? true : false;
		}
	}

    if(parsingErrorsN)
    {
        std::cout << "Using default number of simulations value: " << nSim << std::endl << std::endl;
    }

    if(parsingErrorsSF)
    {
        std::cout << "Using default save figure option value: p [plot]" << std::endl << std::endl;
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
    for(uint32_t i = 0; i < nSim;)
    {
        std::vector<double> fissionProducts = fGen.generateFissionValues(distMass, distAtom, massNumbers, atomNumbers);

        int a1 = static_cast<int>(fissionProducts.at(0));
        countsPerA[a1 - MIN_MASS_NUMBER] += 1;

        int z1 = static_cast<int>(fissionProducts.at(1));
        countsPerZ[z1 - MIN_ATOMIC_NUMBER] += 1;

        int a2 = static_cast<int>(fissionProducts.at(2));
        countsPerA[a2 - MIN_MASS_NUMBER] += 1;

        int z2 = static_cast<int>(fissionProducts.at(3));
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

    std::cout << std::endl << "Writing script . . ." << std::endl;
    PythonGenerator pyGen("plot.py");
    pyGen.writePythonScript(distMass, distAtom, relCountsPerA,
                            relCountsPerZ, convertIntToDoubleVector(atomNumbers), convertIntToDoubleVector(massNumbers),
                            freeNeutrons, energies, nSim, saveFig);
    std::cout<< "Done" << std::endl << std::endl;
    if(saveFig)
    {
        std::cout << "Saving report . . ." << std::endl;
    }
    else
    {
        std::cout << "Plotting report . . ." << std::endl;
    }
    system("python plot.py");

    return 0;
}
