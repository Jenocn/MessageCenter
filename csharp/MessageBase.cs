/*
	By Jenocn
	https://jenocn.github.io/
*/

/// <summary>
/// 消息类型接口
/// </summary>
public interface IMessage {
	int GetMessageID();
}

/// <summary>
/// 消息基类
/// </summary>
public class MessageBase<T> : ClassType<T, IMessage>, IMessage where T : MessageBase<T> {
	public int GetMessageID() {
		return GetClassType();
	}
}