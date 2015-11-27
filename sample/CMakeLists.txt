CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)

ADD_EXECUTABLE(SampleBarbette SampleBarbette.cpp ../src/NerfSpiderControl.cpp)
ADD_EXECUTABLE(SampleLaserControl SampleLaserControl.cpp ../src/NerfSpiderControl.cpp)
ADD_EXECUTABLE(SampleShootControl SampleShootControl.cpp ../src/NerfSpiderControl.cpp)
ADD_EXECUTABLE(SampleShooterControl SampleShooterControl.cpp ../src/NerfSpiderControl.cpp)
ADD_EXECUTABLE(SampleWalk SampleWalk.cpp ../src/NerfSpiderControl.cpp)

TARGET_LINK_LIBRARIES(SampleBarbette mraa pthread)
TARGET_LINK_LIBRARIES(SampleLaserControl mraa pthread)
TARGET_LINK_LIBRARIES(SampleShootControl mraa pthread)
TARGET_LINK_LIBRARIES(SampleShooterControl mraa pthread)
TARGET_LINK_LIBRARIES(SampleWalk mraa pthread)
