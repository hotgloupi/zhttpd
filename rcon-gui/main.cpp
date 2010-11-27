#include "Rcon.hpp"

int main(int ac, char** av)
{
    zhttpd::RCON::Rcon rcon(ac, av);
    return rcon.run();
}

