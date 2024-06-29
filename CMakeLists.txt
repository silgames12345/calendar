cmake_minimum_required(VERSION 3.15)

project(calendar)

find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-4.0)

add_subdirectory(src)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

    # Install desktop entry
    install(FILES data/openCalendar.desktop DESTINATION /usr/share/applications )

endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")