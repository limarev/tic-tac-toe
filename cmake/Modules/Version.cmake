
find_package(Git REQUIRED)

macro(_git)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} ${ARGN}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            RESULT_VARIABLE _git_res
            OUTPUT_VARIABLE _git_out OUTPUT_STRIP_TRAILING_WHITESPACE ECHO_OUTPUT_VARIABLE
            ERROR_VARIABLE _git_err ERROR_STRIP_TRAILING_WHITESPACE ECHO_ERROR_VARIABLE
            )
endmacro()

if (DEFINED ENV{THIS_PROJECT_VERSION} AND DEFINED ENV{THIS_PROJECT_SHA})
    set(git_tag "${THIS_PROJECT_VERSION}")
    set(git_hash "${THIS_PROJECT_SHA}")
else ()
    _git(describe --tags --abbrev=0)
    set(git_tag "${_git_out}")

    _git(rev-parse --short HEAD)
    set(git_hash "${_git_out}")

endif ()

if (NOT git_tag)
    message(FATAL_ERROR "Failed to get git tag. Cannot setup project version.")
endif ()

if (NOT git_hash)
    message(FATAL_ERROR "Failed to get git hash. Cannot setup commit hash.")
endif ()

set(THIS_PROJECT_SHA ${git_hash})

if (git_tag MATCHES "^([0-9]*).([0-9]*).([0-9]*)$")
    set(THIS_PROJECT_VERSION ${CMAKE_MATCH_1}.${CMAKE_MATCH_2}.${CMAKE_MATCH_3})
    set(THIS_PROJECT_VERSION_MAJOR ${CMAKE_MATCH_1})
    set(THIS_PROJECT_VERSION_MINOR ${CMAKE_MATCH_2})
    set(THIS_PROJECT_VERSION_PATCH ${CMAKE_MATCH_3})
    message("Project version: ${THIS_PROJECT_VERSION}")
else ()
    message(WARNING "Git tag isn't valid semantic version: [${git_tag}]\n")
    set(THIS_PROJECT_VERSION ${git_tag})
endif ()