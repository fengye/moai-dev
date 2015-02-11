cmake_minimum_required ( VERSION 2.8.5 )
project ( moai-fmod-ex )


  set ( CMAKE_CURRENT_SOURCE_DIR "${MOAI_ROOT}/src/moai-fmod-ex/" )
 

  set ( MOAI_FMOD_EX_INCLUDES 
    "${MOAI_ROOT}/src/"
    "${CMAKE_BINARY_DIR}/libmoai/config"
    )

  # Use all cpp files in directory as source code
  file ( GLOB MOAI_FMOD_EX_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h" )

  include_directories ( ${MOAI_FMOD_EX_INCLUDES} )

  add_library ( moai-fmod-ex STATIC ${MOAI_FMOD_EX_SRC} )

  # get_target_property (BOX2D_INCLUDES box2d INCLUDE_DIRECTORIES)

  set(BUILD_INCLUDES ${MOAI_FMOD_EX_INCLUDES})
 

  target_include_directories (moai-fmod-ex PUBLIC 
        "$<BUILD_INTERFACE:${BUILD_INCLUDES}>"
        "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>"
       
  )

  target_link_libraries ( moai-fmod-ex moai-sim moai-core moai-util zlcore fmodex_vc.lib )
  set_target_properties(moai-fmod-ex PROPERTIES FOLDER Modules)  

  install(TARGETS moai-fmod-ex EXPORT libmoai ARCHIVE DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include/moai-fmod-ex
          FILES_MATCHING PATTERN "*.h")
  
