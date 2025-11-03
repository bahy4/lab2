#include <algorithm>
#include <iostream>
#include <stdexcept>
template <typename T>
class Grid final {
public:
    using value_type = T;
    using size_type = unsigned;

    //правило 5
    //деструктор
    ~Grid() {
        delete[] data;
    }

    //конструктор копирования
    Grid(Grid const &other) : data(nullptr), y_size(other.y_size), x_size(other.x_size) {
        if (other.data) {
            data = new T[y_size * x_size];
            std::copy(other.data, other.data + y_size * x_size, data);
        }
    }

    //оператор присваивания копированием
    Grid& operator=(Grid const &other) {
        if (this != &other) {
            delete[] data;
            y_size = other.y_size;
            x_size = other.x_size;
            data = nullptr;
            if (other.data) {
                data = new T[y_size * x_size];
                std::copy(other.data, other.data + y_size * x_size, data);
            }
        }
        return *this;
    }

    // Оператор присваивания перемещением
    Grid& operator=(Grid &&other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            y_size = other.y_size;
            x_size = other.x_size;
            other.data = nullptr;
            other.y_size = 0;
            other.x_size = 0;
        }
        return *this;
    }

    //конструктор перемещения
    Grid(Grid &&other) noexcept : data(other.data), y_size(other.y_size), x_size(other.x_size) {
        other.data = nullptr;
        other.y_size = 0;
        other.x_size = 0;
    }

    //конструктор 1
    Grid(T const &t) : data(new T[1]), y_size(1), x_size(1) {
        data[0] = t;
    }

    //конструктор 2
    Grid(size_type y = 0, size_type x = 0) : data(nullptr), y_size(y), x_size(x) {
        if (y_size > 0 && x_size > 0) {
            data = new T[y_size * x_size];
        }
    }

    //конструктор 3
    Grid(size_type y, size_type x, T const &t) : data(nullptr), y_size(y), x_size(x) {
        if (y_size > 0 && x_size > 0) {
            data = new T[y_size * x_size];
            for (size_type i = 0; i < y_size * x_size; ++i) {
                data[i] = t;
            }
        }
    }

    T operator()(size_type y_idx, size_type x_idx) const {
        return data[y_idx * x_size + x_idx];
    }

    T& operator()(size_type y_idx, size_type x_idx) {
        return data[y_idx * x_size + x_idx];
    }

    class RowProxy {
    private:
        T* row_start;
        size_type row_length;
        
    public:
        RowProxy(T* start, size_type length) 
            : row_start(start), row_length(length) {}
        
        T& operator[](size_type x_idx) {
            if (x_idx >= row_length) {
                throw std::out_of_range("Column index out of range");
            }
            return row_start[x_idx];
        }
    };

    RowProxy operator[](size_type y_idx) {
        if (!data) throw std::runtime_error("Accessing empty grid");
        if (y_idx >= y_size) {
            throw std::out_of_range("Row index out of range");
        }
        return RowProxy(data + y_idx * x_size, x_size);
    }

    size_type get_y_size() const { return y_size; }
    size_type get_x_size() const { return x_size; }

private:
    T * data;
    size_type y_size, x_size;    

    
};

#include <cassert>
int main() {
    Grid<float> g(3, 2, 0.0f);
    assert(3 == g.get_y_size());
    assert(2 == g.get_x_size());

    using gsize_t = Grid<float>::size_type;

    for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
        for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
            assert(0.0f == g[y_idx][x_idx]);

    for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
        for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
            g[y_idx][x_idx] = 1.0f;

    for (gsize_t y_idx = 0; y_idx != g.get_y_size(); ++y_idx)
        for (gsize_t x_idx = 0; x_idx != g.get_x_size(); ++x_idx)
            assert(1.0f == g(y_idx, x_idx));
    return 0;
}