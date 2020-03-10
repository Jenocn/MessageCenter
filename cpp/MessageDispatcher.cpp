/*
	By Jenocn
	https://jenocn.github.io/
*/

#include "MessageDispatcher.h"

void MessageDispatcher::RemoveListener(const std::string& name)
{
	auto ite = _listenerMap.find(name);
	if (ite == _listenerMap.end()) { return; }
	delete ite->second;
	_listenerMap.erase(ite);
}

void MessageDispatcher::Send(MessagePtr message)
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

void MessageDispatcher::Push(MessagePtr message)
{
	if (!message) { return; }
	_GetIdleQueue().emplace_back(message);
}

void MessageDispatcher::OnDispatch()
{
	_activeQueueSign = !_activeQueueSign;

	for (auto message : _GetActiveQueue())
	{
		Send(message);
	}
	_GetActiveQueue().clear();
}

void MessageDispatcher::Clear()
{
	for (auto& pair : _listenerMap)
	{
		delete pair.second;
	}
	_listenerMap.clear();
	_GetActiveQueue().clear();
	_GetIdleQueue().clear();
}

std::list<MessagePtr>& MessageDispatcher::_GetActiveQueue()
{
	return _messageQueue[_activeQueueSign];
}

std::list<MessagePtr>& MessageDispatcher::_GetIdleQueue()
{
	return _messageQueue[!_activeQueueSign];
}
