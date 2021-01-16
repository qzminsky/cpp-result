#pragma once

// Copyright © 2021 Alex Qzminsky.
// License: MIT. All rights reserved.

/// \brief Result variant type
///
/// \author https://github.com/qzminsky
/// \version 1.0.0
/// \date 2021/01/16

#ifndef RESULT_H
#define RESULT_H

static_assert(__cplusplus >= 2017'00, "C++17 or higher is required");

#include <type_traits>
#include <variant>

#if __cplusplus >= 2020'00
#   include <concepts>
#endif

/**
 * \class result
 *
 * \brief Result monad implementation
 *
 * \details Stores an ok/error-state with corresponding value
*/
template <typename Ok_t = std::monostate,
          typename Error_t = std::monostate
>
class result
{
public:

    // ANCHOR Member types
    using ok_type    = Ok_t;
    using error_type = Error_t;

private:

    // Value container
    std::variant<ok_type, error_type> _value;

public:

    /// There is no default constructor for a result
    result () = delete;

    /// Default copy constructor
    result (result const&) = default;

    /// Default move constructor
    result (result&&) = default;

    /**
     * \brief Converting constructor from specified value
     *
     * \param val Value to store in a result
    */
    template <typename T>
    result (T const& val) : _value{ val } {}

    /**
     * \brief Converting constructor from ok-typed variant
     *
     * \param other Variant to construct from
    */
    template <typename Copy_Ok_t>
    result (result<Copy_Ok_t, std::monostate> const& other) { *this = other; }

    /**
     * \brief Converting constructor from error-typed variant
     *
     * \param other Variant to construct from
    */
    template <typename Copy_Error_t>
    result (result<std::monostate, Copy_Error_t> const& other) { *this = other; }

    /// Default copy assignment
    auto operator = (result const&) -> result& = default;

    /// Default move assignment
    auto operator = (result&&) -> result& = default;

    /**
     * \brief Converting assignment from ok-typed variant
     *
     * \param other Variant to extract value from
    */
    template <typename Copy_Ok_t>
    auto operator = (result<Copy_Ok_t, std::monostate> const& other) -> result&
    {
        _value.template emplace<0>(other.unwrap());
        return *this;
    }

    /**
     * \brief Converting assignment from error-typed variant
     *
     * \param other Variant to extract value from
    */
    template <typename Copy_Error_t>
    auto operator = (result<std::monostate, Copy_Error_t> const& other) -> result&
    {
        _value.template emplace<1>(other.unwrap_error());
        return *this;
    }

    /**
     * \brief Constructs a success result object with specified value
     *
     * \param val Success value stored in result
    */
    template <typename T>
    static auto ok (T const& val) -> result<T, std::monostate>
    {
        return result<T, std::monostate>{ val };
    }

    /**
     * \brief Constructs a failure result object with specified value
     *
     * \param val Failure value stored in result
    */
    template <typename T>
    static auto error (T const& val) -> result<std::monostate, T>
    {
        return result<std::monostate, T>{ val };
    }

    /**
     * \brief Predicate. Returns `true` in case of success result
    */
    [[nodiscard]]
    auto is_ok () const noexcept -> bool
    {
        return _value.index() == 0;
    }

    /**
     * \brief Predicate. Returns `true` in case of failure result
    */
    [[nodiscard]]
    auto is_error () const noexcept -> bool
    {
        return _value.index() == 1;
    }

    /**
     * \brief Predicate. Returns `true` in case of success result with matching values
    */
    template <typename T>
    [[nodiscard]]
    auto is_ok (T const& val) const noexcept -> bool
    {
        if constexpr (
            std::is_same_v<ok_type, std::monostate> ||
            std::is_same_v<T, std::monostate>
        ) {
            return false;
        }
        else return is_ok() && (unwrap() == val);
    }

    /**
     * \brief Predicate. Returns `true` in case of failure result with matching values
    */
    template <typename T>
    [[nodiscard]]
    auto is_error (T const& val) const noexcept -> bool
    {
        if constexpr (
            std::is_same_v<error_type, std::monostate> ||
            std::is_same_v<T, std::monostate>
        ) {
            return false;
        }
        else return is_error() && (unwrap_error() == val);
    }

    /**
     * \brief Predicate operator. Returns `true` in case of success result
    */
    [[nodiscard]]
    explicit operator bool () const noexcept
    {
        return is_ok();
    }

    /**
     * \brief Compares tho results by its states equality
     *
     * \param other Result to compare with
     *
     * \return `true` if comparing states is equal to each other; `false` otherwise
    */
    template <typename T1, typename T2>
    [[nodiscard]]
    auto operator == (result<T1, T2> const& other) const noexcept -> bool
    {
        return (is_ok() && other.is_ok(unwrap())) || (is_error() && other.is_error(unwrap_error()));
    }

#if __cplusplus < 2020'00
    /**
     * \brief Compares tho results by its states inequality
     *
     * \param other Result to compare with
     *
     * \return `true` if comparing states is not equal to each other; `false` otherwise
    */
    template <typename T1, typename T2>
    [[nodiscard]]
    auto operator != (result<T1, T2> const& other) const noexcept -> bool
    {
        return !(*this == other);
    }
#endif

    /**
     * \brief Extracts the stored value in case of success result and returns it
     *
     * \throw std::bad_variant_access
    */
    [[nodiscard]]
    auto unwrap () const -> ok_type
    {
        return std::get<0>(_value);
    }

    /**
     * \brief Extracts the stored value in case of failure result and returns it
     *
     * \throw std::bad_variant_access
    */
    [[nodiscard]]
    auto unwrap_error () const -> error_type
    {
        return std::get<1>(_value);
    }

    /**
     * \brief Extracts the stored vavlue in case of success result or a provided default otherwise
     *
     * \param def Default value for error case
    */
    [[nodiscard]]
    auto unwrap_or (ok_type const& def) const -> ok_type
    {
        return is_ok() ? unwrap() : def;
    }

    /**
     * \brief Performs specified execution in case of success result
     *
     * \param func Functor to invoke
     *
     * \return Reference to original result object
    */
#if __cplusplus >= 2020'00
    template <typename Functor>
    requires
             std::invocable<Functor, ok_type> || std::invocable<Functor>
#else
    template <typename Functor,
              typename = std::enable_if_t<std::disjunction_v<std::is_invocable<Functor, ok_type>, std::is_invocable<Functor>>>
    >
#endif
    auto if_ok (Functor&& func) -> result&
    {
        if (is_ok()) {
            if constexpr (std::is_invocable_v<Functor>) {
                func();
            }
            else func(unwrap());
        }
        return *this;
    }

    /**
     * \brief Performs specified execution in case of failure result
     *
     * \param func Functor to invoke
     *
     * \return Reference to original result object
    */
#if __cplusplus >= 2020'00
    template <typename Functor>
    requires
             std::invocable<Functor, error_type> || std::invocable<Functor>
#else
    template <typename Functor,
              typename = std::enable_if_t<std::disjunction_v<std::is_invocable<Functor, error_type>, std::is_invocable<Functor>>>
    >
#endif
    auto if_error (Functor&& func) -> result&
    {
        if (is_error()) {
            if constexpr (std::is_invocable_v<Functor>) {
                func();
            }
            else func(unwrap_error());
        }
        return *this;
    }

};  // end class result

#endif  // RESULT_H

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
