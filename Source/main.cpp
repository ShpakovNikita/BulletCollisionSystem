#include <stdlib.h>
#include <stdexcept>
#include <iostream>
#include "ApplicationImpl.hpp"

int main(int, char**)
{
    ApplicationImpl app;

    try
    {
        app.Run();
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
