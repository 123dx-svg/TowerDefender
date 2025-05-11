#ifndef __MANAGER_H__
#define __MANAGER_H__


template<typename T>
class Manager
{

public:
	static T* Instance()
	{
		if (!manager)
		{
			manager = new T();
		}
		return manager;
	}

private:
	static T* manager;


	//保证所有派生类都没有拷贝构造函数和赋值运算符
protected:
	~Manager() = default;
	Manager()= default;
	Manager(const Manager&)= delete;
	Manager& operator=(const Manager&)= delete;
};


template<typename T>
T* Manager<T>::manager = nullptr;


#endif // __MANAGER_H__
