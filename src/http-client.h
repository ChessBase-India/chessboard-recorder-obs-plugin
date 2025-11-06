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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <curl/curl.h>

/**
 * Sends a POST request to the specified endpoint on localhost:<httpPort>
 * @param endpoint The endpoint path (e.g., "/recording/start" or "/recording/stop")
 */
void send_recording_request(const char *endpoint);

#ifdef __cplusplus
}
#endif
