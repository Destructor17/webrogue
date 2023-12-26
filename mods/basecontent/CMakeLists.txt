set (CMAKE_CXX_STANDARD 17)

add_library(basecontent ${WEBROGUE_MOD_LIB_TYPE} basecontent.cpp sqlpp_schema.h)

target_link_libraries(basecontent PRIVATE sqlpp11_sqlite3)

include(../tools/sqlpp_scheme_generator.cmake)

make_sqlpp_schema(
    DDL schema.sql
    HEADER sqlpp_schema
    NAMESPACE webrogue::basecontent
)
register_mod(
    NAME basecontent
)