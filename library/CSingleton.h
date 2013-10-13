#pragma once

template <class T>
class CSingleton
{
	static T* ms_Singleton;

public:

	virtual ~CSingleton()
	{
		ms_Singleton = 0;
	}

	static void	CreateSingleton()
	{
		ms_Singleton = new T;
	}

	static void	 DestroySingleton()
	{
		delete ms_Singleton;
	}

	static T* GetSingleton()
	{
		return ms_Singleton;
	}

protected:
	CSingleton()
	{
	}
};

template <class T> T* CSingleton<T>::ms_Singleton = 0;
