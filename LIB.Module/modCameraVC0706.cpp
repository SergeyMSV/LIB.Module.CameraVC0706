#include "modCameraVC0706.h"
/*
namespace mod
{

tCameraVC0706::tCameraVC0706(utils::log::tLog *log, tCameraVC0706Settings settings)
{
	p_log = log;

	m_Settings = settings;

	p_State = tStateHalt::Instance(this, true);
}

tCameraVC0706::~tCameraVC0706()
{
	delete p_State;
}

void tCameraVC0706::Tick10ms()
{
	p_State->Tick10ms();
}

void tCameraVC0706::Tick100ms()
{
	p_State->Tick100ms();
}

void tCameraVC0706::Control()
{
	p_State->Control();
}

void tCameraVC0706::Start()
{
	p_State->Start();
}

void tCameraVC0706::Halt()
{
	p_State->Halt();
}

bool tCameraVC0706::IsReady()
{
	return p_State->IsReady();
}

bool tCameraVC0706::GetImageReady()
{
	return p_State->GetImageReady();
}

bool tCameraVC0706::GetImageChunk(int chunkSize)
{
	return p_State->GetImageChunk(chunkSize);
}

#ifdef LIB_MODULE_CAMERA_VC0706_CONFIG
bool tCameraVC0706::GetConfig(CameraVC0706::Packet::tMemoryDevice memory, int address, int size)
{
	return p_State->GetConfig(memory, address, size);
}

bool tCameraVC0706::SetConfig(CameraVC0706::Packet::tMemoryDevice memory, int address, std::vector<char>& data)
{
	return p_State->SetConfig(memory, address, data);
}
#endif//LIB_MODULE_CAMERA_VC0706_CONFIG

void tCameraVC0706::Board_OnReceived(std::vector<char>& data)
{
	p_State->Receive(data);
}

void tCameraVC0706::ChangeState(tState *state)
{
	tState* prev = p_State;

	p_State = state;

	delete prev;
}

}
*/