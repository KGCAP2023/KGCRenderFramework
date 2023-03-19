#pragma once
#include "pch.h"
#include "Framework.h"

class IFrameworkFactory
{
private:
    static IFramework* f;
public:
    static IFramework* createFramework() {
        if (f != nullptr) return f;
        else return new Framework;
    }
};

IFramework* IFrameworkFactory::f;