/*
	Author: Jenocn
	Date: 2019-4-26
	Email:54467371@qq.com
*/

public interface IMessage {
	int GetMessageID();
}

public class MessageBase<T> : ClassType<T, IMessage>, IMessage where T : MessageBase<T> {
	public int GetMessageID() {
		return GetClassType();
	}
}