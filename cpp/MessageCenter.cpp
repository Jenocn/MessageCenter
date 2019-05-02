#include "MessageCenter.h"

std::map<std::string, IMessageListener*> MessageCenter::_listenerMap;
std::map<bool, std::list<MessagePtr>> MessageCenter::_messageQueue;
bool MessageCenter::_activeQueueSign = false;

void MessageCenter::RemoveListener(const std::string& name)
{
	auto ite = _listenerMap.find(name);
	if (ite == _listenerMap.end()) { return; }
	delete ite->second;
	_listenerMap.erase(ite);
}

void MessageCenter::Send(MessagePtr message)
{
	if (!message) { return; }
	int messageID = message->GetMessageID();
	for (auto& pair : _listenerMap)
	{
		auto listener = pair.second;
		if (listener->GetMessageID() == messageID)
		{
			listener->Invoke(message);
		}
	}
}

void MessageCenter::Push(MessagePtr message)
{
	if (!message) { return; }
	_GetIdleQueue().emplace_back(message);
}

void MessageCenter::OnDispatch()
{
	_activeQueueSign = !_activeQueueSign;

	for (auto message : _GetActiveQueue())
	{
		Send(message);
	}
	_GetActiveQueue().clear();
}

void MessageCenter::Release()
{
	for (auto& pair : _listenerMap)
	{
		delete pair.second;
	}
	_listenerMap.clear();
	_GetActiveQueue().clear();
	_GetIdleQueue().clear();
}

std::list<MessagePtr>& MessageCenter::_GetActiveQueue()
{
	return _messageQueue[_activeQueueSign];
}

std::list<MessagePtr>& MessageCenter::_GetIdleQueue()
{
	return _messageQueue[!_activeQueueSign];
}