#ifndef CrossPlastformMutex_h
#define CrossPlastformMutex_h
#ifdef _WIN32
#include <windows.h>
//#include <synchapi.h>
#else
#include <sys/time.h>
#include <pthread.h>
#endif
#include <vector>
using namespace std;

class MMutex
{
public:

	MMutex();
	~MMutex();

	inline void Lock()
	{
#ifdef WIN32
		::EnterCriticalSection(&fMutex);
#else
		(void)pthread_mutex_lock(&fMutex);
#endif
	};

	inline void Unlock()
	{
#ifdef WIN32
		::LeaveCriticalSection(&fMutex);
#else
		pthread_mutex_unlock(&fMutex);
#endif
	};

#ifdef WIN32
	inline CRITICAL_SECTION& getMutex(){
		return
			fMutex;
	}
#else
	inline pthread_mutex_t& getMutex(){
		return
			fMutex;
	}
#endif

private:

#ifdef WIN32
	CRITICAL_SECTION fMutex;
#else
	pthread_mutex_t fMutex;
#endif      
};

class MLock
{
public:
	MLock(MMutex& cs) : _cs(cs)
	{
		_cs.Lock();
	}
	~MLock()
	{
		_cs.Unlock();
	}
private:
	MMutex& _cs;
};

class MSemaphore
{
#ifdef WIN32
public:
	MSemaphore()
	{
		h = CreateSemaphore(NULL, 0, MAXLONG, NULL);
	}
	void Signal()
	{
		ReleaseSemaphore(h, 1, NULL);
	}

	bool Wait()
	{
		return WaitForSingleObject(h, INFINITE) ==
			WAIT_OBJECT_0;
	}

	bool Wait(unsigned int msecond)
	{
		return WaitForSingleObject(h, msecond) ==
			WAIT_OBJECT_0;
	}

	~MSemaphore()
	{
		CloseHandle(h);
	}

private:
	HANDLE h;
#else
public:
	MSemaphore() : count(0)
	{
		pthread_cond_init(&cond, NULL);
	}
	void Signal()
	{
		MLock l(cs);
		count++;
		pthread_cond_signal(&cond);
	}

	bool Wait()
	{
		MLock l(cs);
		if (count == 0) {
			pthread_cond_wait(&cond, &cs.getMutex());
		}
		count--;
		return true;
	}

	bool Wait(unsigned int msecond)
	{
		MLock l(cs);
		if (count == 0) {
			timeval timev;
			timespec t;
			gettimeofday(&timev, NULL);
			t.tv_sec = timev.tv_sec + (msecond / 1000);
			t.tv_nsec = (msecond % 1000) * 1000 * 1000;
			int ret = pthread_cond_timedwait(&cond,
				&cs.getMutex(), &t);
			if (ret != 0) {
				return false;
			}
		}
		count--;
		return true;
	}

private:
	pthread_cond_t cond;
	MMutex cs;
	int count;
#endif
public:
	MSemaphore(const MSemaphore&);
	MSemaphore& operator=(const MSemaphore&);
};

template<typename T>
class MSingleton {
public:
	MSingleton() { };
	virtual ~MSingleton() { };

	static T* getInstance() {
		return instance;
	}

	static void newInstance() {
		if (instance)
			delete instance;

		instance = new T();
	}

	static void deleteInstance() {
		if (instance)
			delete instance;
		instance = NULL;
	}
protected:
	static T* instance;
private:
	MSingleton(const MSingleton&);
	MSingleton& operator=(const MSingleton&);

};
template<typename T> T* MSingleton<T>::instance =
NULL;

template<typename Listener>
class Speaker {
	typedef vector<Listener*> ListenerList;
	typedef typename ListenerList::iterator
		ListenerIter;

public:
	Speaker() { };
	virtual ~Speaker() { };

	void fire(typename Listener::Types type) throw()
	{
		MLock l(listenerCS);
		ListenerList tmp = listeners;
		for (ListenerIter i = tmp.begin(); i != tmp.end();
			++i) {
			(*i)->onAction(type);
		}
	}

	template<class T>
	void fire(typename Listener::Types type, const
		T& param) throw () {
		MLock l(listenerCS);
		ListenerList tmp = listeners;
		for (ListenerIter i = tmp.begin(); i !=
			tmp.end(); ++i) {
			(*i)->onAction(type, param);
		}
	}

	template<class T, class T2>
	void fire(typename Listener::Types type, const
		T& p, const T2& p2) throw() {
		MLock l(listenerCS);
		ListenerList tmp = listeners;
		for (ListenerIter i = tmp.begin(); i !=
			tmp.end(); ++i) {
			(*i)->onAction(type, p, p2);
		}
	}
	template<class T, class T2, class T3>
	void fire(typename Listener::Types type, const
		T& p, const T2& p2, const T3& p3) throw() {
		MLock l(listenerCS);
		ListenerList tmp = listeners;
		for (ListenerIter i = tmp.begin(); i !=
			tmp.end(); ++i) {
			(*i)->onAction(type, p, p2, p3);
		}
	}
	template<class T, class T2, class T3, class T4>
	void fire(typename Listener::Types type, const
		T& p, const T2& p2, const T3& p3, const T4& p4)
		throw() {
		MLock l(listenerCS);
		ListenerList tmp = listeners;
		for (ListenerIter i = tmp.begin(); i !=
			tmp.end(); ++i) {
			(*i)->onAction(type, p, p2, p3, p4);
		}
	}
	template<class T, class T2, class T3, class T4,
	class T5>
		void fire(typename Listener::Types type, const
		T& p, const T2& p2, const T3& p3, const T4& p4,
		const T5& p5) throw() {
		MLock l(listenerCS);
		ListenerList tmp = listeners;
		for (ListenerIter i = tmp.begin(); i !=
			tmp.end(); ++i) {
			(*i)->onAction(type, p, p2, p3, p4, p5);
		}
	}
	template<class T, class T2, class T3, class T4,
	class T5, class T6>
		void fire(typename Listener::Types type, const
		T& p, const T2& p2, const T3& p3, const T4& p4,
		const T5& p5, const T6& p6) throw() {
		MLock l(listenerCS);
		ListenerList tmp = listeners;
		for (ListenerIter i = tmp.begin(); i !=
			tmp.end(); ++i) {
			(*i)->onAction(type, p, p2, p3, p4, p5, p6);
		}
	}


	void addListener(Listener* aListener) {
		MLock l(listenerCS);
		if (find(listeners.begin(), listeners.end(),
			aListener) == listeners.end())
			listeners.push_back(aListener);
	}

	void removeListener(Listener* aListener) {
		MLock l(listenerCS);

		ListenerIter i = find(listeners.begin(),
			listeners.end(), aListener);
		if (i != listeners.end())
			listeners.erase(i);
	}

	void removeListeners() {
		MLock l(listenerCS);
		listeners.clear();
	}
protected:
	ListenerList listeners;
	MMutex listenerCS;
};


#endif