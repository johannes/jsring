/*
 * JSRING
 *
 * Copyright 2018 Johannes Schlüter
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
#ifndef JSRING
#define JSRING

#include <array>
#include <memory>

namespace jsring {

template <typename T, T max, T min = 0> class wrapping_integer {
  static_assert(std::is_integral<T>::value, "The type must be an integral type");
  T _value;

public:
  constexpr wrapping_integer(T t = 0) noexcept : _value{t} {}

  wrapping_integer operator--() {
    if (_value == min) {
      _value = max;
    } else {
      _value--;
    }
    return *this;
  }

  wrapping_integer operator++() {
    if (_value == max) {
      _value = min;
    } else {
      ++_value;
    }
    return *this;
  }

  constexpr wrapping_integer operator+(T t) const {
    t += _value;
    while (t > max) {
      t -= max + 1;
    }
    return t;
  }

  constexpr wrapping_integer operator-(T t) const {
    t = (t > _value) ? t - _value : _value - t;
    while (t > max) {
      t -= max + 1;
    }
    return t;
  }

  constexpr operator T() const { return _value; }
};

template <typename T, std::size_t N> class ring {
  static_assert(N > 0, "The js::ring size has to be positive");

public:
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = size_type;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = value_type *;

private:
  using container_type = std::array<T, N>;

  container_type _c;
  size_type _size;
  wrapping_integer<size_type, N - 1> _start;

  constexpr size_type _begin_offset() const noexcept { return _start; }

  constexpr size_type _end_offset() const noexcept { return _start + _size; }

  constexpr typename container_type::size_type
  _map_offset(size_type offset) const noexcept {
    offset += _begin_offset();
    if (offset >= N) {
      offset -= N;
    }
    return offset;
  }

public:
  class iterator {
    ring<value_type, N> *_r;
    size_type _pos;

  public:
    explicit constexpr iterator(ring<value_type, N> *r, size_type pos) noexcept
        : _r{r}, _pos{pos} {}

    T &operator*() noexcept { return (*_r)[_pos]; }
    constexpr bool operator!=(const iterator &other) const noexcept {
      return other._r != _r || other._pos != _pos;
    }
    constexpr bool operator==(const iterator &other) const noexcept {
      return !(*this != other);
    }
    constexpr iterator operator++() noexcept {
      ++_pos;
      return *this;
    }
    constexpr iterator operator--() noexcept {
      --_pos;
      return *this;
    }
  };

  class const_iterator {
    const ring<value_type, N> *_r;
    size_type _pos;

  public:
    explicit constexpr const_iterator(const ring<value_type, N> *r,
                                      size_type pos) noexcept
        : _r{r}, _pos{pos} {}

    constexpr const T &operator*() const noexcept { return (*_r)[_pos]; }
    constexpr bool operator!=(const const_iterator &other) const noexcept {
      return other._r != _r || other._pos != _pos;
    }
    constexpr bool operator==(const const_iterator &other) const noexcept {
      return !(*this != other);
    }
    constexpr const_iterator operator++() noexcept {
      ++_pos;
      return *this;
    }
    constexpr const_iterator operator--() noexcept {
      --_pos;
      return *this;
    }
  };

  constexpr ring() : _c{}, _start{0}, _size{0} {}

  constexpr ring(const ring &r) noexcept(noexcept(container_type(r._c)))
      : _c{r._c}, _start{r._start}, _size{r._size} {}

  constexpr ring(ring &&r) noexcept
      : _c{std::move(r._c)}, _start{r._start}, _size{r._size} {}

  template <typename InputIt>
  constexpr ring(InputIt first, InputIt last) : _c{}, _start{0}, _size{0} {
    while (last - first > N) {
      ++first;
    }

    auto diff = last - first;
    for (int i = 0; i < diff; ++i, ++first) {
      _c[i] = *first;
    }

    _size = diff;
  }

  constexpr ring(std::initializer_list<T> l) : ring{l.begin(), l.end()} {}

  reference operator=(const_reference other) noexcept(noexcept(_c = other._c)) {
    _c = other._c;
    _start = other._start;
    _size = other._size;
  }

  constexpr reference
  operator=(ring &&other) noexcept(noexcept(_c = other._c)) {
    _c(std::forward(other._c));
    _start = other._start;
    _size = other._size;
  }

  constexpr iterator begin() noexcept { return iterator{this, 0}; }
  constexpr const_iterator cbegin() const noexcept { return const_iterator{this, 0}; }
  constexpr const_iterator begin() const noexcept { return cbegin(); }

  constexpr iterator end() noexcept { return iterator{this, _size}; }
  constexpr const_iterator cend() const noexcept { return const_iterator{this, _size}; }
  constexpr const_iterator end() const noexcept { return cend(); }

  constexpr reference operator[](size_type offset) noexcept {
    return _c[_map_offset(offset)];
  }
  constexpr const_reference operator[](size_type offset) const noexcept {
    return _c[_map_offset(offset)];
  }

  constexpr reference at(size_type offset) {
    if (offset >= _size || offset < 0) {
      throw std::out_of_range{"offset out of range"};
    }
    return _c[_map_offset(offset)];
  }
  constexpr const_reference at(size_type offset) const {
    if (offset >= _size) {
      throw std::out_of_range{"offset out of range"};
    }
    return _c[_map_offset(offset)];
  }

  constexpr reference front() noexcept { return _c[_map_offset(0)]; }
  constexpr const_reference front() const noexcept {
    return _c[_map_offset(0)];
  }

  constexpr reference back() noexcept { return _c[_map_offset(_size - 1)]; }
  constexpr const_reference back() const noexcept {
    return _c[_map_offset(_size - 1)];
  }

  constexpr size_type size() const noexcept { return _size; }
  constexpr size_type max_size() const noexcept { return N; }

  constexpr bool empty() noexcept { return _size == 0; }
  constexpr void clear() noexcept { _start = _size = 0; }
  constexpr void fill(const T &val) noexcept(noexcept(_c.fill(val))) {
    _c.fill(val);
    _size = N;
    _start = 0;
  }

  constexpr void swap(ring &other) noexcept(noexcept(_c.swap(other._c))) {
    _c.swap(other._c);
    auto start = _start;
    auto size = _size;
    _start = other._start;
    _size = other._size;
    other._start = start;
    other._size = size;
  }

  constexpr void push_back(const T& val) noexcept {
    auto pos = _start + _size;
    _c[pos] = val;
    if (_size < N) {
      ++_size;
    } else {
      ++_start;
    }
  }

  constexpr void push_back(T&& val) /*noexcept*/ {
    auto pos = _start + _size;
    _c[pos] = std::move(val);
    if (_size < N) {
      ++_size;
    } else {
      ++_start;
    }
  }
  
  constexpr void push_front(const T& val) noexcept {
    auto pos = _start - wrapping_integer<size_t, N>{1};
    _c[pos] = val;
    if (_size < N) {
      ++_size;
    }
    --_start;
  }
  
  constexpr void push_front(T&& val) noexcept {
    auto pos = _start - wrapping_integer<size_t, N>{1};
    _c[pos] = std::move(val);
    if (_size < N) {
      ++_size;
    }
    --_start;
  }

  constexpr void pop_front() noexcept {
    ++_start;
    --_size;
  }

  constexpr void pop_back() noexcept { --_size; }
};
}

namespace std {
template <class T, std::size_t N>
void swap(::jsring::ring<T, N> &lhs,
          ::jsring::ring<T, N> &rhs) noexcept(noexcept(lhs.swap(rhs))) {
  lhs.swap(rhs);
}

// For these to make sense we also need std::get() support
template <typename T, size_t N>
class tuple_size<::jsring::ring<T, N>> : public integral_constant<size_t, N> {};

template <std::size_t I, class T, std::size_t N>
class tuple_element<I, ::jsring::ring<T, N>> {
  using type = T;
};
}
#endif
