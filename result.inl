#pragma once

// Copyright © 2021 Alex Qzminsky.
// License: MIT. All rights reserved.

/// \brief Result variant type: simple constructors extension
///
/// \author https://github.com/qzminsky
/// \version 0.1.0
/// \date 2021/01/06

#ifndef RESULT_INL
#define RESULT_INL

static_assert(__cplusplus >= 2017'00, "C++17 or higher is required");

#include "result.hpp"

/**
 * \brief Constructs a success result object with specified value
 *
 * \param val Success value stored in result
*/
template <typename T>
inline auto Ok (T const& val)
{
    return result<>::ok(val);
}

/**
 * \brief Constructs a failure result object with specified value
 *
 * \param val Failure value stored in result
*/
template <typename T>
inline auto Error (T const& val)
{
    return result<>::error(val);
}

#endif  // RESULT_INL

// MIT License
//
// Copyright (c) 2021 Alex Qzminsky
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
