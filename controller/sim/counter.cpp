#include "counter.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <easylogging++.h>

using namespace sim;

Counter::Counter(Line *line):m_line(line)
{ }

QRectF Counter::boundingRect() const {
  qreal adjust = 0.5;
  return QRectF(-50 - adjust, -50 - adjust, 100 + adjust, 50 + adjust);
}

QPainterPath Counter::shape() const {
  QPainterPath path;
  path.addRect(-70, -70, 140, 140);
  return path;
}

void Counter::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                 QWidget *) {
  painter->drawText(100, 100, 100, 100, 0, QString::number(m_line->getTerminalStateCount()));
}

void Counter::advance(int step) {
  if (!step)
    return;
}
