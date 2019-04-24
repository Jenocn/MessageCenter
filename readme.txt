如果只是使用,只需要关心两个类
MessageCenter用于发消息和添加监听者
MessageBase<T>作为基类让你的消息继承它

//
#include "MessageCenter.h"

//
class YourMessage : public MessageBase<YourMessage>
{
public:
	// some parameters
	std::string param;
};

//
auto msg = YourMessage::Create();
// msg->param = "some";
// 发送立即生效
MessageCenter::Send(msg);
// 或者入队
MessageCenter::Push(msg);
// 等待OnDispatch派发
MessageCenter::OnDispatch();

//
MessageCenter::AddListener<YourMessage>("yourListener", [](std::shared_ptr<YourMessage> msg){
	// use msg;
	msg->param;
});