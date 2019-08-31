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
#include <functional>

namespace MatSig {



//A base class made just to be able to add all signals to a collection to be called at once
class SignalBase {
public:
	//Signals can only be created and deleted on the main thread
	SignalBase();
	virtual ~SignalBase() noexcept(false);

	virtual void flush() = 0;

	static void flushAll();

protected:
	typedef std::lock_guard<std::mutex> LockGuard;
	std::mutex _mutex;
	bool _onlySaveLast = false;
};




//A class used for queuing up signal calls
template <typename _argument>
struct ArgumentClass {
	ArgumentClass(_argument arg): data(arg) {}
	_argument data;
};


//A specialized version with void arguments
template <>
struct ArgumentClass<void> {
};




//A special version of the std::function class
//But with a object that is comparable so that
//a signal can be disconnected
template <typename _functionType, typename _returnType>
struct StdFunctionConnection {
	bool operator==(void *compareObject) {
		return reference && reference == compareObject;
	}

	template <typename _argument, typename T = _returnType>
	T callReturn(_argument arg) {
		return function(arg);
	}

	template <typename _argument>
	void call(_argument arg) {
		function(arg);
	}

	template <typename T = _returnType>
	T callReturn () {
		return function();
	}

	void call () {
		function();
	}

	// Checks if the connection is still valid
	operator bool() {
		return function? true: false;
	}

	// Make the function ponter invalid
	void clear() {
		function = 0;
	}


	//Pointer to function to be called
	std::function<_functionType> function;
	//The pointer is never used, only used for comparison
	void *reference;
};


// The loop used to flush signals
// Defined once to avoid maintenance in multiple places
// The macro iterates over all connections and removes invalid ones
#define matsig_flush_loop(callStatement) \
for (auto it = this->begin(); it != this->end(); ) { \
	if (*it) { \
		callStatement; \
		++it; \
	} \
	else { \
		LockGuard guard(_mutex); \
		it = this->erase(it); \
	} \
}


// Standard case of signal _with_ arguments
template <typename _argument = void, typename _returnValue = void>
class Signal: SignalBase, protected std::list<StdFunctionConnection<_returnValue(_argument), _returnValue>> {
public:
	typedef std::function<_returnValue(_argument)> _functionType;
	Signal(bool onlySaveLast = false) {
		_onlySaveLast = onlySaveLast;
	}

	//Put a call on the queue
	void emit(_argument arg) {
		if (!this->empty()) { //Dont bother to broadcast if no object listens
			if (_onlySaveLast) { //Use this with objects emitting very many signals
				if (_queue.empty()) {
					_queue.push(ArgumentClass<_argument>(arg));
				}
				else {
					//Note: possible non-atomic operation
					_queue.front() = ArgumentClass<_argument>(arg); //Replace previous argument
				}
			}
			else {
				_queue.push(ArgumentClass<_argument>(arg));
			}
		}
	}


	//Shorthand for function emit(...)
	inline void operator()(_argument arg) {
		emit(arg);
	}

	//Call all connected functions
	void flush() override{
		while (!_queue.empty()) {
			//removes element from que so that a new element can be added directly
			//in case it is a sigle element list
			auto arg = _queue.front();
			_queue.pop();
			matsig_flush_loop(it->call(arg.data));
		}
	}


	//Call without putting on the queue
	//Remember that this probably only should be called from the gui thread
	template <typename T = _returnValue>
	typename std::enable_if<std::is_void<T>::value>::type //This checks if the return value is void
	directCall(_argument arg = 0) {
		if (!this->empty()) {
			matsig_flush_loop(it->call(arg));
		}
	}

	//The same as above but with return value
	//Note that it only returns the last return value
	template <typename T = _returnValue>
	typename std::enable_if<!std::is_void<T>::value, T>::type //Check for non void return value
	directCall(_argument arg = 0) {
		if (!this->empty()) {
			_returnValue ret = 0;
			matsig_flush_loop(ret = it->callReturn(arg));
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
		connect(std::bind(f, c, std::placeholders::_1), c);
	}


	//Connect to a class function with no arguments
	//Usage:
	//signalname.connect(this, &NameOfClass::nameOfMemberFunction)
	template <class _classType>
	void connect(_classType *c, void (_classType::*f)()) {
		auto bound = std::bind(f, c);
		connect([bound](_argument) {
			bound(); //Ignore argument
		}, c);
	}



	//Generic connection method
	//Example usage:
	//signalname.connect([] (argument_type arg) {}
	//signalname.connect(nameOfFunction)
	void connect(_functionType f, void *reference = 0) {
		LockGuard guard(_mutex);
		this->push_back({f, reference});
	}


	// Connect a function with void arguments
	void connect(std::function<_returnValue()> f, void *reference = 0) {
		LockGuard guard(_mutex);
		this->push_back({[f](_argument arg) {
			// Just ignoring argument
			return f();
		}, reference});
	}


	//Disconnect connection by reference
	template <typename T>
	void disconnect(T *reference) {
		LockGuard guard(_mutex);
		for (auto &connection: *this) {
			if (connection.reference == reference) {
				connection.clear(); // Clears connection to be removed later
			}
		}
	}

	// Remove all connections
	void disconnectAll() {
		LockGuard guard(_mutex);
		for (auto &connection: *this) {
			connection.clear();
		}
	}


	//Clear signals queue
	void clearQueue() {
		//A efficient way to clear lists according to the internet
		std::queue<ArgumentClass<_argument>> emptyQueue;
		std::swap( _queue, emptyQueue );
	}


	void onlySaveLast(bool value) {
		_onlySaveLast = value;
	}

	bool onlySaveLast() {
		return _onlySaveLast;
	}

protected:
	std::queue<ArgumentClass<_argument>> _queue;
};




// Signal specialization _without_ arguments
template <class _returnValue>
class Signal<void, _returnValue>: SignalBase, protected std::list<StdFunctionConnection<_returnValue(void), _returnValue>>{
public:
	typedef void *_argument;
	typedef std::function<_returnValue(void)> _functionType;
	Signal(bool onlySaveLast = false){
		_onlySaveLast = onlySaveLast;
	}

	//Put a call on the queue
	void emit() {
		if (!this->empty()) { //Dont bother to broadcast if no object listens
			if (_onlySaveLast) { //Use this with objects emitting very many signals
				if (_queue.empty()) {
					_queue.push(0);
				}
				else {
					//Note: possible non-atomic operation
					_queue.front() = 0; //Replace previous argument
				}
			}
			else {
				_queue.push(0);
			}
		}
	}


	//Shorthand for function emit(...)
	inline void operator()() {
		emit();
	}

	//Call all connected functions
	void flush() override{
		while (!_queue.empty()) {
			//removes element from que so that a new element can be added directly
			//in case it is a sigle element list
			_queue.pop();
			matsig_flush_loop(it->call());
		}
	}


	//Call without putting on the queue
	//Remember that this probably only should be called from the gui thread
	template <typename T = _returnValue>
	typename std::enable_if<std::is_void<T>::value>::type //This checks if the return value is void
	directCall() {
		if (!this->empty()) {
			matsig_flush_loop(it->call());
		}
	}

	//The same as above but with return value
	//Note that it only returns the last return value
	template <typename T = _returnValue>
	typename std::enable_if<!std::is_void<T>::value, T>::type //Check for non void return value
	directCall() {
		if (!this->empty()) {
			_returnValue ret = 0;
			matsig_flush_loop(ret = it->callReturn());
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
	void connect(_classType *c, void (_classType::*f)()) {
		connect(std::bind(f, c), c);
	}

	//Connect generic function to signal
	//signalname.connect(nameOfFunction)
	void connect(_functionType f, void *reference = 0) {
		LockGuard guard(_mutex);
		this->push_back({f, reference});
	}

	//Connect to std::function
	template <class _functionType>
	void connect(std::function<_functionType> f, void *reference = 0) {
		LockGuard guard(_mutex);
		this->push_back({f, reference});
	}



	template <typename T>
	void disconnect(T *reference) {
		LockGuard guard(_mutex);
		for (auto &connection: *this) {
			connection.clear();
		}
	}

	void disconnectAll() {
		LockGuard guard(_mutex);
		for (auto &connection: *this) {
			connection.clear();
		}
	}


	//Clear signals queue
	void clearQueue() {
		//A efficient way to clear lists according to the internet
		std::queue<_argument> emptyQueue;
		std::swap( _queue, emptyQueue );
	}


	void onlySaveLast(bool value) {
		_onlySaveLast = value;
	}

	bool onlySaveLast() {
		return _onlySaveLast;
	}

protected:
	std::queue<_argument> _queue;
};





inline void flushSignals() {
	SignalBase::flushAll();
}

void assertMainThread();
void setMainThread();

} //namespace MatSig
