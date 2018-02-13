#ifndef QLEARNINGMODEL_H
#define QLEARNINGMODEL_H

#include "imodel.h"
#include <AIToolbox/MDP/Algorithms/QLearning.hpp>
#include <AIToolbox/MDP/Policies/QGreedyPolicy.hpp>

namespace ai {

/**
 * Distances:
 * 10 - Critical distance which
 * 20
 * 30
 * 40
 * 50
 */
class QLearningModel : public IModel
{
public:
    using distvec = const std::vector<int>;
    QLearningModel();
    ~QLearningModel();

    Action getAction(distvec &distances) override;

private:
    bool isInitialState() const;
    bool isOppositeToPrevAction(Action currAction) const;
    bool isTerminalState(distvec &distances) const;
    float getDiffReward(distvec &distances) const;
    size_t encodeState(distvec &distances) const;

    Action m_prevAction;
    distvec m_prevDistances;
    AIToolbox::MDP::QLearning m_solver;
    constexpr static int STATES_COUNT = 100;
    constexpr static int ACTIONS_COUNT = 4;
  };

}

#endif // QLEARNINGMODEL_H
