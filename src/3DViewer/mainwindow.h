#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QFileDialog>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QStatusBar>

#include "../controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 private:
  Ui::MainWindow *ui;

 private slots:
  void downloadFileClick();
  void changeBackColor();
  void changeLineColor();
  void changeVertexColor();

  void setLineAttribute(int index);
  void changeLineSize(int arg);

  void setVertexAttribute(int index);
  void changeVertexSize(int arg);

  void changeSpinMovX(int arg);
  void changeSpinMovY(int arg);
  void changeSpinMovZ(int arg);

  void changeSliderMovX(int value);
  void changeSliderMovY(int value);
  void changeSliderMovZ(int value);

  void changeDialRotX(int value);
  void changeSpinRotX(int arg);
  void changeDialRotY(int value);
  void changeSpinRotY(int arg);
  void changeDialRotZ(int value);
  void changeSpinRotZ(int arg);
  void changeProjectionType(int index);
  void changeScale(double arg);

  void resetParams();

  void showColor(QColor color, QLabel *label);

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  QString mFilePath;

 public slots:
  void setSettingsOnForm();

 signals:
  void downloadFile();
};
#endif  // MAINWINDOW_H
