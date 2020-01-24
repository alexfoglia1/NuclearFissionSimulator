#include <stdio.h>
#include <string.h>
#include <sstream>
#include "python_generator.h"

const char* code_template =
"import matplotlib.pyplot as plt\n"
"plt.rcParams.update({'font.size': 7})\n"
"truthsA=[%s]\n"
"truthsZ=[%s]\n"
"countsA=[%s]\n"
"countsZ=[%s]\n"
"valuesA=[%s]\n"
"valuesZ=[%s]\n"
"free=[%s]\n"
"energies=[%s]\n"
"fig,a = plt.subplots(2, 2)\n"
"fig.canvas.set_window_title('Experiment Report')\n"
"plt.suptitle('Results out of %d nuclear fission simulations')\n"
"fig.set_size_inches(12.5, 7)\n"
"a[0][0].plot(valuesA, countsA, '.-', label='observed')\n"
"a[0][0].plot(valuesA, truthsA, '.-', label='true values')\n"
"a[0][0].set_xlabel('Mass Numbers (A)')\n"
"a[0][0].set_ylabel('Probability')\n"
"a[0][0].legend()\n"
"a[0][1].plot(valuesZ, countsZ, '.-', label='observed')\n"
"a[0][1].plot(valuesZ, truthsZ, '.-', label='true values')\n"
"a[0][1].set_xlabel('Atomic Numbers (Z)')\n"
"a[0][1].set_ylabel('Probability')\n"
"a[0][1].legend()\n"
"a[1][0].plot(free, '.')\n"
"a[1][0].set_ylim([1, 4])\n"
"a[1][0].set_ylabel('Free neutrons')\n"
"a[1][0].set_xlabel('Simulation number')\n"
"a[1][1].plot(energies)\n"
"a[1][1].set_ylabel('Cumulative average energy produced (MeV)')\n"
"a[1][1].set_xlabel('Simulation number')\n"
"plt.show()";

void PythonGenerator::writePythonScript(std::vector<double> trueDistA, std::vector<double> trueDistZ, std::vector<double> relCountA,
                                        std::vector<double> relCountZ, std::vector<double> zNumbers, std::vector<double> aNumbers,
                                        std::vector<double> freeNeutrons, std::vector<double> energies, int nSim)
{
    std::string trueDistA_vec;
    std::string trueDistZ_vec;
    std::string relCountA_vec;
    std::string relCountZ_vec;
    std::string aNumbers_vec;
    std::string zNumbers_vec;
    std::string free_vec;
    std::string energies_vec;

    stdVectorToString(trueDistA, trueDistA_vec);
    stdVectorToString(trueDistZ, trueDistZ_vec);
    stdVectorToString(relCountA, relCountA_vec);
    stdVectorToString(relCountZ, relCountZ_vec);
    stdVectorToString(aNumbers, aNumbers_vec);
    stdVectorToString(zNumbers, zNumbers_vec);
    stdVectorToString(freeNeutrons, free_vec);
    stdVectorToString(energies, energies_vec);

    FILE* out = fopen(mFilename, "w");
    fprintf(out, code_template, trueDistA_vec.c_str(), trueDistZ_vec.c_str(), relCountA_vec.c_str(), relCountZ_vec.c_str(), aNumbers_vec.c_str(), zNumbers_vec.c_str(), free_vec.c_str(), energies_vec.c_str(), nSim);
    fclose(out);
}

void PythonGenerator::stdVectorToString(std::vector<double> toConvert, std::string& result)
{
    int toConvertSize = toConvert.size();
    for(int i = 0; i < toConvertSize - 1; i++)
    {
        std::ostringstream strs;
        strs << toConvert.at(i);
        std::string num2str = strs.str();
        result += num2str + ",";
    }

    std::ostringstream strs;
    strs << toConvert.at(toConvertSize - 1);
    std::string num2str = strs.str();
    result += num2str;
}
