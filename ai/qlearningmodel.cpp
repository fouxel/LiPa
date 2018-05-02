#include "qlearningmodel.h"
#include <AIToolbox/MDP/Algorithms/QLearning.hpp>
#include <AIToolbox/MDP/Policies/QGreedyPolicy.hpp>

#include <algorithm>
#include <functional>
#include <easylogging++.h>
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

#include "statecoder.h"

using namespace ai;

QLearningModel::QLearningModel(INormalizer &normalizer):
m_normalizer(normalizer),
m_count(0) {
  m_solver.reset(new AIToolbox::MDP::QLearning(STATES_COUNT, ACTIONS_COUNT, 1.0, 0.5));
  std::srand(std::time(nullptr));
}

QLearningModel::~QLearningModel() {}

float QLearningModel::getDiffReward(cdistvec &distances) const {
  int sum = 0;

  std::vector<int> diffs(distances.size());
  assert(distances.size() == m_prevDistances.size());
  std::transform(m_prevDistances.begin(), m_prevDistances.end(),
                 distances.begin(), diffs.begin(), std::minus<int>());

  std::cout << "diff size: " << diffs.size() << std::endl;
  for (const auto &diff : diffs) {
    std::cout << "diff: " << diff << std::endl;
    sum += diff;
  }

  float reward = -0.2;
  if (sum >= 0) {
    reward = 0.2;
  }
  return reward;
}

bool QLearningModel::isTerminalState(cdistvec &distances) const {
  // TODO: Check why it fails if dist < 10;
  auto iter = std::find_if(distances.begin(), distances.end(),
                           [](int dist) -> bool { return dist <= 60; });
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

std::size_t
QLearningModel::encodeState(cdistvec &distances) const {
  std::vector<int> normalizedDistances = m_normalizer.normalize(distances); 
  return StateCoder::encode(normalizedDistances, m_normalizer.maxValue());
}

IModel::Action QLearningModel::getAction(cdistvec &distances) {
  if (isInitialState()) {
    // Pick default action;
    m_prevDistances = distances;
    m_prevAction = ACTION_FORWARD;
    return m_prevAction;
  }

  if (isTerminalState(distances)) {
    std::cout << "Terminal state" << std::endl;
    m_solver->stepUpdateQ(encodeState(m_prevDistances), m_prevAction,
                          encodeState(distances), -100000);
    m_prevDistances.clear();
    m_count++;
    return ACTION_TERMINATE;
  }

  // Pick current action
  std::ofstream file;
  file.open("logi", std::ios_base::app);
  file << "############# QFUNCTION" << std::endl;
  file << m_count << std::endl;
  file << m_solver->getQFunction() << std::endl;
  file.close();

  
  AIToolbox::MDP::QGreedyPolicy policy(m_solver->getQFunction());
  Action action;
  if(encodeState(distances) == 1330) {
    action = ACTION_FORWARD;
    std::cout << "#### FORWARD ACTION" << std::endl;
  } else {
    if (m_count % 5 == 0) {
      std::cout << "#### POLICY ACTION" << std::endl;
      action = static_cast<Action>(policy.sampleAction(encodeState(distances)));
    } else {
      std::cout << "#### RANDOM ACTION" << std::endl;
      int r = rand() % 3;
      if (r == 0) {
        action = ACTION_LEFT;
      } else if (r == 1) {
        action = ACTION_RIGHT;
      } else {
        action = ACTION_FORWARD;
      }
    }
  }
  std::cout << "Action: " << action << std::endl;
  float diffReward = getDiffReward(distances);
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
    std::cout << "Bad action" << std::endl;
    break;
  }

  float totalReward = actionReward + diffReward + oppositeReward;
  std::cout <<"Action Reward: " << actionReward << std::endl;
  std::cout <<"Diff Reward: " << diffReward << std::endl;
  std::cout <<"Opposite Reward: " << oppositeReward << std::endl;
  std::cout <<"Total Reward: " << totalReward << std::endl;
  std::cout <<"State: " << encodeState(distances) << std::endl;
  m_solver->stepUpdateQ(encodeState(m_prevDistances), m_prevAction,
                        encodeState(distances), totalReward);

  m_prevDistances = distances;
  m_prevAction = action;
  return action;
}

bool QLearningModel::isInitialState() const { return !m_prevDistances.size(); }

// https://stackoverflow.com/questions/13728430/element-wise-multiplication-of-two-vectors-in-c
// Określanie reworda. Algorytm działa następująco
/*
function [reward, terminal] = getReward(action, prev_action, sensor1, sensor2,
krasj)

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
