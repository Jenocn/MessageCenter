/*
	By Jenocn
	https://jenocn.github.io/
*/

#include "MessageDispatcher.h"

void MessageDispatcher::Send(std::shared_ptr<IMessage> message) {
	if (!message) {
		return;
	}
	int messageId = message->GetMessageId();
	for (auto& pair : _listenerMap[messageId]) {
		pair.second->Invoke(message);
	}
}

void MessageDispatcher::Push(std::shared_ptr<IMessage> message) {
	if (!message) {
		return;
	}
	_GetIdleQueue().emplace_back(message);
}

void MessageDispatcher::Dispatch() {
	_activeQueueIndex ^= 1;
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

std::list<std::shared_ptr<IMessage>>& MessageDispatcher::_GetActiveQueue() {
	return _messageQueue[_activeQueueIndex];
}

std::list<std::shared_ptr<IMessage>>& MessageDispatcher::_GetIdleQueue() {
	return _messageQueue[_activeQueueIndex ^ 1];
}
