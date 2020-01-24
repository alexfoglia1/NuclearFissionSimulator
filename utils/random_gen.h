#include <vector>
#include <stdexcept>
#include <random>
#include <utility>
#include <stdio.h>

static std::mt19937_64 mRng;

class IntegerRandomGenerator
{
public:
    IntegerRandomGenerator(std::vector<double> distribution, std::vector<int> values)
    {
        size_t size_dist = distribution.size();
        size_t size_val  = values.size();
        if( size_dist != size_val )
        {
            throw std::invalid_argument("distribution and values vectors shall have the same length");
        }
        else
        {
            for( double p_i : distribution )
            {
                mDist.push_back(p_i);
            }
            for( int x_i : values )
            {
                mValues.push_back(x_i);
            }
        }
    }

    void print();
    int generate();

private:
    std::vector<double> mDist;
    std::vector<int>   mValues;
};
