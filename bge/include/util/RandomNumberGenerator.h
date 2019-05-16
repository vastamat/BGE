#pragma once

#include <chrono>
#include <random>

namespace bge
{

/**
 * class for random number generation
 */
class RandomNumberGenerator
{
public:
  RandomNumberGenerator();

  /**
   * Generate a random number of type T (integer) between [min, max]
   */
  template <typename T> T GenRandInt(T min, T max)
  {
    std::uniform_int_distribution<T> intDis(min, max);
    return intDis(m_Generator);
  }

  /**
   * Generate a random number of type T (real) between [min, max]
   */
  template <typename T> T GenRandReal(T min, T max)
  {
    std::uniform_real_distribution<T> realDis(min, max);
    return realDis(m_Generator);
  }

private:
  std::mt19937 m_Generator; ///< the mersene twister engine for generation
};

} // namespace bge