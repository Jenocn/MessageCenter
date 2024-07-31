/*
    By Jenocn
    https://jenocn.github.io/
*/

#pragma once

#include "MessageTypeId.h"

class IMessage {
public:
    virtual ~IMessage() = default;
    virtual int GetMessageId() const = 0;
};

template <typename _Ty>
class MessageBase : public IMessage {
public:
    virtual ~MessageBase() = default;
    virtual int GetMessageId() const;

    static std::shared_ptr<_Ty> Create();
};

template <typename _Ty>
int MessageBase<_Ty>::GetMessageId() const {
    return MessageTypeId<_Ty>::id.index;
}

template <typename _Ty>
std::shared_ptr<_Ty> MessageBase<_Ty>::Create() {
    return std::shared_ptr<_Ty>(new _Ty);
}
