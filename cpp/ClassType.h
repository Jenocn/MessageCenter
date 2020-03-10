/*
	By Jenocn
	https://jenocn.github.io/
*/

#ifndef __CLASS_TYPE_H_H__
#define __CLASS_TYPE_H_H__

template<typename T>
class ClassTypeCounter final
{
public:
	static int _count;
public:
	ClassTypeCounter()
	{
		_index = _count;
		++_count;
	}
public:
	int _index{ 0 };
};

template<typename T>
int ClassTypeCounter<T>::_count = 0;

//=================================================

template<typename TClass, typename TCounter>
class ClassType
{
private:
	static ClassTypeCounter<TCounter> __counter;
public:
	static int GetClassType() { return __counter._index; }
	static int GetTypeCount() { return ClassTypeCounter<TCounter>::_count; }
};

template<typename TClass, typename TCounter>
ClassTypeCounter<TCounter> ClassType<TClass, TCounter>::__counter;

#endif // !__CLASS_TYPE_H_H__