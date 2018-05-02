#include "normalizer.h"

#include <algorithm>
#include <functional>

Normalizer::Normalizer()
{

}

distvec Normalizer::normalize(cdistvec &distances) {
  distvec normalizedDistances(distances.size());
  std::transform(distances.begin(), distances.end(),
                 normalizedDistances.begin(),
                 [](int distance) -> int { return (distance / 10); });
  return normalizedDistances;
}

size_t Normalizer::maxValue() {
  return 5;
}