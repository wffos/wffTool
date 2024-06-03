#ifndef _H_WFF_SINGLETON_H_
#define _H_WFF_SINGLETON_H_

namespace wff
{
	template <class T>
	class singleton
	{
	private:
		singleton(const singleton &) {}
		singleton &operator=(const singleton &) {}

	protected:
		singleton() {}
		virtual ~singleton(){};

	public:
		static T &getInstance()
		{
			static T instance;
			return instance;
		}
	};
}

#endif