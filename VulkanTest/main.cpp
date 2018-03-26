#pragma comment(lib, "vulkan-1.lib")    // I know I should link dynamically 
                                        // but for not I just want to focus on 
                                        // the API.
#pragma comment(lib, "gdi32.lib")

#include <Windows.h>
#include <vulkan.h>
#include <vulkan_win32.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

//#define LOG OutputDebugStringA
#define LOG printf

static LRESULT CALLBACK WindowProcess(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int main()
{
    struct SwapchainBuffers
    {
        VkImage image;
        VkImageView view;
    };

    const uint32_t kCommandBufferCount = 1;
    const uint32_t kWindowWidth = 640;
    const uint32_t kWindowHeight = 480;
    const char* kWindowTitle = "VulkanTest";

    HINSTANCE moduleInstance;
    HWND windowHandle = NULL;
    VkDevice vulkanDevice = NULL;
    VkInstance vulkanInstance = NULL;
    VkCommandPool vulkanCommandPool = NULL;
    VkCommandBuffer vulkanCommandBuffers[1] = { NULL };
    VkSurfaceKHR vulkanSurface = NULL;
    VkSwapchainKHR vulkanSwapchain = NULL;
    VkPhysicalDevice* pPhysicalDevices = NULL;
    VkQueueFamilyProperties* pQueueFamilyProperties = NULL;
    SwapchainBuffers* pSwapchainBuffers = NULL;

    uint32_t swapchainBufferCount = 0;
    uint32_t physicalDeviceCount = 0;
    uint32_t queueFamilyPropertyCount = 0;
    uint32_t queueGraphicsFamilyIndex = 0xFFFFFFFFU;
    uint32_t queuePresentFamilyIndex = 0xFFFFFFFFU;

    LOG("VulkanTest Begin\n\n");

    #pragma region Create Window
    {
        WNDCLASS wndClass;
        HWND wndHandle;
        DWORD style = WS_VISIBLE | WS_CAPTION | WS_SYSMENU;
        RECT fullScreen = { 0, 0, (LONG)kWindowWidth, (LONG)kWindowHeight };

        moduleInstance = GetModuleHandle(NULL);
        if (moduleInstance == NULL)
        {
            LOG("Failed: Create Window");
            goto Exit;
        }

        wndClass.style = CS_OWNDC;
        wndClass.lpfnWndProc = &WindowProcess;
        wndClass.cbClsExtra = 0;
        wndClass.cbWndExtra = 0;
        wndClass.hInstance = moduleInstance;
        wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        wndClass.lpszMenuName = NULL;
        wndClass.lpszClassName = "WindowClass0";
        RegisterClass(&wndClass);
        AdjustWindowRect(&fullScreen, style, 0);
        wndHandle = CreateWindow(
            "WindowClass0", kWindowTitle,
            style,
            CW_USEDEFAULT, CW_USEDEFAULT,
            fullScreen.right - fullScreen.left, fullScreen.bottom - fullScreen.top,
            NULL, NULL, moduleInstance, NULL
        );

        windowHandle = wndHandle;

        if (windowHandle == NULL)
        {
            LOG("Failed: Create Window");
            goto ExitDestroyVulkanCommandBuffers;
        }
    }
    #pragma endregion
    LOG("Passed: Create Window\n");

    #pragma region Create Vulkan Instance
    {
        const char* kExtensionNames[] = { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME };
        VkInstanceCreateInfo vulkanInstanceCreateInfo = {};
        VkApplicationInfo vulkanAppInfo = {};
        VkResult result;

        vulkanAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        vulkanAppInfo.pNext = NULL;
        vulkanAppInfo.pApplicationName = "VulaknTest";
        vulkanAppInfo.applicationVersion = 1;
        vulkanAppInfo.pEngineName = "NoEngine";
        vulkanAppInfo.engineVersion = 1;
        vulkanAppInfo.apiVersion = VK_API_VERSION_1_0;

        vulkanInstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        vulkanInstanceCreateInfo.pNext = NULL;
        vulkanInstanceCreateInfo.flags = 0;
        vulkanInstanceCreateInfo.pApplicationInfo = &vulkanAppInfo;
        vulkanInstanceCreateInfo.enabledLayerCount = 0;
        vulkanInstanceCreateInfo.ppEnabledLayerNames = NULL;
        vulkanInstanceCreateInfo.enabledExtensionCount = 2;
        vulkanInstanceCreateInfo.ppEnabledExtensionNames = kExtensionNames;

        result = vkCreateInstance(&vulkanInstanceCreateInfo, NULL, &vulkanInstance);

        if (result == VK_ERROR_INCOMPATIBLE_DRIVER)
        {
            LOG("Failed: Not Vulkan driver found\n");
            goto ExitDestroyWindow;
        }
        else if (result)
        {
            LOG("Failed: Unknown Vulkan Error\n");
            goto ExitDestroyWindow;
        }
    }
    #pragma endregion
    LOG("Passed: Create Vulkan Instance\n");

    #pragma region Create Surface
    {
        VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
        surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        surfaceCreateInfo.pNext = NULL;
        surfaceCreateInfo.hinstance = moduleInstance;
        surfaceCreateInfo.hwnd = windowHandle;

        VkResult result = vkCreateWin32SurfaceKHR(vulkanInstance, &surfaceCreateInfo, NULL, &vulkanSurface);

        if (result != VK_SUCCESS)
        {
            LOG("Failed: Create Surface\n");
            goto ExitDetroyInstance;
        }
    }
    #pragma endregion
    LOG("Passed: Create Surface\n");

    #pragma region Enumerate Physical Devices
    {
        vkEnumeratePhysicalDevices(vulkanInstance, &physicalDeviceCount, NULL);
        if (physicalDeviceCount <= 0)
        {
            LOG("Failed: Enumerate Physical Devices\n");
            goto ExitDestroySurface;
        }
        pPhysicalDevices = (VkPhysicalDevice*)malloc(physicalDeviceCount * sizeof(VkPhysicalDevice));
        vkEnumeratePhysicalDevices(vulkanInstance, &physicalDeviceCount, pPhysicalDevices);
    }
    #pragma endregion
    LOG("Passed: Enumerate Physical Devices\n");

    #pragma region Create Logical Device
    {
        vkGetPhysicalDeviceQueueFamilyProperties(pPhysicalDevices[0], &queueFamilyPropertyCount, NULL);
        if (queueFamilyPropertyCount <= 0)
        {
            LOG("Failed: Get Queue Family Properties\n");
            goto ExitFreePhysicalDevices;
        }
        pQueueFamilyProperties = (VkQueueFamilyProperties*)malloc(queueFamilyPropertyCount * sizeof(VkQueueFamilyProperties));
        vkGetPhysicalDeviceQueueFamilyProperties(pPhysicalDevices[0], &queueFamilyPropertyCount, pQueueFamilyProperties);

        VkBool32* pCanPresent = (VkBool32*)malloc(sizeof(VkBool32) * physicalDeviceCount);
        for (uint32_t index = 0; index < physicalDeviceCount; ++index)
        {
            vkGetPhysicalDeviceSurfaceSupportKHR(pPhysicalDevices[index], index, vulkanSurface, &pCanPresent[index]);
        }

        // For now just focus on graphics queue
        bool pass = false;
        for (uint32_t index = 0; index < queueFamilyPropertyCount; ++index)
        {
            if ((pQueueFamilyProperties[index].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
            {
                if (queueGraphicsFamilyIndex == 0xFFFFFFFFU)
                    queueGraphicsFamilyIndex = index;
    
                if (pCanPresent[index] == VK_TRUE)
                {
                    queuePresentFamilyIndex = index;
                    pass = true;
                    break;
                }
            }
        }

        free(pCanPresent);

        if (!pass)
        {
            LOG("Failed: Couldn't get a family queue that handles graphics commands.\n");
            goto ExitFreeQueueFamilyProperties;
        }

        VkDeviceQueueCreateInfo queueCreateInfo = {};
        float priorities[1] = { 0.0f };
        queueCreateInfo.queueFamilyIndex = queueGraphicsFamilyIndex;
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.pNext = NULL;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = priorities;

        // Logical Device Creation
        const char* kDeviceExtensions[1] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
        VkDeviceCreateInfo deviceCreateInfo = {};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pNext = NULL;
        deviceCreateInfo.queueCreateInfoCount = 1;
        deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
        deviceCreateInfo.enabledExtensionCount = 1;
        deviceCreateInfo.ppEnabledExtensionNames = kDeviceExtensions;
        deviceCreateInfo.enabledLayerCount = 0;
        deviceCreateInfo.ppEnabledLayerNames = NULL;
        deviceCreateInfo.pEnabledFeatures = NULL;

        VkResult result = vkCreateDevice(pPhysicalDevices[0], &deviceCreateInfo, NULL, &vulkanDevice);
        if (result != VK_SUCCESS)
        {
            LOG("Failed: Create Logical Device\n");
            goto ExitFreeQueueFamilyProperties;
        }
    }
    #pragma endregion
    LOG("Passed: Create Logical Device\n");

    #pragma region Create Swapchain
    {
        uint32_t formatCount = 0;
        uint32_t presentModeCount = 0;
        VkFormat format;
        VkResult result = vkGetPhysicalDeviceSurfaceFormatsKHR(pPhysicalDevices[0], vulkanSurface, &formatCount, NULL);
        if (result != VK_SUCCESS)
        {
            LOG("Failed: Create Swapchain\n");
            goto ExitDestroyVulkanDevice;
        }

        VkSurfaceFormatKHR* pSurfaceFormat = (VkSurfaceFormatKHR*)malloc(formatCount * sizeof(VkSurfaceFormatKHR));
        result = vkGetPhysicalDeviceSurfaceFormatsKHR(pPhysicalDevices[0], vulkanSurface, &formatCount, pSurfaceFormat);
        if (result != VK_SUCCESS)
        {
            LOG("Failed: Create Swapchain\n");
            free(pSurfaceFormat);
            goto ExitDestroyVulkanDevice;
        }

        if (formatCount == 1 && pSurfaceFormat[0].format == VK_FORMAT_UNDEFINED)
        {
            // Surface has no preferred format.
            format = VK_FORMAT_R8G8B8A8_UNORM;
        }
        else
        {
            if (formatCount <= 0)
            {
                LOG("Failed: Create Swapchain\n");
                free(pSurfaceFormat);
                goto ExitDestroyVulkanDevice;
            }

            format = pSurfaceFormat[0].format;
        }
        free(pSurfaceFormat);

        VkSurfaceCapabilitiesKHR surfaceCapabilities = {};
        result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pPhysicalDevices[0], vulkanSurface, &surfaceCapabilities);
        if (result != VK_SUCCESS)
        {
            LOG("Failed: Create Swapchain\n");
            goto ExitDestroyVulkanDevice;
        }

        result = vkGetPhysicalDeviceSurfacePresentModesKHR(pPhysicalDevices[0], vulkanSurface, &presentModeCount, NULL);
        if (result != VK_SUCCESS)
        {
            LOG("Failed: Create Swapchain\n");
            goto ExitDestroyVulkanDevice;
        }

        VkPresentModeKHR* pPresentModes = (VkPresentModeKHR*)malloc(presentModeCount * sizeof(VkPresentModeKHR));
        result = vkGetPhysicalDeviceSurfacePresentModesKHR(pPhysicalDevices[0], vulkanSurface, &presentModeCount, NULL);
        if (result != VK_SUCCESS)
        {
            LOG("Failed: Create Swapchain\n");
            goto ExitDestroyVulkanDevice;
        }

        VkExtent2D swapchainExtent = { 0 };

        if (surfaceCapabilities.currentExtent.width == 0xFFFFFFFF)
        {
            // Surface size is undefined.
            swapchainExtent.width = kWindowWidth;
            swapchainExtent.height = kWindowHeight;

            if (swapchainExtent.width < surfaceCapabilities.minImageExtent.width)
            {
                swapchainExtent.width = surfaceCapabilities.minImageExtent.width;
            }
            else if (swapchainExtent.height < surfaceCapabilities.minImageExtent.height)
            {
                swapchainExtent.height = surfaceCapabilities.minImageExtent.height;
            }

            if (swapchainExtent.width > surfaceCapabilities.maxImageExtent.width)
            {
                swapchainExtent.width = surfaceCapabilities.maxImageExtent.width;
            }
            else if (swapchainExtent.height > surfaceCapabilities.maxImageExtent.height)
            {
                swapchainExtent.height = surfaceCapabilities.maxImageExtent.height;
            }
        }
        else
        {
            swapchainExtent = surfaceCapabilities.currentExtent;
        }

        // Default present mode
        VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
        uint32_t desiredImagesForSwapchain = surfaceCapabilities.minImageCount;

        VkSurfaceTransformFlagBitsKHR preTransform;
        if ((surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) != 0)
        {
            preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        }
        else
        {
            preTransform = surfaceCapabilities.currentTransform;
        }

        // Default composite alpha mode
        VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        VkCompositeAlphaFlagBitsKHR compositeAlphaFlags[4] = {
            VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
            VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
            VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR
        };

        for (uint32_t index = 0; index < 4; ++index)
        {
            if ((surfaceCapabilities.supportedCompositeAlpha & compositeAlphaFlags[index]) != 0)
            {
                compositeAlpha = compositeAlphaFlags[index];
                break;
            }
        }

        VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
        swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainCreateInfo.pNext = NULL;
        swapchainCreateInfo.surface = vulkanSurface;
        swapchainCreateInfo.minImageCount = desiredImagesForSwapchain;
        swapchainCreateInfo.imageFormat = format;
        swapchainCreateInfo.imageExtent.width = swapchainExtent.width;
        swapchainCreateInfo.imageExtent.height = swapchainExtent.height;
        swapchainCreateInfo.preTransform = preTransform;
        swapchainCreateInfo.compositeAlpha = compositeAlpha;
        swapchainCreateInfo.imageArrayLayers = 1;
        swapchainCreateInfo.presentMode = swapchainPresentMode;
        swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
        swapchainCreateInfo.clipped = VK_TRUE;
        swapchainCreateInfo.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
        swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        if (queueGraphicsFamilyIndex != queuePresentFamilyIndex)
        {
            swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            swapchainCreateInfo.queueFamilyIndexCount = 0;
            swapchainCreateInfo.pQueueFamilyIndices = NULL;
        }
        else
        {
            uint32_t indices[2] = { queueGraphicsFamilyIndex, queuePresentFamilyIndex };
            swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            swapchainCreateInfo.queueFamilyIndexCount = 2;
            swapchainCreateInfo.pQueueFamilyIndices = indices;
        }

        result = vkCreateSwapchainKHR(vulkanDevice, &swapchainCreateInfo, NULL, &vulkanSwapchain);
        if (result != VK_SUCCESS)
        {
            LOG("Failed: Create Swapchain\n");
            goto ExitDestroyVulkanDevice;
        }

        result = vkGetSwapchainImagesKHR(vulkanDevice, vulkanSwapchain, &swapchainBufferCount, NULL);
        if (result != VK_SUCCESS)
        {
            LOG("Failed: Create Swapchain\n");
            goto ExitDetroySwapchain;
        }

        VkImage* pSwapchainImages = (VkImage*)malloc(swapchainBufferCount * sizeof(VkImage));
        result = vkGetSwapchainImagesKHR(vulkanDevice, vulkanSwapchain, &swapchainBufferCount, pSwapchainImages);
        if (result != VK_SUCCESS)
        {
            LOG("Failed: Create Swapchain\n");
            goto ExitDetroySwapchain;
        }

        pSwapchainBuffers = (SwapchainBuffers*)malloc(swapchainBufferCount * sizeof(SwapchainBuffers));
        ZeroMemory(pSwapchainBuffers, swapchainBufferCount * sizeof(SwapchainBuffers));
        for (uint32_t index = 0; index < swapchainBufferCount; ++index)
        {
            pSwapchainBuffers[index].image = pSwapchainImages[index];
        }
        free(pSwapchainImages);

        for (uint32_t index = 0; index < swapchainBufferCount; ++index)
        {
            VkImageViewCreateInfo colorImageView = {};
            colorImageView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            colorImageView.pNext = NULL;
            colorImageView.flags = 0;
            colorImageView.image = pSwapchainBuffers[index].image;
            colorImageView.viewType = VK_IMAGE_VIEW_TYPE_2D;
            colorImageView.format = format;
            colorImageView.components.r = VK_COMPONENT_SWIZZLE_R;
            colorImageView.components.g = VK_COMPONENT_SWIZZLE_G;
            colorImageView.components.b = VK_COMPONENT_SWIZZLE_B;
            colorImageView.components.a = VK_COMPONENT_SWIZZLE_A;
            colorImageView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            colorImageView.subresourceRange.baseMipLevel = 0;
            colorImageView.subresourceRange.levelCount = 1;
            colorImageView.subresourceRange.baseArrayLayer = 0;
            colorImageView.subresourceRange.layerCount = 1;

            result = vkCreateImageView(vulkanDevice, &colorImageView, NULL, &pSwapchainBuffers[index].view);
            if (result != VK_SUCCESS)
            {
                LOG("Failed: Create Swapchain\n");
                goto ExitFreeSwapchainBuffers;
            }
        }
    }
    #pragma endregion
    LOG("Passed: Create Swapchain\n");

    #pragma region Create Command Pool
    {
        VkCommandPoolCreateInfo commandPoolCreateInfo = {};
        commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolCreateInfo.pNext = NULL;
        commandPoolCreateInfo.queueFamilyIndex = queueGraphicsFamilyIndex;
        commandPoolCreateInfo.flags = 0;

        VkResult result = vkCreateCommandPool(vulkanDevice, &commandPoolCreateInfo, NULL, &vulkanCommandPool);

        if (result != VK_SUCCESS)
        {
            LOG("Failed: Create Command Pool");
            goto ExitFreeSwapchainBuffers;
        }
    }
    #pragma endregion
    LOG("Passed: Create Command Pool\n");

    #pragma region Create Command Buffer
    {
        VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
        commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocateInfo.pNext = NULL;
        commandBufferAllocateInfo.commandPool = vulkanCommandPool;
        commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        commandBufferAllocateInfo.commandBufferCount = 1;

        VkResult result = vkAllocateCommandBuffers(vulkanDevice, &commandBufferAllocateInfo, vulkanCommandBuffers);

        if (result != VK_SUCCESS)
        {
            LOG("Failed: Create Command Buffer\n");
            goto ExitDestroyVulkanCommandPool;
        }
    }
    #pragma endregion
    LOG("Passed: Create Command Buffer\n");

    #pragma region Destroy and Exit

ExitDestroyVulkanCommandBuffers:
    vkFreeCommandBuffers(vulkanDevice, vulkanCommandPool, kCommandBufferCount, vulkanCommandBuffers);

ExitDestroyVulkanCommandPool:
    vkDestroyCommandPool(vulkanDevice, vulkanCommandPool, NULL);

ExitFreeSwapchainBuffers:
    for (uint32_t index = 0; index < swapchainBufferCount; ++index)
    {
        if (pSwapchainBuffers[index].view != NULL)
        {
            vkDestroyImageView(vulkanDevice, pSwapchainBuffers[index].view, NULL);
        }
    }
    free(pSwapchainBuffers);

ExitDetroySwapchain:
    vkDestroySwapchainKHR(vulkanDevice, vulkanSwapchain, NULL);

ExitDestroyVulkanDevice:
    vkDestroyDevice(vulkanDevice, NULL);

ExitFreeQueueFamilyProperties:
    free(pQueueFamilyProperties);

ExitFreePhysicalDevices:
    free(pPhysicalDevices);

ExitDestroySurface:
    vkDestroySurfaceKHR(vulkanInstance, vulkanSurface, NULL);

ExitDetroyInstance:
    vkDestroyInstance(vulkanInstance, NULL);

ExitDestroyWindow:
    CloseWindow(windowHandle);

Exit:
    #pragma endregion
    LOG("\nVulkanTest End\n");

    getchar();

    return 0;
}

static LRESULT CALLBACK WindowProcess(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        return 0;
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    case WM_DESTROY:
        return 0;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            return 0;
        }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}
