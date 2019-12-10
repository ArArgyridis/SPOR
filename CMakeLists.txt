cmake_minimum_required(VERSION 2.8)

project(SPOR)

add_definitions(-Wall -g -std=c++11 -DPQXX_HIDE_EXP_OPTIONAL)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3  -lxml2 -lpqxx -fopenmp")

set (PQXX_DIR .)
set (POSTGRES_DIR .)
set(DLIB_DIR .)

find_package(LibXml2 REQUIRED)
find_package(PQXX REQUIRED)
find_package(dlib REQUIRED)

if (LIBXML2_FOUND)
    include_directories(${LIBXML2_INCLUDE_DIR})
endif()

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

set(SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/andnode/andnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/classrelatedfeaturenode/classrelatedfeaturenode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/complementnode/complementnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/crispdatatype/crispdatatype.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/crispfeaturenode/crispfeaturenode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/datatype/datatype.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/featurenode/featurenode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fuzzydatatype/fuzzydatatype.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fuzzyevaluator/fuzzyevaluator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fuzzyfunctions/fuzzyfunctions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/fuzzyobjectproperty/fuzzyobjectproperty.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/helperfunctions/helperfunctions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/logicnode/logicnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/machinelearning/machinelearningdatatype.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/machinelearning/CDBN/HiddenLayer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/machinelearning/CDBN/CDBN.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/machinelearning/CDBN/CRBM.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/machinelearning/CDBN/functions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/machinelearning/CDBN/DBN.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/machinelearning/CDBN/LogisticRegression.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/machinelearning/CDBN/RBM.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/machinelearning/SVM/svm.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/machinelearningnode/machinelearningnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ontologyclass/ontologyclass.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ontologycontainer/ontologycontainer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ontologydata/ontologydata.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ontologyparser/ontologyparser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ornode/ornode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/superclassnode/superclassnode.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/treenode/treenode.cpp
)

set (HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/andnode/andnode.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/classrelatedfeaturenode/classrelatedfeaturenode.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/complementnode/complementnode.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/crispdatatype/crispdatatype.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/crispfeaturenode/crispfeaturenode.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/datatype/datatype.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/featurenode/featurenode.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/fuzzydatatype/fuzzydatatype.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/fuzzyevaluator/fuzzyevaluator.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/fuzzyfunctions/fuzzyfunctions.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/fuzzyobjectproperty/fuzzyobjectproperty.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/helperfunctions/helperfunctions.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/logicnode/logicnode.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/machinelearning/machinelearningdatatype.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/machinelearning/CDBN/HiddenLayer.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/machinelearning/CDBN/CDBN.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/machinelearning/CDBN/CRBM.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/machinelearning/CDBN/functions.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/machinelearning/CDBN/DBN.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/machinelearning/CDBN/LogisticRegression.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/machinelearning/CDBN/RBM.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/machinelearning/SVM/svm.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/machinelearningnode/machinelearningnode.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/ontologyclass/ontologyclass.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/ontologycontainer/ontologycontainer.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/ontologydata/ontologydata.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/ontologyparser/ontologyparser.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/ornode/ornode.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/superclassnode/superclassnode.hxx
    ${CMAKE_CURRENT_SOURCE_DIR}/treenode/treenode.hxx
)


add_executable(${PROJECT_NAME} ${SOURCE} ${HEADERS} "main.cpp")
target_link_libraries(${PROJECT_NAME}  ${LIBXML2_LIBRARIES} ${PQXX_LIBRARIES} ${Boost_LIBRARIES}  dlib::dlib)
