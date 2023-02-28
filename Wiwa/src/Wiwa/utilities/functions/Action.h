#pragma once
class Callable {};

template<class... _params>
class Action {
private:
	void (Callable::* action)(_params...);
	Callable* obj;
public:
	Action() { obj = NULL; };

	template<class T>
	Action(void (T::* _action)(_params...), void* _obj) {
		action = (void (Callable::*)(_params...))_action;
		obj = (Callable*)_obj;
	}

	void execute(_params... params) {
		(obj->*action)(params...);
	}

	void operator()(_params... params) {
		execute(params...);
	}

	bool HasAction() { return obj != NULL; }

	void* getObj() { return obj; }
	void* getActionPtr() { return (void*&)action; }
};