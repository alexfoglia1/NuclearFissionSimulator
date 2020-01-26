#include "random_gen.h"
#include <iostream>

void IntegerRandomGenerator::print()
{
    int size = mDist.size();
    for(int i = 0; i < size; i++)
    {
        std::cout << "P(X <= " << mValues.at(i) << ") = " << mDist.at(i) << std::endl;
    }
}

int IntegerRandomGenerator::generate()
{
    std::uniform_real_distribution<double> unif(0,1);
    double random = unif(mRng);

    uint32_t distSize = static_cast<uint32_t>(mDist.size());
    for(uint32_t i = 0; i < distSize; i++)
    {
        if(random < mDist.at(i))
        {
            return mValues.at(i);
        }
        else
        {
            random -= mDist.at(i);
        }
    }

    return -1;
}
