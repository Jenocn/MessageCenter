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
YourMessage msg;
msg.param = "some";
```

```C++
// 发送消息
MessageCenter::Send(msg);
```

```C++
// 添加监听
MessageCenter::AddListener<YourMessage>(this, [](const YourMessage& msg){
	// 使用参数
	std::cout << msg.param << std::endl;
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