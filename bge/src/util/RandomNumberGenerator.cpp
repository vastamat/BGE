#include "util/RandomNumberGenerator.h"

using HighResClock = std::chrono::high_resolution_clock;

namespace bge
{

RandomNumberGenerator::RandomNumberGenerator()
{
  m_Generator.seed(static_cast<unsigned int>(
      HighResClock::now().time_since_epoch().count()));
}

} // namespace bge