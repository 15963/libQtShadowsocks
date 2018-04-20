set(TASK "shadowsocks-libqss")
set(SLIB "shadowsocks-lib")
set(SLIB_TEST "shadowsocks-lib-test")

find_package(Boost COMPONENTS system thread filesystem context chrono coroutine)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})


set(APP_SOURCE
   main.cpp
   ) 

set(LIB_HEARDERS
   qsslib.h
   error.h
   client.h
   )

set(LIB_SOURCE
   qsslib.cpp
   )    

set(SOURCE
    client.cpp
    utils.cpp
    )

set(LIB_TEST_SOURCE
    qsslibtest.cpp
    )

add_executable(${TASK} ${SOURCE} ${APP_SOURCE})
add_library(${SLIB} STATIC ${LIB_HEARDERS} ${SOURCE} ${LIB_SOURCE})
add_executable(${SLIB_TEST} ${LIB_TEST_SOURCE})

target_link_libraries(${TASK} Qt5::Core Qt5::Network QtShadowsocks)
target_link_libraries(${SLIB} Qt5::Core Qt5::Network QtShadowsocks ${Boost_LIBRARIES} )
target_link_libraries(${SLIB_TEST} shadowsocks-lib)

target_include_directories(${TASK}
    PUBLIC ${PROJECT_SOURCE_DIR}/lib)
target_include_directories(${SLIB}
    PUBLIC ${PROJECT_SOURCE_DIR}/lib)
target_include_directories(${SLIB_TEST}
    PUBLIC ${PROJECT_SOURCE_DIR}/lib)

install(TARGETS ${TASK} RUNTIME DESTINATION bin)
