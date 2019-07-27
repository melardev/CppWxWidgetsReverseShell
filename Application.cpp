#include "Application.h"
#include "Client.h"
#include "ProcessHandler.h"

void Application::Run()
{
#if  defined(_WIN32) || defined(_WIN64)
	const wxString cmd = "cmd";

#else
		wxString cmd;
		if (wxFileExists("/bin/bash"))
			cmd.Append("/bin/bash");
		else if (wxFileExists("/bin/sh"))
			cmd.Append("/bin/sh");
		else
		{
			wxPuts("Shell could not be located on this system");
			// wxExit();
			exit(2);
		}
#endif

	wxApp::SetInstance(new wxAppConsole);
	int argc = 0;
	char** argv = nullptr;
	wxEntryStart(argc, argv);

	m_ProcHandler = new ProcessHandler(this, cmd);
	m_Client = new Client(this);
	m_Client->Start();

	wxTheApp->OnInit();
	wxTheApp->OnRun();

	m_ProcHandler->FinishSync();
	m_Client->FinishSync();

	// Exit the app
	wxTheApp->OnExit();
	// Cleanup the wxWidgets framework
	wxEntryCleanup();
}

void Application::onConnectionSuccessful() const
{
	m_ProcHandler->Start();
}

void Application::OnMessageReceived(const wxString& command)
{
	m_ProcHandler->WriteIntoProcess(command);
}

void Application::OnProcOutput(const wxString& processOutput)
{
	m_Client->SendData(processOutput);
}
