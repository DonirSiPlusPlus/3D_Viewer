#ifndef SRC_S21_OBJECT_H
#define SRC_S21_OBJECT_H

#include <math.h>

#include <fstream>
#include <stack>
#include <vector>

namespace s21 {

/* Хранитель исходных данных */
class SceneMemento {
 public:
  SceneMemento(const std::vector<double> points, const double max_lngth)
      : Points_{points}, Max_Distance_{max_lngth} {}

  void Restore(std::vector<double> &p, double &m) const {
    p = Points_;
    m = Max_Distance_;
  }

 private:
  std::vector<double> Points_;  //  массив координат вершин
  double Max_Distance_{0};
};

/* Данные 3д-модели */
struct SceneData {
  std::vector<size_t> sizes_;  //  массив количества полигонов
  std::vector<int> vertexes_;   //  массив индексов вершин
  std::vector<double> points_;  //  массив координат вершин

  size_t count_of_vertexes_{0};
  size_t count_of_facets_{0};

  double Max_Distance_{0};

  void clear();

  SceneData &SetInfo(const SceneData &other) noexcept;
};

/* Интерфейс для работы с данными */
class Scene3D {
 public:
  Scene3D() {}
  ~Scene3D() {}

  const SceneData &GetInfo() const { return sceneData_; }
  const std::vector<double> &GetMatrix3d() const { return sceneData_.points_; }
  const std::vector<int> &GetVertexes() const { return sceneData_.vertexes_; }

  const size_t &GetCountVertexes() const {
    return sceneData_.count_of_vertexes_;
  }

  const size_t &GetCountFacets() const { return sceneData_.count_of_facets_; }
  const double &GetMaxDistance() const { return sceneData_.Max_Distance_; }

  void clear();

  void SetInfo(const SceneData info) { sceneData_.SetInfo(info); }

  void SaveState() noexcept {
    saves_.push(SceneMemento(GetMatrix3d(), GetMaxDistance()));
  }

  void RestoreState() noexcept;

 private:
  SceneData sceneData_;
  std::stack<SceneMemento> saves_;  // история сохранений
};

/* Cчитыватель данных из файла */
class FileReader {
 public:
  FileReader() {}
  ~FileReader() {}

  void LoadFile(Scene3D *const scene3d, const std::string filename);

 private:
  std::string filename_{""};
  Scene3D *scene3d_{nullptr};
  SceneData sceneData_;

  double xMin_{0};
  double xMax_{0};
  double yMin_{0};
  double yMax_{0};
  double zMin_{0};
  double zMax_{0};

  void clear() { xMin_ = xMax_ = yMin_ = yMax_ = zMin_ = zMax_ = 0; }

  void Parser();
  void ParsVertex(const std::string string_of_file) noexcept;
  void ParsFacet(const std::string string_of_file);

  void CheckMaxMinAxes(const int axis, const double value);
  void CheckMaxLength();
};

/* Типы преобразований */
enum type_transf { moving, rotation, scaling };

/* Преобразователь */
class Transformer {
 public:
  Transformer() {}
  ~Transformer() {}

  void select_transf(Scene3D *const data, const type_transf type, double x,
                     double y = 0, double z = 0) const noexcept;

 private:
  /* Абстрактное представление афф. преобразований */
  class Transformation {
   public:
    Transformation(Scene3D *const data)
        : scene3d_{data}, sceneData_{data->GetInfo()} {}

    virtual void affin(double x, double y, double z) = 0;

   protected:
    Scene3D *scene3d_;
    SceneData sceneData_;
  };

  /* Перемещение 3д-модели */
  class MoveScene : Transformation {
   public:
    // or using Transformation::Transformation;
    MoveScene(Scene3D *const data) : Transformation{data} {}

    void affin(double x, double y, double z) override {
      moveX(x);
      moveY(y);
      moveZ(z);
      scene3d_->SetInfo(sceneData_);
    }

   private:
    void moveX(const double value);
    void moveY(const double value);
    void moveZ(const double value);
  };

  /* Вращение 3д-модели */
  class RotateScene : Transformation {
   public:
    RotateScene(Scene3D *const data) : Transformation{data} {}

    void affin(double x, double y, double z) override {
      turnX(x);
      turnY(y);
      turnZ(z);
      scene3d_->SetInfo(sceneData_);
    }

   private:
    void turnX(const double angle_of_rotation);
    void turnY(const double angle_of_rotation);
    void turnZ(const double angle_of_rotation);
  };

  /* Масштабирование 3д-модели */
  class ScaleScene : Transformation {
   public:
    ScaleScene(Scene3D *const data) : Transformation{data} {}

    void affin(double x, double y = 0, double z = 0) override {
      y = z = 0;
      scale(x - z + y);
      scene3d_->SetInfo(sceneData_);
    }

   private:
    void scale(const double scaling_factor);
  };
};

/* Интерфейс для фронтенда */
class Facade {
 public:
  Facade() {}
  Facade(const std::string filename) {
    FileReader_.LoadFile(&Scene3D_, filename);
  }
  ~Facade() {}

  void Set2DTextures(const std::string filename) {
    FileReader_.LoadFile(&Scene3D_, filename);
  }

  void Transformation(const type_transf type, double x, double y = 0,
                      double z = 0) noexcept {
    Transf_.select_transf(&Scene3D_, type, x, y, z);
  }

  void RestoreOriginal() noexcept { Scene3D_.RestoreState(); }

  const double &GetMaxLength() const noexcept {
    return Scene3D_.GetMaxDistance();
  }

  const std::vector<double> &GetMatrix3d() const noexcept {
    return Scene3D_.GetMatrix3d();
  }

  const size_t &GetCountOfVertexes() const noexcept {
    return Scene3D_.GetCountVertexes();
  }

  const size_t &GetCountOfFacets() const noexcept {
    return Scene3D_.GetCountFacets();
  }

  const std::vector<int> &GetVertexes() const noexcept {
    return Scene3D_.GetVertexes();
  }

  size_t GetSizeOfVertexes() const noexcept {
    return Scene3D_.GetVertexes().size();
  }

 private:
  Scene3D Scene3D_;
  FileReader FileReader_;
  Transformer Transf_;
};

}  // namespace s21

#endif  // SRC_S21_OBJECT_H
