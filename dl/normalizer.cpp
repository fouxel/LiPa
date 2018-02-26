#include "normalizer.h"

#include <algorithm>
#include <functional>

Normalizer::Normalizer()
{

}

distvec Normalizer::normalize(cdistvec &distances) {
  distvec normalizedDistances;
  normalizedDistances.reserve(distances.size());
  std::transform(distances.begin(), distances.end(),
                 normalizedDistances.begin(),
                 [](int distance) -> int { return (distance / 10) + 1; });
  return normalizedDistances;
}