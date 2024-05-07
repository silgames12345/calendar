cmake_minimum_required(VERSION 3.15)

project(calendar)

find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-4.0)

add_subdirectory(src)