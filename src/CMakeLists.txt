link_directories(
    ${GTKMM_LIBRARY_DIRS}  )

include_directories(
    ${GTKMM_INCLUDE_DIRS}  )

add_executable(calendar "main.cpp" "main.h" "gui.cpp" "gui.h")
target_link_libraries(calendar ${GTKMM_LIBRARIES})