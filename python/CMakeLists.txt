# Python bindings using nanobind
if (NOT SKBUILD)
  message(WARNING "\
  This CMake file is meant to be executed using 'scikit-build-core'.
  Running it directly will almost certainly not produce the desired
  result. If you are a user trying to install this package, use the
  command below, which will install all necessary build dependencies,
  compile the package in an isolated environment, and then install it.
  =====================================================================
   $ pip install .
  =====================================================================
  If you are a software developer, and this is your own package, then
  it is usually much more efficient to install the build dependencies
  in your environment once and use the following command that avoids
  a costly creation of a new virtual environment at every compilation:
  =====================================================================
   $ pip install nanobind scikit-build-core[pyproject]
   $ pip install --no-build-isolation -ve .
  =====================================================================
  You may optionally add -Ceditable.rebuild=true to auto-rebuild when
  the package is imported. Otherwise, you need to rerun the above
  after editing C++ files.")
endif()
find_package(Python 3.8
  REQUIRED COMPONENTS Interpreter Development.Module
  OPTIONAL_COMPONENTS Development.SABIModule)
find_package(nanobind CONFIG REQUIRED)

if(APPLE)
    set(BASEPOINT @loader_path)
else()
    set(BASEPOINT $ORIGIN)
endif()
set(CMAKE_INSTALL_RPATH ${BASEPOINT} ${BASEPOINT}/${CMAKE_INSTALL_LIBDIR})

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Create the Python module
nanobind_add_module(
    _pykm2arec
    STABLE_ABI
    bindings.cc
)

# Generate stub files for type hints
nanobind_add_stub(
    _pykm2arec_stub
    MODULE _pykm2arec
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/_pykm2arec.pyi
    PYTHON_PATH $<TARGET_FILE_DIR:_pykm2arec>
    DEPENDS _pykm2arec
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/_pykm2arec.pyi DESTINATION pykm2arec)
# Link against the main library and ROOT
target_link_libraries(_pykm2arec PRIVATE 
    LHAASOFitLib
    ${ROOT_LIBRARIES}
)
# Install the module to the python directory
install(TARGETS _pykm2arec LIBRARY DESTINATION pykm2arec)
install(TARGETS LHAASOFitLib LIBRARY DESTINATION pykm2arec)

install(CODE "
    # 1. 在安装时（此时编译已完成），去构建目录搜索 .pcm 文件
    file(GLOB FOUND_PCM_FILES \"${CMAKE_BINARY_DIR}/*.pcm\")

    # 2. 检查是否找到了文件（可选，为了调试方便）
    if(NOT FOUND_PCM_FILES)
        message(WARNING \"No .pcm files found in ${CMAKE_BINARY_DIR} to install!\")
    else()
        message(STATUS \"Installing PCM files: \${FOUND_PCM_FILES}\")
        
        # 3. 使用 file(INSTALL ...) 进行实际的安装复制
        # 注意：这是 CMake 内部底层的安装命令，专门用在安装脚本里
        file(INSTALL DESTINATION \"\${CMAKE_INSTALL_PREFIX}/pykm2arec\"
             TYPE FILE 
             FILES \${FOUND_PCM_FILES})
    endif()
")

