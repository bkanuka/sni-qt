project(systray)

# Source
set(systray_SRCS
    main.cpp
    window.cpp
    )

qt4_automoc(${systray_SRCS})

qt4_add_resources(systray_RC systray.qrc)

add_executable(systray ${systray_SRCS} ${systray_RC})

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTGUI_INCLUDE_DIR}
    )

target_link_libraries(systray
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    )
