#pragma once
#include <wx/socket.h>


class Application;

class Client : public wxEvtHandler
{
public:
	Client(Application* app);

	void OnSocketEvent(wxSocketEvent& event);

	void ReadFromSocket(wxSocketBase* sock);

	void Start();
	
	void SendData(const wxString& data) const;

	void FinishSync() const;

private:
	wxSocketBase* clientSocket;
	char receivingBuffer[1024];
	Application* app;
wxDECLARE_EVENT_TABLE();
};
