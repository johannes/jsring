jsring - A toy ring buffer
==========================

[![Build Status](https://travis-ci.org/johannes/jsring.svg?branch=master)](https://travis-ci.org/johannes/jsring)

This is a simple implementation of a ring buffer with a compile-time defined size. This is an weekend afternoon hack and not supposed to be used in production. There certainly are more robust/efficient/... implementations.

Usage
-----

```c++
#include <jsring.h>
#include <iostream>

jsring::ring<int, 10> ring{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
ring.push_back(11);
ring.push_back(12);
ring.push_front(0);

for (auto i : ring) {
  std::cout << i << " ";
}
std::cout << '\n';
```

This will print

    0 3 4 5 6 7 8 9 10 11

See the [tests](test/main.cpp) for more examples.

Installation
------------

All code is included in `include/jsring.h`. Also a `CMakeLists.txt` is provided which allows installation using CMake. After installing other projects can add a dependency via

```cmake
find_package(jsring CONFIG REQUIRED)
target_link_libraries(YourApp jsring)
```

Tests are using GTest. CMake will load this dependency when needed.

License
-------

This library is licensed under the terms of the MIT License.

> Copyright 2018 Johannes Schlüter
> 
> Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
>
> The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
