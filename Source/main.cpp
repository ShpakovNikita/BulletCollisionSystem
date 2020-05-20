#include "ApplicationImpl.hpp"

#include <stdexcept>
#include <iostream>

int main(int, char**)
{
    try
    {
        ApplicationImpl app;
        app.Run();
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
