// Local includes
#include "gvw.hpp"

template<typename UserT, typename VulkanT>
std::vector<UserT> gvw::UserItemsMissingInVulkanArray(
    const std::vector<UserT>& User_Array,
    const std::vector<VulkanT>& Vulkan_Array,
    bool (*Is_Identical)(UserT, VulkanT))
{
    std::vector<UserT> userStringsMissing = {};
    std::list<VulkanT> vulkanArrayCopy(Vulkan_Array.begin(),
                                       Vulkan_Array.end());
    for (UserT userItem : User_Array) {
        bool matchFound = false;
        for (auto vulkanIt = vulkanArrayCopy.begin();
             vulkanIt != vulkanArrayCopy.end();
             ++vulkanIt) {
            if (Is_Identical(userItem, *vulkanIt)) {
                vulkanIt = vulkanArrayCopy.erase(vulkanIt);
                matchFound = true;
                break;
            }
        }
        if (!matchFound) {
            userStringsMissing.emplace_back(userItem);
        }
    }
    return userStringsMissing;
}

template<typename UserT, typename VulkanT>
std::vector<UserT> gvw::UserItemsFoundInVulkanArray(
    const std::vector<UserT>& User_Array,
    const std::vector<VulkanT>& Vulkan_Array,
    bool (*Is_Identical)(UserT, VulkanT))
{
    std::vector<UserT> userStringsFound = {};
    std::list<VulkanT> vulkanArrayCopy(Vulkan_Array.begin(),
                                       Vulkan_Array.end());
    for (UserT userItem : User_Array) {
        bool matchFound = false;
        for (auto vulkanIt = vulkanArrayCopy.begin();
             vulkanIt != vulkanArrayCopy.end();
             ++vulkanIt) {
            if (Is_Identical(userItem, *vulkanIt)) {
                vulkanIt = vulkanArrayCopy.erase(vulkanIt);
                matchFound = true;
                break;
            }
        }
        if (matchFound) {
            userStringsFound.emplace_back(userItem);
        }
    }
    return userStringsFound;
}

template<typename T>
T gvw::ReadFile(const char* Absolute_Path)
{
    std::ifstream file(Absolute_Path, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        gvwErrorCallback(("Failed to open shader file: \"" +
                          std::string(Absolute_Path) + "\"")
                             .c_str());
    }
    size_t fileSize = file.tellg();
    T charBuffer;
    charBuffer.resize(fileSize);
    file.seekg(0);
    file.read(charBuffer.data(),
              static_cast<std::streamsize>(charBuffer.size()));
    file.close();

    return charBuffer;
}
