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

#include <obs-module.h>
#include <obs-frontend-api.h>
#include <plugin-support.h>
#include "http-client.h"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

static void frontend_event_callback(enum obs_frontend_event event, void *data)
{
	UNUSED_PARAMETER(data);

	switch (event) {
	case OBS_FRONTEND_EVENT_RECORDING_STARTED:
		blog(LOG_INFO, "[%s] Recording started - sending POST to localhost:%d/recording/start", PLUGIN_NAME,
		     HTTP_PORT);
		send_recording_request("/recording/start");
		break;
	case OBS_FRONTEND_EVENT_RECORDING_STOPPED:
		blog(LOG_INFO, "[%s] Recording stopped - sending POST to localhost:%d/recording/stop", PLUGIN_NAME,
		     HTTP_PORT);
		send_recording_request("/recording/stop");
		break;
	default:
		break;
	}
}

bool obs_module_load(void)
{
	blog(LOG_INFO, "[%s] plugin loaded successfully (version %s)", PLUGIN_NAME, PLUGIN_VERSION);

	// Register the frontend event callback to listen for recording events
	obs_frontend_add_event_callback(frontend_event_callback, NULL);

	return true;
}

void obs_module_unload(void)
{
	// Remove the frontend event callback before unloading
	obs_frontend_remove_event_callback(frontend_event_callback, NULL);

	blog(LOG_INFO, "[%s] plugin unloaded", PLUGIN_NAME);
}
