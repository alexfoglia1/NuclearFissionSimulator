#include <string>
#include <vector>

#define inf -1

const uint16_t N_ELEMENTS = 118;
const uint16_t MAX_ISOTOPES = 46;

struct element
{
    std::string name;
    uint16_t z;
    std::vector<uint16_t> isotopes;
    std::vector<double> half_lives;
};

class PeriodicTable
{

private:
    PeriodicTable();
    void addIsotopesTo(element& x);

public:
    static PeriodicTable INSTANCE;
    std::vector<element> table;

    int16_t fromAtomicMassToIsotopeIndex(uint16_t z, uint16_t a);
    bool isotopeExists(uint16_t z, uint16_t a);

};

