/*
	By Jenocn
	https://jenocn.github.io/
*/

#include "MessageDispatcher.h"

void MessageDispatcher::Send(std::shared_ptr<IMessage> message) {
	if (!message) {
		assert(!"MessageDispatcher::Send: Message is null!");
		return;
	}

	std::size_t messageId = message->GetMessageId();
	auto listenerIte = _listenerMap.find(messageId);
	if (listenerIte == _listenerMap.end() || listenerIte->second.empty()) {
		return;
	}

	if (_messageInvokePool.find(messageId) != _messageInvokePool.end()) {
		assert(!("MessageDispatcher::Send: Message is recursive send! -> " + std::string(typeid(*message).name())).c_str());
		return;
	}

	_messageInvokePool.emplace(messageId);
	for (auto& pair : listenerIte->second) {
		pair.second->Invoke(message);
	}
	_messageInvokePool.erase(messageId);

	if (!_messageInvokePool.empty() || _listenerCacheQueue.empty()) {
		return;
	}

	for (auto& item : _listenerCacheQueue) {
		auto listener = std::get<2>(item);
		if (listener) {
			_AddListener(std::get<0>(item), std::get<1>(item), listener);
		} else {
			_RemoveListener(std::get<0>(item), std::get<1>(item));
		}
	}
	_listenerCacheQueue.clear();
}

void MessageDispatcher::Clear() {
	if (!_messageInvokePool.empty()) {
		assert(!"MessageDispatcher::Clear: Cannot perform this operation while processing the message.");
		return;
	}
	for (auto& pair : _listenerMap) {
		for (auto& kv : pair.second) {
			delete kv.second;
		}
	}
	_listenerMap.clear();
}

void MessageDispatcher::_AddListener(std::size_t messageId, std::size_t senderKey, IMessageListener* listener) {
	auto& tmpMap = _listenerMap[messageId];
	auto ite = tmpMap.find(senderKey);
	if (ite != tmpMap.end()) {
		delete listener;
		assert(!"MessageDispatcher::AddListener: The sender is exist!");
		return;
	}
	tmpMap.emplace(senderKey, listener);
}
void MessageDispatcher::_RemoveListener(std::size_t messageId, std::size_t senderKey) {
	auto ite = _listenerMap.find(messageId);
	if (ite == _listenerMap.end()) {
		return;
	}
	auto iteSender = ite->second.find(senderKey);
	if (iteSender!= ite->second.end()) {
		delete iteSender->second;
		ite->second.erase(iteSender);
	}
}