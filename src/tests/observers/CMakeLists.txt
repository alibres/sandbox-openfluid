CMAKE_MINIMUM_REQUIRED (VERSION 2.6)


ADD_DEFINITIONS(-DOPENFLUID_VERSION=${FULL_VERSION})
ADD_DEFINITIONS(-DBUILDINGDLL)

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src")

ADD_OBSERVER(tests.empty tests.empty ${TEST_OUTPUT_PATH})

ADD_OBSERVER(tests.hopla tests.hopla ${TEST_OUTPUT_PATH})

ADD_TEST(functions-HoplaObserver "${BIN_OUTPUT_PATH}/openfluid-engine" 
                                 "-i${TESTS_DATASETS_PATH}/OPENFLUID.IN.PrimitivesHopla"
                                 "-o${TESTS_OUTPUTDATA_PATH}/OPENFLUID.OUT.PrimitivesHopla" 
                                 "-p${TEST_OUTPUT_PATH}"
                                 "-n${TEST_OUTPUT_PATH}")                            