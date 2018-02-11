#ifndef LINE_H
#define LINE_H

#include <QGraphicsItem>
#include "imodel.h"

namespace sim {
class Line : public QGraphicsItem
{
public:
    Line(const std::vector<std::reference_wrapper<QPainterPath>> paths, QGraphicsItem* world,
         ai::IModel &model);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

protected:
    void advance(int step) override;

private:
    const std::vector<std::reference_wrapper<QPainterPath>> m_paths;
    std::vector<QLineF> m_lines;
    QGraphicsItem* m_world;
    ai::IModel &m_model;
};
}

#endif // LINE_H
