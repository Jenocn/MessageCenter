/*
	By Jenocn
	https://jenocn.github.io/
*/

using System.Collections.Generic;

/// <summary>
/// 消息派发器
/// </summary>
public class MessageDispatcher {
	private Dictionary<int, Dictionary<object, IMessageListener>> _listenerDict = new Dictionary<int, Dictionary<object, IMessageListener>>();
	private Dictionary<bool, LinkedList<IMessage>> _messageQueue = new Dictionary<bool, LinkedList<IMessage>>();

	// Tuple<添加/删除, MessageID, 对象, 监听器>
	private LinkedList<System.Tuple<bool, int, object, IMessageListener>> _safeListenerQueue = new LinkedList<System.Tuple<bool, int, object, IMessageListener>>();

	private int _lockCount = 0;
	private bool _activeQueueSign = false;

	public MessageDispatcher() {
		_messageQueue[true] = new LinkedList<IMessage>();
		_messageQueue[false] = new LinkedList<IMessage>();
	}

	/// <summary>
	/// 添加消息监听者
	/// </summary>
	public void AddListener<T>(object obj, System.Action<T> func) where T : MessageBase<T> {
		if (func != null) {
			var tType = MessageBase<T>.GetClassType();
			if (!_listenerDict.TryGetValue(tType, out var dict)) {
				dict = new Dictionary<object, IMessageListener>();
				_listenerDict.Add(tType, dict);
			}

			if (!dict.ContainsKey(obj)) {
				var listener = new MessageListener<T>(func);
				if (IsSafeLocked()) {
					_safeListenerQueue.AddLast(new System.Tuple<bool, int, object, IMessageListener>(true, tType, obj, listener));
				} else {
					dict.Add(obj, listener);
				}
			}
		}
	}

	/// <summary>
	/// 删除监听者
	/// </summary>
	public void RemoveListener<T>(object obj) where T : MessageBase<T> {
		var tType = MessageBase<T>.GetClassType();
		if (_listenerDict.TryGetValue(tType, out var dict)) {
			if (IsSafeLocked()) {
				if (dict.ContainsKey(obj)) {
					_safeListenerQueue.AddLast(new System.Tuple<bool, int, object, IMessageListener>(false, tType, obj, null));
				}
			} else {
				dict.Remove(obj);
			}
		}
	}

	/// <summary>
	/// 立即发送消息
	/// </summary>
	public void Send(IMessage message) {
		if (message == null) { return; }

		if (_listenerDict.TryGetValue(message.GetMessageID(), out var dict)) {
			_SafeLock();
			foreach (var item in dict) {
				item.Value.Invoke(message);
			}
			_SafeUnlock();

			if (!IsSafeLocked()) {
				_ExecuteSafeQueue();
			}
		}
	}

	/// <summary>
	/// 入队消息,待OnDispathch时发送
	/// </summary>
	public void Push(IMessage message) {
		if (message == null) {
			return;
		}
		_messageQueue[!_activeQueueSign].AddLast(message);
	}

	/// <summary>
	/// 派发队列中的消息
	/// </summary>
	public void OnDispatch() {
		_activeQueueSign = !_activeQueueSign;
		foreach (var message in _messageQueue[_activeQueueSign]) {
			Send(message);
		}
		_messageQueue[_activeQueueSign].Clear();
	}

	/// <summary>
	/// 清空消息和监听者
	/// </summary>
	public void Clear() {
		_listenerDict.Clear();
		_messageQueue[true].Clear();
		_messageQueue[false].Clear();
	}
	public bool IsSafeLocked() {
		return _lockCount > 0;
	}

	private void _SafeLock() {
		++_lockCount;
	}
	private void _SafeUnlock() {
		--_lockCount;
	}

	private void _ExecuteSafeQueue() {
		var p = _safeListenerQueue.First;
		while (p != null) {
			var data = p.Value;
			if (data.Item1) {
				if (!_listenerDict.TryGetValue(data.Item2, out var dict)) {
					dict = new Dictionary<object, IMessageListener>();
					_listenerDict.Add(data.Item2, dict);
				}
				if (!dict.ContainsKey(data.Item3)) {
					dict.Add(data.Item3, data.Item4);
				}
			} else {
				if (_listenerDict.TryGetValue(data.Item2, out var dict)) {
					dict.Remove(data.Item3);
				}
			}
			p = p.Next;
		}
		_safeListenerQueue.Clear();
	}
}