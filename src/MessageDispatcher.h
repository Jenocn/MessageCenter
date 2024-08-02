/*
	By Jenocn
	https://jenocn.github.io/
*/

#pragma once

#include "MessageListener.h"

class MessageDispatcher {
public:
	template<typename _Ty>
	void AddListener(const void* sender, std::function<void(std::shared_ptr<_Ty>)> func);
	template<typename _Ty>
	void RemoveListener(const void* sender);
	void Send(std::shared_ptr<IMessage> message);
	void Clear();
private:
	void _AddListener(std::size_t messageId, std::size_t senderKey, IMessageListener* listener);
	void _RemoveListener(std::size_t messageId, std::size_t senderKey);
private:
	std::unordered_map<std::size_t, std::unordered_map<std::uintptr_t, IMessageListener*>> _listenerMap;
	std::list<std::tuple<std::size_t, std::size_t, IMessageListener*>> _listenerCacheQueue;
	std::unordered_set<std::size_t> _messageInvokePool;
};

template<typename _Ty>
void MessageDispatcher::AddListener(const void* sender, std::function<void(std::shared_ptr<_Ty>)> func) {
	if (!sender) {
		assert(!"MessageDispatcher::AddListener: Sender is null!");
		return;
	}
	if (func == nullptr) {
		assert(!"MessageDispatcher::AddListener: Func is null!");
		return;
	}
	auto messageId = MessageTypeId<_Ty>::id.index;
	auto senderKey = static_cast<std::size_t>(reinterpret_cast<std::uintptr_t>(sender));
	if (!_messageInvokePool.empty()) {
		_listenerCacheQueue.emplace_back(std::tuple<std::size_t, std::size_t, IMessageListener*>(
			messageId, senderKey, new MessageListener<_Ty>(func)));
		return;
	}

	auto& lisMap = _listenerMap[messageId];
	if (lisMap.find(senderKey) != lisMap.end()) {
		assert(!"MessageDispatcher::AddListener: sender is exist!");
		return;
	}
	auto listener = new MessageListener<_Ty>(func);
	lisMap.emplace(senderKey, listener);
}

template<typename _Ty>
void MessageDispatcher::RemoveListener(const void* sender) {
	if (!sender) {
		assert(!"MessageDispatcher::RemoveListener: Sender is null!");
		return;
	}
	auto messageId = MessageTypeId<_Ty>::id.index;
	auto senderKey = static_cast<std::size_t>(reinterpret_cast<std::uintptr_t>(sender));

	if (!_messageInvokePool.empty()) {
		_listenerCacheQueue.emplace_back(std::tuple<std::size_t, std::size_t, IMessageListener*>(
			messageId, senderKey, nullptr));
		return;
	}

	_RemoveListener(messageId, senderKey);
}

