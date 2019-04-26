/*
	Author: Jenocn
	Date: 2019-4-26
	Email:54467371@qq.com
*/

public sealed class ClassTypeCounter<T> {
	public static int count { get; private set; }
	public int index { get; private set; }
	static ClassTypeCounter() {
		count = 0;
	}
	public ClassTypeCounter() {
		index = count;
		++count;
	}
}

public class ClassType<TClass, TCounter> {
	private static ClassTypeCounter<TCounter> __counter = new ClassTypeCounter<TCounter>();

	public static int GetClassType() {
		return __counter.index;
	}
	public static int GetTypeCount() {
		return ClassTypeCounter<TCounter>.count;
	}
}