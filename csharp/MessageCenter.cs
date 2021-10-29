/*
	By Jenocn
	https://jenocn.github.io/
*/

using System.Collections.Generic;

/// <summary>
/// 消息中心,一个全局消息派发器
/// </summary>
public static class MessageCenter {
	public static MessageDispatcher messageDispatcher { get => _messageDispatcher; }
	private static MessageDispatcher _messageDispatcher = new MessageDispatcher();
	private static Dictionary<string, MessageDispatcher> _dispathcerDict = new Dictionary<string, MessageDispatcher>();

	/// <summary>
	/// 派发队列中的消息
	/// <para>若要使用Push功能则需要在一个Update中主动调用此方法</para>
	/// </summary>
	public static void OnDispatch() {
		_messageDispatcher.OnDispatch();
		foreach (var item in _dispathcerDict) {
			item.Value.OnDispatch();
		}
	}

	/// <summary>
	/// 获取指定名称的自定义消息派发器
	/// <para>如果不存在则会创建一个新的,无论如何都会返回有效结果</para>
	/// </summary>
	public static MessageDispatcher GetCustomDispatcher(string name) {
		if (!_dispathcerDict.TryGetValue(name, out var ret)) {
			ret = new MessageDispatcher();
			_dispathcerDict.Add(name, ret);
		}
		return ret;
	}

	/// <summary>
	/// 向中心派发器添加消息监听者
	/// </summary>
	public static void AddListener<T>(object obj, System.Action<T> func) where T : MessageBase<T> {
		_messageDispatcher.AddListener(obj, func);
	}

	/// <summary>
	/// 从中心派发器删除监听者
	/// </summary>
	public static void RemoveListener<T>(object obj) where T : MessageBase<T> {
		_messageDispatcher.RemoveListener<T>(obj);
	}

	/// <summary>
	/// 中心派发器立即发送消息
	/// </summary>
	public static void Send(IMessage message) {
		_messageDispatcher.Send(message);
	}

	/// <summary>
	/// 入队消息到中心派发器,待OnDispathch时发送
	/// </summary>
	public static void Push(IMessage message) {
		_messageDispatcher.Push(message);
	}

	/// <summary>
	/// 清空中心派发器的消息和监听者
	/// </summary>
	public static void Clear() {
		_messageDispatcher.Clear();
	}
}