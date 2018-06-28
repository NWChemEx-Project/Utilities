macro(start_hunter)
    include("cmake/HunterGate.cmake")
    set(__HUNTER_VERSION "0.22.14")
    set(GH_URL "https://github.com/ruslo/hunter/archive/")
    set(GH_URL "${GH_URL}${__HUNTER_VERSION}.tar.gz")
    set(__HUNTER_SHA1 "f194eab02248f7d8792f7fc0158f6194d005bf86")
    HunterGate(
            URL  "${GH_URL}"
            SHA1 "${HUNTER_SHA1}"
    )
endmacro()

function(prefix_paths __prefix __list)
    foreach(__file ${${__list}})
        list(APPEND __temp_list ${__prefix}/${__file})
    endforeach()
    set(${__list} ${__temp_list} PARENT_SCOPE)
endfunction()

function(make_full_paths __list)
    set(__prefix "${CMAKE_CURRENT_LIST_DIR}")
    prefix_paths(${__prefix} ${__list})
    set(${__list} ${${__list}} PARENT_SCOPE)
endfunction()

function(add_catch_cxx_tests __name __srcs __targets)
    enable_testing()
    add_executable(${__name} "${__srcs}")
    target_link_libraries(${__name} PRIVATE Catch2::Catch ${__targets})
    add_test(NAME "${__name}" COMMAND ${__name})
endfunction()


#Writes the files needed for find_package(XXX CONFIG)
#Uses the variables:
#   - PROJECT_NAME
#   - PROJECT_VERSION
function(install_targets __targets __headers)
    # Introduce variables:
    # * CMAKE_INSTALL_LIBDIR
    # * CMAKE_INSTALL_BINDIR
    # * CMAKE_INSTALL_INCLUDEDIR
    include(GNUInstallDirs)

    # Layout. This works for all platforms:
    #   * <prefix>/lib*/cmake/<PROJECT-NAME>
    #   * <prefix>/lib*/
    #   * <prefix>/include/
    set(config_install_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

    set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

    # Configuration
    set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
    set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
    set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")
    set(namespace "${PROJECT_NAME}::")

    # Include module with fuction 'write_basic_package_version_file'
    include(CMakePackageConfigHelpers)

    # Configure '<PROJECT-NAME>ConfigVersion.cmake'
    # Use:
    #   * PROJECT_VERSION
    write_basic_package_version_file(
        "${version_config}" COMPATIBILITY SameMajorVersion
    )

    # Configure '<PROJECT-NAME>Config.cmake'
    # Use variables:
    #   * TARGETS_EXPORT_NAME
    #   * PROJECT_NAME
    configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/Config.cmake.in"
        "${project_config}"
        INSTALL_DESTINATION "${config_install_dir}"
    )

    # Install targets
    install(
            TARGETS ${__targets}
            EXPORT "${TARGETS_EXPORT_NAME}"
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    )

    install(
            FILES ${__headers}
            DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    )

    # Signal the need to install the config files
    install(
            FILES "${project_config}" "${version_config}"
            DESTINATION "${config_install_dir}"
    )
    install(
            EXPORT "${TARGETS_EXPORT_NAME}"
            NAMESPACE "${namespace}"
            DESTINATION "${config_install_dir}"
    )


endfunction()
