# 使用方法

```C++
// 包含头文件
#include "Dispatcher.h"
// 应用命名空间
using namespace Message;
```
```C++
// 定义任意结构的消息, 并发送
int int_msg { 10 };
MessageCenter::Send(int_msg);

//
std::string str_msg { "hello" };
MessageCenter::Send(str_msg);

//
struct Custom {
	int value;
};
Custom custom_msg;
MessageCenter::Send(custom_msg);

// ...
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
