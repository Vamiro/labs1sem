#ifndef ARRAY_H
#define ARRAY_H

#include <cstdlib>
#include <stdexcept>
#include <concepts>
#include <string>

template<typename T>
class Array final {
public:
    using size_type = size_t;

    Array();
    Array(size_type capacity);
    Array(std::initializer_list<T> const& items);
    Array(const Array& other);
    Array(Array&& other);
    ~Array();

    void reserve(size_type newCapacity);
    size_type push_back(const T& value);
    size_type insert(size_type index, const T& value);

    void remove(size_type index);

    const T& operator[](size_type index) const;
    T& operator[](size_type index);

    Array& operator=(const Array& other);
    Array& operator=(Array&& other);

    size_type size() const;
    size_type capacity() const;

    class Iterator {
    public:
        using value_type = T;
        using difference_type = ptrdiff_t;

        Iterator();
        Iterator(size_type index, Array* array);
        Iterator(const Iterator& other);
        Iterator(Iterator&& other);

        const T& get() const;
        void set(const T& value);
        void next();
        bool hasNext() const;

        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;
        bool operator<(const Iterator& other) const;
        bool operator>(const Iterator& other) const;
        bool operator<=(const Iterator& other) const;
        bool operator>=(const Iterator& other) const;
        Iterator& operator++();
        Iterator operator++(int);
        Iterator& operator--();
        Iterator operator--(int);
        T& operator*() const;
        T* operator->() const;
        Iterator& operator=(const Iterator& other);
        Iterator& operator+=(ptrdiff_t n);
        Iterator& operator-=(ptrdiff_t n);
        ptrdiff_t operator-(const Iterator& other) const;
        Iterator operator-(ptrdiff_t n) const;
        Iterator operator+(ptrdiff_t n) const;
        T& operator[](ptrdiff_t n);

    private:
        size_type index_;
        Array* array_;
    };

    class ConstIterator {
    public:
        using value_type = T;
        using difference_type = ptrdiff_t;

        ConstIterator();
        ConstIterator(size_type index, const Array* array);
        ConstIterator(const ConstIterator& other);
        ConstIterator(ConstIterator&& other);

        const T& get() const;
        void next();
        bool hasNext() const;

        bool operator==(const ConstIterator& other) const;
        bool operator!=(const ConstIterator& other) const;
        bool operator<(const ConstIterator& other) const;
        bool operator>(const ConstIterator& other) const;
        bool operator<=(const ConstIterator& other) const;
        bool operator>=(const ConstIterator& other) const;
        ConstIterator& operator++();
        ConstIterator operator++(int);
        ConstIterator& operator--();
        ConstIterator operator--(int);
        const T& operator*() const;
        const T* operator->() const;
        ConstIterator& operator=(const ConstIterator& other);
        ConstIterator& operator+=(ptrdiff_t n);
        ConstIterator& operator-=(ptrdiff_t n);
        ptrdiff_t operator-(const ConstIterator& other) const;
        ConstIterator operator-(ptrdiff_t n) const;
        ConstIterator operator+(ptrdiff_t n) const;
        const T& operator[](ptrdiff_t n) const;
    private:
        size_type index_;
        const Array* array_;
    };

    Iterator begin();
    ConstIterator cbegin() const;
    Iterator end();
    ConstIterator cend() const;


    Iterator iterator();
    ConstIterator iterator() const;

    Iterator reverseIterator();
    ConstIterator reverseIterator() const;
private:
    size_type capacity_;
    size_type size_;
    T* buf_;
};

template<typename T>
inline Array<T>::Array() : capacity_(0), size_(0), buf_(nullptr) {
}

template<typename T>
inline Array<T>::Array(size_type capacity) : capacity_(capacity), size_(capacity), buf_(nullptr) {
    if ((capacity_ != 0) && !(buf_ = (T*)malloc(sizeof(T) * capacity_))) {
        throw std::bad_alloc();
    }
}

template<typename T>
inline Array<T>::Array(std::initializer_list<T> const& items) : capacity_(items.size()), size_(items.size()), buf_(nullptr) {
    if (!(buf_ = (T*)malloc(sizeof(T) * capacity_))) {
        throw std::bad_alloc();
    }
    size_t i = 0;
    for (const auto& item : items) {
        new(&buf_[i++]) T(item);
    }
}

template<typename T>
inline Array<T>::Array(const Array& other) : capacity_(0), size_(0), buf_(nullptr) {
    *this = other;
}

template<typename T>
inline Array<T>::Array(Array&& other) : capacity_(0), size_(0), buf_(nullptr) {
    *this = std::move(other);
}

template<typename T>
inline Array<T>::~Array() {
    if (buf_) {
        for (size_type i = 0; i < size_; i++) {
            buf_[i].~T();
        }
        free(buf_);
    }
}

template<typename T>
inline void Array<T>::reserve(size_type newCapacity) {
    if (newCapacity == 0) {
        newCapacity = 16;
    }
    if (newCapacity > capacity_) {
        T* ptr = (T*)malloc(sizeof(T) * newCapacity);

        if (ptr == nullptr) throw std::bad_alloc();

        if (buf_ != nullptr) {
            for (size_type i = 0; i < size_; i++) {
                if (std::movable<T>) {
                    new (&ptr[i]) T(std::move(buf_[i]));
                    buf_[i].~T();
                } else {
                    new (&ptr[i]) T(buf_[i]);
                    buf_[i].~T();
                }
            }
            free(buf_);
        }
        buf_ = ptr;
        capacity_ = newCapacity;
    }
}

template<typename T>
inline typename Array<T>::size_type Array<T>::push_back(const T& value) {
    if (capacity_ == size_) {
        reserve(capacity_ * 2);
    }
    
    new (&buf_[size_++]) T(value);
    return size_ - 1;
}

template<typename T>
inline typename Array<T>::size_type Array<T>::insert(size_type index, const T& value) {
    if (index == size_) {
        push_back(value);
    } else if (index > size_) {
        throw std::out_of_range("");
    } else {
        if (capacity_ == size_) {
            reserve(capacity_ * 2);
        }
        for (size_type i = size_; i > index; i--) {
            if (std::movable<T>) {
                new (&buf_[i]) T(std::move(buf_[i - 1]));
            } else {
                new (&buf_[i]) T(buf_[i - 1]);
            }
        }
        new (&buf_[index]) T(value);
        size_++;
    }
    return index;
}

template<typename T>
inline void Array<T>::remove(size_type index) {
    if (index >= size_) {
        throw std::out_of_range("");
    } else if (index == size_ - 1) {
        buf_[index].~T();
    } else {
        for (size_type i = index; i < size_ - 1; i++) {
            if (std::movable<T>) {
                new (&buf_[i]) T(std::move(buf_[i + 1]));
            }
            else {
                new (&buf_[i]) T(buf_[i - 1]);
            }
        }
        buf_[size_ - 1].~T();
    }
    size_--;
}

template<typename T>
inline const T& Array<T>::operator[](size_type index) const {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }
    return buf_[index];
}

template<typename T>
inline T& Array<T>::operator[](size_type index) {
    if (index >= size_) {
        throw std::out_of_range("Index out of range");
    }
    return buf_[index];
}

template<typename T>
inline Array<T>& Array<T>::operator=(const Array& other) {
    if (buf_ != nullptr) this->~Array();
    capacity_ = other.capacity_;
    size_ = other.size_;

    if (capacity_ == 0) {
        buf_ = nullptr;
    } else if (!(buf_ = (T*)malloc(sizeof(T) * capacity_))) {
        throw std::bad_alloc();
    } else {
        for (size_type i = 0; i < size_; i++) {
            new (&buf_[i]) T(other.buf_[i]);
        }
    }
    return *this;
}

template<typename T>
inline Array<T>& Array<T>::operator=(Array&& other) {
    if (buf_ != nullptr) this->~Array();
    capacity_ = other.capacity_;
    size_ = other.size_;
    buf_ = other.buf_;
    other.capacity_ = 0;
    other.size_ = 0;
    other.buf_ = nullptr;
    return *this;
}

template<typename T>
inline typename Array<T>::size_type Array<T>::size() const {
    return size_;
}

template<typename T>
inline typename Array<T>::size_type Array<T>::capacity() const {
    return capacity_;
}

template<typename T>
inline typename Array<T>::Iterator Array<T>::begin() {
    return Iterator(0, this);
}

template<typename T>
inline typename Array<T>::ConstIterator Array<T>::cbegin() const {
    return ConstIterator(0, this);
}

template<typename T>
inline typename Array<T>::Iterator Array<T>::end() {
    return Iterator(size_, this);
}

template<typename T>
inline typename Array<T>::ConstIterator Array<T>::cend() const {
    return ConstIterator(size_, this);
}

template<typename T>
inline typename Array<T>::Iterator Array<T>::iterator() {
    return Iterator(0, this);
}

template<typename T>
inline typename Array<T>::ConstIterator Array<T>::iterator() const {
    return ConstIterator(0, this);
}

template<typename T>
inline typename Array<T>::Iterator Array<T>::reverseIterator() {
    return Iterator(size_ - 1, this);
}

template<typename T>
inline typename Array<T>::ConstIterator Array<T>::reverseIterator() const {
    return ConstIterator(size_ - 1, this);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline Array<T>::Iterator::Iterator() : index_(0), array_(nullptr) {}

template<typename T>
inline Array<T>::Iterator::Iterator(size_type index, Array* array) : index_(index), array_(array) {}

template<typename T>
inline Array<T>::Iterator::Iterator(const Iterator& other) : index_(other.index_), array_(other.array_) {}

template<typename T>
inline Array<T>::Iterator::Iterator(Iterator&& other) : index_(other.index_), array_(other.array_) {
    other.index_ = 0;
    other.array_ = nullptr;
}

template<typename T>
inline const T& Array<T>::Iterator::get() const {
    return (*array_)[index_];
}

template<typename T>
inline void Array<T>::Iterator::set(const T& value) {
    array_[index_] = value;
}

template<typename T>
inline void Array<T>::Iterator::next() {
    if (hasNext()) index_++;
}

template<typename T>
inline bool Array<T>::Iterator::hasNext() const {
    return index_ < array_->size() - 1;
}

template<typename T>
inline bool Array<T>::Iterator::operator==(const Iterator& other) const {
    return index_ == other.index_ && array_ == other.array_;
}

template<typename T>
inline bool Array<T>::Iterator::operator!=(const Iterator& other) const {
    return index_ != other.index_ || array_ != other.array_;
}

template<typename T>
inline bool Array<T>::Iterator::operator<(const Iterator& other) const {
    return index_ < other.index_ && array_ == other.array_;
}

template<typename T>
inline bool Array<T>::Iterator::operator>(const Iterator& other) const {
    return index_ > other.index_ && array_ == other.array_;
}

template<typename T>
inline bool Array<T>::Iterator::operator<=(const Iterator& other) const {
    return index_ <= other.index_ && array_ == other.array_;
}

template<typename T>
inline bool Array<T>::Iterator::operator>=(const Iterator& other) const {
    return index_ >= other.index_ && array_ == other.array_;
}

template<typename T>
inline typename Array<T>::Iterator& Array<T>::Iterator::operator++() {
    if (index_ < array_->size()) index_++;
    return *this;
}

template<typename T>
inline typename Array<T>::Iterator Array<T>::Iterator::operator++(int) {
    Iterator iter(*this);
    if (index_ < array_->size()) index_++;
    return iter;
}

template<typename T>
inline typename Array<T>::Iterator& Array<T>::Iterator::operator--() {
    if (index_ > 0) {
        index_--;
    }
    else if (index_ == 0) {
        index_ = array_->size();
    }
    return *this;
}

template<typename T>
inline typename Array<T>::Iterator Array<T>::Iterator::operator--(int) {
    Iterator iter(*this);
    if (index_ > 0) {
        index_--;
    }
    else if (index_ == 0) {
        index_ = array_->size();
    }
    return iter;
}

template<typename T>
inline T& Array<T>::Iterator::operator*() const {
    if (index_ == array_->size()) return (*array_)[index_ - 1];
    return (*array_)[index_];
}

template<typename T>
inline T* Array<T>::Iterator::operator->() const {
    if (index_ == array_->size()) return &(*array_)[index_ - 1];
    return &(*array_)[index_];
}

template<typename T>
inline typename Array<T>::Iterator& Array<T>::Iterator::operator=(const Iterator& other) {
    index_ = other.index_;
    array_ = other.array_;
    return *this;
}

template<typename T>
inline typename Array<T>::Iterator& Array<T>::Iterator::operator+=(ptrdiff_t n) {
    index_ += n;
    if (index_ >= array_->size()) index_ = array_->size();
    return *this;
}

template<typename T>
inline typename Array<T>::Iterator& Array<T>::Iterator::operator-=(ptrdiff_t n) {
    index_ -= n;
    if (index_ >= array_->size()) index_ = array_->size();
    return *this;
}

template<typename T>
inline ptrdiff_t Array<T>::Iterator::operator-(const Iterator& other) const {
    return index_ - other.index_;
}

template<typename T>
inline typename Array<T>::Iterator Array<T>::Iterator::operator-(ptrdiff_t n) const {
    size_type newIndex = index_ - n;
    if (newIndex > array_->size()) newIndex = array_->size();
    return Iterator(newIndex, array_);
}

template<typename T>
inline typename Array<T>::Iterator Array<T>::Iterator::operator+(ptrdiff_t n) const {
    size_type newIndex = index_ + n;
    if (newIndex > array_->size()) newIndex = array_->size();
    return Iterator(newIndex, array_);
}

template<typename T>
inline T& Array<T>::Iterator::operator[](ptrdiff_t n) {
    if (index_ + n >= array_->size() || index_ + n < 0) return (*array_)[n - 1];
    return (*array_)[index_ + n];
}

template<typename T>
typename Array<T>::Iterator operator+(ptrdiff_t n, typename Array<T>::Iterator iter) {
    return iter + n;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline Array<T>::ConstIterator::ConstIterator() : index_(0), array_(nullptr) {}

template<typename T>
inline Array<T>::ConstIterator::ConstIterator(size_type index, const Array* array) : index_(index), array_(array) {}

template<typename T>
inline Array<T>::ConstIterator::ConstIterator(const ConstIterator& other) : index_(other.index_), array_(other.array_) {}

template<typename T>
inline Array<T>::ConstIterator::ConstIterator(ConstIterator&& other) : index_(other.index_), array_(other.array_) {
    other.index_ = 0;
    other.array_ = nullptr;
}

template<typename T>
inline const T& Array<T>::ConstIterator::get() const {
    return (*array_)[index_];
}

template<typename T>
inline void Array<T>::ConstIterator::next() {
    if (hasNext()) index_++;
}

template<typename T>
inline bool Array<T>::ConstIterator::hasNext() const {
    return index_ < array_->size() - 1;
}

template<typename T>
inline bool Array<T>::ConstIterator::operator==(const ConstIterator& other) const {
    return index_ == other.index_ && array_ == other.array_;
}

template<typename T>
inline bool Array<T>::ConstIterator::operator!=(const ConstIterator& other) const {
    return index_ != other.index_ || array_ != other.array_;
}

template<typename T>
inline bool Array<T>::ConstIterator::operator<(const ConstIterator& other) const {
    return index_ < other.index_ && array_ == other.array_;
}

template<typename T>
inline bool Array<T>::ConstIterator::operator>(const ConstIterator& other) const {
    return index_ > other.index_ && array_ == other.array_;
}

template<typename T>
inline bool Array<T>::ConstIterator::operator<=(const ConstIterator& other) const {
    return index_ <= other.index_ && array_ == other.array_;
}

template<typename T>
inline bool Array<T>::ConstIterator::operator>=(const ConstIterator& other) const {
    return index_ >= other.index_ && array_ == other.array_;
}


template<typename T>
inline typename Array<T>::ConstIterator& Array<T>::ConstIterator::operator++() {
    if (index_ < array_->size()) index_++;
    return *this;
}

template<typename T>
inline typename Array<T>::ConstIterator Array<T>::ConstIterator::operator++(int) {
    ConstIterator iter(*this);
    if (index_ < array_->size()) index_++;
    return iter;
}

template<typename T>
inline typename Array<T>::ConstIterator& Array<T>::ConstIterator::operator--() {
    if (index_ > 0) {
        index_--;
    }
    else if (index_ == 0) {
        index_ = array_->size();
    }
    return *this;
}

template<typename T>
inline typename Array<T>::ConstIterator Array<T>::ConstIterator::operator--(int) {
    ConstIterator iter(*this);
    if (index_ > 0) {
        index_--;
    }
    else if (index_ == 0) {
        index_ = array_->size();
    }
    return iter;
}

template<typename T>
inline const T& Array<T>::ConstIterator::operator*() const {
    if (index_ == array_->size()) return (*array_)[index_ - 1];
    return (*array_)[index_];
}

template<typename T>
inline const T* Array<T>::ConstIterator::operator->() const {
    if (index_ == array_->size()) return &(*array_)[index_ - 1];
    return &(*array_)[index_];
}

template<typename T>
inline typename Array<T>::ConstIterator& Array<T>::ConstIterator::operator=(const ConstIterator& other) {
    index_ = other.index_;
    array_ = other.array_;
    return *this;
}

template<typename T>
inline typename Array<T>::ConstIterator& Array<T>::ConstIterator::operator+=(ptrdiff_t n) {
    index_ += n;
    if (index_ >= array_->size()) index_ = array_->size();
    return *this;
}

template<typename T>
inline typename Array<T>::ConstIterator& Array<T>::ConstIterator::operator-=(ptrdiff_t n) {
    index_ -= n;
    if (index_ >= array_->size()) index_ = array_->size();
    return *this;
}

template<typename T>
inline ptrdiff_t Array<T>::ConstIterator::operator-(const ConstIterator& other) const {
    return index_ - other.index_;
}

template<typename T>
inline typename Array<T>::ConstIterator Array<T>::ConstIterator::operator-(ptrdiff_t n) const {
    size_type newIndex = index_ - n;
    if (newIndex > array_->size()) newIndex = array_->size();
    return Iterator(newIndex, array_);
}

template<typename T>
inline typename Array<T>::ConstIterator Array<T>::ConstIterator::operator+(ptrdiff_t n) const {
    size_type newIndex = index_ - n;
    if (newIndex > array_->size()) newIndex = array_->size();
    return Iterator(newIndex, array_);
}

template<typename T>
inline const T& Array<T>::ConstIterator::operator[](ptrdiff_t n) const {
    if (index_ + n >= array_->size() || index_ + n < 0) return (*array_)[n - 1];
    return (*array_)[index_ + n];
}

template<typename T>
typename Array<T>::ConstIterator operator+(ptrdiff_t n, typename Array<T>::ConstIterator iter) {
    return iter + n;
}

#endif // ARRAY_H
