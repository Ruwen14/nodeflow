#pragma once

namespace nf
{
	template <typename T>
	class Singleton
	{
	public:
		// = initialization and termination methods
		Singleton(const Singleton&) = delete;   // copy constructor
		Singleton(Singleton&&) = delete;   // move constructor
		Singleton& operator=(const Singleton&) = delete;  // assignment operator
		Singleton& operator=(Singleton&&) = delete;  // copy move operator
		// = accessor methods.
		static T& instance()
		{
			static T _instance;
			return _instance;
		}
	protected:
		Singleton() = default;                          // default constructor
		~Singleton() = default;                       // default destructor
	}; // end template class T
} // end namespace std

