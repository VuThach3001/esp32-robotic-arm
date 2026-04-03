idf_component_register(
    SRCS "main.c"
    ${CMAKE_SOURCE_DIR}/software-architecture/src/Mcal/I2C/I2c.c
    REQUIRES main
    INCLUDE_DIRS
        "." 
        "../software-architecture/inc"
        "../software-architecture/inc/Mcal/I2C"
        "../software-architecture/common"
        "../software-architecture/common/CUCEL/standard"
        "../software-architecture/common/CUCEL/compiler"
        "../software-architecture/common/CUCEL/platform"
        "../GConf"
)