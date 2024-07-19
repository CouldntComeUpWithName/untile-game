add_subdirectory(glad)
add_subdirectory(GLFW)
add_subdirectory(spdlog)
add_subdirectory(imgui)




#setting target properties
#glfw
# set_target_properties(glfw PROPERTIES FOLDER "Deps")
# set_target_properties(docs PROPERTIES FOLDER "Deps/GLFW3")
# set_target_properties(uninstall PROPERTIES FOLDER "Deps/GLFW3")
# set_target_properties(update_mappings PROPERTIES FOLDER "Deps/GLFW3")

#spdlog
set_target_properties(spdlog PROPERTIES FOLDER "Deps")

#imgui
set_target_properties(imgui PROPERTIES FOLDER "Deps")
