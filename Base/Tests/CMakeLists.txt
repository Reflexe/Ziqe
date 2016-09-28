macro (zq_add_test name)
    add_executable ("zqtest_${name}" "${name}.${zq_cpp_ext}")
    add_test ("${name}_ZqTest" "./${name}")
    target_link_libraries ("zqtest_${name}" "zq_Base")
endmacro ()

zq_add_test (LinkedListTest)
