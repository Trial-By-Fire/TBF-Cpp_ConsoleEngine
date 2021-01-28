// Parent Header
#include "Windows_Platform.hpp"



// Includes

#include "CString.hpp"
#include "Execution.hpp"



namespace OSPlatform
{
	namespace StdHandle
	{
		HANDLE Invalid()
		{
			return INVALID_HANDLE_VALUE;
		}
	}


	// Static Data

	CTS_CString SConsole_In   = "CONIN$";
	CTS_CString SConsole_Out  = "CONOUT$";
	CTS_CString SConsole_Null = "NUL:";

	CTS_CString SReadCode  = "r";
	CTS_CString SWriteCode = "w";



	// Functions


	// Public

	FILE* StdInput(void)
	{
		return stdin;
	}

	FILE* StdOutput(void)
	{
		return stdout;
	}

	FILE* StdError(void)
	{
		return stderr;
	}

	bool Bind_IOBufferTo_Console(void)
	{
		unbound CompileTime int IO_NoBuffer = _IONBF;


		FILE* dummyFile = nullptr;


		freopen_s(&dummyFile, SConsole_In , SReadCode , StdInput ());
		freopen_s(&dummyFile, SConsole_Out, SWriteCode, StdOutput());
		freopen_s(&dummyFile, SConsole_Out, SWriteCode, StdError ());

		// Redirect STDIN if the console has an input handle	
		if (GetStdHandle(StdHandle::Input) != StdHandle::Invalid())
		{
			if (freopen_s(&dummyFile, SConsole_In, SReadCode, StdInput()) != 0)
			{
				return false;
			}
			else
			{
				setvbuf(StdInput(), nullptr, IO_NoBuffer, 0);
			}
		}

		// Redirect STDOUT if the console has an output handle
		if (GetStdHandle(StdHandle::Output) != StdHandle::Invalid)
		{
			if (freopen_s(&dummyFile, SConsole_Out, SWriteCode, StdOutput()) != 0)
			{
				return false;
			}
			else
			{
				setvbuf(StdOutput(), nullptr, IO_NoBuffer, 0);
			}
		}

		// Redirect STDERR if the console has an error handle
		if (GetStdHandle(StdHandle::Output) != StdHandle::Invalid())
		{
			if (freopen_s(&dummyFile, SConsole_Out, SWriteCode, StdError()) != 0)
			{
				return false;
			}
			else
			{
				setvbuf(StdError (), nullptr, IO_NoBuffer, 0);
			}
		}

		return true;
	}

	bool RequestConsole(void)
	{
		return AllocConsole();
	}

	bool Unbind_IOBufferTo_Console(void)
	{
		unbound CompileTime int IO_NoBuffer = _IONBF;


		FILE* dummyFile;


		// Just to be safe, redirect standard IO to NUL before releasing.

		// Redirect STDIN to NUL
		if (freopen_s(&dummyFile, SConsole_Null, SReadCode, StdInput()) != 0)
		{
			return false;
		}
		else
		{
			setvbuf(StdInput(), nullptr, IO_NoBuffer, 0);
		}

		// Redirect STDOUT to NUL
		if (freopen_s(&dummyFile, SConsole_Null, SWriteCode, StdOutput()) != 0)
		{
			return false;
		}
		else
		{
			setvbuf(StdOutput(), nullptr, IO_NoBuffer, 0);
		}

		// Redirect STDERR to NUL
		if (freopen_s(&dummyFile, SConsole_Null, SWriteCode, StdError()) != 0)
		{
			return false;
		}
		else
		{
			setvbuf(StdError(), nullptr, IO_NoBuffer, 0);
		}

		return true;
	}

	bool GetKeySignal(EKeyCode _key)
	{
		if (GetAsyncKeyState((int)_key) & 0x8000)
		{
			return true;
		}
		else
		{
			return false;
		}
	}	
}



// Private

INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	EntryPoint();

	return 0;
}
