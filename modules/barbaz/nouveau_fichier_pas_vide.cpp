
#include <iostream>

//#include "BarbazModuleManager.hpp"
//
//extern "C"
//{
//    ZHTTPD_EXPORT zhttpd::api::IModuleManager* getInstance()
//    {
//        return new BarbazModuleManager();
//    }
//}


enum DataType
  {
    User = 0x01,
    Song = 0x02
  };

enum ViewType
  {
    Json = 0x10,
    Html = 0x20
  };




struct ViewAdapter
{
private:

public:
  ViewAdapter(int dt, ViewType vt)
    {
      View::registerAdapter(dt, vt, this);
    }
    virtual char* convert(IType) = 0;
};

class MakeAdapter
{
private:
  static std::map<std::pair<int, ViewType>, ViewAdapter*> _map;

public:
  static registerAdapter(int dt, ViewType vt, ViewAdapter* v)
    {
        assert(MakeAdapter::_map.find(t) == MakeAdapter::_map.end());
        MakeAdapter::_map[std::pair(dt, vt)] = v;
    }
  static ViewAdapter& findAdapter(DataType t, ViewType v)
  {
    return *MakeAdapter::_map.find(t + v)->second;
  }
};
static int typeCounter = 0;

static int User2 = typeCounter++;
class UserJsonConverter: public ViewAdapter
{
  UserJsonConverter() : ViewAdapter(User2, Json)
    {
    }

    virtual char* convert(IType) {return "user";}
}

static UserJsonView a;

ViewType view = AllViews[ext]; //IJsonView

adapter = MakeAdapter::findAdapter(data.getType(), view) // data == User&

return adapter.convert(data);













