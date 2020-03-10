/*
	By Jenocn
	https://jenocn.github.io/
*/

#ifndef __MESSAGE_BASE_H_H__
#define __MESSAGE_BASE_H_H__

#include <map>
#include <list>
#include <string>
#include <memory>
#include <functional>
#include "ClassType.h"

class IMessage
{
public:
	virtual ~IMessage() {}
	virtual int GetMessageID() const = 0;
};

using MessagePtr = std::shared_ptr<IMessage>;

template<typename T>
class MessageBase : public ClassType<T, IMessage>, public IMessage
{
public:
	static std::shared_ptr<T> Create();
	virtual ~MessageBase() {}
	virtual int GetMessageID() const;
};

template<typename T>
std::shared_ptr<T> MessageBase<T>::Create()
{
	return std::shared_ptr<T>(new T());
}

template<typename T>
int MessageBase<T>::GetMessageID() const
{
	return MessageBase<T>::GetClassType();
}

#endif // !__MESSAGE_BASE_H_H__