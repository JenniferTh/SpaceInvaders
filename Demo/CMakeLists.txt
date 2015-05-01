# OpenGL
find_package(OpenGL REQUIRED)
IF(NOT OPENGL_FOUND)
    MESSAGE(FATAL_ERROR
      "OpenGL not found."
    )
ENDIF()

# include OpenGL
include_directories(${OPENGL_INCLUDE_DIR})

# GLFW3
# GLFW3 ist ein Sondefall und kann nicht mit normalen find_package gefunden werden.
# Es wird hierfür PkgConfig benötigt.
find_package(PkgConfig REQUIRED)
IF(NOT PKGCONFIG_FOUND)
    MESSAGE(
      FATAL_ERROR
        "PkgConfig not found."
    )
ENDIF()

pkg_search_module(GLFW REQUIRED glfw3)
IF(NOT GLFW_FOUND)
    MESSAGE(
      FATAL_ERROR
        "GLFW3 not found"
      )
ENDIF()
# include GLFW3
include_directories(${GLFW_INCLUDE_DIRS})

set(LIBRARIES
    ${LIBRARIES}
    ${GLFW_STATIC_LIBRARIES}
    ${OPENGL_LIBRARIES}
    # add other libs here
    # ${OPENGL_LIBRARIES} sind nach dem find_package() definiert.
	  PARENT_SCOPE
)

set(SOURCES
    ${SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/vec3.cpp
    # add new source files here
	  PARENT_SCOPE
)