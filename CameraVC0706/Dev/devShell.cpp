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
		case 3:
		{
			g_DataSetMainControl.Thread_CAM_State = tDataSetMainControl::tStateCAM::UserTaskScriptStart;

			std::lock_guard<std::mutex> Lock(g_DataSetMainControl.Thread_CAM_State_UserTaskScriptIDMtx);

			g_DataSetMainControl.Thread_CAM_State_UserTaskScriptID = data[2];
			break;
		}
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
		std::cout << "restart\n";
		std::cout << "halt\n";
		std::cout << std::setw(g_ShellCommandListCol2Pos) << std::setfill(' ') << std::left << "exit" << "halt the engine and close the program\n";
		std::cout << std::setw(g_ShellCommandListCol2Pos) << std::setfill(' ') << std::left << "q" << "start reading\n";
		std::cout << std::setw(g_ShellCommandListCol2Pos) << std::setfill(' ') << std::left << "w" << "get a chunk of 16 bytes\n";
		std::cout << std::setw(g_ShellCommandListCol2Pos) << std::setfill(' ') << std::left << "e" << "get a chunk of 768 bytes\n";
		std::cout << std::setw(g_ShellCommandListCol2Pos) << std::setfill(' ') << std::left << "a" << "get whole image chunk by chunk of 16 bytes each\n";
		std::cout << std::setw(g_ShellCommandListCol2Pos) << std::setfill(' ') << std::left << "s" << "get whole image chunk by chunk of 512 bytes each\n";
		std::cout << std::setw(g_ShellCommandListCol2Pos) << std::setfill(' ') << std::left << "d" << "get whole image chunk by chunk of 768 bytes each\n";
		std::cout << std::setw(g_ShellCommandListCol2Pos) << std::setfill(' ') << std::left << "f" << "get whole image chunk by chunk of 51200 bytes each\n";
//#ifdef LIB_MODULE_CAMERA_VC0706_CONFIG
		std::cout << std::setw(g_ShellCommandListCol2Pos) << std::setfill(' ') << std::left << "i" << "read config SerialPort\n";
		std::cout << std::setw(g_ShellCommandListCol2Pos) << std::setfill(' ') << std::left << "o" << "write config SerialPort=UART - default\n";
		std::cout << std::setw(g_ShellCommandListCol2Pos) << std::setfill(' ') << std::left << "p" << "write config SerialPort=HSUART\n";
		std::cout << std::setw(g_ShellCommandListCol2Pos) << std::setfill(' ') << std::left << "k" << "write config UART BR=38400 - default\n";
		std::cout << std::setw(g_ShellCommandListCol2Pos) << std::setfill(' ') << std::left << "l" << "write config UART BR=115200\n";
		std::cout << std::setw(g_ShellCommandListCol2Pos) << std::setfill(' ') << std::left << "z" << "read config VideoResolution\n";
		std::cout << std::setw(g_ShellCommandListCol2Pos) << std::setfill(' ') << std::left << "x" << "write config VideoResolution=640x480 (VGA)\n";
		std::cout << std::setw(g_ShellCommandListCol2Pos) << std::setfill(' ') << std::left << "c" << "write config VideoResolution=320x240 (QVGA) - default\n";
		std::cout << std::setw(g_ShellCommandListCol2Pos) << std::setfill(' ') << std::left << "v" << "write config VideoResolution=160x120\n";
		std::cout << std::setw(g_ShellCommandListCol2Pos) << std::setfill(' ') << std::left << "b" << "read config Compression (default=0x35)\n";
//#endif//LIB_MODULE_CAMERA_VC0706_CONFIG

		/*
			std::cout << "-----------------------------------------" << std::endl;
	std::cout << "ModCameraVC0706 1.5" << std::endl;
	std::cout << "-----------------------------------------" << std::endl;
	std::cout << "S - Start" << std::endl;
	std::cout << "H - Halt" << std::endl;
	std::cout << "q - Start reading" << std::endl;
	std::cout << "w - Get a chunk of 16 bytes" << std::endl;
	std::cout << "e - Get a chunk of 768 bytes" << std::endl;
	std::cout << "a - Get whole image chunk by chunk of 16 bytes each" << std::endl;
	std::cout << "s - Get whole image chunk by chunk of 512 bytes each" << std::endl;
	std::cout << "d - Get whole image chunk by chunk of 768 bytes each" << std::endl;
	std::cout << "f - Get whole image chunk by chunk of 51200 bytes each" << std::endl;
#ifdef LIB_MODULE_CAMERA_VC0706_CONFIG
	std::cout << "i - Read config SerialPort" << std::endl;
	std::cout << "o - Write config SerialPort=UART - default" << std::endl;
	std::cout << "p - Write config SerialPort=HSUART" << std::endl;
	std::cout << "k - Write config UART BR=38400 - default" << std::endl;
	std::cout << "l - Write config UART BR=115200" << std::endl;
	std::cout << "z - Read config VideoResolution" << std::endl;
	std::cout << "x - Write config VideoResolution=640x480 (VGA)" << std::endl;
	std::cout << "c - Write config VideoResolution=320x240 (QVGA) - default" << std::endl;
	std::cout << "v - Write config VideoResolution=160x120" << std::endl;
	std::cout << "b - Read config Compression (default=0x35)" << std::endl;
#endif//LIB_MODULE_CAMERA_VC0706_CONFIG
		*/
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
