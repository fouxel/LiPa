#include "world.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <qmath.h>

using namespace sim;

//#define RECT 1
#define CIRCLE 1

World::World() {
  QPainterPath path;
  path.addRect(-100, -300, 50, 500);
  path.addRect(-100, 200, 700, 50);
  path.addRect(-50, -300, 650, 50);
  path.addRect(550, -250, 50, 450);

#ifdef RECT
  path.addRect(100, 100, 100, 100);

  path.addRect(0, -100, 100, 100);

  path.addRect(400, 00, 100, 100);
#endif

#ifdef CIRCLE
  path.addEllipse(100, 50, 100, 100);
  path.addEllipse(0, -100, 100, 100);
  path.addEllipse(400, 00, 100, 100);
  path.addEllipse(450, -300, 100, 100);
#endif

  m_paths.push_back(path);

#ifdef ANIM
  QPainterPath circle;
  circle.addEllipse(300,-100, 50, 50);
  m_paths.emplace_back(circle);
  moveX = 10;
#endif
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

  for (auto &path : m_paths) {
      painter->drawPath(path);
  }

#ifdef ANIM
  auto pos = m_paths[1].currentPosition();
  if (pos.x() > 500) {
    moveX = -10;
  } if (pos.x() < 300) {
    moveX = +10;
  }
#endif
  scene()->update();
}

void World::advance(int step) {
  if (!step)
    return;
#ifdef ANIM
  m_paths[1].translate(moveX, 0);
#endif
}

const std::vector<std::reference_wrapper<const QPainterPath>>
World::getPaths() const {
  std::vector<std::reference_wrapper<const QPainterPath>> vec = {m_paths.begin(), m_paths.end()};
  return vec;
}
