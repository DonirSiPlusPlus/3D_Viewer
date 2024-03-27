#include <QApplication>

#include "myopenglwidget.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QApplication::setStyle("Fusion");  //Стиль отрисовки форм
  MainWindow view;
  view.show();
  view.setFixedSize(1105, 603);
  return app.exec();
}
