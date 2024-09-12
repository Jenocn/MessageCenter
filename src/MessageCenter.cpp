/*
	By Jenocn
	https://jenocn.github.io/
*/

#include "MessageCenter.h"

using namespace Message;

MessageDispatcher MessageCenter::_messageDispatcher;

void MessageCenter::Send(const IMessage& message) {
	_messageDispatcher.Send(message);
}

void MessageCenter::Clear() {
	_messageDispatcher.Clear();
}
