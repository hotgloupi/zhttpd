
#include <iostream>

#include "BarbazModuleManager.hpp"

extern "C"
{
    ZHTTPD_EXPORT zhttpd::api::IModuleManager* getInstance()
    {
        return new BarbazModuleManager();
    }
}

# include "UserJsonView.hpp"
//
//
//IViewAdapter& findAdapter(int t, IView& v)
//{
//    return v.getAdapter(t);
//}
//
//
//view = AllViews[ext]; //IJsonView
//
//adapter = findAdapter(data.getType(), view) // data == User&
//
//return adapter.convert(data);













