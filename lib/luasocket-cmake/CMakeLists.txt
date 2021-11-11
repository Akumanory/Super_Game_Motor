cmake_minimum_required (VERSION 3.8)

project ("luasocket-cmake")

set(CMAKE_CXX_STANDARD 23)

set(SOCKET_SOURCES
    ../luasocket/src/auxiliar.c
    ../luasocket/src/buffer.c
    ../luasocket/src/except.c
    ../luasocket/src/inet.c
    ../luasocket/src/io.c
    ../luasocket/src/luasocket.c
    ../luasocket/src/options.c
    ../luasocket/src/select.c
    ../luasocket/src/tcp.c
    ../luasocket/src/timeout.c
    ../luasocket/src/udp.c
    ../luasocket/src/wsocket.c
)

set(MIME_SOURCES
    ../luasocket/src/mime.c
)

add_library (socket SHARED ${SOCKET_SOURCES})
target_include_directories(socket PRIVATE ../luasocket/src)
target_compile_definitions(socket PRIVATE -DLUASOCKET_API=__declspec\(dllexport\) -D_WINDOWS -D_USRDLL -DUNICODE -D_UNICODE $<$<CONFIG:Debug>:-DLUASOCKET_DEBUG>)
target_link_libraries(socket liblua-static ws2_32)
set(lualist ftp.lua headers.lua http.lua smtp.lua tp.lua url.lua)
foreach( file_i ${lualist})
    add_custom_command(TARGET socket POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/lib/luasocket/src/${file_i} $<TARGET_FILE_DIR:socket>/socket/socket/${file_i})
endforeach( file_i )
set(lualist ltn12.lua mime.lua socket.lua)
foreach( file_i ${lualist})
    add_custom_command(TARGET socket POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/lib/luasocket/src/${file_i} $<TARGET_FILE_DIR:socket>/socket/${file_i})
endforeach( file_i )
add_custom_command(TARGET socket POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:socket> $<TARGET_FILE_DIR:socket>/socket/socket/core.dll)

add_library (mime SHARED ${MIME_SOURCES})
target_include_directories(mime PRIVATE ../luasocket/src)
target_compile_definitions(mime PRIVATE -DMIME_API=__declspec\(dllexport\) -D_WINDOWS -D_USRDLL -DUNICODE -D_UNICODE)
target_link_libraries(mime liblua-static)
add_custom_command(TARGET mime POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:mime> $<TARGET_FILE_DIR:socket>/socket/mime/core.dll)
