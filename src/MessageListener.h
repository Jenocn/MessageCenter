/*
	Author: Jenocn
	Date: 2019-4-23
	Email:54467371@qq.com
*/

#ifndef __MESSAGE_LISTENER_H_H__
#define __MESSAGE_LISTENER_H_H__

#include "MessageBase.h"

class IMessageListener
{
public:
	virtual ~IMessageListener() {}

	virtual void Invoke(MessagePtr message) = 0;
	virtual int GetMessageID() const = 0;
};

template<typename T>
class MessageListener : public IMessageListener
{
public:
	MessageListener(std::function<void(std::shared_ptr<T>)> func);
	virtual void Invoke(MessagePtr message);
	virtual int GetMessageID() const;
private:
	std::function<void(std::shared_ptr<T>)> _func;
};

template<typename T>
MessageListener<T>::MessageListener(std::function<void(std::shared_ptr<T>)> func)
	: _func(func)
{}

template<typename T>
void MessageListener<T>::Invoke(MessagePtr message)
{
	_func(std::static_pointer_cast<T>(message));
}
template<typename T>
int MessageListener<T>::GetMessageID() const
{
	return MessageBase<T>::GetClassType();
}

#endif // !_MESSAGE_LISTENER_H_H__