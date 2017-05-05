//
// Copyright (c) 2017 Advanced Micro Devices, Inc. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include <vulkan/vulkan.h>
#include <dxgi.h>

// For ComPtr
#include <wrl.h>

#include <iostream>
#include <iomanip>
#include <vector>

using namespace Microsoft::WRL;

std::wostream& operator<< (std::wostream& os, const LUID& luid)
{
    std::wostream ws (os.rdbuf ());
    ws   << std::hex << std::setw (8) << std::setfill (L'0') << luid.HighPart
         << std::hex << std::setw (8) << std::setfill (L'0') << luid.LowPart;
    return os;
}

std::wostream& operator<< (std::wostream& os, uint8_t id [8])
{
    std::wostream ws (os.rdbuf ());
    for (int i = 7; i >= 0; --i) {
        ws  << std::hex << std::setw (2) << std::setfill (L'0') << id [i];
    }

    return os;
}

int main ()
{
    // Enumerate DXGI adapters
    ComPtr<IDXGIFactory> dxgiFactory;
    if (FAILED (CreateDXGIFactory (IID_PPV_ARGS (&dxgiFactory)))) {
        std::wcerr << "Cannot create DXGI factory, exiting" << std::endl;
    }

    std::wcout << "Enumerating DXGI devices" << std::endl;

    UINT i = 0;
    IDXGIAdapter* adapter = nullptr;
    while (dxgiFactory->EnumAdapters (i, &adapter) != DXGI_ERROR_NOT_FOUND) {
        DXGI_ADAPTER_DESC desc;
        adapter->GetDesc (&desc);

        std::wcout << "    DXGI Adapter:  " << desc.Description << " - LUID: " << desc.AdapterLuid << std::endl;
        
        adapter->Release ();

        ++i;
    }

    std::wcout << std::endl;
    std::wcout << "Enumerating Vulkan devices" << std::endl;

    // Enumerate VK Devices
    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

    std::vector<const char*> instanceExtensions = {
        "VK_KHX_external_memory_capabilities",
        "VK_KHR_get_physical_device_properties2"
    };

    instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data ();
    instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t> (instanceExtensions.size ());

    VkInstance instance = nullptr;
    if (vkCreateInstance (&instanceCreateInfo, nullptr, &instance) != VK_SUCCESS) {
        std::wcerr << "Cannot create Vulkan instance with the required extensions, exiting" << std::endl;
        exit (1);
    }

    PFN_vkGetPhysicalDeviceProperties2KHR getPhysicalDeviceProperties2KHR =
        reinterpret_cast<PFN_vkGetPhysicalDeviceProperties2KHR> (
            vkGetInstanceProcAddr (instance, "vkGetPhysicalDeviceProperties2KHR"));

    if (getPhysicalDeviceProperties2KHR == nullptr) {
        std::wcerr << "Could not retrieve 'vkGetPhysicalDeviceProperties2KHR' pointer, exiting" << std::endl;
    }
    
    uint32_t physicalDeviceCount = 0;
    vkEnumeratePhysicalDevices (instance, &physicalDeviceCount, nullptr);

    std::vector<VkPhysicalDevice> devices{ physicalDeviceCount };
    vkEnumeratePhysicalDevices (instance, &physicalDeviceCount,
        devices.data ());

    for (const auto& device : devices) {
        VkPhysicalDeviceProperties2KHR properties = {};
        properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR;

        VkPhysicalDeviceIDPropertiesKHX deviceIdProperties = {};
        deviceIdProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ID_PROPERTIES_KHX;

        properties.pNext = &deviceIdProperties;

        getPhysicalDeviceProperties2KHR (device, &properties);

        std::wcout << "    Vulkan device: " << properties.properties.deviceName << " - LUID: " << deviceIdProperties.deviceLUID << std::endl;
    }
    
    return 0;
}
