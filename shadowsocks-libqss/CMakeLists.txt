set(TASK "shadowsocks-libqss")
set(SLIB "shadowsocks-lib")

set(APPSOURCE
   main.cpp
   ) 

set(LIBSOURCE
   qsslib.cpp
   )    

set(SOURCE
    client.cpp
    utils.cpp
    )

add_executable(${TASK} ${SOURCE} ${APPSOURCE})
add_library(${SLIB} STATIC ${SOURCE} ${LIBSOURCE})

target_link_libraries(${TASK} Qt5::Core Qt5::Network QtShadowsocks)
target_include_directories(${TASK}
    PUBLIC ${PROJECT_SOURCE_DIR}/lib)

install(TARGETS ${TASK} RUNTIME DESTINATION bin)
