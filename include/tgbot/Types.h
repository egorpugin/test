#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace TgBot
{

using Boolean = bool;
using Integer = std::int64_t;
using Float = float;
using String = std::string;

template <class T>
using Optional = std::optional<T>;
template <class T>
using Ptr = std::unique_ptr<T>;
template <class ... Args>
using Variant = std::variant<Args...>;
template <class T>
using Vector = std::vector<T>;

template <class T>
auto createPtr()
{
    return std::make_unique<T>();
}

#include "types.inl.h"

}