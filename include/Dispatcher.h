/*
	By Jenocn
	https://jenocn.github.io/
*/

#pragma once

#include "Listener.h"

namespace Message {

class Dispatcher {
public:
	template<typename _Ty>
	void AddListener(const void* sender, std::function<void(const _Ty&)> func);
	template<typename _Ty>
	void RemoveListener(const void* sender);
	template <typename _Ty>
	void Send(const _Ty& message);
	void Clear();
private:
	void _AddListener(std::size_t message_code, std::intptr_t sender_key, std::unique_ptr<IListener> listener);
	void _RemoveListener(std::size_t message_code, std::intptr_t sender_key);
	void _Assert(const char* msg_info, const char* call_info, const char* msg_name);
private:
	std::unordered_map<std::size_t, std::unordered_map<std::intptr_t, std::unique_ptr<IListener>>> _listener_map;
	std::deque<std::tuple<std::size_t, std::intptr_t, std::unique_ptr<IListener>>> _listener_cache_queue;
	std::unordered_set<std::size_t> _message_invoke_pool;
};

template<typename _Ty>
void Dispatcher::AddListener(const void* sender, std::function<void(const _Ty&)> func) {
	if (!sender) {
		_Assert("Sender is null!", "AddListener", typeid(_Ty).name());
		return;
	}
	if (!func) {
		_Assert("Func is null!", "AddListener", typeid(_Ty).name());
		return;
	}
	auto message_code { Type<_Ty>::TYPE_CODE };
	auto sender_key { reinterpret_cast<std::intptr_t>(sender) };
	if (!_message_invoke_pool.empty()) {
		_listener_cache_queue.emplace_back(std::make_tuple(message_code, sender_key, std::make_unique<Listener<_Ty>>(func)));
		return;
	}

	auto& lis_map { _listener_map[message_code] };
	if (lis_map.find(sender_key) != lis_map.end()) {
		_Assert("Sender is exist!", "AddListener", typeid(_Ty).name());
		return;
	}
	lis_map.emplace(sender_key, std::make_unique<Listener<_Ty>>(func));
}

template<typename _Ty>
void Dispatcher::RemoveListener(const void* sender) {
	if (!sender) {
		_Assert("Sender is null!", "AddListener", typeid(_Ty).name());
		return;
	}
	auto message_code { Type<_Ty>::TYPE_CODE };
	auto sender_key { reinterpret_cast<std::intptr_t>(sender) };

	if (!_message_invoke_pool.empty()) {
		_listener_cache_queue.emplace_back(std::make_tuple(message_code, sender_key, nullptr));
		return;
	}

	_RemoveListener(message_code, sender_key);
}

template <typename _Ty>
void Dispatcher::Send(const _Ty& message) {
	std::size_t message_code { Type<_Ty>::TYPE_CODE };
	auto listener_ite { _listener_map.find(message_code) };
	if (listener_ite == _listener_map.end() || listener_ite->second.empty()) {
		return;
	}

	if (_message_invoke_pool.find(message_code) != _message_invoke_pool.end()) {
		_Assert("Type is recursive send!", "Send", Type<_Ty>::TYPE.name());
		return;
	}

	_message_invoke_pool.emplace(message_code);
	for (auto& pair : listener_ite->second) {
		auto listener { static_cast<Listener<_Ty>*>(pair.second.get()) };
		listener->Invoke(message);
	}
	_message_invoke_pool.erase(message_code);

	if (!_message_invoke_pool.empty() || _listener_cache_queue.empty()) {
		return;
	}

	for (auto& item : _listener_cache_queue) {
		auto listener { std::move(std::get<2>(item)) };
		if (listener) {
			_AddListener(std::get<0>(item), std::get<1>(item), std::move(listener));
		} else {
			_RemoveListener(std::get<0>(item), std::get<1>(item));
		}
	}
	_listener_cache_queue.clear();
}

}