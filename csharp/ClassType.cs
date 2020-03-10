/*
	By Jenocn
	https://jenocn.github.io/
*/

/// <summary>
/// 类计数器
/// </summary>
public sealed class ClassTypeCounter<T> {
	/// <summary>
	/// 当前计数数值
	/// </summary>
	public static int count { get; private set; }

	/// <summary>
	/// 某个实例的index序号,从0开始
	/// </summary>
	public int index { get; private set; }
	static ClassTypeCounter() {
		count = 0;
	}
	public ClassTypeCounter() {
		index = count;
		++count;
	}
}

/// <summary>
/// 类型归类基类
/// </summary>
public class ClassType<TClass, TCounter> {
	private static ClassTypeCounter<TCounter> __counter = new ClassTypeCounter<TCounter>();

	/// <summary>
	/// 获得自身类型值
	/// </summary>
	public static int GetClassType() {
		return __counter.index;
	}

	/// <summary>
	/// 获得当前TCounter类型的数量
	/// </summary> 
	public static int GetTypeCount() {
		return ClassTypeCounter<TCounter>.count;
	}
}