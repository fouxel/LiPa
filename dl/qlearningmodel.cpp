#include "qlearningmodel.h"
#include <algorithm>
#include <functional>

using namespace ai;

QLearningModel::QLearningModel():
m_solver(STATES_COUNT, ACTIONS_COUNT)
{

}

QLearningModel::~QLearningModel()
{

}

float QLearningModel::getDiffReward(distvec& distances) const
{
    int sum = 0;

    std::vector<int> diffs;
    std::transform( m_prevDistances.begin(), m_prevDistances.end(),
                distances.begin(), diffs.begin(), std::minus<int>());
    
    for (const auto& diff : diffs){
	sum += diff;
    }
    
    float reward = -0.2;
    if (sum >= 0) {
	reward = 0.2;
    }
    return reward;
}

bool QLearningModel::isTerminalState(distvec &distances) const
{
    auto iter = std::find(distances.begin(), distances.end(), 10);
    if (iter == distances.end()) {
	return true;
    }
    return false;
}

bool QLearningModel::isOppositeToPrevAction(Action currAction) const {
    if (currAction == ACTION_LEFT && m_prevAction == ACTION_RIGHT) {
	return true;
    } else if (currAction == ACTION_RIGHT && m_prevAction == ACTION_LEFT) {
	return true;
    }
    return false;
}

std::size_t QLearningModel::encodeState(QLearningModel::distvec& distances) const
{
    std::vector<int> normalizedDistances;
    std::transform(distances.begin(), distances.end(), normalizedDistances.begin(),
		   [](int distance) -> int { return (distance/10) + 1; });
    std::size_t state;
    unsigned multiplier = 1;
    for (int d : normalizedDistances) {
	state += multiplier * d;
	multiplier *= 6; //6 is a max value for normalizedDistance
    }
    return state;
}

/*
CoordType decodeState(size_t state) {
    CoordType coords;
    for ( auto & c : coords ) {
        c = state % SQUARE_SIZE;
        state /= SQUARE_SIZE;
    }
    return coords;
}
 */

IModel::Action QLearningModel::getAction(distvec &distances)
{
    if (isInitialState()) {
	// Pick default action;
	return ACTION_FORWARD;
    }
    
    // First, calculate the reward for the previous action.
    if (isTerminalState(distances)) {
	//             void stepUpdateQ(size_t s, size_t a, size_t s1, double rew);
	m_solver.stepUpdateQ(encodeState(m_prevDistances), m_prevAction, encodeState(distances), -100);
	return ACTION_TERMINATE;
    }

    float diffReward = getDiffReward(distances);

    //Pick current action
    AIToolbox::MDP::QGreedyPolicy policy(m_solver.getQFunction());
    auto action = static_cast<Action>(policy.sampleAction(encodeState(distances)));
    
    float oppositeReward = 0;
    if (isOppositeToPrevAction(action)) {
	oppositeReward = -0.8f;
    }
    
    float actionReward = 0;
    switch (action) {
      case ACTION_FORWARD:
	actionReward = 0.2;
	break;
      case ACTION_LEFT:
      case ACTION_RIGHT:
	actionReward = -0.1;
	break;
      default:
	abort();
    }

    float totalReward = actionReward + diffReward + oppositeReward;
    //NOTE pass here m_prevAction or current action?
    m_solver.stepUpdateQ(encodeState(m_prevDistances), m_prevAction, encodeState(distances), totalReward);
    
    return ACTION_LEFT;
}

bool QLearningModel::isInitialState() const {
    return !m_prevDistances.size();
}

// https://stackoverflow.com/questions/13728430/element-wise-multiplication-of-two-vectors-in-c
// Określanie reworda. Algorytm działa następująco
/*
function [reward, terminal] = getReward(action, prev_action, sensor1, sensor2, krasj)

    % Reward function for Q-learning with table

    r1 = 0;
    r2 = 0;
    r3 = 0;
    reward = 0;

    status_krasj = ~isempty(find(krasj(2),1));
    terminal = false;
    a = action;

    switch(a)

        case 1, r1 = 0.2;
        case 2, r1 = -0.1;
        case 3, r1 = -0.1;

    end

    if (sum(sensor2 - sensor1) >= 0)

        r2 = 0.2;
    else

        r2 = -0.2;
    end

   if(prev_action == 2 && a == 3)

       r3 =  -0.8;
   elseif(prev_action == 3 && a == 2)

       r3 = -0.8;
   end

   reward = r1 + r2 + r3;

   if(status_krasj)

       terminal = true;
       reward = -100;
   end                       
*/
