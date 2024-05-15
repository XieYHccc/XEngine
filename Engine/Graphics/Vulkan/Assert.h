#pragma once
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vk_mem_alloc.h>

#define VK_ASSERT(x)                                                             \
    do {                                                                         \
        VkResult err = x;                                                        \
        if (err) {                                                               \
            CORE_LOGC("Detected Vulkan error: {}", string_VkResult(err)) \
            abort();                                                             \
        }                                                                        \
    } while (0)





