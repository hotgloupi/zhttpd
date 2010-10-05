
#include <iostream>


//
// Illustration du probleme avec ISocketHandler inutilisable en l'etat
//
// sauf si t'as une solution magique que j'ai pas trouvé
//

class IHandler
{
public:
    typedef void (IHandler::*callback_t)(void);
};

class Handler : public IHandler
{
public:
    void handle(void)
    {
        std::cout << "ca compile meme pas lol" << std::endl;
    }
};

class MachinPool
{
public:
    template<typename Class>
    void ReceiveHandler(Class& handler, void (Class::*callback)() )
    {
        IHandler* h = static_cast<IHandler*>(&handler);
        IHandler::callback_t cb = static_cast<IHandler::callback_t>(callback);
        (h->*cb)();
        (handler.*callback)();
    }
};


int main()
{
    MachinPool poule;
    Handler h;
    poule.ReceiveHandler(h, &Handler::handle);
}

