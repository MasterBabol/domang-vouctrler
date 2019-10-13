#pragma once

#include <assert.h>

typedef unsigned int UINT;

template <typename T>
struct node_t
{
	T _t;
	int _prev;
	int _next;
	node_t() : _t(), _prev(0), _next(0)
	{
	}
};

template <typename T>
class opzStaticList
{
public:
	opzStaticList(UINT size);
	~opzStaticList(void);

	int GetBeginIndex(){return _beginIndex; }
	node_t<T> *Next(const node_t<T> *node) const
	{
		if(node->_next == _size)
			return NULL;
		else
			return &_nodes[node->_next];
	}

	bool New(const node_t<T> &newNode);
	node_t<T> *Free(node_t<T> *node);

private:
	int			_size;
	node_t<T>	*_nodes;
	int			_hBeginIndex;
	int			_uhBeginIndex;
};

template <typename T>
opzStaticList<T>::opzStaticList(UINT size)
	: _size(size), _nodes(NULL), _hBeginIndex(0), _uhBeginIndex(0)
{
	_nodes = new node_t<T>[size];
	_hBeginIndex = size;

	int i;
	for(i = 0; i < size; ++i)
	{
		_nodes[i]->_prev = i - 1;
		_nodes[i]->_next = i + 1;
	}
}

template <typename T>
opzStaticList<T>::~opzStaticList(void)
{
	delete[] _nodes;
}


template <typename T>
bool opzStaticList<T>:: New(const node_t<T> &newNode)
{
	if(_uhBeginIndex == size)
	{
		assert(!L"opzStaticList 메모리 부족!");
		return false;
	}

	int uhNext = _nodes[_uhBeginIndex]->_next;
	_nodes[uhNext]->_prev = -1;
	
	if(_hBeginIndex == size)
	{
		_hBeginIndex = _uhBeginIndex;
		_nodes[_hBeginIndex]->_next = size;
	}
	else
	{
		_nodes[_hBeginIndex]->_prev = _uhBeginIndex;	
		_nodes[_uhBeginIndex]->_next = _hBeginIndex;
		_hBeginIndex = _uhBeginIndex;
	}
	_nodes[_hBeginIndex]->_prev = -1;

	_uhBeginIndex = uhNext;

	return true;
}

template <typename T>
node_t<T> *opzStaticList<T>::Free(node_t<T> *node)
{
	if(_hBeginIndex == size)
	{
		assert(!L"opzStaticList 할당받은 게 없는 상태에서 해제시도!");
		return NULL;
	}

	int hNext = node->_next;
	int hPrev = node->_prev;

	if(hNext != size)
		nodes[hNext]->_prev = hPrev;
	
	if(hPrev == -1)
		_hBeginIndex = hNext;
	else
		nodes[hPrev]->_next = hNext;

	if(_uhBeginIndex == size)
	{
		_uhBeginIndex = hNext;
		_nodes[_uhBeginIndex]->_next = size;
	}
	else
	{
		_nodes[_uhBeginIndex]->_prev = _hBeginIndex;
		_nodes[_hBeginIndex]->_next = _uhBeginIndex;
		_uhBeginIndex = _hBeginIndex;
	}
	_nodes[_uhBeginIndex]->_prev = -1;

	_hBeginIndex = hNext;

	return _nodes[_];
}