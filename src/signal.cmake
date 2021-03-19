add_library(CsSignal STATIC "")

target_compile_definitions(
   CsSignal
   PUBLIC
   -DCS_SIGNAL_EXPORTS=
   -DCS_SIGNAL_NS=cs_signal
)

target_compile_features(
   CsSignal
   PUBLIC
   cxx_std_17
)

set_target_properties(CsSignal PROPERTIES
  COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}"
  COMPILE_PDB_NAME CsSignal
)

target_sources(CsSignal
   PRIVATE
   ${CMAKE_CURRENT_SOURCE_DIR}/src/cs_signal.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/cs_slot.cpp
)

target_include_directories(
   CsSignal
   PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

set(CS_SIGNAL_INCLUDE
   ${CMAKE_CURRENT_SOURCE_DIR}/src/cs_internal.h
   ${CMAKE_CURRENT_SOURCE_DIR}/src/cs_macro.h
   ${CMAKE_CURRENT_SOURCE_DIR}/src/cs_signal.h
   ${CMAKE_CURRENT_SOURCE_DIR}/src/cs_slot.h

   ${CMAKE_CURRENT_SOURCE_DIR}/src/cs_rcu_guarded.h
   ${CMAKE_CURRENT_SOURCE_DIR}/src/cs_rcu_list.h
)

install(
   TARGETS CsSignal
   RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
   FILES ${CS_SIGNAL_INCLUDE}
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/CsSignal
   COMPONENT Devel
)

if (WIN32)
  install(
    FILES "$<TARGET_FILE_DIR:CsSignal>/CsSignal.pdb"
    DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()
