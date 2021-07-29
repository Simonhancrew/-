#pragma once

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS // for msvc use strcpy
#endif
#include <iostream>
#include <cstring>

#define N 1024


// using namespace std;

// 1 为什么需要构造函数
// 2 不做虚构函数会发生什么
// 3 同类，同包下的问题
// 4 有哪些需要考虑防卫声明？

class hstring {
public:
	hstring(const char* str = nullptr);//construct
	hstring(const hstring& str);// copy
	~hstring(); // 析构
	hstring& operator=(const hstring& str);  //赋值
	hstring operator+(const hstring& str); 
	hstring& operator+=(const hstring& str);
	bool operator==(const hstring& str);
	char& operator[](int n) const;
	size_t len() const;
	const char* hc_str() const;
	friend std::istream& operator>>(std::istream& is, hstring& str);
	friend std::ostream& operator<<(std::ostream& os, hstring& str);
private:
	size_t size;
	char* data;
};



hstring::hstring(const char* str) {
	// 防卫
	if (str == nullptr) {
		data = new char[1];
		data[0] = '\0';
		size = 0;
	}
	else {
		size = strlen(str);
		data = new char[(long long)size + 1];
		strcpy(data,str);
	}
}

hstring::hstring(const hstring& str) {
	size = str.len();
	data = new char[size + 1];
	strcpy(data, str.hc_str());
}


hstring::~hstring(){
	size = 0;
	delete[] data;
}

hstring& hstring::operator=(const hstring& str) {
	if (this == &str) {
		return *this;
	}
	// 防卫，除去原有
	delete[] data;
	size = strlen(str.data);
	data = new char[size + 1];
	strcpy(data, str.data);
	return *this;
}

hstring hstring::operator+(const hstring& str) {
	hstring newstring;
	newstring.size = size + str.len();
	newstring.data = new char[newstring.size + 1];
	strcpy(newstring.data, data);
	strcat(newstring.data, str.data);
	return newstring;
}

hstring& hstring::operator+=(const hstring& str) {
	size += str.len();
	char* newdata = new char[size + 1];
	strcpy(newdata, data);
	strcat(newdata, str.data);
	delete[] data;//原有删除
	data = newdata;
	return *this;
}

// 可以内联
inline bool hstring::operator==(const hstring& str) {
	if (&str == this) return true;
	return false;
}

inline char& hstring::operator[](int n) const {
	if (n >= size) return data[size - 1];
	return data[n];
}	


inline size_t hstring::len() const {
	return size;
}

std::istream& operator>>(std::istream& is, hstring& str) {
	char tmp[N];
	is >> tmp;
	str.size = strlen(tmp);
	str.data = new char[str.size + 1];
	strcpy(str.data, tmp);
	return is;
}

std::ostream& operator<<(std::ostream& os, hstring& str) {
	os << str.data;
	return os;	
}

inline const char* hstring::hc_str() const {
	return data;
}