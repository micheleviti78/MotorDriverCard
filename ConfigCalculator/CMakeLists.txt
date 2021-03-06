project(ConfigCalculator)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src calculator_library_sources)

#not a shared library, just link it to all the executables statically
add_library(${PROJECT_NAME} ${calculator_library_sources} )
target_link_libraries(${PROJECT_NAME} mtca4u-MotorDriverCard)    

add_executable(MotorConfigCalculator executables/ConfigCalculatorCLI.cpp)
target_link_libraries(MotorConfigCalculator ${PROJECT_NAME})    
install(TARGETS MotorConfigCalculator
        RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
)

add_executable(write_default_config executables/write_default_config.cpp)
target_link_libraries(write_default_config ${PROJECT_NAME})
add_custom_target(MotorDriverCardDefaultConfig.xml ALL write_default_config
  DEPENDS write_default_config)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/MotorDriverCardDefaultConfig.xml DESTINATION share/doc/mtca4u-MotorDriverCard-${mtca4u-MotorDriverCard_SOVERSION})

if(TESTING_IS_ENABLED)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tests/include)
  aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/tests/src calculatorTestSources)
  foreach( testSourceFile ${calculatorTestSources})
    #NAME_WE means the base name without path and (longest) extension
    get_filename_component(excutableName ${testSourceFile} NAME_WE)
    add_executable(${excutableName} ${testSourceFile})
    target_link_libraries(${excutableName} ${PROJECT_NAME} ${Boost_LIBRARIES})    
    add_test(${excutableName} ${excutableName})
  endforeach( testSourceFile )
  
  # add a test for the CLI executable
  FILE(COPY tests/generatedVT21Config.xml tests/testMotorConfigCalculator.sh
       DESTINATION ${PROJECT_BINARY_DIR})
  add_test(testMotorConfigCalculator
    ${PROJECT_BINARY_DIR}/testMotorConfigCalculator.sh)

endif(TESTING_IS_ENABLED)

FIND_PACKAGE(Qt4)

if(QT4_FOUND)
  add_subdirectory(GUI)
endif(QT4_FOUND)
