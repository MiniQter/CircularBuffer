#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <assert.h>
#include <iostream>

template<typename T, int N>
class CircularBuffer
{
public:
    CircularBuffer()
        : beg_index_(0)
        , end_index_(0)
        , size_(0)
        , capacity_(N)
    {
        data_ = new T[N];
    }
    ~CircularBuffer() { delete[] data_; }

    size_t capacity() const { return capacity_; }
    bool empty() const { return beg_index_ == end_index_; }
    size_t size() const { return size_; }

    // Return number of bytes written.
    void write(const T &data) { write(&data, 1); }
    void write(const T *data, size_t bytes)
    {
        if (bytes == 0)
            return;

        size_t capacity = capacity_;

        // Ignore overflow datas
        if (bytes > capacity) {
            data += bytes - capacity;
            bytes = capacity;
        }

        size_t bytes_to_write = std::min(bytes, capacity - size_);

        // Write in a single step
        if (bytes_to_write <= capacity - end_index_) {
            memcpy(data_ + end_index_, data, bytes_to_write * sizeof(T));
            end_index_ += bytes_to_write;
            if (end_index_ == capacity)
                end_index_ = 0;
        }
        // Write in two steps
        else {
            size_t size_1 = capacity - end_index_;
            memcpy(data_ + end_index_, data, size_1 * sizeof(T));
            size_t size_2 = bytes_to_write - size_1;
            memcpy(data_, data + size_1, size_2 * sizeof(T));
            end_index_ = size_2;
        }

        size_ += bytes_to_write;
    }

    // Return number of bytes read.
    T read()
    {
        assert(!empty());
        T ret = data_[beg_index_ % capacity_];
        beg_index_ = (beg_index_ + 1) % capacity_;
        return ret;
    }
    size_t read(T *data, size_t bytes)
    {
        if (bytes == 0)
            return 0;

        size_t capacity = capacity_;
        size_t bytes_to_read = std::min(bytes, size_);

        // Read in a single step
        if (bytes_to_read <= capacity - beg_index_) {
            memcpy(data, data_ + beg_index_, bytes_to_read * sizeof(T));
            beg_index_ += bytes_to_read;
            if (beg_index_ == capacity)
                beg_index_ = 0;
        }
        // Read in two steps
        else {
            size_t size_1 = capacity - beg_index_;
            memcpy(data, data_ + beg_index_, size_1 * sizeof(T));
            size_t size_2 = bytes_to_read - size_1;
            memcpy(data + size_1, data_, size_2 * sizeof(T));
            beg_index_ = size_2;
        }

        size_ -= bytes_to_read;
        return bytes_to_read;
    }

    void clear() { beg_index_ = beg_index_ = 0U; }

private:
    size_t beg_index_, end_index_, size_, capacity_;
    T *data_;
};
#endif // CIRCULARBUFFER_H
