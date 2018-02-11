#include "qlearningmodel.h"

using namespace ai;

QLearningModel::QLearningModel()
{

}

QLearningModel::~QLearningModel()
{

}

IModel::Action QLearningModel::getAction(const std::vector<int> &distances)
{
    return ACTION_RIGHT;
}
