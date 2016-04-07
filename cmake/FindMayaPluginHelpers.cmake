set (MPH_COMPILE_DEFINITIONS "REQUIRE_IOSTREAM;_BOOL")

if (UNIX)
  # GNU/Linux
  set (MPH_COMPILE_DEFINITIONS "${MAYA_COMPILE_DEFINITIONS};LINUX")
  set (MPH_PLUGIN_EXTENSION ".so" CACHE STRING "Maya plugin extension")

elseif (APPLE)
  # Mac OS
  set (MPH_COMPILE_DEFINITIONS "${MAYA_COMPILE_DEFINITIONS};OSMac_")
  set (MPH_PLUGIN_EXTENSION ".bundle" CACHE STRING "Maya plugin extension")

elseif (WIN32)
  # Windows
  set (MAYA_COMPILE_DEFINITIONS "${MAYA_COMPILE_DEFINITIONS};NT_PLUGIN")
  set (MPH_PLUGIN_EXTENSION ".mll" CACHE STRING "Maya plugin extension")

else ()
  message (FATAL_ERROR "Unrecognized operating system")

endif ()


function (mph_maya_plugin _target)
  # if (WIN32)
  #   set_target_properties (${_target} PROPERTIES
  #     LINK_FLAGS "/export:initializePlugin /export:uninitializePlugin"
  #     )
  # endif ()
  set_target_properties(${_target} PROPERTIES
    COMPILE_DEFINITIONS "${MPH_COMPILE_DEFINITIONS}"
    PREFIX ""
    SUFFIX ${MPH_PLUGIN_EXTENSION})
endfunction (mph_maya_plugin)


function (mph_display_variables _display_values)
  get_cmake_property (_vars VARIABLES)
  foreach (_v ${_vars})
    if (${_display_values})
      get_property (_value VARIABLE PROPERTY ${_v})
      message (STATUS "VARIABLE: ${_v}: ${_value}")
    else ()
      message (STATUS "VARIABLE: ${_v}")
    endif ()
  endforeach ()
endfunction(mph_display_variables)


# Given Maya's version, return the destination directory plugins
# should be installed in.
function (mph_destination_directory _var _version)
  string (SUBSTRING ${_version} 0 4 _truncated)
  set (${_var} "~/maya/${_truncated}/plug-ins/" PARENT_SCOPE)
endfunction (mph_destination_directory)


include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (MayaPluginHelpers DEFAULT_MSG MPH_PLUGIN_EXTENSION)
