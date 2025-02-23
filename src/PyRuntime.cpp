#include "PyRuntime.h"
#include "PyModule.h" // Include this once

namespace Source2Py {

	bool PyRuntime::s_InterpreterRunning = false;

	bool PyRuntime::Init() {
		if (s_InterpreterRunning) {
			//Log::Error("PyRuntime is already running!");
			Message("PyRuntime is already running!\n");
			return false;
		}
		
		try {
			py::initialize_interpreter();
			s_InterpreterRunning = true;
		}
		catch (std::exception& e) {
			Message("%s\n", e.what());
			//Log::Error(e.what());
			return false;
		}

		Message("PyRuntime started\n");
		//Log::Write("PyRuntime started");
		
		return true;
	}

	void PyRuntime::Close() {
		if (!s_InterpreterRunning) {
			Message("PyRuntime is not running!\n");
			//Log::Error("PyRuntime is not running!");
			return;
		}

		py::finalize_interpreter();
		s_InterpreterRunning = false;

		Message("PyRuntime stopped\n");
		//Log::Write("PyRuntime stopped");
	}
}