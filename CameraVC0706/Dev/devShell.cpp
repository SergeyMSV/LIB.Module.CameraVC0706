#include "devShell.h"

#include "devDataSet.h"
#include "devLog.h"

#include <iostream>
#include <iomanip>

extern tDataSetMainControl g_DataSetMainControl;

namespace dev
{

static const std::vector<utils::shell::tShellCommandList> g_ShellCommandList
{
	{ (char*)"?",      (char*)"help",				tShell::HandlerHelp },
	{ (char*)"help",   (char*)"help",				tShell::HandlerHelp },
	{ (char*)"echo",   (char*)"ECHO 0-off, 1-on",	tShell::HandlerEcho },
	{ (char*)"log",    (char*)"log  0-off, 1-on",	tShell::HandlerLog },
	{ (char*)"cam",    (char*)"",					tShell::HandlerCAM },
	{ (char*)"exit",   (char*)"",					tShell::HandlerEXIT },
	{ 0 }
};

const uint8_t g_ShellCommandListCol2Pos = 10;

tShell* g_pShell = nullptr;

tShell::tShell(const utils::shell::tShellCommandList* cmdList, size_t cmdListSize)
	:utils::shell::tShell(cmdList, cmdListSize)
{
	g_pShell = this;

	OnShell();
}

tShell::~tShell()
{
	g_pShell = nullptr;
}

bool tShell::HandlerEcho(const std::vector<std::string>& data)
{
	if (data.size() == 2 && g_pShell)
	{
		g_pShell->SetEcho(data[1] != "0");

		return true;
	}

	return false;
}

bool tShell::HandlerHelp(const std::vector<std::string>& data)
{
	if (data.size() == 1)
	{
		for (size_t i = 0; i < g_ShellCommandList.size(); ++i)
		{
			if (g_ShellCommandList[i].Command == 0)
				break;

			std::cout << std::setw(g_ShellCommandListCol2Pos) << std::setfill(' ') << std::left << g_ShellCommandList[i].Command << g_ShellCommandList[i].Description << '\n';
		}

		return true;
	}

	return false;
}

bool tShell::HandlerLog(const std::vector<std::string>& data)
{
	if (data.size() == 2 && data[1] == "cam")
	{
		tLog::LogSettings.Field.CAM = !tLog::LogSettings.Field.CAM;
		return true;
	}
	else
	{
		std::cout << std::setw(g_ShellCommandListCol2Pos) << std::setfill('.') << std::left << "cam" << tLog::LogSettings.Field.CAM << '\n';
		return true;
	}
}

bool tShell::HandlerCAM(const std::vector<std::string>& data)
{
	if (data.size() >= 2 && data[1] == "start")
	{
		switch (data.size())
		{
		case 2: g_DataSetMainControl.Thread_CAM_State = tDataSetMainControl::tStateCAM::Start; break;
		//case 3:
		//{
		//	g_DataSetMainControl.Thread_CAM_State = tDataSetMainControl::tStateCAM::UserTaskScriptStart;

		//	std::lock_guard<std::mutex> Lock(g_DataSetMainControl.Thread_CAM_State_UserTaskScriptIDMtx);

		//	g_DataSetMainControl.Thread_CAM_State_UserTaskScriptID = data[2];
		//	break;
		//}
		}
	}
	else if (data.size() == 2 && data[1] == "restart")
	{
		g_DataSetMainControl.Thread_CAM_State = tDataSetMainControl::tStateCAM::Restart;
	}
	else if (data.size() == 2 && (data[1] == "halt" || data[1] == "stop"))
	{
		g_DataSetMainControl.Thread_CAM_State = tDataSetMainControl::tStateCAM::Halt;
	}
	else if (data.size() == 2 && data[1] == "exit")
	{
		g_DataSetMainControl.Thread_CAM_State = tDataSetMainControl::tStateCAM::Exit;
	}
	else
	{
		std::cout << "start\n";
		std::cout << "halt\n";
		std::cout << std::setw(g_ShellCommandListCol2Pos) << std::setfill(' ') << std::left << "exit" << "halt the engine and close the program\n";
	}
	return true;
}

bool tShell::HandlerEXIT(const std::vector<std::string>& data)
{
	g_DataSetMainControl.Thread_CAM_State = tDataSetMainControl::tStateCAM::Exit;

	return true;
}

void tShell::Board_Send(char data) const
{
	std::cout << data;
}

void tShell::Board_Send(const std::vector<char>& data) const
{
	for (size_t i = 0; i < data.size(); ++i)
	{
		std::cout << data[i];
	}
}

void tShell::OnShell() const
{
	std::cout << "# ";
}

void tShell::OnFailed(std::vector<std::string>& data) const
{
	std::cout << "OnFailed:" << std::endl;

	ShowReceivedCmd(data);
}

void tShell::ShowReceivedCmd(const std::vector<std::string>& data)
{
	for (size_t i = 0; i < data.size(); ++i)
	{
		std::cout << i << " " << data[i] << std::endl;
	}
}

void ThreadFunShell()
{
	dev::tShell Shell(g_ShellCommandList.data(), g_ShellCommandList.size());

	while (g_DataSetMainControl.Thread_CAM_State != tDataSetMainControl::tStateCAM::Exit)
	{
		int Byte = getchar();

		Byte = tolower(Byte);

		Shell.Board_OnReceived(static_cast<char>(Byte));
	}
}

}
