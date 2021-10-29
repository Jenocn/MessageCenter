/*
	By Jenocn
	https://jenocn.github.io/
*/

#ifndef __MESSAGE_CENTER_H_H__
#define __MESSAGE_CENTER_H_H__

#include "MessageListener.h"
#include "MessageDispatcher.h"

class MessageCenter
{
public:
	template<typename T>
	static void AddListener(const std::string& name, std::function<void(std::shared_ptr<T> message)> func);
	template<typename T>
	static void RemoveListener(const std::string& name);
	static void Send(MessagePtr message);
	static void Push(MessagePtr message);
	static void OnDispatch();
	static void Clear();
private:
	static MessageDispatcher* _messageDispatcher;
};

template<typename T>
void MessageCenter::AddListener(const std::string& name, std::function<void(std::shared_ptr<T> message)> func)
{
	_messageDispatcher->AddListener<T>(name, func);
}

template<typename T>
void MessageCenter::RemoveListener(const std::string& name)
{
	_messageDispatcher->RemoveListener<T>(name);
}

#endif // !__MESSAGE_CENTER_H_H__
