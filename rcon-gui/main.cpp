#include "Rcon.hpp"

int main(int ac, char** av)
{
    ZHTTPD::RCON::Rcon rcon(ac, av);
    return rcon.run();
}

