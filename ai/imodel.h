#ifndef IMODEL_H
#define IMODEL_H

#include <vector>
#include "types.h"

namespace ai {

class IModel
{
public:
    enum Action {
        ACTION_FORWARD = 0, // Move forward.
        ACTION_LEFT, // Turn left.
        ACTION_RIGHT, // Turn right.
        ACTION_TERMINATE // Agent reached critical state. It is usually not possible
                         // to move out from this state. Agent's shall be restored
                         // to its initial state in order to continue.
    };
    virtual ~IModel() {}

    /**
     * @brief Gets action basing on given distances from obstacles and previous experience.
     *
     */
    virtual Action getAction(cdistvec &distances) = 0;

};

} // namespace ai

#endif // IMODEL_H
