#ifndef SRC_CONTROLLER_H
#define SRC_CONTROLLER_H

#include "../Model/s21_object.h"

namespace s21 {
class Controller {
 public:
  Controller() = default;
  Controller(std::string fileName) { facade_ = new s21::Facade(fileName); }
  ~Controller() { delete facade_; }

  bool empty() {
    if (facade_ == nullptr) {
      return true;
    } else {
      return false;
    }
  }

  Controller &operator=(const Controller &other) = delete;
  Controller &operator=(Controller &&other) = delete;

  void SetDataInModel(std::string fileName) {
    facade_->Set2DTextures(fileName);
  }

  void ModelTransform(type_transf transf_type, double x, double y = 0,
                      double z = 0) {
    facade_->Transformation(transf_type, x, y, z);
  }

  const double &GetMaxLength() const noexcept {
    return facade_->GetMaxLength();
  }

  const std::vector<double> &GetMatrix3d() const noexcept {
    return facade_->GetMatrix3d();
  }

  const size_t &GetCountOfVertexes() const noexcept {
    return facade_->GetCountOfVertexes();
  }

  const size_t &GetCountOfFacets() const noexcept {
    return facade_->GetCountOfFacets();
  }

  const std::vector<int> &GetVertexes() const noexcept {
    return facade_->GetVertexes();
  }

  size_t GetSizeOfVertexes() const noexcept {
    return facade_->GetSizeOfVertexes();
  }

  void RestoreModel() { facade_->RestoreOriginal(); }

 private:
  s21::Facade *facade_;
};  // class Controller

}  // namespace s21

#endif  // SRC_CONTROLLER_H
