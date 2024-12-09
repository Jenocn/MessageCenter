/*
	By Jenocn
	https://jenocn.github.io/
*/

#pragma once

#include "Type.h"

namespace Message {

struct IListener {
	virtual ~IListener() = default;
	virtual const std::type_index& GetType() const = 0;
};

template<typename _Ty>
class Listener final: public IListener {
public:
	Listener(std::function<void(const _Ty&)> func): _func(func) {}

	void Invoke(const _Ty& message) { _func(message); }

	virtual const std::type_index& GetType() const override { return Type<_Ty>::TYPE; }
private:
	std::function<void(const _Ty&)> _func;
};

}