/**
 *
 *
 * Compile cmd:
 * g++ -o print_info.exe print_info.cpp -I /opt/rocm/include/ -L /opt/rocm/lib/ -lOpenCL
 * ./print_info.exe
 */
#define CL_TARGET_OPENCL_VERSION     220
#define CL_HPP_TARGET_OPENCL_VERSION 200

#include <CL/cl2.hpp>
#include <iostream>
#include <string>


std::string parse_devtype(unsigned int devtype)
{
    std::string devtypestr;
    switch(devtype) {
        case CL_DEVICE_TYPE_DEFAULT:     devtypestr = "default"; break;
        case CL_DEVICE_TYPE_CPU:         devtypestr = "CPU";     break;
        case CL_DEVICE_TYPE_GPU:         devtypestr = "GPU";     break;
        case CL_DEVICE_TYPE_ACCELERATOR: devtypestr = "accel";   break;
        case CL_DEVICE_TYPE_CUSTOM:      devtypestr = "custom";  break;
        case CL_DEVICE_TYPE_ALL:         devtypestr = "all";     break;
    }
    return devtypestr;
}

int main()
{

    /**
     * Search for all the OpenCL platforms available and check
     * if there are any.
     * */

    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    if (platforms.empty()){
        std::cerr << "No platforms found!" << std::endl;
        return -1;
    }

    /**
     * Iterate over platforms.
     */
    for( auto platform = platforms.begin(); platform != platforms.end(); ++platform )
    {
        std::vector<cl::Device> devices;
        platform->getDevices(CL_DEVICE_TYPE_ALL, &devices);

        if (devices.empty()){
            std::cerr << "No devices found!" << std::endl;
            return -1;
        }

        /**
         * Print current device information.
         */
        auto device       = devices.front();
        auto name         = device.getInfo<CL_DEVICE_NAME>();
        auto devtype      = device.getInfo<CL_DEVICE_TYPE>();
        auto vendor       = device.getInfo<CL_DEVICE_VENDOR>();
        auto version      = device.getInfo<CL_DEVICE_VERSION>();
        auto workItems    = device.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();
        auto workGroups   = device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
        auto computeUnits = device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
        auto globalMemory = device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
        auto localMemory  = device.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>();

        std::cout << "OpenCL Device Info: "
            << "\nName:      " << name
            << "\nType:      " << parse_devtype(devtype)
            << "\nVendor:    " << vendor
            << "\nVersion:   " << version
            << "\nMax size of work-items:    (" << workItems[0] << "," << workItems[1] << "," << workItems[2] << ")"
            << "\nMax size of work-groups:    " << workGroups
            << "\nNumber of compute units:    " << computeUnits
            << "\nGlobal memory size (bytes): " << globalMemory
            << "\nLocal memory size per compute unit (bytes): " << localMemory/computeUnits
            << "\n" << std::endl;
    }

    return 0;
}
