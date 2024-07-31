/*
	By Jenocn
	https://jenocn.github.io/
*/

#pragma once

#include <memory>
#include <unordered_map>
#include <functional>
#include <array>
#include <iostream>

class _MessageTypeCounter final {
public:
	const std::size_t index;
	_MessageTypeCounter();
private:
	static std::size_t _count;
};

template<typename _Ty>
class MessageTypeId final {
public:
	static const _MessageTypeCounter id;
};

template<typename _Ty>
const _MessageTypeCounter MessageTypeId<_Ty>::id;