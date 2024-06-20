#pragma once
#include "Common_Vulkan.h"
#include "Graphic/Buffer.h"

namespace graphic {

class Buffer_Vulkan : public Buffer {
    friend class Device_Vulkan;
    friend class CommandList_Vulkan;
public:
    ~Buffer_Vulkan();
    Buffer_Vulkan(Device_Vulkan* device, const BufferDesc& desc, const void* init_data = nullptr);

private:
    Device_Vulkan* device_;
    VkBuffer handle_;
    VmaAllocation allocation_;
    VmaAllocationInfo allocInfo_;
    VkDeviceAddress bufferDeviceAddress_;

};

CONVERT_TO_VULKAN_INTERNAL(Buffer)
}