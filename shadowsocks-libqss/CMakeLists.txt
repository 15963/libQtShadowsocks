set(TASK "shadowsocks-libqss")
set(SLIB "shadowsocks-lib")

set(APP_SOURCE
   main.cpp
   ) 

set(LIB_SOURCE
   qsslib.cpp
   )    

set(SOURCE
    client.cpp
    utils.cpp
    )

add_executable(${TASK} ${SOURCE} ${APP_SOURCE})
add_library(${SLIB} STATIC ${SOURCE} ${LIB_SOURCE})

target_link_libraries(${TASK} Qt5::Core Qt5::Network QtShadowsocks)
target_include_directories(${TASK}
    PUBLIC ${PROJECT_SOURCE_DIR}/lib)

install(TARGETS ${TASK} RUNTIME DESTINATION bin)
