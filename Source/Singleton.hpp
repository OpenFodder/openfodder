#include <cassert>

template <typename tBase> class cSingleton {

	static tBase* m_Singleton;

public:

	cSingleton() {

		assert( !m_Singleton );

		m_Singleton = static_cast<tBase*> (this);
	}

	~cSingleton() {
		assert( m_Singleton ); 
		m_Singleton = 0;
	}

	static tBase& GetSingleton() {

		assert( m_Singleton ); 
		return *m_Singleton;
	}

	static tBase* GetSingletonPtr() {

		return m_Singleton;
	}
};

template <typename tBase> tBase* cSingleton<tBase>::m_Singleton = 0;
