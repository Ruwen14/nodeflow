#pragma once

template <typename T>
class Singleton {
public:
	static T& instance() {
		static T _instance;
		return _instance;
	}
protected:
	Singleton() = default;
	~Singleton() = default;
	Singleton(const Singleton& s) = delete;
	Singleton& operator=(const Singleton& s) = delete;
};
