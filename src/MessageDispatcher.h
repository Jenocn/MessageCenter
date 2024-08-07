/*
	By Jenocn
	https://jenocn.github.io/
*/

#pragma once

#include "MessageListener.h"

class MessageDispatcher {
public:
	template<typename _Ty>
	void AddListener(const void* sender, std::function<void(const _Ty*)> func);
	template<typename _Ty>
	void RemoveListener(const void* sender);
	void Send(std::shared_ptr<IMessage> message);
	void Clear();
private:
	void _AddListener(std::size_t messageId, std::uintptr_t senderKey, IMessageListener* listener);
	void _RemoveListener(std::size_t messageId, std::uintptr_t senderKey);
private:
	std::unordered_map<std::size_t, std::unordered_map<std::uintptr_t, IMessageListener*>> _listenerMap;
	std::list<std::tuple<std::size_t, std::uintptr_t, IMessageListener*>> _listenerCacheQueue;
	std::unordered_set<std::size_t> _messageInvokePool;
};

template<typename _Ty>
void MessageDispatcher::AddListener(const void* sender, std::function<void(const _Ty*)> func) {
	if (!sender) {
		std::cerr << "[Error] MessageDispatcher::AddListener: Sender is null!" << std::endl;
		return;
	}
	if (func == nullptr) {
		std::cerr << "[Error] MessageDispatcher::AddListener: Func is null!" << std::endl;
		return;
	}
	auto messageId = MessageTypeId<_Ty>::id.index;
	auto senderKey = reinterpret_cast<std::uintptr_t>(sender);
	if (!_messageInvokePool.empty()) {
		_listenerCacheQueue.emplace_back(std::tuple<std::size_t, std::uintptr_t, IMessageListener*>(
			messageId, senderKey, new MessageListener<_Ty>(func)));
		return;
	}

	auto& lisMap = _listenerMap[messageId];
	if (lisMap.find(senderKey) != lisMap.end()) {
		std::cerr << "[Error] MessageDispatcher::AddListener: sender is exist!" << std::endl;
		return;
	}
	lisMap.emplace(senderKey, new MessageListener<_Ty>(func));
}

template<typename _Ty>
void MessageDispatcher::RemoveListener(const void* sender) {
	if (!sender) {
		std::cerr << "[Error] MessageDispatcher::RemoveListener: Sender is null!" << std::endl;
		return;
	}
	auto messageId = MessageTypeId<_Ty>::id.index;
	auto senderKey = reinterpret_cast<std::uintptr_t>(sender);

	if (!_messageInvokePool.empty()) {
		_listenerCacheQueue.emplace_back(std::tuple<std::size_t, std::uintptr_t, IMessageListener*>(
			messageId, senderKey, nullptr));
		return;
	}

	_RemoveListener(messageId, senderKey);
}

