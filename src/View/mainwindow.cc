#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  setWindowTitle("3D_Viewer");

  connect(ui->download_file, SIGNAL(clicked()), this,
          SLOT(downloadFileClick()));
  connect(this, &MainWindow::downloadFile, ui->MyWidget,
          &MyOpenGLWidget::inputName);
  connect(ui->reset, SIGNAL(clicked()), this, SLOT(resetParams()));
  connect(ui->But_color_back, SIGNAL(clicked()), this, SLOT(changeBackColor()));
  connect(ui->But_color_line, SIGNAL(clicked()), this, SLOT(changeLineColor()));
  connect(ui->But_color_vertex, SIGNAL(clicked()), this,
          SLOT(changeVertexColor()));
  connect(ui->line_attribute, &QComboBox::activated, this,
          &MainWindow::setLineAttribute);
  connect(ui->size_line, &QSpinBox::valueChanged, this,
          &MainWindow::changeLineSize);
  connect(ui->vertex_attribute, &QComboBox::activated, this,
          &MainWindow::setVertexAttribute);
  connect(ui->size_vertex, &QSpinBox::valueChanged, this,
          &MainWindow::changeVertexSize);
  connect(ui->spin_mov_X, &QSpinBox::valueChanged, this,
          &MainWindow::changeSpinMovX);
  connect(ui->slider_mov_X, &QSlider::valueChanged, this,
          &MainWindow::changeSliderMovX);
  connect(ui->spin_mov_Y, &QSpinBox::valueChanged, this,
          &MainWindow::changeSpinMovY);
  connect(ui->slider_mov_Y, &QSlider::valueChanged, this,
          &MainWindow::changeSliderMovY);
  connect(ui->spin_mov_Z, &QSpinBox::valueChanged, this,
          &MainWindow::changeSpinMovZ);
  connect(ui->slider_mov_Z, &QSlider::valueChanged, this,
          &MainWindow::changeSliderMovZ);
  connect(ui->dial_rot_X, &QDial::valueChanged, this,
          &MainWindow::changeDialRotX);
  connect(ui->spin_rot_X, &QSpinBox::valueChanged, this,
          &MainWindow::changeSpinRotX);
  connect(ui->dial_rot_Y, &QDial::valueChanged, this,
          &MainWindow::changeDialRotY);
  connect(ui->spin_rot_Y, &QSpinBox::valueChanged, this,
          &MainWindow::changeSpinRotY);
  connect(ui->dial_rot_Z, &QDial::valueChanged, this,
          &MainWindow::changeDialRotZ);
  connect(ui->spin_rot_Z, &QSpinBox::valueChanged, this,
          &MainWindow::changeSpinRotZ);
  connect(ui->projection_type, &QComboBox::activated, this,
          &MainWindow::changeProjectionType);
  connect(ui->spin_scale, &QDoubleSpinBox::valueChanged, this,
          &MainWindow::changeScale);
  connect(ui->MyWidget, &MyOpenGLWidget::setSettingsOnForm, this,
          &MainWindow::setSettingsOnForm);
}

MainWindow::~MainWindow() { delete ui; }

//отсылка сигнала, что нужно загузить файл на виджет, и вставка его имени в
//текстовое поле
void MainWindow::downloadFileClick() {
  emit downloadFile();
  ui->box_file_name->setText(mFilePath);
}

// Цвет фона
void MainWindow::changeBackColor() {
  ui->MyWidget->colorElement.colorBack =
      QColorDialog::getColor(QColor(0, 0, 0, 255));
  if (!ui->MyWidget->colorElement.colorBack.isValid()) {
    qDebug() << "Выбор цвета фона отменён!";
  } else {
    ui->MyWidget->colorElement.colorBackRed =
        ui->MyWidget->colorElement.colorBack.redF();
    ui->MyWidget->colorElement.colorBackGreen =
        ui->MyWidget->colorElement.colorBack.greenF();
    ui->MyWidget->colorElement.colorBackBlue =
        ui->MyWidget->colorElement.colorBack.blueF();
    ui->MyWidget->colorElement.colorBackAlpha =
        ui->MyWidget->colorElement.colorBack.alphaF();
    // Раскраска квадратика-индикатора
    showColor(ui->MyWidget->colorElement.colorBack, ui->show_color_back);
  }
}

// Выбор цвета линий
void MainWindow::changeLineColor() {
  ui->MyWidget->colorElement.colorLine =
      QColorDialog::getColor(QColor(38, 38, 38, 255));
  if (!ui->MyWidget->colorElement.colorLine.isValid()) {
    qDebug() << "Выбор цвета фона отменён!";
  } else {
    ui->MyWidget->colorElement.colorLineRed =
        ui->MyWidget->colorElement.colorLine.redF();
    ui->MyWidget->colorElement.colorLineGreen =
        ui->MyWidget->colorElement.colorLine.greenF();
    ui->MyWidget->colorElement.colorLineBlue =
        ui->MyWidget->colorElement.colorLine.blueF();
    ui->MyWidget->colorElement.colorLineAlpha =
        ui->MyWidget->colorElement.colorLine.alphaF();
    // Раскраска квадратика-индикатора
    showColor(ui->MyWidget->colorElement.colorLine, ui->show_color_line);
  }
}

// Выбор цвета вершин
void MainWindow::changeVertexColor() {
  ui->MyWidget->colorElement.colorVertex =
      QColorDialog::getColor(QColor(38, 38, 38, 255));
  if (!ui->MyWidget->colorElement.colorVertex.isValid()) {
    qDebug() << "Выбор цвета фона отменён!";
  } else {
    ui->MyWidget->colorElement.colorVertexRed =
        ui->MyWidget->colorElement.colorVertex.redF();
    ui->MyWidget->colorElement.colorVertexGreen =
        ui->MyWidget->colorElement.colorVertex.greenF();
    ui->MyWidget->colorElement.colorVertexBlue =
        ui->MyWidget->colorElement.colorVertex.blueF();
    ui->MyWidget->colorElement.colorVertexAlpha =
        ui->MyWidget->colorElement.colorVertex.alphaF();
    // Раскраска квадратика-индикатора
    showColor(ui->MyWidget->colorElement.colorVertex, ui->show_color_vertex);
  }
}

// Аттрибуты линий
void MainWindow::setLineAttribute(int index) {
  switch (index) {
    case 0:  // нет отображения линий
      ui->MyWidget->typeLine = 0;
      break;
    case 1:  // сплошные линии
      ui->MyWidget->typeLine = 1;
      break;
    case 2:  // пунктирные линии
      ui->MyWidget->typeLine = 2;
      break;
  }
  ui->MyWidget->repaint();
}

void MainWindow::changeLineSize(int arg) {
  ui->MyWidget->sizeLine = arg;
  ui->MyWidget->repaint();
}

// Атрибуты вершин
void MainWindow::setVertexAttribute(int index) {
  switch (index) {
    case 0:  // нет отображения вершин
      ui->MyWidget->typeVertex = 0;
      break;
    case 1:  // круг
      ui->MyWidget->typeVertex = 1;
      break;
    case 2:  // квадрат
      ui->MyWidget->typeVertex = 2;
      break;
  }
  ui->MyWidget->repaint();
}

void MainWindow::changeVertexSize(int arg) {
  ui->MyWidget->sizeVertex = arg;
  ui->MyWidget->repaint();
}

// Перемещение
void MainWindow::changeSpinMovX(int arg) {
  ui->MyWidget->movX = arg;
  ui->slider_mov_X->setSliderPosition(arg);
  ui->MyWidget->repaint();
}

void MainWindow::changeSliderMovX(int value) {
  ui->MyWidget->movX = value;
  ui->spin_mov_X->setValue(value);
  ui->MyWidget->repaint();
}

void MainWindow::changeSpinMovY(int arg) {
  ui->MyWidget->movY = arg;
  ui->slider_mov_Y->setSliderPosition(arg);
  ui->MyWidget->repaint();
}

void MainWindow::changeSliderMovY(int value) {
  ui->MyWidget->movY = value;
  ui->spin_mov_Y->setValue(value);
  ui->MyWidget->repaint();
}

void MainWindow::changeSpinMovZ(int arg) {
  ui->MyWidget->movZ = arg;
  ui->slider_mov_Z->setSliderPosition(arg);
  ui->MyWidget->repaint();
}

void MainWindow::changeSliderMovZ(int value) {
  ui->MyWidget->movZ = value;
  ui->spin_mov_Z->setValue(value);
  ui->MyWidget->repaint();
}

// Вращение
void MainWindow::changeDialRotX(int value) {
  ui->MyWidget->rotX = value;
  ui->spin_rot_X->setValue(value);
  ui->MyWidget->repaint();
}

void MainWindow::changeSpinRotX(int arg) {
  ui->MyWidget->rotX = arg;
  ui->dial_rot_X->setValue(arg);
  ui->MyWidget->repaint();
}

void MainWindow::changeDialRotY(int value) {
  ui->MyWidget->rotY = value;
  ui->spin_rot_Y->setValue(value);
  ui->MyWidget->repaint();
}

void MainWindow::changeSpinRotY(int arg) {
  ui->MyWidget->rotY = arg;
  ui->dial_rot_Y->setValue(arg);
  ui->MyWidget->repaint();
}

void MainWindow::changeDialRotZ(int value) {
  ui->MyWidget->rotZ = value;
  ui->spin_rot_Z->setValue(value);
  ui->MyWidget->repaint();
}

void MainWindow::changeSpinRotZ(int arg) {
  ui->MyWidget->rotZ = arg;
  ui->dial_rot_Z->setValue(arg);
  ui->MyWidget->repaint();
}

// Выбор типа проекции
void MainWindow::changeProjectionType(int index) {
  switch (index) {
    case 0:  // Центральная
      ui->MyWidget->modelProjection = 0;
      break;
    case 1:  // Параллельная
      ui->MyWidget->modelProjection = 1;
      break;
    default:
      break;
  }
  ui->MyWidget->repaint();
}

// Масштаб
void MainWindow::changeScale(double arg) {
  ui->MyWidget->scaleModel = arg;
  ui->MyWidget->repaint();
}

// Сброс настроек к дефолтным
void MainWindow::resetParams() {
  ui->MyWidget->typeLine = 1;
  ui->line_attribute->setCurrentIndex(1);

  ui->MyWidget->sizeLine = 1;
  ui->size_line->setValue(1);

  ui->MyWidget->typeVertex = 1;
  ui->vertex_attribute->setCurrentIndex(1);

  ui->MyWidget->sizeVertex = 1;
  ui->size_vertex->setValue(1);

  ui->MyWidget->movX = 0;
  ui->spin_mov_X->setValue(0);

  ui->MyWidget->movY = 0;
  ui->spin_mov_Y->setValue(0);

  ui->MyWidget->movZ = 0;
  ui->spin_mov_Z->setValue(0);

  ui->MyWidget->rotX = 0;
  ui->dial_rot_X->setValue(0);
  ui->spin_rot_X->setValue(0);

  ui->MyWidget->rotY = 0;
  ui->dial_rot_Y->setValue(0);
  ui->spin_rot_Y->setValue(0);

  ui->MyWidget->rotZ = 0;
  ui->dial_rot_Z->setValue(0);
  ui->spin_rot_Z->setValue(0);

  ui->MyWidget->scaleModel = 1;
  ui->spin_scale->setValue(1);

  ui->MyWidget->colorElement.colorVertexRed = 1;
  ui->MyWidget->colorElement.colorVertexGreen = 0;
  ui->MyWidget->colorElement.colorVertexBlue = 0;

  ui->MyWidget->colorElement.colorLineRed = 0;
  ui->MyWidget->colorElement.colorLineGreen = 1;
  ui->MyWidget->colorElement.colorLineBlue = 0;

  ui->MyWidget->colorElement.colorBackRed = 0;
  ui->MyWidget->colorElement.colorBackGreen = 0;
  ui->MyWidget->colorElement.colorBackBlue = 0;

  ui->MyWidget->modelProjection = 0;
  ui->projection_type->setCurrentIndex(0);

  ui->MyWidget->colorElement.colorBack =
      ui->MyWidget->colorElement.colorBack.fromRgbF(
          ui->MyWidget->colorElement.colorBackRed,
          ui->MyWidget->colorElement.colorBackGreen,
          ui->MyWidget->colorElement.colorBackBlue, 1);
  ui->MyWidget->colorElement.colorVertex =
      ui->MyWidget->colorElement.colorVertex.fromRgbF(
          ui->MyWidget->colorElement.colorVertexRed,
          ui->MyWidget->colorElement.colorVertexGreen,
          ui->MyWidget->colorElement.colorVertexBlue, 1);
  ui->MyWidget->colorElement.colorLine =
      ui->MyWidget->colorElement.colorLine.fromRgbF(
          ui->MyWidget->colorElement.colorLineRed,
          ui->MyWidget->colorElement.colorLineGreen,
          ui->MyWidget->colorElement.colorLineBlue, 1);

  showColor(ui->MyWidget->colorElement.colorBack, ui->show_color_back);
  showColor(ui->MyWidget->colorElement.colorVertex, ui->show_color_vertex);
  showColor(ui->MyWidget->colorElement.colorLine, ui->show_color_line);

  ui->MyWidget->repaint();
}

// Установка значений из конфига на форму
void MainWindow::setSettingsOnForm() {
  ui->line_attribute->setCurrentIndex(ui->MyWidget->typeLine);
  ui->size_line->setValue(ui->MyWidget->sizeLine);
  ui->vertex_attribute->setCurrentIndex(ui->MyWidget->typeVertex);
  ui->size_vertex->setValue(ui->MyWidget->sizeVertex);
  ui->projection_type->setCurrentIndex(ui->MyWidget->modelProjection);

  // Конвертация закружаемых значений RGB в QColor
  ui->MyWidget->colorElement.colorBack =
      ui->MyWidget->colorElement.colorBack.fromRgbF(
          ui->MyWidget->colorElement.colorBackRed,
          ui->MyWidget->colorElement.colorBackGreen,
          ui->MyWidget->colorElement.colorBackBlue, 1);
  ui->MyWidget->colorElement.colorVertex =
      ui->MyWidget->colorElement.colorVertex.fromRgbF(
          ui->MyWidget->colorElement.colorVertexRed,
          ui->MyWidget->colorElement.colorVertexGreen,
          ui->MyWidget->colorElement.colorVertexBlue, 1);
  ui->MyWidget->colorElement.colorLine =
      ui->MyWidget->colorElement.colorLine.fromRgbF(
          ui->MyWidget->colorElement.colorLineRed,
          ui->MyWidget->colorElement.colorLineGreen,
          ui->MyWidget->colorElement.colorLineBlue, 1);

  showColor(ui->MyWidget->colorElement.colorBack, ui->show_color_back);
  showColor(ui->MyWidget->colorElement.colorVertex, ui->show_color_vertex);
  showColor(ui->MyWidget->colorElement.colorLine, ui->show_color_line);
}

// Раскраска квадратика-индикатора в выбранный цвет
void MainWindow::showColor(QColor color, QLabel *label) {
  QVariant variant = color;
  QString colorString = variant.toString();
  label->setStyleSheet("QLabel {background-color:" + colorString + ";}");
}
