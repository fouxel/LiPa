#ifndef QLEARNINGMODEL_H
#define QLEARNINGMODEL_H

#include "imodel.h"

namespace ai {

class QLearningModel : public IModel
{
public:
    QLearningModel();
    ~QLearningModel();

    Action getAction(const std::vector<int> &distances) override;

};

}

#endif // QLEARNINGMODEL_H
