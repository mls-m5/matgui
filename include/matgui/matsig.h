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

namespace MatSig {


//Base class for signal connection
//It is automatically created by the signals connect function
template <class _returnType, class _argument>
class ConnectionBase {
public:
	virtual ~ConnectionBase() {}

	virtual _returnType call(_argument arg) = 0;
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

	virtual ~ClassConnection() {}

	_returnType call(_argument arg) override {
		return (*_objectPointer.*_functionPointer)(arg);
	}

	bool isObject(void *o) override {
		return (o == (void *) _objectPointer);
	}


	_classType * _objectPointer; // this could lead to seg fault if object is destroyed before the signal
	_functionType _functionPointer;
};



//This connection enables connection to a void function even if the signal
//has arguments
//It is created automatically by the signals connect function
template <class _classType, class _returnType, class _argument>
class VoidClassConnection : public ConnectionBase<_returnType, _argument> {
public:;
	typedef void (_classType::*_functionType )(void);

	VoidClassConnection(_classType *c, _functionType f) {
		_objectPointer = c;
		_functionPointer = f;
	}

	virtual ~VoidClassConnection() {}

	//This function does not use the argument
	_returnType call(_argument arg) override {
		return (*_objectPointer.*_functionPointer)();
	}

	bool isObject(void *o) override {
		return (o == (void *) _objectPointer);
	}

	_classType * _objectPointer; // this could lead to seg fault if object is destroyed before the signal
	_functionType _functionPointer;
};


//Specialized class for callback to functions without object
template <class _returnType, class _argument>
class FunctionConnection : public ConnectionBase<_returnType, _argument> {
public:;
	typedef _returnType (*_functionType )(_argument);

	FunctionConnection(_functionType f) {
		_functionPointer = f;
	}

	virtual ~FunctionConnection() {}

	_returnType call(_argument arg) override {
		return _functionPointer(arg);
	}

	bool isObject(void *o) override {
		return (o == (void *) _functionPointer);
	}

	_functionType _functionPointer;
};


//A base class made just to be able to add all signals to a collection to be called at once
class SignalBase {
public:
	//Signals can only be created and deleted on the main thread
	SignalBase();
	virtual ~SignalBase();

	virtual void flush() = 0;

	static void flushAll();
};






//Specialized class for callback to static function with signal as reference
template <class _returnType, class _argument>
class FunctionConnectionWithReference : public ConnectionBase<_returnType, _argument> {
public:;
	typedef _returnType (*_functionType )(SignalBase*, _argument);

	FunctionConnectionWithReference(SignalBase* signal, _functionType f) {
		_functionPointer = f;
		_signal = signal;
	}

	virtual ~FunctionConnectionWithReference() {}

	void call(_argument arg) override {
		_functionPointer(_signal, arg);
	}

	bool isObject(void *o) override {
		return (o == (void *) _functionPointer);
	}

	_functionType _functionPointer;
	SignalBase *_signal;
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


template <typename _argument = void*, typename _returnValue = void>
class Signal: SignalBase, std::list<std::shared_ptr<ConnectionBase<_returnValue, _argument>>> {
public:
	typedef std::lock_guard<std::mutex> LockGuard;
	Signal(bool onlySaveLast = false): _onlySaveLast(onlySaveLast) {}

	//Put a call on the queue
	void emit(_argument arg = 0) {
		if (!this->empty()) { //Dont bother to broadcast if no object listens
			if (_onlySaveLast) { //Use this with objects emitting very many signals
				if (_queue.empty()) {
					_queue.push(arg);
				}
				else {
					//Note: possible non-atomic operation
					_queue.front() = arg; //Replace previous argument
				}
			}
			else {
				_queue.push(arg);
			}
		}
	}

	//Shorthand for function emit(...)
	inline void operator()(_argument arg = 0) {
		emit(arg);
	}

	//Call all connected functions
	void flush() override{
		while (!_queue.empty()) {
			//removes element from que so that a new element can be added directly
			//in case it is a sigle element list
			auto arg = _queue.front();
			_queue.pop();
			for (auto connection: *this) {
				connection->call(arg);
			}
		}
	}


	//Call without putting on the queue
	//Remember that this probably only should be called from the gui thread
	template <typename T = _returnValue>
	typename std::enable_if<std::is_void<T>::value>::type //This checks if the return value is void
	directCall(_argument arg = 0) {
		if (!this->empty()) {
			for (auto connection: *this) {
				connection->call(arg);
			}
		}
	}

	//The same as above but with return value
	//Note that it only returns the last return value
	template <typename T = _returnValue>
	typename std::enable_if<!std::is_void<T>::value, T>::type //Check for non void return value
	directCall(_argument arg = 0) {
		if (!this->empty()) {
			_returnValue ret = 0;
			for (auto connection: *this) {
				ret = connection->call(arg);
			}
			return ret;
		}
		return 0;
	}


	//this operator is used to check if it is any use to emit a signal
	operator bool() {
		return !this->empty();
	}

	//Connect to a class function
	//Usage:
	//signalname.connect(this, &NameOfClass::nameOfMemberFunction)
	template <class _classType>
	void connect(_classType *c, void (_classType::*f)(_argument)) {
		LockGuard guard(_mutex);
		std::shared_ptr<ConnectionBase<_returnValue, _argument>> connection(new ClassConnection<_classType, void, _argument> (c, f));
		this->push_back(connection);
	}


	//Connect a class function with no arguments to signal
	//Usage:
	//signalname.connect(this, &NameOfClass::nameOfMemberFunctionWithNoArguments)
	template <class _classType>
	void connect(_classType *c, void (_classType::*f)(void)) {
		LockGuard guard(_mutex);
		std::shared_ptr<ConnectionBase<_returnValue, _argument>> connection(new VoidClassConnection<_classType, void, _argument> (c, f));
		this->push_back(connection);
	}


	//Connect to a static function
	//Usage:
	//signalname.connect(nameOfFunction)
	void connect(void (*f)( _argument)) {
		LockGuard guard(_mutex);
		std::shared_ptr<ConnectionBase<_returnValue, _argument>> connection(
				new FunctionConnection<_returnValue, _argument> (f));
		this->push_back(connection);
	}


	//Connect to a static function that sends a reference to the emitting signal
	//Usage:
	//signalname.connect(nameOfFunction)
	void connect(void (*f)(SignalBase*, _argument)) {
		LockGuard guard(_mutex);
		std::shared_ptr<ConnectionBase<_returnValue, _argument>> connection(
				new FunctionConnectionWithReference<_returnValue, _argument> (this, f));
		this->push_back(connection);
	}



	//Remove connection to object to avoid segmentation fault
	template <class _objectType>
	void disconnect(_objectType *o) {
		LockGuard guard(_mutex);
		for (auto it = this->begin(); it != this->end();) {
			if ((*it)->isObject((void *)o)) {
				it = this->erase(it);
			}
			else {
				++it;
			}
		}
	}


	//Clear signals queue
	void clearQueue() {
		//A efficient way to clear lists according to the internet
		std::queue<_argument> empty;
		std::swap( _queue, empty );
	}


	void onlySaveLast(bool value) {
		_onlySaveLast = value;
	}

	bool onlySaveLast() {
		return _onlySaveLast;
	}

protected:
	std::mutex _mutex;
	std::queue<_argument> _queue;
	bool _onlySaveLast = false;
};


inline void flushSignals() {
	SignalBase::flushAll();
}

void assertMainThread();
void setMainThread();

} //namespace MatSig