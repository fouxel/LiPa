#include "world.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <qmath.h>

using namespace sim;

World::World() {
  QPainterPath *path = new QPainterPath;
  path->addRect(-100, -300, 50, 500);
  path->addRect(-100, 200, 700, 50);
  path->addRect(-50, -300, 650, 50);
  path->addRect(550, -250, 50, 450);
  std::unique_ptr<QPainterPath> tmp(path);
  m_paths.emplace_back(std::move(tmp));
  
  moveX = 10;
  m_circle.addEllipse(10, 0, 50, 50);
}

QRectF World::boundingRect() const {
  qreal adjust = 0.5;
  return QRectF(-18 - adjust, -22 - adjust, 36 + adjust, 60 + adjust);
}

QPainterPath World::shape() const {
  QPainterPath path;
  path.addRect(-10, -20, 20, 40);
  return path;
}

void World::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                  QWidget *) {
  painter->setBrush(QColor(100, 100, 100));

  QPainterPath &p = *(m_paths[0]);
  painter->drawPath(p);
  painter->drawPath(m_circle);

  auto pos = m_circle.currentPosition();
  printf("X: %d\n", (int) pos.x());
  printf("Y: %d\n", (int) pos.y());
  if (pos.x() > 230) {
    moveX = -10;
  } if (pos.x() < 0) {
    moveX = +10;
  }
  scene()->update();
}

void World::advance(int step) {
  if (!step)
    return;
  printf("advance");
  m_circle.translate(moveX, 0);

}

const std::vector<std::reference_wrapper<QPainterPath>>
World::getPaths() const {
  std::vector<std::reference_wrapper<QPainterPath>> vec;
  for (auto &path : m_paths) {
    vec.push_back(*path);
  }
  return vec;
}
