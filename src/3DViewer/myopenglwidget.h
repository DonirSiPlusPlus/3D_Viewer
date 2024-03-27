#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#define GL_SILENCE_DEPRECATION
#include <QColor>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QSettings>

#include "mainwindow.h"

typedef struct colorStruct {
  QColor colorVertex;
  QColor colorLine;
  QColor colorBack;

  double colorVertexRed;
  double colorVertexGreen;
  double colorVertexBlue;
  double colorVertexAlpha = 255;

  double colorLineRed;
  double colorLineGreen;
  double colorLineBlue;
  double colorLineAlpha = 255;

  double colorBackRed;
  double colorBackGreen;
  double colorBackBlue;
  double colorBackAlpha = 255;
} colorData;

class MyOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT
 public:
  MyOpenGLWidget(QWidget *parent = nullptr);
  ~MyOpenGLWidget();

 public slots:
  void inputName();

 signals:
  void setSettingsOnForm();

 private:
  colorData colorElement;

  // размеры окна
  int sizeMyWidgetW;
  int sizeMyWidgetH;

  // Атрибуты линий и вершин
  int sizeLine;
  int typeLine;
  int sizeVertex;
  int typeVertex;

  // перемещение и вращение
  double movX;
  double movY;
  double movZ;

  double rotX;
  double rotY;
  double rotZ;

  // масштаб
  double scaleModel;

  // проекция
  int modelProjection;

  s21::Controller *s_;
  std::string fileName_{""};
  double maxLenght_{0};

  float xRot, yRot, zRot;
  QPoint mPos;

  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override;

  void drawFigure();
  void modelCentering();
  void affinTransform();
  void checkProjection(double maxLenght);
  void configParamsSave();
  void configParamsLoad();

  void restoreModel();

  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;

  friend class MainWindow;
};

#endif  // MYOPENGLWIDGET_H
