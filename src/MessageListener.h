/*
	By Jenocn
	https://jenocn.github.io/
*/

#pragma once

#include "MessageBase.h"

class IMessageListener {
public:
	virtual ~IMessageListener() = default;

	virtual void Invoke(std::shared_ptr<IMessage> message) = 0;
};

template<typename _Ty>
class MessageListener : public IMessageListener {
public:
	MessageListener(std::function<void(std::shared_ptr<_Ty>)> func);
	virtual void Invoke(std::shared_ptr<IMessage> message);
private:
	std::function<void(std::shared_ptr<_Ty>)> _func;
};

template<typename _Ty>
MessageListener<_Ty>::MessageListener(std::function<void(std::shared_ptr<_Ty>)> func)
	: _func(func) {
}

template<typename _Ty>
void MessageListener<_Ty>::Invoke(std::shared_ptr<IMessage> message) {
	_func(std::static_pointer_cast<_Ty>(message));
}

