cmake_minimum_required (VERSION 3.0)

project (MayaPluginTemplate)

# Specify a list of directories to search for cmake modules.
set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package (Maya 201400)
find_package (MayaPluginHelpers)

set (MY_SOURCE_FILES
  "src/main.cpp"
  "src/mstreams.cpp"
  )

add_library (${PROJECT_NAME} SHARED ${MY_SOURCE_FILES})
target_link_libraries (${PROJECT_NAME} ${MAYA_OpenMaya_LIBRARY})

include_directories (${MAYA_INCLUDE_DIR})
mph_maya_plugin (${PROJECT_NAME})

mph_destination_directory (MY_DESTINATION_DIRECTORY ${MAYA_API_VERSION})

# TODO: Set PYTHONPATH to include scripts/yama!
add_custom_target (
  deploy
  python2 -m "yama.reload" ${PROJECT_NAME}
  "${CMAKE_SOURCE_DIR}/scripts/install.sh \
     ${MY_DESTINATION_DIRECTORY}          \
     $<TARGET_FILE:${PROJECT_NAME}>"
  DEPENDS ${PROJECT_NAME}
  )
