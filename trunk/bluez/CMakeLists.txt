cmake_minimum_required(VERSION 2.8)

SET (SIMPLESCAN_SRCS ./simplescan.c)

add_executable(simplescan ${SIMPLESCAN_SRCS})
target_link_libraries(simplescan bluetooth dbus-1)

SET (RFCOMM_SERVER_SRCS ./rfcomm-server.c)
add_executable(rfcomm-server ${RFCOMM_SERVER_SRCS})
target_link_libraries(rfcomm-server bluetooth dbus-1)

SET (RFCOMM_CLIENT_SRCS ./rfcomm-client.c)
add_executable(rfcomm-client ${RFCOMM_CLIENT_SRCS})
target_link_libraries(rfcomm-client bluetooth dbus-1)
