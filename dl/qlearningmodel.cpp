#include "qlearningmodel.h"
#include <algorithm>
#include <functional>
#include <AIToolbox/MDP/Policies/QGreedyPolicy.hpp>
#include <AIToolbox/MDP/Algorithms/QLearning.hpp>

#include <iostream>
#include <ctime>

using namespace ai;

QLearningModel::QLearningModel()
{
    m_solver.reset(new AIToolbox::MDP::QLearning(STATES_COUNT, ACTIONS_COUNT));
    std::srand(std::time(nullptr));
}

QLearningModel::~QLearningModel()
{

}

float QLearningModel::getDiffReward(distvec& distances) const
{
    int sum = 0;

    std::vector<int> diffs(5);
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
    /*
      std::transform(distances.begin(), distances.end(), normalizedDistances.begin(),
		   [](int distance) -> int { return (distance/10) + 1; });
     */
  
    auto iter = std::find_if(distances.begin(), distances.end(),
		[](int dist) -> bool { return dist <= 10; }
    );
    if (iter == distances.end()) {
	return false;
    }
    return true;
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
    std::vector<int> normalizedDistances(5);
    std::transform(distances.begin(), distances.end(), normalizedDistances.begin(),
		   [](int distance) -> int { return (distance/10) + 1; });
    std::size_t state = 0;
    unsigned multiplier = 1;
    for (int d : normalizedDistances) {
	state += multiplier * d;
	multiplier *= 6; //6 is a max value for normalizedDistance
    }
    std::cout << "state: " << state <<  std::endl;
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
}*/
 
IModel::Action QLearningModel::getAction(distvec &distances)
{
    static int t = 0;
    t++;
    if (isInitialState()) { 
	std::cout <<"InitialState!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
	// Pick default action;
	m_prevDistances = distances;
	m_prevAction = static_cast<Action>(t % 3);
        return m_prevAction;
    }
    for (auto c : distances) {
	std::cout << "dist" << c << std::endl;
    }
    // First, calculate the reward for the previous action.
    if (isTerminalState(distances)) {
	std::cout << "Terminal state" << std::endl;
	//             void stepUpdateQ(size_t s, size_t a, size_t s1, double rew);
	m_solver->stepUpdateQ(encodeState(m_prevDistances), m_prevAction, encodeState(distances), -10000);
	return ACTION_TERMINATE;
    }

    float diffReward = getDiffReward(distances);

    //Pick current action
    AIToolbox::MDP::QGreedyPolicy policy(m_solver->getQFunction());
    Action action = ACTION_FORWARD;
    //if (t++ > 30) {
      action = static_cast<Action>(policy.sampleAction(encodeState(distances)));
    //} else {
    //  action = static_cast<Action>(std::rand() % 3);
    //}
    float oppositeReward = 0;
    if (isOppositeToPrevAction(action)) {
	oppositeReward = -0.8f;
    }
    
    float actionReward = 0;
    std::cout << "Action: " << action << std::endl;
    switch (action) {
      case ACTION_FORWARD:
	actionReward = 0.2;
	break;
      case ACTION_LEFT:
      case ACTION_RIGHT:
	actionReward = -0.1;
	break;
      default:
	std::cout << "Bad action" << std::endl;
	break;
	//abort();
    }

    float totalReward = actionReward + diffReward + oppositeReward;
    //NOTE pass here m_prevAction or current action?
    m_solver->stepUpdateQ(encodeState(m_prevDistances), m_prevAction, encodeState(distances), totalReward);
    
    m_prevDistances.clear();
    for (auto e : distances) {
      m_prevDistances.push_back(e);
    }
    m_prevAction = action;
    return action;
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
