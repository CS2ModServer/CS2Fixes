#pragma once

//#include "entities.h"
#include "entity/ccsplayercontroller.h"
//#include "cs2fixes.h"
#include <string>

namespace Source2Py {
	class testclass
	{
	public:
		CUtlVector<const char*> frompython;
		testclass()
		{
			frompython.Purge();
		}
		void append(const char* string)
		{
			frompython.AddToTail(string);
		};
		~testclass()
		{
			frompython.PurgeAndDeleteElements();
		}
	};
	class PyAPI {
	public:

		// Print message to client chat
		static void PrintToChat(int playerSlot, std::string message);

		// Print message to console
		static void ConPrint(std::string message);

		// Print message in client console
		static void ClientConPrint(int playerSlot, std::string message);

		// Issue server command
		static void ServerCommand(const std::string& command);

		// Issue client command (mimics client entering command in console)
		static void ClientCommand(int playerSlot, const std::string& command);

		// Set timescale
		static void SetTimescale(float timeScale);


		// Get of userid's near coordinates and send to python.
		// return list of just playerid
		static py::list GetPlayersNearCoords_list(py::dict vec, py::float_ distance, py::list ignore);
		static py::list GetPlayersNearPlayerID_list(py::int_ playerid, py::float_ distance, py::list ignore);

		// return dict of {playerid: dict(coordinates)}
		static py::dict GetPlayersNearCoords_dict(py::dict vec, py::float_ distance, py::list ignore, bool return_difference = true);
		static py::dict GetPlayersNearPlayerID_dict(py::int_ playerid, py::float_ distance, py::list ignore, bool return_difference = true);

	};
}
