/*
	Author: Jenocn
	Date: 2019-4-26
	Email:54467371@qq.com
*/

public interface IMessageListener {
	void Invoke(IMessage message);
	int GetMessageID();
}

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