link_directories(
    ${GTKMM_LIBRARY_DIRS}  )

include_directories(
    ${GTKMM_INCLUDE_DIRS}  )

add_executable(calendar "main.cpp" "main.h" "gui.cpp" "gui.h" "activity.cpp" "activity.h")
target_link_libraries(calendar ${GTKMM_LIBRARIES})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

    # Install desktop entry
    install(TARGETS calendar DESTINATION bin/OpenCal)

endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")