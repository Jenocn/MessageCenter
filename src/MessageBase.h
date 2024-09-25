/*
	By Jenocn
	https://jenocn.github.io/
*/

#pragma once

#include <memory>
#include <unordered_map>
#include <functional>
#include <unordered_set>
#include <tuple>
#include <list>

namespace Message {

class IMessage {
public:
	virtual ~IMessage() = default;
	virtual std::size_t GetMessageId() const = 0;
};

template <typename _Ty>
class MessageBase : public IMessage {
public:
	virtual ~MessageBase() = default;
	virtual std::size_t GetMessageId() const;
	static const std::size_t id;
};

template <typename _Ty>
std::size_t MessageBase<_Ty>::GetMessageId() const {
	return MessageBase<_Ty>::id;
}

template <typename _Ty>
const std::size_t MessageBase<_Ty>::id = typeid(_Ty).hash_code();

}
