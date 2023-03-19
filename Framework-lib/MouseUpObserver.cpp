#include "pch.h"
#include "MouseUpObserver.h"

void MouseUpObserver::AddObserver(const std::string& _obName, IDefaultObserver* _ob)
{
	if (mouseUpListenerList.find(_obName) != mouseUpListenerList.end()) {
		assert("ม฿บน");
	}

	mouseUpListenerList.insert(std::make_pair<>(_obName, _ob));

}

void MouseUpObserver::RemoveObserver(const std::string& _obName)
{

	if (mouseUpListenerList.find(_obName) != mouseUpListenerList.end()) {
		mouseUpListenerList.erase(_obName);
	}

}


void MouseUpObserver::OnNotify()
{
	for (auto& _o : mouseUpListenerList) {
		_o.second->notify();
	}
}

