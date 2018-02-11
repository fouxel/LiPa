#ifndef IMODEL_H
#define IMODEL_H

#include <vector>

namespace ai {

class IModel
{
public:
    enum Action {
        ACTION_FORWARD,
        ACTION_LEFT,
        ACTION_RIGHT
    };
    virtual ~IModel() {}

    /**
      @brief Gets action basing on given distances from obstacles.
    */
    virtual Action getAction(const std::vector<int> &distances) = 0;

};

} // namespace ai

#endif // IMODEL_H
