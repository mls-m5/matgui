/*
 * Signal.h
 *
 *  Created on: 22 jan 2015
 *      Author: Mattias Larsson Sköld
 */


#pragma once


#include <list>
#include <queue>
#include <mutex>
#include <memory>


template <class _returnType, class _argument>
class ConnectionBase {
public:
	virtual ~ConnectionBase() {

	}

	virtual void call(_argument arg) = 0;
	virtual bool isObject(void *o) = 0;
};

template <class _classType, class _returnType, class _argument>
class ClassConnection : public ConnectionBase<_returnType, _argument> {
public:;
	typedef _returnType (_classType::*_functionType )(_argument);

	ClassConnection(_classType *c, _functionType f) {
		_objectPointer = c;
		_functionPointer = f;
	}

	void call(_argument arg) override {
		(*_objectPointer.*_functionPointer)(arg);
	}

	bool isObject(void *o) override {
		return (o == (void *) _objectPointer);
	}


	_classType * _objectPointer; // this could lead to seg fault if object is destroyed before the signal
	_functionType _functionPointer;
};



//This connection enables connection to a void function even if the signal
//has arguments
template <class _classType, class _returnType, class _argument>
class VoidClassConnection : public ConnectionBase<_returnType, _argument> {
public:;
	typedef void (_classType::*_functionType )(void);

	VoidClassConnection(_classType *c, _functionType f) {
		_objectPointer = c;
		_functionPointer = f;
	}

	//This function does not use the argument
	void call(_argument arg) override {
		(*_objectPointer.*_functionPointer)();
	}

	bool isObject(void *o) override {
		return (o == (void *) _objectPointer);
	}

	_classType * _objectPointer; // this could lead to seg fault if object is destroyed before the signal
	_functionType _functionPointer;
};

template <class _returnType, class _argument>
class FunctionConnection : public ConnectionBase<_returnType, _argument> {
public:;
	typedef _returnType (*_functionType )(_argument);

	FunctionConnection(_functionType f) {
		_functionPointer = f;
	}

	void call(_argument arg) override {
		_functionPointer(arg);
	}

	_functionType _functionPointer;
};



template <class _classType, class _function, class _returnType, class _argument>
class Connection: public ConnectionBase<_returnType, _argument> {
public:
	void call(_argument arg) override {
		std::shared_ptr<_function> functionPoter;
	}
};



//A base class made just to be able to add all signals to a collection to be called at once
class SignalBase {
public:
	SignalBase() {
		_signals.push_back(this);
	}
	virtual ~SignalBase() {
		_signals.remove(this);
	}

	virtual void flush() = 0;

	static void flushAll() {
		for (auto signal: _signals) {
			signal->flush();
		}
	}
	static std::list<SignalBase*> _signals;
};

template< class T >
struct TypeIsVoid
{
    static const bool value = false;
};

template<>
struct TypeIsVoid< void >
{
    static const bool value = true;
};


template <typename _argument>
class Signal: SignalBase, std::list<std::shared_ptr<ConnectionBase<void, _argument>>> {
public:

	void emit(_argument arg = 0) {
		if (!this->empty()) { //Dont bother to broadcast if nobody listen
			_queue.push(arg);
		}
	}

	//Call all connected functions
	void flush() override{
		while (!_queue.empty()) {
			_argument arg = _queue.front();
			_queue.pop();
			for (auto connection: *this) {
				connection->call(arg);
			}
		}
	}


	//Connect to a class function
	//Usage:
	//signalname.connect(this, &NameOfClass::nameOfMemberFunction)
	template <class _classType>
	void connect(_classType *c, void (_classType::*f)(_argument)) {
		_mutex.lock();
		std::shared_ptr<ConnectionBase<void, _argument>> connection(new ClassConnection<_classType, void, _argument> (c, f));
		this->push_back(connection);
		_mutex.unlock();
	}


	//Connect a class function with no arguments to signal
	//Usage:
	//signalname.connect(this, &NameOfClass::nameOfMemberFunctionWithNoArguments)
	template <class _classType>
	void connect(_classType *c, void (_classType::*f)(void)) {
		_mutex.lock();
		std::shared_ptr<ConnectionBase<void, _argument>> connection(new VoidClassConnection<_classType, void, _argument> (c, f));
		this->push_back(connection);
		_mutex.unlock();
	}


	//Connect to a class function
	//Usage:
	//signalname.connect(nameOfFunction)
	void connect(void (*f)( _argument)) {
		_mutex.lock();
		std::shared_ptr<ConnectionBase<void, _argument>> connection(new FunctionConnection<void, _argument> (f));
		this->push_back(connection);
		_mutex.unlock();
	}



	//Remove connection to object to avoid segmentation fault
	template <class _objectType>
	void disconnect(_objectType *o) {
		_mutex.lock();
		for (auto it = this->begin(); it != this->end();) {
			if ((*it)->isObject((void *)o)) {
				it = this->erase(it);
			}
			else {
				++it;
			}
		}
		_mutex.unlock();
	}


	struct Apa {
		int bepa() {
			return 0;
		}
	};



	std::mutex _mutex;
	std::queue<_argument> _queue; //A queue with a list as underlying container to be able to remove objects
};


