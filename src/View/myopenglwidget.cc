#include "myopenglwidget.h"

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
  sizeMyWidgetW = 0;
  sizeMyWidgetH = 0;

  movX = 0.0;
  movY = 0.0;
  movZ = 0.0;

  scaleModel = 1;

  s_ = new s21::Controller();  // there
}

MyOpenGLWidget::~MyOpenGLWidget() { delete s_; }

void MyOpenGLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);
  configParamsLoad();  // для функции самого виджета
}

void MyOpenGLWidget::paintGL() {
  glClearColor(colorElement.colorBackRed, colorElement.colorBackGreen,
               colorElement.colorBackBlue, 1);  // цвет и глубина фона
  glClear(GL_COLOR_BUFFER_BIT |
          GL_DEPTH_BUFFER_BIT);  // очистка буфера цвета и буфера глубины

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if (!s_->empty()) {
    maxLenght_ = s_->GetMaxLength();

    //  выбор проекции
    checkProjection(maxLenght_);

    // Центрирование
    modelCentering();
  }

  drawFigure();
  configParamsSave();
}

void MyOpenGLWidget::resizeGL(int w, int h) {
  sizeMyWidgetW = w;
  sizeMyWidgetH = h;
  glViewport(0, 0, sizeMyWidgetW, sizeMyWidgetH);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
}

void MyOpenGLWidget::mousePressEvent(QMouseEvent *mo) { mPos = mo->pos(); }

void MyOpenGLWidget::mouseMoveEvent(QMouseEvent *mo) {
  xRot = 1 / M_PI * (mo->pos().y() - mPos.y());
  yRot = 1 / M_PI * (mo->pos().x() - mPos.x());
  update();
}

//обработка входящего сигнала
void MyOpenGLWidget::inputName() {
  //по сигналу открывается окно выбора файла
  QString filePath = QFileDialog::getOpenFileName(
      this, tr("Open file"), ".", tr("Object files (*.obj)"), 0,
      QFileDialog::DontUseNativeDialog);

  //отпраавка переменной в класс, который посылает сигнал
  MainWindow *m = qobject_cast<MainWindow *>(sender());
  m->mFilePath = filePath;
  fileName_ = filePath.toStdString();
  //Перезагрузка данных модели из нового файла
  try {
    if (s_->empty()) {
      s_ = new s21::Controller(fileName_);  // there
    } else {
      s_->SetDataInModel(fileName_);
    }
  } catch (const std::exception &e) {
    QMessageBox::critical(this, "Warning", e.what());
  }

  // Вывод информации в статус бар
  int comPos = fileName_.rfind("/") + 1;
  int comEndPos = fileName_.find(".obj", comPos);
  std::string subString = fileName_.substr(comPos, comEndPos - comPos);
  QString nameFile = subString.c_str();
  QString countVertices = QString::number(s_->GetCountOfVertexes());
  QString countEdges =
      QString::number(s_->GetCountOfVertexes() + s_->GetCountOfFacets() - 2);
  m->statusBar()->showMessage("  Name: " + nameFile + "   Vertices: " +
                              countVertices + "   Edges: " + countEdges);
}

void MyOpenGLWidget::drawFigure() {
  //Проверка что модель создалась
  if (!s_->empty()) {
    // Вращение по мышке
    glRotatef(xRot, 1, 0, 0);
    glRotatef(yRot, 0, 1, 0);

    restoreModel();
    affinTransform();

    glVertexPointer(3, GL_DOUBLE, 0, s_->GetMatrix3d().data());
    glEnableClientState(GL_VERTEX_ARRAY);

    //Отрисовка вершин
    glPointSize(sizeVertex);  // размер вершины в пикселях
    glColor3d(colorElement.colorVertexRed,
              colorElement.colorVertexGreen,  // по умолчанию альфа=1
              colorElement.colorVertexBlue);
    if (typeVertex != 0) {
      if (typeVertex == 1) {  // выбор формы отображения вершины
        glEnable(GL_POINT_SMOOTH);  // отображать вершины в форме круга
      } else if (typeVertex == 2) {
        glDisable(GL_POINT_SMOOTH);  //отображать вершины в форме квадрата
      }
      glDrawArrays(GL_POINTS, 0,
                   s_->GetCountOfVertexes());  // there rewrite by controller
    }

    // Отрисовка линий
    glLineWidth(sizeLine);
    glColor3d(colorElement.colorLineRed, colorElement.colorLineGreen,
              colorElement.colorLineBlue);
    if (typeLine != 0) {
      if (typeLine == 1) {  // сплошная линия
        glDisable(GL_LINE_STIPPLE);  // отключение режима пунктирной линии
      } else if (typeLine == 2) {  // пунктирная линия
        glEnable(GL_LINE_STIPPLE);  // включение режима пунктирной линии
        glLineStipple(1, 0x00F0);
      }
      glDrawElements(GL_LINES, s_->GetSizeOfVertexes(), GL_UNSIGNED_INT,
                     s_->GetVertexes().data());
    }

    glDisableClientState(GL_VERTEX_ARRAY);
  }
}

// Пересчет для расположение модели в середине поля отрисовки (центрирование)
void MyOpenGLWidget::modelCentering() {
  double zTmp = s_->GetMaxLength();
  zTmp = (zTmp < 2) ? 2 : zTmp;
  glTranslatef(0, 0, -(zTmp));
}

void MyOpenGLWidget::affinTransform() {
  // Перемещение
  if (movX != 0 || movY != 0 || movZ != 0) {
    s_->ModelTransform(s21::moving, movX, movY, movZ);
  }
  // Вращение
  if (rotX || rotY || rotZ) {
    s_->ModelTransform(s21::rotation, rotX, rotY, rotZ);
  }
  // Масштаб
  if (scaleModel != 1 && scaleModel != 0) {
    s_->ModelTransform(s21::scaling, scaleModel);
  }
}

/* Проверка проекции отрисовки */
void MyOpenGLWidget::checkProjection(double maxLenght) {
  double zFar = maxLenght * 6;
  double shift = fabs(movZ) + fabs(movY) + fabs(movX);
  if (scaleModel > 1) {
    zFar *= scaleModel;
    shift *= scaleModel;
  }
  if (modelProjection == 0) {
    glFrustum(-1, 1, -1, 1, 1, zFar + shift);
  } else if (modelProjection == 1) {
    glOrtho(-maxLenght, maxLenght, -maxLenght, maxLenght, 0.0001,
            zFar * 2 + shift);
  }
}

// Запись в файл настроек
void MyOpenGLWidget::configParamsSave() {
  QSettings settings("3DViewer_settings.ini", "3D_Viewer");
  settings.beginGroup("Settings");
  settings.setValue("colorElement.colorBackRed", colorElement.colorBackRed);
  settings.setValue("colorElement.colorBackGreen", colorElement.colorBackGreen);
  settings.setValue("colorElement.colorBackBlue", colorElement.colorBackBlue);
  settings.setValue("colorElement.colorVertexRed", colorElement.colorVertexRed);
  settings.setValue("colorElement.colorVertexGreen",
                    colorElement.colorVertexGreen);
  settings.setValue("colorElement.colorVertexBlue",
                    colorElement.colorVertexBlue);
  settings.setValue("colorElement.colorLineRed", colorElement.colorLineRed);
  settings.setValue("colorElement.colorLineGreen", colorElement.colorLineGreen);
  settings.setValue("colorElement.colorLineBlue", colorElement.colorLineBlue);
  settings.setValue("sizeLine", sizeLine);
  settings.setValue("typeLine", typeLine);
  settings.setValue("sizeVertex", sizeVertex);
  settings.setValue("typeVertex", typeVertex);
  settings.setValue("modelProjection", modelProjection);
  settings.endGroup();
}

// Чтение данный из файла
void MyOpenGLWidget::configParamsLoad() {
  QSettings settings("3DViewer_settings.ini", "3D_Viewer");
  settings.beginGroup("Settings");
  colorElement.colorBackRed =
      settings.value("colorElement.colorBackRed", 0).toDouble();
  colorElement.colorBackGreen =
      settings.value("colorElement.colorBackGreen", 0).toDouble();
  colorElement.colorBackBlue =
      settings.value("colorElement.colorBackBlue", 0).toDouble();
  colorElement.colorVertexRed =
      settings.value("colorElement.colorVertexRed", 1).toDouble();
  colorElement.colorVertexGreen =
      settings.value("colorElement.colorVertexGreen", 0).toDouble();
  colorElement.colorVertexBlue =
      settings.value("colorElement.colorVertexBlue", 0).toDouble();
  colorElement.colorLineRed =
      settings.value("colorElement.colorLineRed", 0).toDouble();
  colorElement.colorLineGreen =
      settings.value("colorElement.colorLineGreen", 1).toDouble();
  colorElement.colorLineBlue =
      settings.value("colorElement.colorLineBlue", 0).toDouble();
  sizeLine = settings.value("sizeLine", 1).toInt();
  typeLine = settings.value("typeLine", 1).toInt();
  sizeVertex = settings.value("sizeVertex", 1).toInt();
  typeVertex = settings.value("typeVertex", 1).toInt();
  modelProjection = settings.value("modelProjection", 0).toInt();
  settings.endGroup();
  emit setSettingsOnForm();
}

void MyOpenGLWidget::restoreModel() { s_->RestoreModel(); }
