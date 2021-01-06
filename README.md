# cpp-result

![version](https://img.shields.io/badge/version-0.1-brightgreen)
[![license](https://img.shields.io/badge/license-MIT-blue)](LICENSE.md)
![cpp-version](https://img.shields.io/badge/C%2B%2B-≥17-blue)

The Result monad implementation in C++

## Installation
1. ![download-solid](https://user-images.githubusercontent.com/54913619/76699933-4a559500-66c3-11ea-978a-48808ab0f852.png) [Download]() the library source;
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

The short form looks much prettier, isn't it?

## Usage example

```C++
auto func () -> result<int, std::string>
{

}
```

## Improvements



## License
See the [LICENSE](LICENSE.md) file for license rights and limitations (MIT).
