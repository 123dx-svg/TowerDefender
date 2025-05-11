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


	//��֤���������඼û�п������캯���͸�ֵ�����
protected:
	~Manager() = default;
	Manager()= default;
	Manager(const Manager&)= delete;
	Manager& operator=(const Manager&)= delete;
};


template<typename T>
T* Manager<T>::manager = nullptr;


#endif // __MANAGER_H__
