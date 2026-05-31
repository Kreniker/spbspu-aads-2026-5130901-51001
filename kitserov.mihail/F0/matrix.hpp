#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstddef>
#include <ostream>
#include <stdexcept>
#include <utility>

namespace kitserov
{
  template< class T = int >
  class Vector
  {
  private:
    T* data_;
    size_t size_;
    size_t capacity_;

    void release() noexcept
    {
      delete[] data_;
      data_ = nullptr;
      size_ = 0;
      capacity_ = 0;
    }

  public:
    Vector() noexcept :
      data_(nullptr),
      size_(0),
      capacity_(0)
    {}

    explicit Vector(size_t size, const T& value = T()) :
      data_(size == 0 ? nullptr : new T[size]),
      size_(size),
      capacity_(size)
    {
      for (size_t index = 0; index < size_; ++index)
      {
        data_[index] = value;
      }
    }

    Vector(const Vector& other) :
      data_(other.capacity_ == 0 ? nullptr : new T[other.capacity_]),
      size_(other.size_),
      capacity_(other.capacity_)
    {
      for (size_t index = 0; index < size_; ++index)
      {
        data_[index] = other.data_[index];
      }
    }

    Vector(Vector&& other) noexcept :
      data_(other.data_),
      size_(other.size_),
      capacity_(other.capacity_)
    {
      other.data_ = nullptr;
      other.size_ = 0;
      other.capacity_ = 0;
    }

    ~Vector()
    {
      release();
    }

    Vector& operator=(const Vector& other)
    {
      if (this == &other)
      {
        return *this;
      }

      Vector tmp(other);
      swap(tmp);
      return *this;
    }

    Vector& operator=(Vector&& other) noexcept
    {
      if (this == &other)
      {
        return *this;
      }

      swap(other);
      return *this;
    }

    void swap(Vector& other) noexcept
    {
      using std::swap;
      swap(data_, other.data_);
      swap(size_, other.size_);
      swap(capacity_, other.capacity_);
    }

    void reserve(size_t newCapacity)
    {
      if (newCapacity <= capacity_)
      {
        return;
      }

      T* newData = new T[newCapacity];
      for (size_t index = 0; index < size_; ++index)
      {
        newData[index] = data_[index];
      }

      delete[] data_;
      data_ = newData;
      capacity_ = newCapacity;
    }

    void resize(size_t newSize, const T& value = T())
    {
      if (newSize > capacity_)
      {
        reserve(newSize);
      }

      if (newSize > size_)
      {
        for (size_t index = size_; index < newSize; ++index)
        {
          data_[index] = value;
        }
      }

      size_ = newSize;
    }

    void pushBack(const T& value)
    {
      if (size_ == capacity_)
      {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
      }

      data_[size_++] = value;
    }

    T& operator[](size_t index) noexcept
    {
      return data_[index];
    }

    const T& operator[](size_t index) const noexcept
    {
      return data_[index];
    }

    T& at(size_t index)
    {
      if (index >= size_)
      {
        throw std::out_of_range("Vector index out of range");
      }
      return data_[index];
    }

    const T& at(size_t index) const
    {
      if (index >= size_)
      {
        throw std::out_of_range("Vector index out of range");
      }
      return data_[index];
    }

    size_t size() const noexcept
    {
      return size_;
    }

    size_t capacity() const noexcept
    {
      return capacity_;
    }
  };

  template< class T = int >
  class Matrix
  {
    Vector< Vector< T > > data_;
    size_t rows_;
    size_t cols_;

  public:
    Matrix() noexcept :
      data_(),
      rows_(0),
      cols_(0)
    {}

    Matrix(size_t rows, size_t cols, const T& value = T()) :
      data_(rows),
      rows_(rows),
      cols_(cols)
    {
      for (size_t row = 0; row < rows_; ++row)
      {
        data_[row] = Vector< T >(cols_, value);
      }
    }

    size_t rows() const noexcept
    {
      return rows_;
    }

    size_t cols() const noexcept
    {
      return cols_;
    }

    T& operator()(size_t row, size_t col)
    {
      if (row >= rows_ || col >= cols_)
      {
        throw std::out_of_range("Matrix index out of range");
      }
      return data_[row][col];
    }

    const T& operator()(size_t row, size_t col) const
    {
      if (row >= rows_ || col >= cols_)
      {
        throw std::out_of_range("Matrix index out of range");
      }
      return data_[row][col];
    }

    friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix)
    {
      for (size_t row = 0; row < matrix.rows_; ++row)
      {
        for (size_t col = 0; col < matrix.cols_; ++col)
        {
          if (col != 0)
          {
            out << ' ';
          }
          out << matrix(row, col);
        }
        if (row + 1 < matrix.rows_)
        {
          out << '\n';
        }
      }
      return out;
    }
  };
}

#endif
