set(COMMON_SRC "webrtc.cpp" "main.cpp" "http.cpp")

if(IDF_TARGET STREQUAL linux)
	idf_component_register(
		SRCS ${COMMON_SRC}
		REQUIRES peer esp_http_client)
else()
	idf_component_register(
		SRCS ${COMMON_SRC} "wifi.cpp" "media.cpp" "OpenAI_logo.c"
		REQUIRES driver esp_wifi nvs_flash peer esp_psram esp_http_client)
endif()

idf_component_get_property(lib peer COMPONENT_LIB)
target_compile_options(${lib} PRIVATE -Wno-error=restrict)
target_compile_options(${lib} PRIVATE -Wno-error=stringop-truncation)

idf_component_get_property(lib sepfy__srtp COMPONENT_LIB)
target_compile_options(${lib} PRIVATE -Wno-error=incompatible-pointer-types)
