
set(CBOXDIR Base)
set(FBOXDIR F_BaseLib)
set(CBNDRYDIR Boundary)
set(CPARTDIR Particle)
set(CAMRCOREDIR AmrCore)
set(CAMRDIR Amr)
set(CMGDIR LinearSolvers/C_CellMG)
set(CTMGDIR LinearSolvers/C_TensorMG)
set(CFMGDIR LinearSolvers/C_to_F_MG)
set(FMGDIR LinearSolvers/F_MG)
set(CAMRDATADIR Extern/amrdata)

# Pick one place to put modules so everyone can see them
set(CMAKE_Fortran_MODULE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/mod_files CACHE PATH "Folder for fortran module files")
install(DIRECTORY ${CMAKE_Fortran_MODULE_DIRECTORY}/ DESTINATION include)

set(CBOXLIB_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/${CBOXDIR} ${CMAKE_CURRENT_SOURCE_DIR}/${CBNDRYDIR} ${CMAKE_CURRENT_SOURCE_DIR}/${CPARTDIR} ${CMAKE_CURRENT_SOURCE_DIR}/${CAMRDIR} ${CMAKE_CURRENT_SOURCE_DIR}/${CAMRCOREDIR} ${CMAKE_CURRENT_SOURCE_DIR}/${CMGDIR} ${CMAKE_CURRENT_SOURCE_DIR}/${CTMGDIR} ${CMAKE_CURRENT_SOURCE_DIR}/${CFMGDIR} ${CMAKE_CURRENT_SOURCE_DIR}/${CAMRDATADIR} ${AMREX_EXTRA_CXX_INCLUDE_PATH})

include(PreprocessAMReXFortran)
include(PreprocessAMReXFortran90)

add_subdirectory(${CBOXDIR})
add_subdirectory(${CBNDRYDIR})
add_subdirectory(${CPARTDIR})
add_subdirectory(${CAMRDIR})
add_subdirectory(${CAMRCOREDIR})
add_subdirectory(${CMGDIR})
add_subdirectory(${CTMGDIR})

set(FBOXLIB_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/${FBOXDIR} ${CMAKE_CURRENT_SOURCE_DIR}/${FMGDIR} ${AMREX_EXTRA_Fortran_INCLUDE_PATH})

add_subdirectory(${FBOXDIR})
add_subdirectory(${FMGDIR})

add_subdirectory(${CFMGDIR})

add_library(cboxlib $<TARGET_OBJECTS:box_c> $<TARGET_OBJECTS:box_cbndry> $<TARGET_OBJECTS:box_cpart> $<TARGET_OBJECTS:box_camr> $<TARGET_OBJECTS:box_camrcore> $<TARGET_OBJECTS:box_cmg> $<TARGET_OBJECTS:box_ctmg> $<TARGET_OBJECTS:box_cfmg>)
add_library(cfboxlib $<TARGET_OBJECTS:box_cfmg>)
add_library(fboxlib $<TARGET_OBJECTS:box_f> $<TARGET_OBJECTS:box_fmg>)
add_install_library(cboxlib)
add_install_library(cfboxlib)
add_install_library(fboxlib)

# Move Tools dir to install location
install(CODE "MESSAGE(\"-- Copying   : ${CCSE_TOOLS_DIR} --> ${CCSE_INSTALL_DIR}\")" )
install(CODE "FILE( COPY ${CCSE_TOOLS_DIR} DESTINATION ${CCSE_INSTALL_DIR} )")
SET_TARGET_PROPERTIES(cboxlib PROPERTIES LINKER_LANGUAGE C)
SET_TARGET_PROPERTIES(cfboxlib PROPERTIES LINKER_LANGUAGE C)
SET_TARGET_PROPERTIES(fboxlib PROPERTIES LINKER_LANGUAGE C)

add_subdirectory(${CAMRDATADIR})


