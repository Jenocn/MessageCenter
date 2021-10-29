/*
	By Jenocn
	https://jenocn.github.io/
*/

#ifndef __MESSAGE_DISPATCHER_H_H__
#define __MESSAGE_DISPATCHER_H_H__

#include "MessageListener.h"

class MessageDispatcher
{
public:
	template<typename T>
	void AddListener(const std::string& name, std::function<void(std::shared_ptr<T> message)> func);
	template<typename T>
	void RemoveListener(const std::string& name);
	void Send(MessagePtr message);
	void Push(MessagePtr message);
	void OnDispatch();
	void Clear();
	bool IsSafeLocked() const;
private:
	std::list<MessagePtr>& _GetActiveQueue();
	std::list<MessagePtr>& _GetIdleQueue();
	void _SafeLock();
	void _SafeUnlock();
	void _ExecuteSafeQueue();
private:
	std::map<int, std::map<std::string, IMessageListener*>> _listenerMap;
	std::map<bool, std::list<MessagePtr>> _messageQueue;
	std::list<std::tuple<bool, int, std::string, IMessageListener*>> _safeListenerQueue;
	int _lockCount{ 0 };
	bool _activeQueueSign{ false };
};

template<typename T>
void MessageDispatcher::AddListener(const std::string& name, std::function<void(std::shared_ptr<T> message)> func) {
	if (name.empty()) { return; }
	if (func == nullptr) { return; }
	int messageID = T::GetClassType();
	auto& lisMap = _listenerMap[messageID];
	if (lisMap.find(name) != lisMap.end()) { return; }
	auto listener = new MessageListener<T>(func);
	if (IsSafeLocked()) {
		_safeListenerQueue.emplace_back(std::make_tuple(true, messageID, name, listener));
	} else {
		lisMap.emplace(name, listener);
	}
}
template<typename T>
void MessageDispatcher::RemoveListener(const std::string& name) {
	auto messageID = T::GetClassType();
	auto& lisMap = _listenerMap[messageID];
	auto ite = lisMap.find(name);
	if (ite != lisMap.end()) {
		if (IsSafeLocked()) {
			_safeListenerQueue.emplace_back(std::make_tuple(false, messageID, name, nullptr));
		} else {
			delete ite->second;
			lisMap.erase(ite);
		}
	}
}

#endif // !__MESSAGE_DISPATCHER_H_H__
