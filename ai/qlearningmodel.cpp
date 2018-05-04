#include "qlearningmodel.h"
#include <AIToolbox/MDP/Algorithms/QLearning.hpp>
#include <AIToolbox/MDP/Policies/QGreedyPolicy.hpp>
#include <AIToolbox/MDP/Policies/QSoftmaxPolicy.hpp>

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
  std::transform(distances.begin(), distances.end(),
                 m_prevDistances.begin(), diffs.begin(), std::minus<int>());

  std::cout << "diff size: " << diffs.size() << std::endl;
  for (const auto &diff : diffs) {
    std::cout << "diff: " << diff << std::endl;
    sum += diff;
  }

  float reward = -0.3;
  if (sum >= 0) {
    reward = 0.3;
  }
  return reward;
}

bool QLearningModel::isTerminalState(cdistvec &distances) const {
  // TODO: Check why it fails if dist < 10;
  auto iter = std::find_if(distances.begin(), distances.end(),
                           [](int dist) -> bool { return dist <= 20; });
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
                          encodeState(distances), -1000);
    m_prevDistances.clear();
    m_count++;

    std::ofstream file;
    file.open("logi", std::ios_base::app);
    file << "############# QFUNCTION" << std::endl;
    file << m_count << std::endl;
    file << m_solver->getQFunction() << std::endl;
    file.close();
    return ACTION_TERMINATE;
  }

  double temp = 0;
  if (m_count < 20) {
    temp = 10;
  } else if (m_count < 60) {
    temp = 3;
  }
  AIToolbox::MDP::QSoftmaxPolicy policy(m_solver->getQFunction(), 1);
  
  //AIToolbox::MDP::QGreedyPolicy policy(m_solver->getQFunction());
  Action action;
  if (encodeState(distances) == 6220) {
    action = ACTION_FORWARD;
  } else {
    action = static_cast<Action>(policy.sampleAction(encodeState(distances)));
  }
  std::cout << "Action: " << action << std::endl;
  std::cout << "State: " << encodeState(distances) << std::endl;
  float diffReward = getDiffReward(distances);
  float oppositeReward = 0;
  if (isOppositeToPrevAction(action)) {
    oppositeReward = -0.8f;
  }

  float actionReward = 0;
  switch (m_prevAction) {
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

  float totalReward = actionReward  + diffReward + oppositeReward;
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