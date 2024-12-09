/*
	By Jenocn
	https://jenocn.github.io/
*/

#pragma once

#include <typeindex>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <tuple>
#include <deque>

namespace Message {

template <typename T>
struct Type final {
	static const std::type_index TYPE;
	static const std::size_t TYPE_CODE;
};

template <typename T>
const std::type_index Type<T>::TYPE { typeid(T) };

template <typename T>
const std::size_t Type<T>::TYPE_CODE { typeid(T).hash_code() };

}
