/*++
List.h - class list template
Copyright (c) 1999, Junichi MUTO. ALL RIGHTS RESERVED.
--*/

#ifndef __LIST_H__
#define __LIST_H__

#include "trace.h"
#include <iostream>
#include <assert.h>

template <class T>

class List
{
 protected:
	 unsigned int _size;
	 unsigned int _currentsize;
	 T **_list;
	 
 public:
	 List(unsigned int ListSize);
	 List();
	 virtual ~List();
	 
	 void add(char* filename);
	 void del(unsigned int index);
	 void add(char* filename, unsigned int index){
		 assert(filename);
		 delete _list[index];
		 _list[index] = new T(filename);
	 }
	 int resize();
	 int size() { return _currentsize; }
	 
#ifdef DEBUG_LIST
	 void copy(unsigned int dest, unsigned int src);
	 T* operator[](unsigned int index);
	 T* get(unsigned int index);
#else // no debug
	 void copy(unsigned int dest, unsigned int src) {
		 if(!_list[dest]) _list[dest] = new T();
		 *(_list[dest]) = *(_list[src]);
	 };
	 T* operator[](unsigned int index) { return _list[index]; }
	 T* get(unsigned int index) { return _list[index]; }
#endif
};



#ifdef DEBUG_LIST
#include <stdlib.h>
#endif

template <class T>
List<T>::List()
{
	TRACE("List::List()");
	
	_size = 1;
	_currentsize = 0;
	_list = new T*[1];
	_list[0] = NULL;
}

template <class T>
List<T>::List(unsigned int ListSize)
{
	TRACE("List::List(unsigned int)");
	
	_size = ListSize;
	_currentsize = 0;
	if(_size == 0) _size = 1;
	_list = new T*[_size];
	for(int i = 0 ; i < _size ; i++) _list[i] = NULL;
}

template <class T>
List<T>::~List()
{
	TRACE("List::~List()");
	
	for(int i = 0 ; i < _size ; i++) delete _list[i];
	delete [] _list;
}

template <class T>
void List<T>::add(char* filename)
{
	TRACE("List::add(char*)");
	
	assert(filename);
	
	if(_size == _currentsize){
		int i;
		_size++;
		T** newlist = new T*[_size];
		for( i = 0 ; i < _size - 1; i++) 
			newlist[i] = _list[i];
		newlist[i] = new T(filename);
		delete [] _list;
		_list = newlist;
	} else 
		_list[_currentsize] = new T(filename);
	
	_currentsize++;
}

template <class T>
void List<T>::del(unsigned int index)
{
	TRACE("List::del(unsigned int)");
	
	delete _list[index];
	_list[index] = NULL;
}

template <class T>
int List<T>::resize()
{
	int newSize = 0;
	int  i,j;
	for(i = 0; i < _size; i++)
		if(_list[i] != NULL) newSize++;
		_currentsize = _size = newSize;
		
		T** newlist = new T*[_size];
		for(i = 0, j = 0; i < _size; i++)
			if(_list[i]) newlist[++j] = _list[i];
			delete [] _list;
			_list = newlist;
			return _size;
}

#ifdef DEBUG_LIST
template <class T>
void List<T>::copy(unsigned int dest, unsigned int src)
{
	TRACE("List::copy(unsigned int, unsigned int)");
	if(src >= _currentsize || dest >= _size){
		cerr << "List::copy error: src or dest index is illegal -- src "
			<< src << ", dest " << dest <<endl;
		assert(NULL);
	}
	
	if(src == dest){
		cerr << "List::copy error: same object - src , dest" << endl;
		assert(NULL);
	}
	
	if(!_list[src]){
		cerr << "List::copy error: not allocate memory on src - list[" << src << "]." <<endl;
		assert(_list[src]);
	}
	
	if(!_list[dest]) _list[dest] = new T;
	
	*(_list[dest]) = *(_list[src]);
}

template <class T>
T* List<T>::operator[](unsigned int index)
{
	//	TRACE("List::operator[](unsigned int)");
	
	if(index >= _size){
		cerr << "List::operator[] error: larger than List Index - " << index << endl;
		abort();
	}
	return _list[index];
}

template <class T>
T* List<T>::get(unsigned int index)
{
	//	TRACE("List::get(unsigned int)");
	
	if(index >= _size){
		cerr << "List::get error: larger than List Index - " << index << endl;
		abort();
	}
	return _list[index];
}
#endif

#endif // __LIST_H__

