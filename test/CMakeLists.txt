include_directories(./include/ ${CMAKE_SOURCE_DIR}/include/chttp)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

add_executable(slist src/test_slist.c)
add_executable(cookie src/test_cookie.c)
add_executable(http src/test_chttp.c)
add_executable(string src/test_string.c)

target_link_libraries(slist chttp)
target_link_libraries(cookie chttp)


target_link_libraries(http chttp)
target_link_libraries(string chttp)


