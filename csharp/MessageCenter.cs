/*
	Author: Jenocn
	Date: 2019-4-26
	Email:54467371@qq.com
*/

using System.Collections.Generic;

public static class MessageCenter {
	private static Dictionary<string, IMessageListener> _listenerDict = new Dictionary<string, IMessageListener>();
	private static Dictionary<bool, LinkedList<IMessage>> _messageQueue = new Dictionary<bool, LinkedList<IMessage>>();
	private static bool _activeQueueSign = false;

	static MessageCenter() {
		_messageQueue[true] = new LinkedList<IMessage>();
		_messageQueue[false] = new LinkedList<IMessage>();
	}

	public static void AddListener<T>(string name, System.Action<T> func) where T : MessageBase<T> {
		if (string.IsNullOrEmpty(name) || func == null) {
			return;
		}
		if (_listenerDict.ContainsKey(name)) {
			return;
		}
		var listener = new MessageListener<T>(func);
		_listenerDict.Add(name, listener);
	}

	public static void RemoveListener(string name) {
		_listenerDict.Remove(name);
	}

	public static void Send(IMessage message) {
		if (message == null) { return; }
		var messageID = message.GetMessageID();
		foreach (var item in _listenerDict) {
			if (item.Value.GetMessageID() == messageID) {
				item.Value.Invoke(message);
			}
		}
	}

	public static void Push(IMessage message) {
		if (message == null) {
			return;
		}
		_messageQueue[!_activeQueueSign].AddLast(message);
	}

	public static void OnDispatch() {
		_activeQueueSign = !_activeQueueSign;
		foreach (var message in _messageQueue[_activeQueueSign]) {
			Send(message);
		}
		_messageQueue[_activeQueueSign].Clear();
	}

	public static void Clear() {
		_listenerDict.Clear();
		_messageQueue[true].Clear();
		_messageQueue[false].Clear();
	}
}