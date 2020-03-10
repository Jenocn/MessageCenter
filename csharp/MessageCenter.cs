/*
	By Jenocn
	https://jenocn.github.io/
*/

/// <summary>
/// 消息中心,一个全局消息派发器
/// </summary>
public static class MessageCenter {
	private static MessageDispatcher _messageDispatcher = new MessageDispatcher();

	/// <summary>
	/// 添加消息监听者
	/// </summary>
	public static void AddListener<T>(object obj, System.Action<T> func) where T : MessageBase<T> {
		_messageDispatcher.AddListener(obj, func);
	}

	/// <summary>
	/// 删除监听者
	/// </summary>
	public static void RemoveListener<T>(object obj) where T : MessageBase<T> {
		_messageDispatcher.RemoveListener<T>(obj);
	}

	/// <summary>
	/// 立即发送消息
	/// </summary>
	public static void Send(IMessage message) {
		_messageDispatcher.Send(message);
	}

	/// <summary>
	/// 入队消息,待OnDispathch时发送
	/// </summary>
	public static void Push(IMessage message) {
		_messageDispatcher.Push(message);
	}

	/// <summary>
	/// 派发队列中的消息
	/// </summary>
	public static void OnDispatch() {
		_messageDispatcher.OnDispatch();
	}

	/// <summary>
	/// 清空消息和监听者
	/// </summary>
	public static void Clear() {
		_messageDispatcher.Clear();
	}
}