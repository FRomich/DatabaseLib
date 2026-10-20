
cmake_minimum_required(VERSION 3.21)

project(DatabaseLib VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32 AND DEFINED ENV{QTDIR})
    list(PREPEND CMAKE_PREFIX_PATH "$ENV{QTDIR}")
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Sql)

qt_standard_project_setup()

qt_add_library(DatabaseLib SHARED
    include/DatabaseProvider.h
    include/DatabaseSettings.h
    include/IDatabase.h
    include/DatabaseLibExport.h
    include/DatabaseFactory.h

    src/QtDatabase.h
    src/QtDatabase.cpp    
    src/DatabaseFactory.cpp
)

set_target_properties(DatabaseLib PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

target_compile_definitions(DatabaseLib
    PRIVATE DATABASELIB_LIBRARY
)

target_include_directories(DatabaseLib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(DatabaseLib
    PUBLIC
        Qt6::Core
        Qt6::Sql
)


if(WIN32)
    set(POSTGRESQL_BIN
        "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/postgresql"
    )

    set(POSTGRESQL_DLLS
        libpq.dll
        libssl-1_1-x64.dll
        libcrypto-1_1-x64.dll
        libintl-8.dll
        libiconv-2.dll
    )

     set(POSTGRESQL_RUNTIME_DIR "${CMAKE_BINARY_DIR}")

    foreach(DLL IN LISTS POSTGRESQL_DLLS)
        if(EXISTS "${POSTGRESQL_BIN}/${DLL}")
            add_custom_command(TARGET DatabaseLib POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${POSTGRESQL_BIN}/${DLL}"
                    "${POSTGRESQL_RUNTIME_DIR}"
                COMMENT "Copying PostgreSQL dependency: ${DLL}"
                VERBATIM
            )
        else()
            message(FATAL_ERROR
                "PostgreSQL dependency not found: ${POSTGRESQL_BIN}/${DLL}"
            )
        endif()
    endforeach()
endif()

if(WIN32)
    set(MYSQL_BIN
        "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/mysql"
    )

    set(MYSQL_DLLS
        libssl-3-x64.dll
        libmysql.dll
        libcrypto-3-x64.dll
        mysql_native_password.dll
    )

    set(MYSQL_RUNTIME_DIR "${CMAKE_BINARY_DIR}")

    foreach(DLL IN LISTS MYSQL_DLLS)
        if(EXISTS "${MYSQL_BIN}/${DLL}")
            add_custom_command(TARGET DatabaseLib POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${MYSQL_BIN}/${DLL}"
                    "${MYSQL_RUNTIME_DIR}"
                COMMENT "Copying MySQL dependency: ${DLL}"
                VERBATIM
            )
        else()
            message(FATAL_ERROR
                "MySQL dependency not found: ${MYSQL_BIN}/${DLL}"
            )
        endif()
    endforeach()
endif()



