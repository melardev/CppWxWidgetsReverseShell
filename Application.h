#pragma once
#include <wx/app.h>
#include "Client.h"
#include "ProcessHandler.h"


class Application
{
public:
	Application() = default;
	~Application() = default;

	void Run();
	void onConnectionSuccessful() const;
	void OnMessageReceived(const wxString& command);
	void OnProcOutput(const wxString& processOutput);
	Client* m_Client;
	ProcessHandler* m_ProcHandler;
};



