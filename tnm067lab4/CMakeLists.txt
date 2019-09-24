#--------------------------------------------------------------------
# Inviwo TNM067Lab4 Module
ivw_module(TNM067Lab4)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    #${CMAKE_CURRENT_SOURCE_DIR}/tnm067lab4processor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/jacobian.h
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/glyphrenderer.h
)
ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    #${CMAKE_CURRENT_SOURCE_DIR}/tnm067lab4processor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/jacobian.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/processors/glyphrenderer.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})


#--------------------------------------------------------------------
# Add shaders
set(SHADER_FILES
    #${CMAKE_CURRENT_SOURCE_DIR}/glsl/tnm067lab4processor.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/tensor_glyphrenderer.frag
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/tensor_glyphrenderer.geom
    ${CMAKE_CURRENT_SOURCE_DIR}/glsl/tensor_glyphrenderer.vert
)
ivw_group("Shader Files" ${SHADER_FILES})


#--------------------------------------------------------------------
# Add Unittests
set(TEST_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unittests/jacobian-test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unittests/tnm067lab4-unittest-main.cpp
)
ivw_add_unittest(${TEST_FILES})

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES})

#--------------------------------------------------------------------
# Add shader directory to pack
# ivw_add_to_module_pack(${CMAKE_CURRENT_SOURCE_DIR}/glsl)

ivw_folder(inviwo-module-tnm067lab4 TNM067)
