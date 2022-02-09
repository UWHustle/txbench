include(FetchContent)

FetchContent_Declare(
        sqlite3
        URL https://www.sqlite.org/2022/sqlite-amalgamation-3370200.zip
        URL_HASH SHA3_256=7f535314ac30f1c7847df2a66a9e16a322f55dae6e83b264178cf02114cd0617
)
FetchContent_MakeAvailable(sqlite3)

add_library(sqlite3 ${sqlite3_SOURCE_DIR}/sqlite3.c)

add_executable(tatp_sqlite3 tatp_sqlite3.cpp)
target_link_libraries(tatp_sqlite3 txbench_tatp sqlite3)
