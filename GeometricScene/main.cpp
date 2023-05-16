#include <iostream>
#include <dlfcn.h>

typedef void (*PFN_SAY_NAME)();

int main(int argc, char* argv[])
{
    std::cout << "Result" << std::endl;
    void* handle = dlopen("libGeom.dylib", RTLD_LAZY);
    if (!handle) {
        std::cout << "Could not open the library" << std::endl;
        return 1;
    }

    PFN_SAY_NAME RunDemoDyn = reinterpret_cast<PFN_SAY_NAME>(dlsym(handle, "RunDemo"));
    if (!RunDemoDyn) {
        std::cout << "Could not find RunDemo" << std::endl;
        dlclose(handle);
        return 1;
    }

    RunDemoDyn();
    dlclose(handle);
    
    return 0;
}

