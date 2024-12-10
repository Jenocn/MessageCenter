# 使用方法

```C++
// 包含头文件
#include "Message.h"

// 实例化一个派发器
Message::Dispatcher dispatcher;

// 定义消息
struct Custom {
	int value;
};

// 添加监听
dispatcher.AddListener<Custom>(this, [](const int& msg){
	// 使用参数
	std::cout << msg.param << std::endl;
});
// 删除监听
dispatcher.RemoveListener<Custom>(this);

// 发送消息
Custom custom_msg;
dispatcher.Send(custom_msg);
```

```C++
// 支持任意结构的消息
int int_msg { 10 };
dispatcher.Send(int_msg);

std::string str_msg { "hello" };
dispatcher.Send(str_msg);
```
