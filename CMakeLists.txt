cmake_minimum_required (VERSION 2.6)
project (qacdp)
find_package(Qt4 REQUIRED)
find_package(Automoc4 REQUIRED)
add_definitions(${QT_DEFINITIONS})
include_directories(${QT_INCLUDES} ${CMAKE_BINARY_DIR} )
set (qacdp_SOURCES
    acdp.cpp
    login.cpp
    main.cpp
    getcalendartips.cpp
	 calendarthread.cpp
    mainwindow.cpp
    )

set(SOURCES_H
    acdp.h
    login.h
	 getcalendartips.h
	 calendarthread.h
    mainwindow.h)
qt4_wrap_cpp(qacdp_MOCS ${SOURCES_H})

set(qacdp_UI login.ui mainwindow.ui)
qt4_wrap_ui(qacdp_UI_SRCS ${qacdp_UI})
add_executable(qacdp ${qacdp_SOURCES} ${qacdp_UI_SRCS} ${qacdp_MOCS})

target_link_libraries(qacdp ${QT_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTXML_LIBRARY} ${QT_QTWEBKIT_LIBRARY})

install(TARGETS qacdp DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
