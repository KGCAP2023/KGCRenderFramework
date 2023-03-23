#pragma once
#include "pch.h"
#include "Framework.h"

class IFrameworkFactory
{
private:
    static Framework* f;
public:
    static Framework* createFramework() {
        if (f != nullptr) return f;
        else return new Framework;
    }
};

Framework* IFrameworkFactory::f;