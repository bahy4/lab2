#include <algorithm>
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

    size_type get_y_size() const { return y_size; }
    size_type get_x_size() const { return x_size; }

private:
    T * const data;
    size_type y_size, x_size;    

    
};