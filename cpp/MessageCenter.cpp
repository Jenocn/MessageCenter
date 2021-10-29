/*
	By Jenocn
	https://jenocn.github.io/
*/

#include "MessageCenter.h"
#include "MessageDispatcher.h"

MessageDispatcher* MessageCenter::_messageDispatcher = new MessageDispatcher();

void MessageCenter::Send(MessagePtr message)
{
	_messageDispatcher->Send(message);
}

void MessageCenter::Push(MessagePtr message)
{
	_messageDispatcher->Push(message);
}

void MessageCenter::OnDispatch()
{
	_messageDispatcher->OnDispatch();
}

void MessageCenter::Clear()
{
	_messageDispatcher->Clear();
}
