# 使用方法

```C++
// 包含头文件
#include "MessageCenter.h"
```
```C++
// 定义消息
class YourMessage : public MessageBase<YourMessage>
{
public:
	// 一些参数
	std::string param;
	// ...
};
```

```C++
// 创建消息
auto msg = YourMessage::Create();
```

```C++
// msg->param = "some";
// 发送立即生效
MessageCenter::Send(msg);
```

```C++
// 或者入队
MessageCenter::Push(msg);
// 调用Dispatch派发
MessageCenter::Dispatch();
```
```C++
// 添加监听
MessageCenter::AddListener<YourMessage>(this, [](std::shared_ptr<YourMessage> msg){
	// 使用参数
	msg->param;
});
// 删除监听
MessageCenter::RemoveListener<YourMessage>(this);
```

```C++
// 使用自定义的消息派发器
// 方法和MessageCenter相同
MessageDispatcher dispather;
dispather.AddListener<YourMessage>(....);
dispather.Send(....);
dispather.RemoveListener<YourMessage>(....);
// ......
```