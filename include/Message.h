/*
	By Jenocn
	https://jenocn.github.io/
*/

#pragma once

#include <typeindex>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <tuple>
#include <deque>

// debug config
#ifndef NDEBUG
#include <cassert>
#include <iostream>

#define MESSAGE_ASSERT(__INFO__, __CALL__, __MSG__) do {\
	std::cerr << "Message Error: " << __INFO__ << " / " << __CALL__ << " / " << __MSG__ << std::endl;\
	assert(false);\
} while(false)

#else
#define MESSAGE_ASSERT(__INFO__, __CALL__, __MSG__) (void(0))
#endif

#pragma warning(push)
#pragma warning(disable: 4514)

namespace Message {

class Dispatcher {
private:
	template <typename T>
	struct Type final {
		static const std::type_index TYPE;
		static const std::size_t TYPE_CODE;
	};

	struct IListener {
		virtual ~IListener() = default;
		virtual const std::type_index& GetType() const = 0;
	};

	template<typename _Ty>
	struct Listener final: public IListener {
	public:
		Listener(std::function<void(const _Ty&)> func): call(func) {}

		virtual const std::type_index& GetType() const override { return Type<_Ty>::TYPE; }

		std::function<void(const _Ty&)> call;
	};

public:
	//
	template<typename _Ty>
	void AddListener(const void* sender, std::function<void(const _Ty&)> func) {
		if (!sender) {
			MESSAGE_ASSERT("Sender is null!", "AddListener", typeid(_Ty).name());
			return;
		}
		if (!func) {
			MESSAGE_ASSERT("Func is null!", "AddListener", typeid(_Ty).name());
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
			MESSAGE_ASSERT("Sender is exist!", "AddListener", typeid(_Ty).name());
			return;
		}
		lis_map.emplace(sender_key, std::make_unique<Listener<_Ty>>(func));
	}

	//
	template<typename _Ty>
	void RemoveListener(const void* sender) {
		if (!sender) {
			MESSAGE_ASSERT("Sender is null!", "AddListener", typeid(_Ty).name());
			return;
		}
		auto message_code { Type<_Ty>::TYPE_CODE };
		auto sender_key { reinterpret_cast<std::intptr_t>(sender) };

		if (!_message_invoke_pool.empty()) {
			_listener_cache_queue.emplace_back(std::make_tuple(message_code, sender_key, nullptr));
			return;
		}

		_listener_map[message_code].erase(sender_key);
	}

	//
	template <typename _Ty>
	void Send(const _Ty& message) {
		std::size_t message_code { Type<_Ty>::TYPE_CODE };
		auto listener_ite { _listener_map.find(message_code) };
		if (listener_ite == _listener_map.end() || listener_ite->second.empty()) {
			return;
		}

		if (_message_invoke_pool.find(message_code) != _message_invoke_pool.end()) {
			MESSAGE_ASSERT("Type is recursive send!", "Send", Type<_Ty>::TYPE.name());
			return;
		}

		_message_invoke_pool.emplace(message_code);
		for (auto& pair : listener_ite->second) {
			static_cast<Listener<_Ty>*>(pair.second.get())->call(message);
		}
		_message_invoke_pool.erase(message_code);

		if (!_message_invoke_pool.empty() || _listener_cache_queue.empty()) {
			return;
		}

		for (auto& [item_msg_code, item_sender_key, item_listener] : _listener_cache_queue) {
			if (item_listener) {
				auto& tmp_map { _listener_map[item_msg_code] };
				if (tmp_map.count(item_sender_key)) {
					MESSAGE_ASSERT("Sender is exist!", "AddListener", item_listener->GetType().name());
					return;
				}
				tmp_map.emplace(item_sender_key, std::move(item_listener));
			} else {
				_listener_map[item_msg_code].erase(item_sender_key);
			}
		}
		_listener_cache_queue.clear();
	}

	//
	void Clear() {
		if (!_message_invoke_pool.empty()) {
			MESSAGE_ASSERT("Cannot perform this operation while processing the message.", "Clear", "");
			return;
		}
		_listener_map.clear();
	}

private:
	std::unordered_map<std::size_t, std::unordered_map<std::intptr_t, std::unique_ptr<IListener>>> _listener_map;
	std::deque<std::tuple<std::size_t, std::intptr_t, std::unique_ptr<IListener>>> _listener_cache_queue;
	std::unordered_set<std::size_t> _message_invoke_pool;
};

template <typename T>
const std::type_index Dispatcher::Type<T>::TYPE { typeid(T) };

template <typename T>
const std::size_t Dispatcher::Type<T>::TYPE_CODE { typeid(T).hash_code() };

}

#pragma warning(pop)