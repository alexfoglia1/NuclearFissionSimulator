#include <vector>
#include <string>

class PythonGenerator
{
public:
    PythonGenerator(const char* filename)
    {
        mFilename = filename;
    }

    void writePythonScript(std::vector<double> trueDistA, std::vector<double> trueDistZ, std::vector<double> relCountA,
                           std::vector<double> relCountZ, std::vector<double> zNumbers, std::vector<double> aNumbers,
                           std::vector<double> freeNeutrons, std::vector<double> energies, int nSim, bool saveFig);



private:
    const char* mFilename;
    void stdVectorToString(std::vector<double> toConvert, std::string& result);
    std::string doubleToString(double toConvert);
};
