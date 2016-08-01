

######################################################################################
### 1) Include the sources
######################################################################################

INCLUDE_DIRECTORIES(../src)
INCLUDE_DIRECTORIES(/home/jacquin/ParadisEO-2.0/eo/src)
INCLUDE_DIRECTORIES(/home/jacquin/ParadisEO-2.0/eo/src/do)

######################################################################################



######################################################################################
### 2) Specify where CMake can find the libraries
######################################################################################

IF(NOT WIN32 OR CYGWIN)  
    LINK_DIRECTORIES(/home/jacquin/ParadisEO-2.0/eo/lib)
ENDIF(NOT WIN32 OR CYGWIN)

# especially for Visual Studio
IF(WIN32 AND NOT CYGWIN)  
  	LINK_DIRECTORIES(/home/jacquin/ParadisEO-2.0/eo/\\lib\\${CMAKE_BUILD_TYPE})
ENDIF(WIN32 AND NOT CYGWIN) 

######################################################################################
ADD_DEFINITIONS(-O3 -std=c++0x)


######################################################################################
### 3) Define your targets
######################################################################################

ADD_EXECUTABLE(OneMachineEA2 OneMachineEA.cpp)
ADD_EXECUTABLE(Test Test.cpp)


######################################################################################


######################################################################################
### 4) Link the librairies for the targets
######################################################################################

TARGET_LINK_LIBRARIES(OneMachineEA2 eo eoutils ga es) 
TARGET_LINK_LIBRARIES(Test eo eoutils ga es) 

######################################################################################

