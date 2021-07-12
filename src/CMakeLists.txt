set(SRCS main.cpp)

add_executable(vulkan-learn ${SRCS})
target_link_libraries(vulkan-learn PRIVATE Vulkan::Vulkan)
