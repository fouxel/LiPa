#include "world.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <qmath.h>

using namespace sim;

World::World()
{
    QPainterPath *path = new QPainterPath;
    path->moveTo(0, -100);
    path->addRect(-50, -150, 50, 250);
    path->addRect(-50, 100, 350, 50);
    path->addRect(0, -150, 300, 50);
    path->addRect(250, -100, 50, 200);
    std::unique_ptr<QPainterPath> tmp (path);
    m_paths.emplace_back(std::move(tmp));
}

QRectF World::boundingRect() const
{
    qreal adjust = 0.5;
    return QRectF(-18 - adjust, -22 - adjust,
                  36 + adjust, 60 + adjust);
}

QPainterPath World::shape() const
{
    QPainterPath path;
    path.addRect(-10, -20, 20, 40);
    return path;
}

void World::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setBrush(QColor(100, 100, 100));

    QPainterPath &p = *(m_paths[0]);
    painter->drawPath(p);
}

void World::advance(int step)
{
    if (!step)
        return;
}

const std::vector<std::reference_wrapper<QPainterPath>> World::getPaths() const{
    std::vector<std::reference_wrapper<QPainterPath> > vec;
    for (auto &path : m_paths) {
        vec.push_back(*path);
    }
    return vec;
}
