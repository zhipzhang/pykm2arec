cmake_minimum_required(VERSION 3.23)
project(KM2AMCrec VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ROOT REQUIRED COMPONENTS Core Tree RIO Matrix Minuit)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCES
    src/EOSopen.cc
    src/G4KM2A_Geometry.cc
    src/G4KM2A_Reconstruction.cc
    src/KM2AEvent.cc
    src/LHCalibration.cc
    src/LHEvent.cc
    src/LHRecEvent.cc
)


# Dictionary generation
# Define the dictionary sources and headers
set(DICTIONARY_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/KM2AEvent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/LHEvent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/LHRecEvent.h
)
set(DICTIONARY_LINKDEF_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/KM2AEventLinkDef.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/LHEventLinkDef.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/LHRecEventLinkDef.h
)
set(DICTIONARY_OUTPUTS
    KM2AEventDict
    LHEventDict
    LHRecEventDict
)

# Generate dictionaries using ROOT_GENERATE_DICTIONARY
# This replaces the manual .pcm file linking
list(LENGTH DICTIONARY_HEADERS N_DICT)
math(EXPR N_DICT "${N_DICT} - 1")

foreach(I RANGE ${N_DICT})
    list(GET DICTIONARY_HEADERS ${I} DICT_HEADER)
    list(GET DICTIONARY_LINKDEF_HEADERS ${I} LINKDEF_HEADER)
    list(GET DICTIONARY_OUTPUTS ${I} DICT_BASE_NAME)
    
    # Generate the dictionary
    ROOT_GENERATE_DICTIONARY(${DICT_BASE_NAME}Dict ${DICT_HEADER}
        LINKDEF ${LINKDEF_HEADER}
        MODULE ${DICT_BASE_NAME}
    )
    
    # Add the generated .cxx file to the sources
    list(APPEND SOURCES ${CMAKE_CURRENT_BINARY_DIR}/${DICT_BASE_NAME}Dict.cxx)
endforeach()

# Create a static library from the sources
add_library(LHAASOFitLib SHARED ${SOURCES})

# Link ROOT libraries and dictionary libraries to the static library
target_link_libraries(LHAASOFitLib PUBLIC ${ROOT_LIBRARIES})
target_include_directories(LHAASOFitLib PUBLIC ${ROOT_INCLUDE_DIRS})

# Create the main executable
add_executable(LHAASOFit src/LHAASOFit.cc)

# Link the static library and ROOT libraries to the executable
target_link_libraries(LHAASOFit LHAASOFitLib ${ROOT_LIBRARIES})

add_subdirectory(test)
add_subdirectory(python)