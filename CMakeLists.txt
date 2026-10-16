
cmake_minimum_required(VERSION 3.21)

project(DatabaseLib VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_PREFIX_PATH "$ENV{QTDIR}")

find_package(Qt6 REQUIRED COMPONENTS Core Sql)

qt_standard_project_setup()

qt_add_library(DatabaseLib SHARED
    include/DatabaseProvider.h
    include/DatabaseSettings.h
    include/IDatabase.h
    include/DatabaseLibExport.h

    src/QtDatabase.h
    src/QtDatabase.cpp
    src/DatabaseFactory.h
    src/DatabaseFactory.cpp
    src/PostgreSqlRuntime.h
    src/PostgreSqlRuntime.cpp
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

    set(POSTGRESQL_RUNTIME_DIR
        "$<TARGET_FILE_DIR:DatabaseLib>/thirdparty/postgresql"
    )

    add_custom_command(TARGET DatabaseLib POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "${POSTGRESQL_RUNTIME_DIR}"
        VERBATIM
    )

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


