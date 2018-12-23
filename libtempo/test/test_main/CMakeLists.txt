file(GLOB SRCS *.cpp)

setTestName()

add_executable(${PROJECT_NAME} ${SRCS})
target_link_libraries(${PROJECT_NAME} ${GOOGLETEST_LIBRARIES})

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
