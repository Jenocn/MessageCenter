/*
	By Jenocn
	https://jenocn.github.io/
*/

#include "MessageDispatcher.h"

void MessageDispatcher::Send(MessagePtr message) {
	if (!message) {
		return;
	}
	int messageID = message->GetMessageID();

	_SafeLock();
	for (auto& pair : _listenerMap[messageID]) {
		pair.second->Invoke(message);
	}
	_SafeUnlock();

	if (!IsSafeLocked()) {
		_ExecuteSafeQueue();
	}
}

void MessageDispatcher::Push(MessagePtr message) {
	if (!message) {
		return;
	}
	_GetIdleQueue().emplace_back(message);
}

void MessageDispatcher::OnDispatch() {
	_activeQueueSign = !_activeQueueSign;

	for (auto message : _GetActiveQueue()) {
		Send(message);
	}
	_GetActiveQueue().clear();
}

void MessageDispatcher::Clear() {
	for (auto& pair : _listenerMap) {
		for (auto& kv : pair.second) {
			delete kv.second;
		}
	}
	_listenerMap.clear();
	_GetActiveQueue().clear();
	_GetIdleQueue().clear();
}

bool MessageDispatcher::IsSafeLocked() const {
	return _lockCount > 0;
}

std::list<MessagePtr>& MessageDispatcher::_GetActiveQueue() {
	return _messageQueue[_activeQueueSign];
}

std::list<MessagePtr>& MessageDispatcher::_GetIdleQueue() {
	return _messageQueue[!_activeQueueSign];
}

void MessageDispatcher::_SafeLock() {
	++_lockCount;
}

void MessageDispatcher::_SafeUnlock() {
	--_lockCount;
}

void MessageDispatcher::_ExecuteSafeQueue() {
	auto ite = _safeListenerQueue.begin();
	while (ite != _safeListenerQueue.end()) {
		const auto& data = (*ite);
		auto bAdd = std::get<0>(data);
		auto messageType = std::get<1>(data);
		const auto& sender = std::get<2>(data);
		auto& tempMap = _listenerMap[messageType];
		if (bAdd) {
			auto listener = std::get<3>(data);
			if (tempMap.find(sender) == tempMap.end()) {
				tempMap.emplace(sender, listener);
			} else {
				delete listener;
			}
		} else {
			auto rmIte = tempMap.find(sender);
			if (rmIte != tempMap.end()) {
				delete rmIte->second;
				tempMap.erase(rmIte);
			}
		}
		++ite;
	}
	_safeListenerQueue.clear();
}