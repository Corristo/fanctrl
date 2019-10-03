cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD_REQUIRED 11)

project("Raspberry Pi fan control"
	VERSION 1.0
	LANGUAGES CXX
)

option(INSTALL_SYSTEMD_SERVICE "Whether to install a systemd service to start fanctrl" ON)

add_executable(fanctrl fanctrl.cpp)

find_path(
  WIRING_PI_INCLUDE_PATH
  "wiringPi.h"
)

if(NOT WIRING_PI_INCLUDE_PATH)
  message(ERROR "Couldn't find the WiringPi headers. See the README on how to install WiringPi.")
endif()

target_include_directories(fanctrl
  PRIVATE
    ${WIRING_PI_INCLUDE_PATH}
)

find_library(WIRING_PI_LIBRARY
  "wiringPi"
)

if(NOT WIRING_PI_LIBRARY)
  message(ERROR "Couln't find the WiringPi library. See the README on how to install WinringPi.")
endif()

target_link_libraries(fanctrl
  PRIVATE
    ${WIRING_PI_LIBRARY}
)

find_package(Threads REQUIRED)
target_link_libraries(fanctrl
  PRIVATE
    Threads::Threads
)

include(GNUInstallDirs)
install(TARGETS fanctrl
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
          PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
	              GROUP_READ GROUP_EXECUTE
		      WORLD_READ WORLD_EXECUTE
)

if(INSTALL_SYSTEMD_SERVICE)
  set(FANCTRL_EXECUTABLE "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/fanctrl")
  configure_file(fanctrl.service.in ${CMAKE_BINARY_DIR}/fanctrl.service)
  install(FILES ${CMAKE_BINARY_DIR}/fanctrl.service
	  DESTINATION /lib/systemd/system
	  PERMISSIONS OWNER_READ OWNER_WRITE
	              GROUP_READ
		      WORLD_READ
  )
endif()
