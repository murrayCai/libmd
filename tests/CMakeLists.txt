add_executable(test_md test.c)
target_link_libraries(test_md mc md)
add_executable(test_template test_template.c)
target_link_libraries(test_template mc md)
add_executable(test_html test_html.c)
target_link_libraries(test_html mc md)
