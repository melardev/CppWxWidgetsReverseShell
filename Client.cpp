#include "Client.h"
#include "Application.h"


wxDECLARE_EVENT(ClientSocketId, wxSocketEvent);
wxDEFINE_EVENT(ClientSocketId, wxSocketEvent);


wxBEGIN_EVENT_TABLE(Client, wxEvtHandler)
		EVT_SOCKET(ClientSocketId, Client::OnSocketEvent)
wxEND_EVENT_TABLE()

Client::Client(Application* app): clientSocket(new wxSocketClient), receivingBuffer{0}, app(app)
{
}

void Client::Start()
{
	wxIPV4address address;
	address.Hostname("localhost");
	address.Service(3002);

	clientSocket->SetEventHandler(*this, ClientSocketId);

	clientSocket->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
	clientSocket->Notify(true);
	((wxSocketClient*)clientSocket)->Connect(address, false);
}

void Client::OnSocketEvent(wxSocketEvent& event)
{
	wxString s = "OnSocketEvent: ";
	wxSocketBase* sock = event.GetSocket();

	switch (event.GetSocketEvent())
	{
	case wxSOCKET_CONNECTION:
		app->onConnectionSuccessful();
		break;
	case wxSOCKET_INPUT:
		{
			ReadFromSocket(sock);
			break;
		}
	case wxSOCKET_LOST:
		{
			app->OnServerDisconnected();
			break;
		}
	default: ;
	}
}

void Client::ReadFromSocket(wxSocketBase* sock)
{
	// We disable input events, so that the test doesn't trigger
	// wxSocketEvent again.
	sock->SetNotify(wxSOCKET_LOST_FLAG);

	sock->Read(receivingBuffer, 1024);
	const wxUint32 readSoFar = sock->GetLastIOReadSize();
	app->OnMessageReceived(wxString(receivingBuffer, 0, readSoFar));
	sock->SetNotify(wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
}

void Client::SendData(const wxString& data) const
{
	clientSocket->Write(data.c_str(), data.size());
}

void Client::FinishSync() const
{
	clientSocket->Close();
}
