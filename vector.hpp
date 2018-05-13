#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include <cstddef>  // Needed for size_t
#include <stdexcept>  // For exceptions
#include <utility>  // For swap
#include <iostream>  // for stream manipulation
#include <cmath>  // log2, ceil

#define SEPARATOR ", "

using std::ostream;
using std::istream;

class Slice {
    size_t stride;
    size_t start;
    size_t end;
    bool has_start;
    bool has_end;
    bool has_stride;
 public:
    Slice() {
        stride = 1;
        start = end = 0;
        has_start = has_end = has_stride = false;
    }

    Slice(size_t start, size_t end) {
        this->start = start;
        this->end = end;
        stride = 1;
        has_start = has_end = true;
        has_stride = false;
    }

    void setStart(size_t new_start) {
        this->start = new_start;
        has_start = true;
    }

    void setEnd(size_t new_end) {
        this->end = new_end;
        has_end = true;
    }

    void setStride(size_t new_stride) {
        this->stride = new_stride;
        has_stride = true;
    }

    bool hasStart() {
        return has_start;
    }

    bool hasEnd() {
        return has_end;
    }

    bool hasStride() {
        return has_stride;
    }

    friend ostream& operator<<(ostream& os, const Slice& r) {
        os << "Slice(" << r.start
           << "," << r.end
           << "," << r.stride
           << ")";
        return os;
    }
};

static const Slice S;  // global dummy object, helps creating slices on the fly

Slice operator,(const Slice& a, size_t b) {
    Slice c = a;
    if (!c.hasStart()) {
        c.setStart(b);
    }
    else if (!c.hasEnd()) {
        c.setEnd(b);
    }
    else {
        c.setStride(b);
    }
    return c;
}

template <typename T>
class Vector {
    T *data;
    size_t size;
    size_t alloc_size;
    static const size_t INIT_CHOP = 10;
    //  static const size_t CHOP_SIZE = 20;
    // static const size_t RESIZE_FACTOR = 2;

    // Sets an specific size for the vector
    void _resize(size_t new_alloc_size) {
        T *aux = new T[new_alloc_size];
        for (size_t i = 0; i < size; i++)
            aux[i] = data[i];
        delete []data;
        data = aux;
        alloc_size = new_alloc_size;
    }

    // Resize for an specific number of elements
    void resize(size_t new_size) {
        if (new_size <= INIT_CHOP)
            _resize(INIT_CHOP);
        else
            _resize(INIT_CHOP*(1<<(size_t) ceil(log2(new_size)-log2(10))));
    }

 public:
    Vector() {
        size = 0;
        alloc_size = INIT_CHOP;
        data = new T[alloc_size];
    }

    Vector(const Vector& other) {
        // std::cout << "Copy constructor called." << std::endl;
        size = other.size;
        alloc_size = other.alloc_size;
        data = new T[alloc_size];
        for (size_t i = 0; i < size; i++) {
            data[i] = other.data[i];  // Remember to prevent mutation
        }
    }

    template <typename VectorIterator>
    Vector(const VectorIterator& first, const VectorIterator& last) {
        size = 0;
        resize(last - first);
        for (VectorIterator it = first; it != last; ++it) {
            data[size] = *it;
            size++;
        }
    }

    ~Vector() {
        // std::cout << "Delete called" << std::endl;
        delete []data;
    }

    T& operator[](size_t pos) const {
        if (pos >= size)
            throw std::out_of_range("The index is out of range.");
        return data[pos];
    }

    // Concatenates two vectors
    Vector operator+(const Vector& r) {
        Vector result;
        for (size_t i = 0; i < size; i++)
            result.append(data[i]);
        for (size_t i = 0; i < r.size; i++)
            result.append(r.data[i]);
        return result;
    }

    // Multiplication element by element
    Vector operator*(const Vector& r) {
        if (size != r.size)
            throw std::length_error("The two vectors have different sizes.");
        Vector result;

        for (size_t i = 0; i < size; i++)
            result.append(data[i]*r.data[i]);

        return result;
    }

    // Multiplication by a scalar
    template <typename G>
    Vector& operator*=(G r) {
        for (size_t i = 0; i < size; i++)
            data[i] *= r;
        return *this;
    }

    // Division by a scalar
    template <typename G>
    Vector& operator/=(G r) {
        for (size_t i = 0; i < size; i++)
            data[i] /= r;
        return *this;
    }

    // Multiplication by a scalar
    template <typename G>
    Vector& operator*(G r) {
        for (size_t i = 0; i < size; i++)
            data[i] *= r;
        return *this;
    }

    // Division by a scalar
    template <typename G>
    Vector& operator/(G r) {
        for (size_t i = 0; i < size; i++)
            data[i] /= r;
        return *this;
    }

    bool operator==(const Vector& r) const {
        if (size != r.size)
            return false;
        for (size_t i = 0; i < size; ++i)
            if (data[i] != r.data[i])
                return false;
        return true;
    }

    bool operator!=(const Vector& r) const {
        return !(*this == r);
    }

    Vector slice(size_t start, size_t slice_size, size_t stride) {
        Vector result;

        for (size_t i = start; i < slice_size; i += stride) {
            if (i >= size)
                break;
            result.append(data[i]);
        }

        return result;
    }

    void append(const T& object) {
        if (size >= alloc_size)
            resize(size+1);
        data[size] = T(object);  // puts a copy inside
        size++;
    }

    void remove(size_t pos) {
        if (pos < size) {
            for (size_t i = pos; i < size - 1; ++i)  // move everything one step back
                data[i] = data[i+1];
            delete data[size - 1];  // delete the last item
            size--;
            resize(size);
        }

    }

    // Useful for the copy and swap idiom implementation of operator=
    void swap(Vector &r) throw() {
        using std::swap;

        swap(data, r.data);
        swap(size, r.size);
        swap(alloc_size, r.alloc_size);
    }

    Vector& operator=(Vector other) {
        // std::cout << "operator= called." << std::endl;
        other.swap(*this);
        return *this;
    }

    friend ostream& operator<<(ostream& os, const Vector& r) {
        for (size_t i = 0; i < r.size; ++i)
            os << r.data[i];
        return os;
    }

    friend istream& operator>>(istream& is, Vector& r) {
        for (T t; is >> t;) {
            r.append(t);
        }
        return is;
    }

    size_t getSize() const {
        return size;
    }

    // Iterator support
    typedef T* iterator;
    iterator begin() {
        return &data[0];
    }
    iterator end() {
        return &data[size];
    }
    // Const version
    iterator begin() const {
        return &data[0];
    }
    iterator end() const {
        return &data[size];
    }

    void operator[](const Slice& thingy) const {
        std::cout << thingy << std::endl;
    }
};

#endif  // VECTOR_HPP_

