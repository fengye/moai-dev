cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-fmod-designer )


  set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-fmod-designer/" )
 

  set ( MOAI_FMOD_DESIGNER_INCLUDES 
    "${MOAI_ROOT}/src/"
    "${CMAKE_BINARY_DIR}/libmoai/config"
    )

  # Use all cpp files in directory as source code
  file ( GLOB MOAI_FMOD_DESIGNER_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.h" )

  include_directories ( ${MOAI_FMOD_DESIGNER_INCLUDES} )

  add_library ( moai-fmod-designer STATIC ${MOAI_FMOD_DESIGNER_SRC} )

  # get_target_property (BOX2D_INCLUDES box2d INCLUDE_DIRECTORIES)

  set(BUILD_INCLUDES ${MOAI_FMOD_DESIGNER_INCLUDES})
 

  target_include_directories (moai-fmod-designer PUBLIC 
        "$<BUILD_INTERFACE:${BUILD_INCLUDES}>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
       
  )

  target_link_libraries ( moai-fmod-designer moai-sim moai-core moai-util zlcore fmodex_vc.lib fmod_event.lib fmod_event_net.lib)
  set_target_properties(moai-fmod-designer PROPERTIES FOLDER Modules)  

  install(TARGETS moai-fmod-designer EXPORT libmoai ARCHIVE DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include/moai-fmod-designer
          FILES_MATCHING PATTERN "*.h")
  
