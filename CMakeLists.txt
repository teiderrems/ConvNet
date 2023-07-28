cmake_minimum_required(VERSION 3.5)

project(ConvNet LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ConvNet main.cpp
    Conv3D.cpp
    Conv3D.h
    Pooling.cpp
    Pooling.h
    cnn.cpp
    cnn.h
    layer.cpp
    layer.h
    image.h
    util.h
    full-connected.cpp
    full-connected.h
    neurone.cpp
    neurone.h

)

install(TARGETS ConvNet
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
