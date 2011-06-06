# Source
set(sni_qt_SRCS
    statusnotifieritem.cpp
    statusnotifieritemfactory.cpp
    )

# Build
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${DBUSMENUQT_INCLUDE_DIRS}
    ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTDBUS_INCLUDE_DIR}
    ${QT_QTGUI_INCLUDE_DIR}
    )

qt4_automoc(${sni_qt_SRCS})

add_library(sni-qt MODULE
    ${sni_qt_SRCS}
    )

link_directories(
    ${DBUSMENUQT_LIBRARY_DIRS}
    )

target_link_libraries(sni-qt
    ${DBUSMENUQT_LIBRARIES}
    ${QT_QTCORE_LIBRARIES}
    ${QT_QTDBUS_LIBRARIES}
    ${QT_QTGUI_LIBRARIES}
    ${QT_QTXML_LIBRARIES}
    )

# Install
install(TARGETS sni-qt
    DESTINATION ${QT_PLUGINS_DIR}/systemtrayicon/
    )
