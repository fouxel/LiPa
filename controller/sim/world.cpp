#include "world.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <qmath.h>
#include <iostream>

using namespace sim;

//#define RECT 1
//#define CIRCLE 1
//#define ANIM 1

World::World() {
  QPainterPath path;
  path.addRect(-100, -300, 50, 500);
  path.addRect(-100, 200, 700, 50);
  path.addRect(-50, -300, 650, 50);
  path.addRect(550, -250, 50, 450);

#ifdef RECT
  path.addRect(100, 100, 100, 100);

  path.addRect(0, -200, 100, 100);

  path.addRect(350, 00, 100, 100);
#endif

#ifdef CIRCLE
  path.addEllipse(100, 50, 100, 100);
  path.addEllipse(0, -100, 100, 100);
  path.addEllipse(400, 00, 100, 100);
  path.addEllipse(450, -250, 100, 100);
#endif

  m_paths.push_back(path);

#ifdef ANIM
  QPainterPath circle;
  circle.addEllipse(300,-100, 70, 70);
  m_paths.emplace_back(circle);
  moveXCircle = 5;
  
  QPainterPath rect;
  rect.addRect(0, 0, 100, 100);
  m_paths.emplace_back(rect);
  moveYRect = 5;
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
    moveXCircle = -5;
  } if (pos.x() < 300) {
    moveXCircle = +5;
  }
  
  pos = m_paths[2].currentPosition();
  std::cout << "pos.y " << pos.y() << std::endl;
  if (pos.y() < -200) {
    moveYRect = +5;
  } if (pos.y() > 100) {
    moveYRect = -5;
  }
#endif
  scene()->update();
}

void World::advance(int step) {
  if (!step)
    return;
#ifdef ANIM
  m_paths[1].translate(moveXCircle, 0);
  m_paths[2].translate(0, moveYRect);
#endif
}

const std::vector<std::reference_wrapper<const QPainterPath>>
World::getPaths() const {
  std::vector<std::reference_wrapper<const QPainterPath>> vec = {m_paths.begin(), m_paths.end()};
  return vec;
}
