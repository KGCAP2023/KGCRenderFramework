
#include "pch.h"
#include "Observer.h"

class MouseUpObserver {

public:
	void AddObserver(const std::string& _obName, IDefaultObserver* _ob);
	void RemoveObserver(const std::string& _obName);
	void OnNotify();


private:
	std::unordered_map<std::string, IDefaultObserver*> mouseUpListenerList;

};

class test : public IDefaultObserver {

public:
	virtual void notify() override {
		std::cout << "notifyed" << std::endl;
	};

};


static class GetMouseUpObserver {


public:

	static MouseUpObserver& GetInstance() {
		static MouseUpObserver instance;
		return instance;
	}

};
