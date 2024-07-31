/*
	By Jenocn
	https://jenocn.github.io/
*/

#pragma once

#include "MessageDispatcher.h"

class MessageCenter {
public:
	template<typename _Ty>
	static void AddListener(const void* sender, std::function<void(std::shared_ptr<_Ty>)> func);
	template<typename _Ty>
	static void RemoveListener(const void* sender);
	static void Send(std::shared_ptr<IMessage> message);
	static void Push(std::shared_ptr<IMessage> message);
	static void Dispatch();
	static void Clear();
private:
	static MessageDispatcher _messageDispatcher;
};

template<typename _Ty>
void MessageCenter::AddListener(const void* sender, std::function<void(std::shared_ptr<_Ty>)> func) {
	_messageDispatcher.AddListener<_Ty>(sender, func);
}

template<typename _Ty>
void MessageCenter::RemoveListener(const void* sender) {
	_messageDispatcher.RemoveListener<_Ty>(sender);
}
