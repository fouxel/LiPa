#ifndef COUNTER_H
#define COUNTER_H

#include <QGraphicsItem>
#include <functional>
#include "line.h"

namespace sim {
class Counter : public QGraphicsItem
{
public:
    Counter(Line *line);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
protected:
    void advance(int step) override;

private:
    std::size_t m_terminalStateCount;
    Line *m_line;
};
}

#endif // COUNTER_H
