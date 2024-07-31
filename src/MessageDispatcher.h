/*
	By Jenocn
	https://jenocn.github.io/
*/

#pragma once

#include "MessageListener.h"

class MessageDispatcher
{
public:
	template<typename _Ty>
	void AddListener(const void* sender, std::function<void(std::shared_ptr<_Ty>)> func);
	template<typename _Ty>
	void RemoveListener(const void* sender);
	void Send(std::shared_ptr<IMessage> message);
	void Push(std::shared_ptr<IMessage> message);
	void Dispatch();
	void Clear();
private:
	std::list<std::shared_ptr<IMessage>>& _GetActiveQueue();
	std::list<std::shared_ptr<IMessage>>& _GetIdleQueue();
private:
	std::unordered_map<std::size_t, std::unordered_map<std::uintptr_t, IMessageListener*>> _listenerMap;
	std::array<std::list<std::shared_ptr<IMessage>>, 2> _messageQueue;
	std::size_t _activeQueueIndex{0};
};

template<typename _Ty>
void MessageDispatcher::AddListener(const void* sender, std::function<void(std::shared_ptr<_Ty>)> func) {
	if (!sender) { return; }
	if (func == nullptr) { return; }
	auto senderKey = reinterpret_cast<std::uintptr_t>(sender);
	auto messageId = MessageTypeId<_Ty>::id.index;
	auto& lisMap = _listenerMap[messageId];
	if (lisMap.find(senderKey) != lisMap.end()) {
		std::cerr << "MessageDispatcher.AddListener: sender is exist!" << std::endl; 
		return;
	}
	auto listener = new MessageListener<_Ty>(func);
	lisMap.emplace(senderKey, listener);
}
template<typename _Ty>
void MessageDispatcher::RemoveListener(const void* sender) {
	auto messageId = MessageTypeId<_Ty>::id.index;
	auto& lisMap = _listenerMap[messageId];
	auto senderKey = reinterpret_cast<std::uintptr_t>(sender);
	auto ite = lisMap.find(senderKey);
	if (ite != lisMap.end()) {
		delete ite->second;
		lisMap.erase(ite);
	}
}

