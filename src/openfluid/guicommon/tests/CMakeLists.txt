INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}/src/tests")


SET(UNITTEST_LINK_LIBRARIES openfluid-core openfluid-base openfluid-guicommon openfluid-fluidx)

DISCOVER_OPENFLUID_UNITTESTS(api)