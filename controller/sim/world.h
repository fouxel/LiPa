#ifndef WORLD_H
#define WORLD_H

#include <QGraphicsItem>
#include <QPainter>
#include <vector>
#include <memory>

namespace  sim {

class World : public QGraphicsItem
{
public:
    World();
    World(const World&) = delete;
    World& operator=(const World&) = delete;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;
    const std::vector<std::reference_wrapper<QPainterPath> > getPaths() const;

protected:
    void advance(int step) override;

private:
     std::vector<std::unique_ptr<QPainterPath> > m_paths;
};

}
#endif // WORLD_H
