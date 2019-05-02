/*
	Author: Jenocn
	Date: 2019-4-23
	Email:54467371@qq.com
*/

#ifndef __MESSAGE_CENTER_H_H__
#define __MESSAGE_CENTER_H_H__

#include "MessageListener.h"

class MessageCenter
{
public:
	template<typename T>
	static void AddListener(const std::string& name, std::function<void(std::shared_ptr<T> message)> func);
	static void RemoveListener(const std::string& name);
	static void Send(MessagePtr message);
	static void Push(MessagePtr message);
	static void OnDispatch();
	static void Release();
private:
	static std::list<MessagePtr>& _GetActiveQueue();
	static std::list<MessagePtr>& _GetIdleQueue();
private:
	static std::map<std::string, IMessageListener*> _listenerMap;
	static std::map<bool, std::list<MessagePtr>> _messageQueue;
	static bool _activeQueueSign;
};

template<typename T>
void MessageCenter::AddListener(const std::string& name, std::function<void(std::shared_ptr<T> message)> func)
{
	if (name.empty()) { return; }
	if (func == nullptr) { return; }
	if (_listenerMap.find(name) != _listenerMap.end()) { return; }
	auto listener = new MessageListener<T>(func);
	_listenerMap.emplace(name, listener);
}

#endif // !__MESSAGE_CENTER_H_H__