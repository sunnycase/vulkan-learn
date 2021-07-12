#include <vulkan/vulkan.hpp>
#include <iostream>

#define DEBUG_DUMP 1

class program
{
public:
    void initialize()
    {
        initialize_instance();
        initialize_device();
        vk::MemoryAllocateInfo alloc_info();
        //device_.GetMemoryT()
    }

private:
    void initialize_instance()
    {
        vk::ApplicationInfo app_info("nncase.runtime", 1, "nncase", 1, VK_API_VERSION_1_1);
        vk::InstanceCreateInfo create_info({}, &app_info);
        instance_ = vk::createInstance(create_info);
    }

    void initialize_device()
    {
        physical_device_ = select_physical_device();
        auto queue_families = physical_device_.getQueueFamilyProperties();
        auto compute_queue_index = select_queue_family(queue_families, vk::QueueFlagBits::eCompute);

        float priorities[] = {0.0f};
        vk::DeviceQueueCreateInfo queue_create_info({}, compute_queue_index, 1, priorities);
        vk::DeviceCreateInfo device_create_info({}, queue_create_info);
        device_ = physical_device_.createDevice(device_create_info);
        compute_queue_ = device_.getQueue(compute_queue_index, 0);

        auto memory_properties = physical_device_.getMemoryProperties();
    }

    vk::PhysicalDevice select_physical_device()
    {
        vk::PhysicalDevice *intergrated = nullptr;

        auto devices = instance_.enumeratePhysicalDevices();
        for (auto &device : devices)
        {
            auto properties = device.getProperties();
#if DEBUG_DUMP
            std::cout << properties.deviceName << std::endl;
#endif
            if (properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
                return device;
            else if (properties.deviceType == vk::PhysicalDeviceType::eIntegratedGpu)
                intergrated = &device;
        }

        if (intergrated)
            return *intergrated;
        else if (!devices.empty())
            return devices.front();
        else
            throw std::runtime_error("No vulkan device found");
    }

    size_t select_queue_family(const std::vector<vk::QueueFamilyProperties> &families, vk::QueueFlagBits desiredFlag)
    {
        size_t nonoptimal = -1;
        for (size_t i = 0; i < families.size(); i++)
        {
            auto &family = families[i];
            if (family.queueFlags == desiredFlag)
                return i;
            else if ((family.queueFlags & desiredFlag) == desiredFlag)
                nonoptimal = i;
        }

        if (nonoptimal != -1)
            return nonoptimal;
        else
            throw std::runtime_error("Vulkan queue not found: " + vk::to_string(desiredFlag));
    }

    size_t select_memory_type(const vk::PhysicalDeviceMemoryProperties &properties)
    {
        for (size_t i = 0; i < properties.memoryTypes.size(); i++)
        {
            auto &type = properties.memoryTypes[i];
        }
        
    }

private:
    vk::Instance instance_;
    vk::PhysicalDevice physical_device_;
    vk::Device device_;
    vk::Queue compute_queue_;
};

int main()
{
    program p;
    p.initialize();

    vk::ApplicationInfo app_info("nncase.runtime", 1, "nncase", 1, VK_API_VERSION_1_1);
    vk::InstanceCreateInfo create_info({}, &app_info);
    auto instance = vk::createInstance(create_info);
    auto physical_devices = instance.enumeratePhysicalDevices();
    size_t optimal_device = -1;
    size_t optimal_queue_familiy = -1;
    size_t backup_device = -1;
    size_t backup_queue_family = -1;

    for (size_t physical_device_id = 0; physical_device_id < physical_devices.size(); physical_device_id++)
    {
        auto &physical_device = physical_devices[physical_device_id];

        auto queue_family_properties = physical_device.getQueueFamilyProperties();
        for (size_t queue_family_id = 0; queue_family_id < queue_family_properties.size(); queue_family_id++)
        {
            auto &queue_property = queue_family_properties[queue_family_id];
            if (queue_property.queueFlags == vk::QueueFlagBits::eCompute)
            {
                optimal_device = physical_device_id;
                optimal_queue_familiy = queue_family_id;
                break;
            }
            else if (queue_property.queueFlags & vk::QueueFlagBits::eCompute)
            {
                backup_device = physical_device_id;
                backup_queue_family = queue_family_id;
            }
        }

        if (optimal_queue_familiy != -1)
            break;
    }
}
