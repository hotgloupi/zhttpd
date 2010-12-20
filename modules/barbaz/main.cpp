
#include <iostream>

#include "BarbazModuleManager.hpp"

extern "C"
{
    ZHTTPD_EXPORT zhttpd::api::IModuleManager* getInstance()
    {
        return new BarbazModuleManager();
    }
}



class IView
{
}


template<typename View>
struct ViewAdapter : IViewAdapter
{
private:

public:
    ViewAdapter(IType const& type)
    {

        View::registerAdapter(type.GetType(), this);
        std::cout << "BITE\n";
    }
    virtual char* convert(IType) = 0;
};

template<typename T>
class MakeAdapter
{
private:
    static std::map<int, IViewAdapter*> _map;

public:
    static registerAdapter(int t, IViewAdapter* v)
    {
        assert(MakeAdapter::_map.find(t) == MakeAdapter::_map.end());
        MakeAdapter::_map[t] = v;
    }
};

class IJsonView : public IView, public MakeAdapter<IJsonView>
{
};

class UserJsonConverter: public ViewAdapter<IJsonView>, public IView
{
    UserJsonConverter() : ViewAdapter<IJsonView>(User())
    {
    }

    virtual char* convert(IType) {return "user";}
}

static UserJsonView a;

A a;



IViewAdapter& findAdapter(int t, IView& v)
{
    return v.getAdapter(t);
}


view = AllViews[ext]; //IJsonView

adapter = findAdapter(data.getType(), view) // data == User&

return adapter.convert(data);













