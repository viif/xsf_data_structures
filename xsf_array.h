#ifndef XSF_ARRAY_H
#define XSF_ARRAY_H

namespace xsf_data_structures {

template <typename T, size_t S>
class XSFArray {
 public:
  constexpr size_t Size() const { return S; }

  T &operator[](size_t index) { return data_[index]; }
  const T &operator[](size_t index) const { return data_[index]; }

  T *Data() { return data_; }
  const T *Data() const { return data_; }

 private:
  T data_[S];
};

}  // namespace xsf_data_structures

#endif  // XSF_ARRAY_H