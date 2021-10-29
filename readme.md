使用方法,只需要关心两个类
MessageCenter用于发消息和添加监听者
MessageBase作为基类让你的消息继承它

```C++
// 包含头文件
#include "MessageCenter.h"
```
```C++
// 定义消息
class YourMessage : public MessageBase<YourMessage>
{
public:
	// some parameters
	std::string param;
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
// 等待OnDispatch派发
MessageCenter::OnDispatch();
```
```C++
// 添加监听
MessageCenter::AddListener<YourMessage>("yourListener", [](std::shared_ptr<YourMessage> msg){
	// use msg;
	msg->param;
});
// 删除监听
MessageCenter::RemoveListener<YourMessage>("youListener");
```