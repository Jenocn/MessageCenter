/*
	By Jenocn
	https://jenocn.github.io/
*/

#pragma once

#include "MessageBase.h"

namespace Message {

class IMessageListener {
public:
	virtual ~IMessageListener() = default;
	virtual void Invoke(const IMessage& message) = 0;
};

template<typename _Ty>
class MessageListener : public IMessageListener {
public:
	MessageListener(std::function<void(const _Ty&)> func);
	virtual void Invoke(const IMessage& message);
private:
	std::function<void(const _Ty&)> _func;
};

template<typename _Ty>
MessageListener<_Ty>::MessageListener(std::function<void(const _Ty&)> func)
	: _func(func) {
}

template<typename _Ty>
void MessageListener<_Ty>::Invoke(const IMessage& message) {
	_func(static_cast<const _Ty&>(message));
}

}