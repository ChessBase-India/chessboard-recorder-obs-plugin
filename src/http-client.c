/*
CBI Chessboard Plugin for OBS
Copyright (C) <2025> <ChessBase India 64 Squares Private Limited> <chessbaseindiatech@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include "http-client.h"
#include <plugin-support.h>
#include <obs-module.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct http_response {
	char *memory;
	size_t size;
};

static size_t write_callback(void *contents, size_t size, size_t nmemb, struct http_response *response)
{
	size_t realsize = size * nmemb;
	char *ptr = realloc(response->memory, response->size + realsize + 1);
	if (!ptr) {
		blog(LOG_ERROR, "[%s] Not enough memory (realloc returned NULL)", PLUGIN_NAME);
		return 0;
	}

	response->memory = ptr;
	memcpy(&(response->memory[response->size]), contents, realsize);
	response->size += realsize;
	response->memory[response->size] = 0;

	return realsize;
}

void send_recording_request(const char *endpoint)
{
	CURL *curl;
	CURLcode res;
	struct http_response response = {0};

	curl = curl_easy_init();
	if (!curl) {
		blog(LOG_ERROR, "[%s] Failed to initialize CURL", PLUGIN_NAME);
		return;
	}

	char url[256];
	snprintf(url, sizeof(url), "http://localhost:%d%s", HTTP_PORT, endpoint);

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3L);

	blog(LOG_INFO, "[%s] Sending POST request to %s", PLUGIN_NAME, url);

	res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		blog(LOG_WARNING, "[%s] HTTP request failed: %s", PLUGIN_NAME, curl_easy_strerror(res));
	} else {
		long response_code;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
		blog(LOG_INFO, "[%s] HTTP request successful. Response code: %ld", PLUGIN_NAME, response_code);
	}

	curl_easy_cleanup(curl);
	if (response.memory) {
		free(response.memory);
	}
}
