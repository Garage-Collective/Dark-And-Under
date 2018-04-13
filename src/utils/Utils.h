#pragma once

constexpr const __FlashStringHelper * FlashString(const char * string) {

	return reinterpret_cast<const __FlashStringHelper *>(string);

}

template<typename T> T absT(const T & v) {

	return (v < 0) ? -v : v;

}
