#include "line.h"

#include "imodel.h"
#include <QGraphicsScene>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <iostream>
#include <qmath.h>
#include <easylogging++.h>

using namespace sim;

Line::Line(const std::vector<std::reference_wrapper<const QPainterPath>> paths,
           QGraphicsItem *world, ai::IModel &model)
    : m_paths(paths), m_world(world), m_model(model), m_terminalStateCount(0) {
  for (auto i = 0; i < 5; ++i) {
    QLineF line;
    line.setP1(QPointF(-1, -1));
    line.setAngle(i * 45);
    m_lines.push_back(line);
  }
}

QRectF Line::boundingRect() const {
  qreal adjust = 0.5;
  return QRectF(-50 - adjust, -50 - adjust, 100 + adjust, 50 + adjust);
}

QPainterPath Line::shape() const {
  QPainterPath path;
  path.addRect(-70, -70, 140, 140);
  return path;
}

void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                 QWidget *) {
  painter->setBrush(QColor(100, 100, 100));
  for (auto &line : m_lines) {
    painter->drawLine(line);
  }
  
}

void Line::advance(int step) {
  if (!step)
    return;
  std::vector<int> distances = {49, 49, 49, 49, 49};
  int count = 0;
  for (auto &line : m_lines) {
    int length = 50;
    bool found = false;
     while (length-- > 0) {
      int freePathsCount = 0;
      line.setLength(length);
      for (const auto &path : m_paths) {
        if (path.get().contains(mapToItem(m_world, line.pointAt(1)))) {
        } else {
          freePathsCount++;
        }
      }
      if (freePathsCount == m_paths.size()) {
        if (distances[count] > length) {
            distances[count] = length;
        }
        found = true;
        break;
      }
    }
    if (found == false) {
      distances[count] = 1;
      
    }
    count++;
  }
  auto rot = rotation();
  auto action = m_model.getAction(distances);
  switch (action) {
  case ai::IModel::ACTION_FORWARD:
    setPos(mapToParent(0, -10.f));
    break;
  case ai::IModel::ACTION_LEFT:
    setRotation(rot+60);
    setPos(mapToParent(0, -10.f));
    break;
  case ai::IModel::ACTION_RIGHT:
    setRotation(rot-60);
    setPos(mapToParent(0, -10.f));
    break;
  case ai::IModel::ACTION_TERMINATE:
    setRotation(0);
    setPos(150, 150);
    m_terminalStateCount++;
    break;
  }
}

int Line::getTerminalStateCount() {
    return m_terminalStateCount;
}