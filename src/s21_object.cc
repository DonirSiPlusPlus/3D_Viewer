#include "s21_object.h"

#include <cstring>

s21::SceneData &s21::SceneData::SetInfo(const SceneData &other) noexcept {
  if (&other == this) return *this;

  this->count_of_facets_ = other.count_of_facets_;
  this->count_of_vertexes_ = other.count_of_vertexes_;
  this->points_ = other.points_;
  this->sizes_ = other.sizes_;
  this->vertexes_ = other.vertexes_;
  this->Max_Distance_ = other.Max_Distance_;

  return *this;
}

void s21::SceneData::clear() {
  sizes_.clear();
  vertexes_.clear();
  points_.clear();
  count_of_vertexes_ = count_of_facets_ = Max_Distance_ = 0;
}

void s21::Scene3D::clear() {
  sceneData_.clear();
  while (!saves_.empty()) {
    saves_.pop();
  }
}

/* Востановление */
void s21::Scene3D::RestoreState() noexcept {
  saves_.top().Restore(sceneData_.points_, sceneData_.Max_Distance_);
}

void s21::FileReader::LoadFile(Scene3D *const scene3d,
                               const std::string filename) {
  if (!filename_.empty()) {
    scene3d_->clear();
    clear();
  }

  scene3d_ = scene3d;
  sceneData_ = scene3d->GetInfo();
  filename_ = filename;
  Parser();
}

/* Считывание данных из obj файла */
void s21::FileReader::Parser() {
  if (filename_.size() < 5 ||
      filename_.substr(filename_.size() - 4) != ".obj") {
    throw std::invalid_argument("Invalid file");
  }

  std::ifstream file_obj;
  file_obj.open(filename_);
  if (!file_obj.is_open()) {
    throw std::runtime_error("File not found");
  }

  std::string string_of_file{""};
  while (!file_obj.eof()) {
    std::getline(file_obj, string_of_file);
    if (string_of_file.substr(0, 2) == "v ") {
      ++sceneData_.count_of_vertexes_;
      ParsVertex(string_of_file);
    } else if (string_of_file.substr(0, 2) == "f ") {
      ++sceneData_.count_of_facets_;
      ParsFacet(string_of_file);
    }
    string_of_file.clear();
  }

  CheckMaxLength();
  file_obj.close();
  scene3d_->SetInfo(sceneData_);
  scene3d_->SaveState();
}

/* Считывание координат вершины */
void s21::FileReader::ParsVertex(const std::string string_of_file) noexcept {
  char c_string[1024] = "";
  const char delimiter[2] = " ";
  strncpy(c_string, string_of_file.substr(2).c_str(), 1024);

  char *token = std::strtok(c_string, delimiter);
  for (int i{0}; token; ++i) {
    double coord = 0;
    std::sscanf(token, "%lf", &coord);
    sceneData_.points_.push_back(coord);
    CheckMaxMinAxes(i, coord);
    token = std::strtok(NULL, delimiter);
  }
}

/* Считывание вершин полигона */
void s21::FileReader::ParsFacet(const std::string string_of_file) {
  char c_string[1024] = "";
  const char delimiter[2] = " ";
  strcpy(c_string, string_of_file.substr(2).c_str());

  int count{0};
  char *token = std::strtok(c_string, delimiter);
  while (token) {
    int vertex{0};
    std::sscanf(token, "%d", &vertex);
    if (vertex < 1) {
      throw std::invalid_argument("Polygon index in file < 1");
    }
    sceneData_.vertexes_.push_back(vertex - 1);
    if (count) {
      sceneData_.vertexes_.push_back(vertex - 1);
    }
    token = std::strtok(NULL, delimiter);
    count += 2;
  }
  int tmp(sceneData_.vertexes_.size() - count + 1);
  sceneData_.vertexes_.push_back(sceneData_.vertexes_[tmp]);
  sceneData_.sizes_.push_back(count);
}

/* Проверка максимумов и минимумов по осям */
void s21::FileReader::CheckMaxMinAxes(const int axis, const double value) {
  switch (axis) {
    case 0:
      xMax_ = std::max(value, xMax_);
      xMin_ = std::min(value, xMin_);
      break;
    case 1:
      yMax_ = std::max(value, yMax_);
      yMin_ = std::min(value, yMin_);
      break;
    case 2:
      zMax_ = std::max(value, zMax_);
      zMin_ = std::min(value, zMin_);
      break;
    default:
      break;
  }
}

/* Проверка максимального расстояния м/у точками */
void s21::FileReader::CheckMaxLength() {
  double xl{pow(xMax_ - xMin_, 2)};
  double yl{pow(yMax_ - yMin_, 2)};
  double zl{pow(zMax_ - zMin_, 2)};
  double distance{sqrt(xl + yl + zl)};

  sceneData_.Max_Distance_ = (distance > sceneData_.Max_Distance_)
                                 ? distance
                                 : sceneData_.Max_Distance_;
}

/* Сдвиг по оси Х */
void s21::Transformer::MoveScene::moveX(const double value) {
  for (size_t i{0}; i < sceneData_.points_.size(); i += 3) {
    sceneData_.points_[i] += value;
  }
}

/* Сдвиг по оси Y */
void s21::Transformer::MoveScene::moveY(const double value) {
  for (size_t i{1}; i < sceneData_.points_.size(); i += 3) {
    sceneData_.points_[i] += value;
  }
}

/* Сдвиг по оси Z */
void s21::Transformer::MoveScene::moveZ(const double value) {
  for (size_t i{2}; i < sceneData_.points_.size(); i += 3) {
    sceneData_.points_[i] += value;
  }
}

/* Масштабирование объекта */
void s21::Transformer::ScaleScene::scale(const double scaling_factor) {
  if (fabs(0. - scaling_factor) <= 1e-7 || scaling_factor < 0 ||
      fabs(1. - scaling_factor) <= 1e-7) {
    return;  // or throw
  }

  for (size_t i{0}; i < sceneData_.points_.size(); i += 3) {
    sceneData_.points_[i] *= scaling_factor;
    sceneData_.points_[i + 1] *= scaling_factor;
    sceneData_.points_[i + 2] *= scaling_factor;
  }
  sceneData_.Max_Distance_ *= scaling_factor;
}

/* Поворот по оси X */
void s21::Transformer::RotateScene::turnX(const double angle_of_rotation) {
  if (fabs(0. - angle_of_rotation) <= 1e-7) {
    return;  // or throw
  }

  double y0{0}, z0{0};
  double xd{angle_of_rotation};

  for (size_t i{0}; i < sceneData_.points_.size(); i += 3) {
    y0 = sceneData_.points_[i + 1];
    z0 = sceneData_.points_[i + 2];
    sceneData_.points_[i + 1] = cos(xd) * y0 - sin(xd) * z0;
    sceneData_.points_[i + 2] = sin(xd) * y0 + cos(xd) * z0;
  }
}

/* Поворот по оси Y */
void s21::Transformer::RotateScene::turnY(const double angle_of_rotation) {
  if (fabs(0. - angle_of_rotation) <= 1e-7) {
    return;  // or throw
  }

  double x0{0}, z0{0};
  double yd{angle_of_rotation};

  for (size_t i{0}; i < sceneData_.points_.size(); i += 3) {
    x0 = sceneData_.points_[i];
    z0 = sceneData_.points_[i + 2];
    sceneData_.points_[i] = cos(yd) * x0 + sin(yd) * z0;
    sceneData_.points_[i + 2] = cos(yd) * z0 - sin(yd) * x0;
  }
}

/* Поворот по оси Z */
void s21::Transformer::RotateScene::turnZ(const double angle_of_rotation) {
  if (fabs(0. - angle_of_rotation) <= 1e-7) {
    return;  // or throw
  }

  double x0{0}, y0{0};
  double zd{angle_of_rotation};

  for (size_t i{0}; i < sceneData_.points_.size(); i += 3) {
    x0 = sceneData_.points_[i];
    y0 = sceneData_.points_[i + 1];
    sceneData_.points_[i] = cos(zd) * x0 - sin(zd) * y0;
    sceneData_.points_[i + 1] = sin(zd) * x0 + cos(zd) * y0;
  }
}

/* Выбор типа преобразования */
void s21::Transformer::select_transf(Scene3D *const data,
                                     const type_transf type, double x, double y,
                                     double z) const noexcept {
  if (type == moving) {
    MoveScene move_scene{data};
    move_scene.affin(x, y, z);
  } else if (type == rotation) {
    RotateScene rotate_scene{data};
    x *= M_PI / 180;  // перевод в градусы
    y *= M_PI / 180;
    z *= M_PI / 180;
    rotate_scene.affin(x, y, z);
  } else {
    ScaleScene scale_scene{data};
    scale_scene.affin(x);
  }
}
