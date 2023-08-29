include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(
	ftxui
	GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
	GIT_TAG cdf28903a7781f97ba94d30b79c3a4b0c97ccce7
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
	FetchContent_Populate(ftxui)
	add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_Declare(
		curlpp
		GIT_REPOSITORY https://github.com/jpbarrette/curlpp.git
		GIT_TAG f738b6be0c6b31f3e21852e5f40189231533d276
)
FetchContent_Declare(
		json
		GIT_REPOSITORY https://github.com/nlohmann/json.git
		GIT_TAG bc889afb4c5bf1c0d8ee29ef35eaaf4c8bef8a5d
)

FetchContent_MakeAvailable(curlpp json)

add_executable(mensa main.cpp Date.cpp DaysView.cpp MealView.cpp MealsView.cpp)
target_link_libraries(mensa ftxui::screen ftxui::dom ftxui::component curlpp nlohmann_json::nlohmann_json)
target_include_directories(mensa PUBLIC ${curlpp_SOURCE_DIR}/include ${ftxui_SOURCE_DIR}/include ${json_SOURCE_DIR}/include)

install(TARGETS mensa DESTINATION bin)
