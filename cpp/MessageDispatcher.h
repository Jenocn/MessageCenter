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
	void RemoveListener(const std::string& name);
	void Send(MessagePtr message);
	void Push(MessagePtr message);
	void OnDispatch();
	void Clear();
private:
	std::list<MessagePtr>& _GetActiveQueue();
	std::list<MessagePtr>& _GetIdleQueue();
private:
	std::map<std::string, IMessageListener*> _listenerMap;
	std::map<bool, std::list<MessagePtr>> _messageQueue;
	bool _activeQueueSign{ false };
};

template<typename T>
void MessageDispatcher::AddListener(const std::string& name, std::function<void(std::shared_ptr<T> message)> func)
{
	if (name.empty()) { return; }
	if (func == nullptr) { return; }
	if (_listenerMap.find(name) != _listenerMap.end()) { return; }
	auto listener = new MessageListener<T>(func);
	_listenerMap.emplace(name, listener);
}

#endif // !__MESSAGE_DISPATCHER_H_H__
