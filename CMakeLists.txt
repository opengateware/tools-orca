cmake_minimum_required(VERSION 3.0)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	message(FATAL_ERROR [[In-source builds are not allowed]])
endif ()

# Project Information
project(ORCA
		VERSION "0.1.0"
		DESCRIPTION "Open ROM Conversion Assistant"
		HOMEPAGE_URL "https://github.com/opengateware/tools-orca"
		LANGUAGES C)

set(BIN_TARGET "orca")

set(APP_AUTHOR "OpenGateware")
set(APP_TITLE "ORCA (Open ROM Conversion Assistant)")
set(APP_COPYRIGHT "(c) 2022, OpenGateware authors and contributors")
set(APP_DESCRIPTION ${PROJECT_DESCRIPTION})
set(APP_VERSION ${PROJECT_VERSION})
set(DOMAIN ${PROJECT_NAME})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for clang-tidy

set(CMAKE_C_STANDARD 99)
#set(CMAKE_VERBOSE_MAKEFILE ON)

configure_file(src/version.h.in version.h)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Ofast")

if (APPLE)
	message("Skipping Static Linking on macOS")
else ()
	set(CMAKE_EXE_LINKER_FLAGS "-static")
endif ()

find_package(Git)
add_compile_definitions(HAVE_ZLIB)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.c ${PROJECT_SOURCE_DIR}/src/*.h)

# These guys are third_party
set(DEPS_INC ${PROJECT_SOURCE_DIR}/deps
	${PROJECT_SOURCE_DIR}/deps/junzip
	${PROJECT_SOURCE_DIR}/deps/md5
	${PROJECT_SOURCE_DIR}/deps/sxmlc
	)

include_directories(${PROJECT_BINARY_DIR} ${APP_INC} ${DEPS_INC})

add_executable(${BIN_TARGET}
			   deps/junzip/junzip.h
			   deps/junzip/junzip.c
			   deps/md5/md5.h
			   deps/md5/md5.c
			   deps/sxmlc/sxmlc.h
			   deps/sxmlc/sxmlc.c
			   ${SOURCE_FILES})

if (WIN32 OR MSVC OR MSYS OR MINGW)
	target_link_libraries(${BIN_TARGET} ${ADDITIONAL_LIBRARIES} -Wl,-Bstatic -lz)
else ()
	target_link_libraries(${BIN_TARGET} ${ADDITIONAL_LIBRARIES} z)
endif ()

# Get the commit SHA1, and whether the building workspace was dirty or not
execute_process(COMMAND
				"${GIT_EXECUTABLE}" describe --match=NeVeRmAtCh --always --abbrev=7 --dirty
				WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
				OUTPUT_VARIABLE GIT_SHA1
				ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

# Get the date of the commit
execute_process(COMMAND
				"${GIT_EXECUTABLE}" log -1 --format=%ad --date=local
				WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
				OUTPUT_VARIABLE GIT_DATE
				ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

# Generate version.h
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in" "${CMAKE_CURRENT_BINARY_DIR}/version.h" @ONLY)

# Files to be installed
install(
	TARGETS ${BIN_TARGET}
	COMPONENT binaries
	RUNTIME DESTINATION "bin/"
)

# Create Package Files
set(CPACK_PACKAGE_VENDOR "OpenGateware")
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_STRIP_FILES YES)
SET(CPACK_OUTPUT_FILE_PREFIX releases)

if (WIN32 OR MSVC OR MSYS OR MINGW)
	set(CPACK_GENERATOR "ZIP")
elseif (APPLE)
	set(CPACK_GENERATOR "ZIP" "TGZ")
else ()
	set(CPACK_DEBIAN_PACKAGE_MAINTAINER "packages@raetro.org")
	set(CPACK_GENERATOR "ZIP" "TGZ" "DEB")
	set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS YES)
endif ()

include(CPack)
