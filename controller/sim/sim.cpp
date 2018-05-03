#include "sim.h"

#include <QtWidgets>

#include "line.h"
#include "counter.h"
#include "qlearningmodel.h"
#include "normalizer.h"
#include "world.h"
#include <math.h>

using namespace sim;

Sim::Sim() {}

int Sim::run(int argc, char **argv) {
  QApplication app(argc, argv);
  QGraphicsScene scene;
  scene.setSceneRect(0, -100, 600, 600);
  scene.setItemIndexMethod(QGraphicsScene::NoIndex);
  //! [2]

  //! [3]
  
  Normalizer norm;
  ai::QLearningModel model(norm);
  auto world = new sim::World();
  world->setPos(20, 150);

  auto line = new sim::Line(world->getPaths(), world, model);
  line->setPos(150, 150);
  scene.addItem(line);
  scene.addItem(world);
  
  auto counter = new sim::Counter(line);
  counter->setPos(550, 00);
  scene.addItem(counter);

  //! [3]

  //! [4]
  QGraphicsView view(&scene);
  view.setRenderHint(QPainter::Antialiasing);
  //  view.setBackgroundBrush(QPixmap(":/images/cheese.jpg"));
  //! [4] //! [5]
  // view.setCacheMode(QGraphicsView::CacheBackground);
  view.setCacheMode(QGraphicsView::CacheNone);
  view.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
  view.setDragMode(QGraphicsView::ScrollHandDrag);
  //! [5] //! [6]
  view.setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Colliding Mice"));
  view.resize(800, 800);
  view.show();

  QTimer timer;
  QObject::connect(&timer, SIGNAL(timeout()), &scene, SLOT(advance()));
  timer.start(10000 / 500);

  return app.exec();
}