#include "fission_gen.h"

std::vector<double> FissionGenerator::generateFissionValues(std::vector<double> aYields, std::vector<double> zYields, std::vector<int> aNumbers, std::vector<int> zNumbers)
{
	std::vector<double> massNormalized, atomicNormalized;

	for (double p_i : aYields)
	{
		massNormalized.push_back(p_i / 2);
	}

	for (double q_i : zYields)
	{
		atomicNormalized.push_back(q_i / 2);
	}

	IntegerRandomGenerator massGenerator(massNormalized, aNumbers);
	IntegerRandomGenerator atomGenerator(atomicNormalized, zNumbers);

	int a_Product1 = massGenerator.generate();
	int z_Product1 = atomGenerator.generate();

	int freeNeutr = mFreeGenerator->generate();

	int a_Product2 = (URANIUM_A + 1) - a_Product1 - freeNeutr;
	int z_Product2 = URANIUM_Z - z_Product1;

    std::vector<double> fissionProducts;

    double initialMass = (URANIUM_N + 1) * NEUTRON_MASS_UMA + URANIUM_Z * PROTON_MASS_UMA;
    double product1_mass = (a_Product1 - z_Product1) * NEUTRON_MASS_UMA + z_Product1 * PROTON_MASS_UMA;
    double product2_mass = (a_Product2 - z_Product2) * NEUTRON_MASS_UMA + z_Product2 * PROTON_MASS_UMA;
    double massDefect = initialMass - (product1_mass + product2_mass);

    double e = massDefect * CODATA_2018_UMA_EQUIV_MEV - URANIUM_BINDING_ENERGY;

    fissionProducts.push_back(static_cast<double>(a_Product1));
    fissionProducts.push_back(static_cast<double>(z_Product1));

    fissionProducts.push_back(static_cast<double>(a_Product2));
    fissionProducts.push_back(static_cast<double>(z_Product2));

    fissionProducts.push_back(freeNeutr);
    fissionProducts.push_back(e);

    return fissionProducts;
}
