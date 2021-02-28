cmake_minimum_required(VERSION 2.8.12)

PROJECT(Week7)
SET(CMAKE_CXX_STANDARD 11)

#SET(OpenCV_DIR /home/hp/workfolder/OpenCV-Installation/installation/OpenCV-master/lib/cmake/opencv4)

######################## EDIT IF REQUIRED ####################
# ###Uncomment the line below and specify the path to OpenCV directory i.e. the path to the OpenCVConfig.cmake file. Check the examples given below.
#SET(OpenCV_DIR Enter-the-path-of-OpenCV-installation-on-your-system)


################### OpenCV_DIR Examples  #####################

### MACOS : /usr/local/Cellar/opencv/3.3.1_1/share/OpenCV/

### UBUNTU : /usr/local/share/OpenCV/

### WINDOWS : C:\Users\yourname\Documents\opencv-3.3.1\build\install

##############################################################




###################   ***DO NOT EDIT***  #####################

#############  Common Instructions for all Users  ############
if(MSVC)
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif()

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS})

MACRO(add_example name)
  ADD_EXECUTABLE(${name} ${name}.cpp)
  TARGET_LINK_LIBRARIES(${name} ${OpenCV_LIBS} )
ENDMACRO()

add_example(Application_Find_Object_using_Image_Features)
add_example(Application_Image_Alignment_Document_Scanner)
add_example(Application_Panorama)
add_example(Feature_Matching)
add_example(Geometric_Transform)
add_example(Image_Features_ORB)
add_example(Application_Perspective_Correction)
add_example(Application_Virtual_Billboard)
add_example(Homography_Example)
