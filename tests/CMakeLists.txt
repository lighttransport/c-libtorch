set(TEST_TARGET_NAME test-c-libtorch)

add_executable(${TEST_TARGET_NAME} test-c-libtorch.cc)
add_sanitizers(${TEST_TARGET_NAME})

if (WIN32)
  add_test(NAME ${TEST_TARGET_NAME} COMMAND "${TEST_TARGET_NAME}.exe"
           WORKING_DIRECTORY $<TARGET_FILE_DIR:${TEST_TARGET_NAME}>  )
else (WIN32)
  add_test(NAME ${TEST_TARGET_NAME} COMMAND "${TEST_TARGET_NAME}"
           WORKING_DIRECTORY $<TARGET_FILE_DIR:${TEST_TARGET_NAME}>  )
endif (WIN32)

target_include_directories(${TEST_TARGET_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(${TEST_TARGET_NAME} PRIVATE ${C_LIBTORCH_BUILD_TARGET})
if (C_LIBTORCH_PYTHON_ENABLED)
  message(STATUS "PYU" ${PYTHON_LIBRARIES})
  target_link_directories(${TEST_TARGET_NAME} PRIVATE ${TORCH_INSTALL_PREFIX}/lib)
  target_link_libraries(${TEST_TARGET_NAME} PRIVATE Python::Python)
endif()
target_link_libraries(${TEST_TARGET_NAME} PUBLIC ${CMAKE_DL_LIBS})


set_target_properties(${TEST_TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

