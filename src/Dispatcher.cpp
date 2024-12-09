/*
	By Jenocn
	https://jenocn.github.io/
*/

#include "Dispatcher.h"
#include <cassert>
#include <iostream>

using namespace Message;

void Dispatcher::Clear() {
	if (!_message_invoke_pool.empty()) {
		_Assert("Cannot perform this operation while processing the message.", "Clear", "");
		return;
	}
	_listener_map.clear();
}

void Dispatcher::_AddListener(std::size_t message_code, std::intptr_t sender_key, std::unique_ptr<IListener> listener) {
	auto& tmp_map { _listener_map[message_code] };
	auto ite { tmp_map.find(sender_key) };
	if (ite != tmp_map.end()) {
		_Assert("Sender is exist!", "AddListener", listener->GetType().name());
		return;
	}
	tmp_map.emplace(sender_key, std::move(listener));
}
void Dispatcher::_RemoveListener(std::size_t message_code, std::intptr_t sender_key) {
	auto ite { _listener_map.find(message_code) };
	if (ite == _listener_map.end()) {
		return;
	}
	auto sender_ite { ite->second.find(sender_key) };
	if (sender_ite != ite->second.end()) {
		ite->second.erase(sender_ite);
	}
}
void Dispatcher::_Assert(const char* info, const char* call, const char* msg_name) {
	std::cerr << "Error:" << std::endl;
	std::cerr << "Info: " << info << std::endl;
	std::cerr << "Call: " << call << std::endl;
	std::cerr << "Name: " << msg_name << std::endl;
	assert(false);
}
