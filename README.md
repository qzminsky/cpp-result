# cpp-result

![version](https://img.shields.io/badge/version-1.0-brightgreen)
[![license](https://img.shields.io/badge/license-MIT-blue)](LICENSE.md)
![cpp-version](https://img.shields.io/badge/C%2B%2B-≥17-blue)

The Result monad implementation in C++

## Installation
1. [Download](https://github.com/qzminsky/cpp-result/archive/master.zip) the library source;
2. `#include` the `result.hpp` (or `result.inl`) file in your C++ project;
3. Enjoy!

> ⚠️ *Note that the library requires C++17 support*

### `result.inl`
The file `result.inl` contains two pretty constructors that duplicate corresponding static member functions in the `result` class. Just compare:

```C++
return result<>::ok(0);
if (func() == result<>::error(1)) { ... }
```
and
```C++
return Ok(0);
if (func() == Error(1)) { ... }
```

The short form looks much prettier, isn't it? Sadly, these functions has very short names in global scope, so, they were brought into a separate optional header.

## Usage example
```C++
// Divide x by y or return an error if y == 0
auto div (int x, int y) -> result<int, std::runtime_error>
{
  using namespace std::string_literals;
  
// return (y == 0) ? Error(...) : Ok(...);  // Wrong: different types!
  
  if (y != 0) {
    return Ok(x / y);
  }
  return Error("Zero division!"s);
  //                           ↑
  // The result type is based on std::variant and can't take arrays
}

auto main () -> int
{
  // Check a result state and extract its value if appropriate
  if (auto res = div(1, 2); res.is_ok()) {
    std::cout << res.unwrap() << std::endl;
  }
  
  // Combined state-value checking
  assert(div(10, 3).is_ok(3));
  assert(div(10, 3) == Ok(3));
  
  // "Silent" error handling
  assert(div(1, 0).unwrap_or(-1), -1);
}
```

## Improvements
As you might have noticed, a returned result must be stored in separate object to operate with it further. Fortunately, the `result` class provides special helpers to simplify some use cases:
```C++
div(1, 2).if_ok([](int res){ std::cout << "Division result is " << res; })
         .if_error([]{ std::cerr << "Division error!"; });
```
These helpers — `if_ok` and `if_error` — takes a functional parameter with own optional parameter. If the exact value is not important, this parameter can be omitted.

## License
See the [LICENSE](LICENSE.md) file for license rights and limitations (MIT).
