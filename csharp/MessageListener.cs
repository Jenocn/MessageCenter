/*
	By Jenocn
	https://jenocn.github.io/
*/

/// <summary>
/// 消息监听者接口
/// </summary>
public interface IMessageListener {
	void Invoke(IMessage message);
	int GetMessageID();
}

/// <summary>
/// 消息监听者
/// </summary>
/// <typeparam name="T"></typeparam>
public sealed class MessageListener<T> : IMessageListener where T : MessageBase<T> {
	private System.Action<T> _func = null;
	public MessageListener(System.Action<T> func) {
		_func = func;
	}
	public int GetMessageID() {
		return MessageBase<T>.GetClassType();
	}

	public void Invoke(IMessage message) {
		_func.Invoke(message as T);
	}
}