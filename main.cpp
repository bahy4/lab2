#include <algorithm>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <cassert>
#include <utility>

template <typename T, size_t N>
class Grid;

template <typename T>
class Grid <T, 1> final {
public:
    using size_type = unsigned;

    

    //правило 5
    //деструктор
    ~Grid() {
        delete[] data;
    }

    //конструктор копирования
    Grid(Grid const &other) : data(nullptr), size_0(other.size_0) {
        if (other.data) {
            data = new T[size_0];
            try{
                std::copy(other.data, other.data + size_0, data);
            } catch(...){
                delete[] data;
                data = nullptr;
                size_0 = 0;
                throw;
            }
        }
    }

    //оператор присваивания копированием
    Grid& operator=(Grid const &other) {
        if (this != &other) {
            delete[] data;
            size_0 = other.size_0;
            data = nullptr;
            if (other.data) {
                data = new T[size_0];
                try{
                    std::copy(other.data, other.data + size_0, data);
                } catch(...){
                delete[] data;
                data = nullptr;
                size_0 = 0;
                throw;
                }
            }
        }
        return *this;
    }

    // Оператор присваивания перемещением
    Grid& operator=(Grid &&other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size_0 = other.size_0;
            other.data = nullptr;
            other.size_0 = 0;
        }
        return *this;
    }

    //конструктор перемещения
    Grid(Grid &&other) noexcept : data(other.data), size_0(other.size_0) {
        other.data = nullptr;
        other.size_0 = 0;
    }

    //конструктор 1
    Grid(T const &t) : data(new T[1]), size_0(1) {
        
        try{
            data[0] = t;
        } catch(...){
            delete[] data;
            data = nullptr;
            size_0 = 0;
            throw;
        }
    }

    //конструктор 2
    Grid(size_type size = 0) : data(nullptr), size_0(size) {
        if (size_0 > 0) {
            data = new T[size_0];
        }
    }

    //конструктор 3
    Grid(size_type size, T const &t) : data(nullptr), size_0(size) {
        if (size_0 > 0) {
            data = new T[size_0];
            try{
                for (size_type i = 0; i < size_0; ++i) {
                    data[i] = t;
                }
            } catch(...){
                delete[] data;
                data = nullptr;
                size_0 = 0;
                throw;
            }
        }
    }

    //доступ через ()

    T operator()(size_type i0) const {
        if (!data) throw std::runtime_error("Accessing empty grid");
        if (i0 >= size_0) throw std::out_of_range("Index out of range");
        return data[i0];
    }
    
    T& operator()(size_type i0) {
        if (!data) throw std::runtime_error("Accessing empty grid");
        if (i0 >= size_0) throw std::out_of_range("Index out of range");
        return data[i0];
    }

    //доступ через []

    T operator[](size_type x) const {
        if (!data) throw std::runtime_error("Accessing empty grid");
        if (x >= size_0) {
            throw std::out_of_range("Row index out of range");
        }
        return data[x];
    }

    T& operator[](size_type x) {
        if (!data) throw std::runtime_error("Accessing empty grid");
        if (x >= size_0) {
            throw std::out_of_range("Row index out of range");
        }
        return data[x];
    }

    size_type get_size() const { return size_0; }

private:
    T * data;
    size_type size_0;    

    
};


template <typename T, size_t N>
class Grid final {
    static_assert(N >= 2, "Dimension must be at least 2");
public:
    using size_type = unsigned;

    Grid() : data(nullptr), size_0(0) {}

    //правило 5
    //деструктор
    ~Grid() {
        delete[] data;
    }

    //конструктор копирования
    Grid(Grid const &other) : data(nullptr), size_0(other.size_0) {
        if (other.data) {
            data = new Grid<T,N-1> [size_0];
            try{
                std::copy(other.data, other.data + size_0, data);
            } catch(...){
                delete[] data;
                data = nullptr;
                size_0 = 0;
                throw;
            }
            
        }
    }

    //оператор присваивания копированием
    Grid& operator=(Grid const &other) {
        if (this != &other) {
            delete[] data;
            size_0 = other.size_0;
            data = nullptr;
            if (other.data) {
                data = new Grid<T,N-1> [size_0];
                try{
                    std::copy(other.data, other.data + size_0, data);
                } catch(...){
                    delete[] data;
                        data = nullptr;
                    size_0 = 0;
                    throw;
                }
            }
        }
        return *this;
    }

    // Оператор присваивания перемещением
    Grid& operator=(Grid &&other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size_0 = other.size_0;
            other.data = nullptr;
            other.size_0 = 0;
        }
        return *this;
    }

    //конструктор перемещения
    Grid(Grid &&other) noexcept : data(other.data), size_0(other.size_0) {
        other.data = nullptr;
        other.size_0 = 0;
    }

    //конструктор 1
    Grid(Grid<T, N-1> const &t) : data(new Grid<T, N-1>), size_0(1) {
        try{
            data[0] = t;
        } catch(...){
            delete[] data;
            data = nullptr;
            size_0 = 0;
            throw;
        }
    }

    //конструктор 2
    template<typename... Args, typename = std::enable_if_t<(sizeof...(Args) == N)>>
    Grid(Args... sizes) {
        
        try{
            initialize_sizes(std::forward(sizes)...);
        } catch(...){
            delete[] data;
            data = nullptr;
            size_0 = 0;
            throw;
        }
    }

    //конструктор 3
    template<typename ... Rest>
    Grid(size_t first, Rest... rest)  {
        static_assert(sizeof...(rest) == N, "Wrong number of arguments");
        try{
            initialize_with_value(first, std::forward<decltype(rest)>(rest)...);
        } catch(...){
            delete[] data;
            data = nullptr;
            size_0 = 0;
            throw;
        }
        
    }

    //доступ через ()

    template<typename... Indices>
    T operator()(size_type i0, Indices... indices) const {
        if (i0 >= size_0) throw std::out_of_range("Index 0 out of range");
        return data[i0](std::forward<decltype(indices)>(indices)...);
    }
    
    template<typename... Indices>
    T& operator()(size_type i0, Indices... indices) {
        if (i0 >= size_0) throw std::out_of_range("Index 0 out of range");
        return data[i0](std::forward<decltype(indices)>(indices)...);
    }

    //доступ через []

    Grid<T, N-1>& operator[](size_type i0) {
        if (i0 >= size_0) throw std::out_of_range("Index 0 out of range");
        return data[i0];
    }
    
    Grid<T, N-1> const& operator[](size_type i0) const {
        if (i0 >= size_0) throw std::out_of_range("Index 0 out of range");
        return data[i0];
    }

    size_type get_size() const { return size_0; }

private:
    Grid<T, N-1> * data;
    size_type size_0;    

    template<typename... Args>
    void initialize_sizes(size_type first, Args... rest) {
        size_0 = first;
        if (size_0 > 0) {
            data = new Grid<T, N-1> [size_0];
            for (size_type i = 0; i < size_0; ++i) {
                data[i] = Grid<T, N-1>(std::forward<decltype(rest)>(rest)...);
            }
        }
    }
    template<typename... Args>
    void initialize_with_value(size_type first, Args... rest) {
        size_0 = first;
        if (size_0 > 0) {
            data = new Grid<T, N-1> [size_0];
            for (size_type i = 0; i < size_0; ++i) {
                data[i] = Grid<T, N-1>(std::forward<decltype(rest)>(rest)...);
            }
        }
    }
    
};



int main() {
    Grid<float,3> const g3(2, 3, 4, 1.0f);
    assert(1.0f == g3(1, 1, 1));

    Grid<float,2> g2(2, 5, 2.0f);
    assert(2.0f == g2(1, 1));

    g2 = g3[1];
    assert(1.0f == g2(1, 1));
    return 0;
}