#include <devConfig.h>

#include <devCamera.h>
#include <devLog.h>
#include <devSettings.h>
#include <devShell.h>

#include <utilsBase.h>
#include <utilsPath.h>

#include <atomic>
#include <chrono>
#include <filesystem>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>

#include <boost/asio.hpp>

namespace dev
{
	void ThreadFunShell();
}

tDataSetMainControl g_DataSetMainControl;

void Thread_CAM_Handler(std::promise<bool>& promise)
{
	dev::tLog Log(dev::tLog::tID::CAM, "CAM");

	Log.LogSettings.Field.CAM = 1;

	boost::asio::io_context IO;

	try
	{
		dev::tCamera Dev(&Log, IO);

		std::thread Thread_IO([&]() { IO.run(); });

		bool Thread_Dev_Exists = true;
		bool Thread_Dev_ExistOnError = false;
		std::thread Thread_Dev([&]()
			{
				try
				{
					//if (g_DataSetMainControl.Thread_CAM_State == tDataSetMainControl::tStateCAM::Exit)//[TBD] TEST
					//	Thread_Dev_Exists = false;
					Dev();
					Thread_Dev_Exists = false;
					const std::string ErrMsg = Dev.GetLastErrorMsg();
					if (!ErrMsg.empty())
					{
						std::cerr << ErrMsg << "\n";
						Thread_Dev_ExistOnError = true;
					}
				}
				catch (...)
				{
					Thread_Dev_Exists = false;
					promise.set_exception(std::current_exception());
				}
			});

		tDataSetMainControl::tStateCAM StateGNSSPrev = g_DataSetMainControl.Thread_CAM_State;

		while (true)
		{
			if (!Thread_Dev_Exists)
				break;

			if (g_DataSetMainControl.Thread_CAM_State != tDataSetMainControl::tStateCAM::Nothing)
			{
				switch (g_DataSetMainControl.Thread_CAM_State)
				{
				case tDataSetMainControl::tStateCAM::Start: Dev.Start(); break;
				case tDataSetMainControl::tStateCAM::Halt: Dev.Halt(); break;
				//case tDataSetMainControl::tStateCAM::Restart: Dev.Restart(); break;
				case tDataSetMainControl::tStateCAM::Exit: Dev.Exit(); break;
				//case tDataSetMainControl::tStateCAM::UserTaskScriptStart:
				//{
				//	std::lock_guard<std::mutex> Lock(g_DataSetMainControl.Thread_CAM_State_UserTaskScriptIDMtx);

				//	if (!g_DataSetMainControl.Thread_CAM_State_UserTaskScriptID.empty())
				//	{
				//		Dev.StartUserTaskScript(g_DataSetMainControl.Thread_CAM_State_UserTaskScriptID);

				//		g_DataSetMainControl.Thread_CAM_State_UserTaskScriptID.clear();
				//	}
				//	break;
				//}
				}

				if (g_DataSetMainControl.Thread_CAM_State == tDataSetMainControl::tStateCAM::Exit)
					break;

				g_DataSetMainControl.Thread_CAM_State = tDataSetMainControl::tStateCAM::Nothing;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}

		Thread_Dev.join();

		IO.stop();

		Thread_IO.join();

		if (!Thread_Dev_Exists)
			promise.set_value(Thread_Dev_ExistOnError);
	}
	catch (...)
	{
		promise.set_exception(std::current_exception());
	}
}

int main(int argc, char* argv[])
{
	const bool ShellEnabled = argc >= 2 && !strcmp(argv[1], "shell");

	dev::tLog::LogSettings.Value = 0;
	dev::tLog::LogSettings.Field.Enabled = ShellEnabled ? 1 : 0;

	try
	{
		const std::filesystem::path Path{ argv[0] };
		std::filesystem::path PathFile = Path.filename();
		if (PathFile.has_extension())
			PathFile.replace_extension();

		std::string FileNameConf = utils::linux::GetPathConfig(PathFile.string());
		if (FileNameConf.empty())
			throw std::runtime_error{ "config file is not found" };

		dev::g_Settings = dev::tSettings(FileNameConf);
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";

		return static_cast<int>(utils::tExitCode::EX_CONFIG);
	}

	utils::tExitCode CErr = utils::tExitCode::EX_OK;
	////////////////////////////////
	std::thread Thread_Shell;

	if (ShellEnabled)
		Thread_Shell = std::thread(dev::ThreadFunShell);
	////////////////////////////////

	dev::config::tPicture Pict = dev::g_Settings.Picture;
	utils::RemoveFilesOutdated(Pict.Path, Pict.Prefix, Pict.QtyMax);

	////////////////////////////////

	std::promise<bool> Thread_CAM_Promise;
	auto Thread_CAM_Future = Thread_CAM_Promise.get_future();

	std::thread Thread_CAM(Thread_CAM_Handler, std::ref(Thread_CAM_Promise));//C++11

	try
	{
		if (Thread_CAM_Future.get())
			CErr = utils::tExitCode::EX_NOINPUT;
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";

		g_DataSetMainControl.Thread_CAM_State = tDataSetMainControl::tStateCAM::Exit;

		CErr = utils::tExitCode::EX_IOERR;
	}

	Thread_CAM.join();

	if (ShellEnabled)
		Thread_Shell.detach();

	return static_cast<int>(CErr);
}

/*#include <process.h>
#include <windows.h>
#include <winbase.h>

#include <string>
#include <vector>

#include <devCamera.h>

#include <iostream>
#include <fstream>

//#define LIB_MODULE_CAMERA_VC0706_CONFIG_TEST_1

std::pair<std::string, int> g_DefaultSettings_SerialPort;
std::string g_DefaultSettings_Files_ImageFileName;

bool g_WriteToFileImage;

HANDLE g_ModCameraUIEvent;
HANDLE g_ModCameraMutex;

unsigned __stdcall Tick10ms(void* data)
{
	dev::tCamera* Entity = (dev::tCamera*)data;

	while (1)
	{
		WaitForSingleObject(g_ModCameraMutex, INFINITE);

		Entity->Tick10ms();

		ReleaseMutex(g_ModCameraMutex);

		Sleep(10);
	}

	return 0;
}

unsigned __stdcall UserInterface(void* data);

void main(int argc, char **argv)
{

	std::string CfgFileName = "App.ini";

	if (argc > 0)
	{
		std::string FileName = argv[0];
		int Start = FileName.rfind("\\") + 1;

		FileName = FileName.substr(Start, FileName.length() - Start);

		if(FileName.length() > 0)
			CfgFileName = FileName + ".ini";
	}

	cfgFile::ConfigurationFile<cfgFile::SectionCOMAndIPSettings> AppCfg1(CfgFileName);

	g_DefaultSettings_SerialPort = AppCfg1.GetSection("SerialPorts").GetValue("Camera");

	cfgFile::ConfigurationFile<cfgFile::SectionSimpleString> AppCfg2(CfgFileName);
	g_DefaultSettings_Files_ImageFileName = AppCfg2.GetSection("Files").GetValue("ImageFileName");

	std::fstream FileImage(g_DefaultSettings_Files_ImageFileName, std::ios_base::binary | std::ios_base::out);
	FileImage.close();

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
	std::cout << "-----------------------------------------" << std::endl;

	g_ModCameraUIEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	g_ModCameraMutex = CreateMutex(NULL, FALSE, NULL);

	dev::tCamera Entity;

	HANDLE UserInterfaceThread = (HANDLE)_beginthreadex(NULL, 0, UserInterface, (void*)&Entity, 0, NULL);

	HANDLE ThreadTick10ms = (HANDLE)_beginthreadex(NULL, 0, Tick10ms, (void*) &Entity, 0, NULL);

	while (1)
	{
		WaitForSingleObject(g_ModCameraMutex, INFINITE);

		Entity.Control();

		ReleaseMutex(g_ModCameraMutex);

		SwitchToThread();//[srg]2017-02-08
	}

	CloseHandle(ThreadTick10ms);
	
	system("pause");
}

unsigned __stdcall UserInterface(void* data)
{
	dev::tCamera* Entity = (dev::tCamera*)data;

	int m_ImageChunkSize = 0;
	
	mod::CameraVC0706::Packet::tMemoryDevice Memory = mod::CameraVC0706::Packet::tMemoryDevice_I2C_EEPROM;

	bool GetImageReady = false;

	while (1)
	{
		char Byte;
		std::cin >> Byte;

		WaitForSingleObject(g_ModCameraMutex, INFINITE);

		switch (Byte)
		{
		case 'S':
		{
			std::cout << "Start" << std::endl;

			Entity->Start();

			break;
		}
		case 'H':
		{
			std::cout << "Halt" << std::endl;

			Entity->Halt();

			break;
		}
		case 'q':
		{
			std::cout << "ModCamera->GetImageReady()" << std::endl;

			std::fstream FileImage(g_DefaultSettings_Files_ImageFileName, std::ios_base::binary | std::ios_base::out);
			FileImage.close();

			if (Entity->GetImageReady())
			{
				SetEvent(g_ModCameraUIEvent);

				GetImageReady = true;
			}

			break;
		}
		case 'w':
		{
			if (GetImageReady)
			{
				std::cout << "ModCamera->GetImageChunk(16)" << std::endl;

				Entity->GetImageChunk(16);

				GetImageReady = false;
			}

			break;
		}
		case 'e':
		{
			if (GetImageReady)
			{
				std::cout << "ModCamera->GetImageChunk(768)" << std::endl;

				Entity->GetImageChunk(768);

				GetImageReady = false;
			}

			break;
		}
		case 'a':
		{
			if (GetImageReady)
			{
				std::cout << "ModCamera->GetImageChunk(16) whole file" << std::endl;

				//m_ImageChunkSize = 256;//OK
				//m_ImageChunkSize = 128;//OK
				//m_ImageChunkSize = 64;//OK
				//m_ImageChunkSize = 32;//OK
				m_ImageChunkSize = 16;//OK
				//m_ImageChunkSize = 20;//doesn't work

				g_WriteToFileImage = true;

				GetImageReady = false;
			}

			break;
		}
		case 's':
		{
			if (GetImageReady)
			{
				std::cout << "ModCamera->GetImageChunk(512) whole file" << std::endl;

				m_ImageChunkSize = 512;

				g_WriteToFileImage = true;

				GetImageReady = false;
			}

			break;
		}
		case 'd':
		{
			if (GetImageReady)
			{
				std::cout << "ModCamera->GetImageChunk(768) whole file" << std::endl;

				m_ImageChunkSize = 768;

				g_WriteToFileImage = true;

				GetImageReady = false;
			}

			break;
		}
		case 'f':
		{
			if (GetImageReady)
			{
				std::cout << "ModCamera->GetImageChunk(51200) whole file" << std::endl;

				m_ImageChunkSize = 51200;

				g_WriteToFileImage = true;

				GetImageReady = false;
			}

			break;
		}
#ifdef LIB_MODULE_CAMERA_VC0706_CONFIG
		case 'i':
		{
			std::cout << "ModCamera->GetConfig(0x0007, 7)" << std::endl;

			Entity->GetConfig(Memory, mod::CameraVC0706::Packet::REG_Address_Settings_SerialPort, 7);

			break;
		}
		case 'o':
		{
			std::cout << "ModCamera->SetConfig(SerialPort=UART)" << std::endl;

			std::vector<char> Data;

			Data.push_back((char)mod::CameraVC0706::Packet::tSerialCommInterface_UART);

			Entity->SetConfig(Memory, mod::CameraVC0706::Packet::REG_Address_Settings_SerialPort, Data);

			break;
		}
		case 'p':
		{
			std::cout << "ModCamera->SetConfig(SerialPort=HSUART)" << std::endl;

			std::vector<char> Data;

			Data.push_back((char)mod::CameraVC0706::Packet::tSerialCommInterface_HSUART);

			Entity->SetConfig(Memory, mod::CameraVC0706::Packet::REG_Address_Settings_SerialPort, Data);

			break;
		}
		case 'k':
		{
			std::cout << "ModCamera->SetConfig(UART BR=38400)" << std::endl;

			std::vector<char> Data;

			Data.push_back(0x2A);
			Data.push_back(0xF2);

			Entity->SetConfig(Memory, mod::CameraVC0706::Packet::REG_Address_Settings_SerialPort_UART_BR, Data);

			break;
		}
		case 'l':
		{
			std::cout << "ModCamera->SetConfig(UART BR=115200)" << std::endl;

			std::vector<char> Data;

			Data.push_back(0x0D);
			Data.push_back(0xA6);

			Entity->SetConfig(Memory, mod::CameraVC0706::Packet::REG_Address_Settings_SerialPort_UART_BR, Data);

			break;
		}
		/////////////////////////////////////////////////////////
		case 'z':
		{
			std::cout << "ModCamera->GetConfig(0x0019, 1)" << std::endl;

			Entity->GetConfig(Memory, mod::CameraVC0706::Packet::REG_Address_Settings_VideoResolution, 1);

			break;
		}
		case 'x':
		{
			std::cout << "ModCamera->SetConfig(VideoResolution=640x480)" << std::endl;

			std::vector<char> Data;

			Data.push_back((char)mod::CameraVC0706::Packet::tVideoResolution_640x480);

			Entity->SetConfig(Memory, mod::CameraVC0706::Packet::REG_Address_Settings_VideoResolution, Data);

			break;
		}
		case 'c':
		{
			std::cout << "ModCamera->SetConfig(VideoResolution=320x240)" << std::endl;

			std::vector<char> Data;

			Data.push_back((char)mod::CameraVC0706::Packet::tVideoResolution_320x240);

			Entity->SetConfig(Memory, mod::CameraVC0706::Packet::REG_Address_Settings_VideoResolution, Data);

			break;
		}
		case 'v':
		{
			std::cout << "ModCamera->SetConfig(VideoResolution=160x120)" << std::endl;

			std::vector<char> Data;

			Data.push_back((char)mod::CameraVC0706::Packet::tVideoResolution_160x120);

			Entity->SetConfig(Memory, mod::CameraVC0706::Packet::REG_Address_Settings_VideoResolution, Data);

			break;
		}
		/////////////////////////////////////////////////////////
		case 'b':
		{
			std::cout << "ModCamera->GetConfig(VideoCompression, 1)" << std::endl;

			Entity->GetConfig(Memory, mod::CameraVC0706::Packet::REG_Address_Settings_VideoCompression, 1);

			break;
		}
#ifdef LIB_MODULE_CAMERA_VC0706_CONFIG_TEST_1
		case '0':
		{
			std::cout << "ModCamera->GetConfig(0x0007, 7)" << std::endl;

			Entity->GetConfig((mod::CameraVC0706::Packet::tMemoryDevice)0, mod::CameraVC0706::Packet::REG_Address_Settings_SerialPort, 7);

			break;
		}
		case '1':
		{
			std::cout << "ModCamera->GetConfig(0x0007, 7)" << std::endl;

			Entity->GetConfig(mod::CameraVC0706::Packet::tMemoryDevice_Chip_Reg, mod::CameraVC0706::Packet::REG_Address_Settings_SerialPort, 7);

			break;
		}
		case '2':
		{
			std::cout << "ModCamera->GetConfig(0x0007, 7)" << std::endl;

			Entity->GetConfig(mod::CameraVC0706::Packet::tMemoryDevice_Sensor_Reg, mod::CameraVC0706::Packet::REG_Address_Settings_SerialPort, 7);

			break;
		}
		case '3':
		{
			std::cout << "ModCamera->GetConfig(0x0007, 7)" << std::endl;

			Entity->GetConfig(mod::CameraVC0706::Packet::tMemoryDevice_CCIR656_Reg, mod::CameraVC0706::Packet::REG_Address_Settings_SerialPort, 7);

			break;
		}
		case '4':
		{
			std::cout << "ModCamera->GetConfig(0x0007, 7)" << std::endl;

			Entity->GetConfig(mod::CameraVC0706::Packet::tMemoryDevice_I2C_EEPROM, mod::CameraVC0706::Packet::REG_Address_Settings_SerialPort, 7);

			break;
		}
		case '5':
		{
			std::cout << "ModCamera->GetConfig(0x0007, 7)" << std::endl;

			Entity->GetConfig(mod::CameraVC0706::Packet::tMemoryDevice_SPI_EEPROM, mod::CameraVC0706::Packet::REG_Address_Settings_SerialPort, 7);

			break;
		}
		case '6':
		{
			std::cout << "ModCamera->GetConfig(0x0007, 7)" << std::endl;

			Entity->GetConfig(mod::CameraVC0706::Packet::tMemoryDevice_SPI_Flash, mod::CameraVC0706::Packet::REG_Address_Settings_SerialPort, 7);

			break;
		}
#endif//LIB_MODULE_CAMERA_VC0706_CONFIG_TEST_1
#endif//LIB_MODULE_CAMERA_VC0706_CONFIG
		}

		ReleaseMutex(g_ModCameraMutex);

		while (g_WriteToFileImage)
		{
			WaitForSingleObject(g_ModCameraMutex, INFINITE);

			Entity->GetImageChunk(m_ImageChunkSize);

			ReleaseMutex(g_ModCameraMutex);

			WaitForSingleObject(g_ModCameraUIEvent, INFINITE);
		}
	}

	return 0;
}

void WriteToFileImage(std::vector<char>& data)
{
	std::fstream File(g_DefaultSettings_Files_ImageFileName, std::ios_base::binary | std::ios_base::app);

	File.write(&data[0], data.size());

	File.close();

	SetEvent(g_ModCameraUIEvent);
}

void WriteToFileImageComplete()
{
	g_WriteToFileImage = false;

	SetEvent(g_ModCameraUIEvent);
}

void OnModReady()
{
	g_WriteToFileImage = false;

	SetEvent(g_ModCameraUIEvent);
}

void OnModHalt()
{
	g_WriteToFileImage = false;

	SetEvent(g_ModCameraUIEvent);
}

void OnModFailed()
{
	g_WriteToFileImage = false;

	SetEvent(g_ModCameraUIEvent);
}*/