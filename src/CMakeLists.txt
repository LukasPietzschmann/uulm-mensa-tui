include(FetchContent)

find_package(Curses REQUIRED)
FetchContent_Declare(
		curlpp
		GIT_REPOSITORY https://github.com/jpbarrette/curlpp.git
		GIT_TAG f738b6be0c6b31f3e21852e5f40189231533d276
)
FetchContent_Declare(
		gzip
		GIT_REPOSITORY https://github.com/mapbox/gzip-hpp.git
		GIT_TAG 7546b35aba5917154a0e9ae43f804b57d22bb966
)
FetchContent_Declare(
	json
	GIT_REPOSITORY https://github.com/nlohmann/json.git
	GIT_TAG bc889afb4c5bf1c0d8ee29ef35eaaf4c8bef8a5d
)

FetchContent_MakeAvailable(curlpp gzip json)

add_executable(tui main.cpp)
target_link_libraries(tui ${CURSES_LIBRARIES} curlpp z nlohmann_json::nlohmann_json)
target_include_directories(tui PUBLIC ${CURSES_INCLUDE_DIR} ${curlpp_SOURCE_DIR}/include ${gzip_SOURCE_DIR}/include ${json_SOURCE_DIR}/include)
