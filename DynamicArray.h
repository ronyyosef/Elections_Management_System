#pragma once
#include <algorithm>
#include <iostream>
#include <functional>
#include <vector>

template <class T>
class DynamicArray
{
private:
	T* _arr;
	int _logicalSize;
	int _physicalSize;
	void resize()
	{
		_physicalSize *= 2;
		T* temp = new T[_physicalSize];
		for (int i = 0; i < _logicalSize; i++)
			temp[i] = _arr[i];

		delete[] _arr;
		_arr = temp;
	}
public:
	DynamicArray(int size = 2)
	{
		_logicalSize = 0;
		if (size >= 1)
			_physicalSize = size;
		else
			_physicalSize = 2;
		_arr = new T[size];
	}
	DynamicArray(const DynamicArray& other) : _arr(nullptr) {
		*this = other;
	}
	~DynamicArray() {
		delete[] _arr;
	}
	const DynamicArray& operator=(const DynamicArray& other) {
		if (this != &other) {
			_logicalSize = other._logicalSize;
			_physicalSize = other._physicalSize;
			delete[] _arr;
			_arr = new T[_physicalSize];
			for (int i = 0; i < _logicalSize; i++)
				_arr[i] = other._arr[i];
		}
		return *this;
	}
	const T& operator[](int i) const { return _arr[i]; }
	T& operator[](int i) { return _arr[i]; }
	void push_back(const T& value) {
		if (_logicalSize == _physicalSize)
			resize();
		_arr[_logicalSize++] = value;
	}
	const T& front()    const { return _arr[0]; }
	int      size()     const { return _logicalSize; }
	int      capacity() const { return _physicalSize; }
	bool     empty()    const { return _logicalSize == 0; }
	void     clear() { _logicalSize = 0; }
	template <bool is_const>
	class base_iterator
		{
		public:
			using ds_type = std::conditional_t<is_const, const DynamicArray, DynamicArray>;
			using iterator_category = std::bidirectional_iterator_tag;
			using different_type = std::ptrdiff_t;
			using value_type = std::conditional_t<is_const, const T, T>;
			using pointer = value_type*;
			using reference = value_type&;
			base_iterator(ds_type& arr, int i) : _da(&arr), _i(i) {}
			base_iterator(const base_iterator&) = default;
			constexpr base_iterator& operator=(const base_iterator&) = default;
			friend class base_iterator<true>;
			template <bool _is_const = is_const, class = std::enable_if_t<_is_const>>
			base_iterator(const base_iterator<false>& other) : _da(other._da), _i(other._i) {}

			bool operator==(const base_iterator& other) const {
				return (_da == other._da) && (_i == other._i);
			}
			bool operator!=(const base_iterator& other) const {
				return !(*this == other);
			}

			reference operator*() {
				return _da->_arr[_i];
			}
			pointer operator->() {
				return &_da->_arr[_i];
			}

			base_iterator& operator++() {
				++_i;
				return *this;
			}
			base_iterator operator++(int) {
				base_iterator temp(*this);
				++_i;
				return temp;
			}
			base_iterator& operator--() {
				--_i;
				return *this;
			}
			base_iterator operator--(int) {
				base_iterator temp(*this);
				--_i;
				return temp;
			}
		private:
			ds_type* _da;
			int			_i;
		};
	class reverse_iterator
		{
		public:
			using ds_type = DynamicArray;
			using iterator_category = std::bidirectional_iterator_tag;
			using different_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;

			reverse_iterator(ds_type& arr, int i) : _arr(&arr), _i(i) {}
			reverse_iterator(const reverse_iterator& other) : _arr(other._da), _i(other._i) {}
			constexpr reverse_iterator& operator=(const reverse_iterator&) = default;
			reference operator*() {
				return _da->_arr[_i];
			}
			pointer operator->() {
				return &_da->_arr[_i];
			}

			reverse_iterator& operator++() {
				--_i;
				return *this;
			}

			reverse_iterator operator++(int) {
				reverse_iterator temp(*this);
				--_i;
				return temp;
			}

			reverse_iterator& operator--() {
				++_i;
				return *this;
			}
			reverse_iterator operator--(int) {
				reverse_iterator temp(*this);
				++_i;
				return temp;
			}

			bool operator==(const base_iterator& other) const {
				return (_da == other._da) && (_i == other._i);
			}

			bool operator!=(const base_iterator& other) const {
				return !(*this == other);
			}
		private:
			ds_type* _da;
			int	_i;
		};
	using iterator = base_iterator<false>;
	using const_iterator = base_iterator<true>;
	void insert(const iterator& pos, const T& val) {
			if (_logicalSize == _physicalSize)
				resize();
			iterator itrEnd = end();
			iterator itrCurrent = itrEnd, itrPrev = --itrEnd;
			while (itrCurrent != pos)
			{
				*itrCurrent = *itrPrev;
				itrCurrent = itrPrev--;
			}
			iterator p = pos;
			*p = val;
			++_logicalSize;
		}
	iterator erase(const iterator& iter)
		{
			iterator begin = begin();
			iterator after = ++begin();
			while (begin != iter && begin != end()) { begin = after++; }
			if (begin == iter)
			{
				while (after != end())
				{
					*begin = *after;
					begin++;
					after++;
				}
				_logicalSize--;
			}
			else
			{
				return begin();
			}
			begin = iter;
			begin--;
			return (begin);
		}
	iterator& erase(const iterator& first, const iterator& last)
		{
			int counter = 0;
			iterator _first = first, _last = last;
			for (; _first != _last; _first++)
				counter++;
			_first = first;
			while (_last != end())
			{
				*_first = *_last;
				_first++;
				_last++;
			}
			_logicalSize -= counter;
			_first = first;
			_first--;
			return (_first);
		}
	iterator begin() {
		return iterator(*this, 0);
	}
	iterator end() {
		return iterator(*this, _logicalSize);
	}
	const_iterator begin() const {
		return const_iterator(*this, 0);
	}
	const_iterator end() const {
		return const_iterator(*this, _logicalSize);
	}
	iterator cbegin() { return const_iterator(*this, 0); }
	iterator cend() { return const_iterator(*this, _logicalSize); }
	void print() const {
		for (int i = 0; i < _logicalSize; i++)
			std::cout << _arr[i] << " ";
		std::cout << std::endl;
	}	
};
template<class T>
void bubble_sort(T& data)
{
	for (auto i = data.begin(); i != data.end(); i++)
	{
		for (auto j = i + 1; j != data.end(); j++)
		{
			if (*i < *j)
			{
				auto b = *i;
				*i = *j;
				*j = b;
			}
		}
	}
}