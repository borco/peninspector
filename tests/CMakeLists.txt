qt_add_executable(test_pressurehistogrammodel
    "${CMAKE_SOURCE_DIR}/src/app/penconfig.cpp"
    "${CMAKE_SOURCE_DIR}/src/app/penconfig.h"
    "${CMAKE_SOURCE_DIR}/src/app/pressurehistogrammodel.cpp"
    "${CMAKE_SOURCE_DIR}/src/app/pressurehistogrammodel.h"
    test_pressurehistogrammodel.cpp
)

set_target_properties(test_pressurehistogrammodel PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(test_pressurehistogrammodel PUBLIC
    Qt::Test
    Qt::Core
)

target_include_directories(test_pressurehistogrammodel PRIVATE
    "${CMAKE_SOURCE_DIR}/src/app"
)
