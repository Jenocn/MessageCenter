/*
	By Jenocn
	https://jenocn.github.io/
*/

#include "MessageCenter.h"

MessageDispatcher MessageCenter::_messageDispatcher; 

void MessageCenter::Send(std::shared_ptr<IMessage> message) {
	_messageDispatcher.Send(message);
}

void MessageCenter::Clear() {
	_messageDispatcher.Clear();
}
