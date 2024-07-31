#include "MessageTypeId.h"

std::size_t _MessageTypeCounter::_count = 0;

_MessageTypeCounter::_MessageTypeCounter()
    : index(++_count)
{
}