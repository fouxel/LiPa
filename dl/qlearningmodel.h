#ifndef QLEARNINGMODEL_H
#define QLEARNINGMODEL_H

#include "imodel.h"

#include <memory>

namespace AIToolbox::MDP {
  class QLearning;
}

namespace ai {

class QLearningModel : public IModel
{
public:
    QLearningModel();
    ~QLearningModel();

    Action getAction(cdistvec &distances) override;
private:
    bool isInitialState() const;
    bool isOppositeToPrevAction(Action currAction) const;
    bool isTerminalState(cdistvec &distances) const;
    float getDiffReward(cdistvec &distances) const;
    size_t encodeState(cdistvec &distances) const;

    Action m_prevAction;
    distvec m_prevDistances;
    constexpr static int STATES_COUNT = 10000;
    constexpr static int ACTIONS_COUNT = 3;
    std::unique_ptr<AIToolbox::MDP::QLearning> m_solver;
  };

}

#endif // QLEARNINGMODEL_H
