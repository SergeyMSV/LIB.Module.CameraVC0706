#include "modCameraVC0706.h"

namespace mod
{

tCameraVC0706::tCameraVC0706(utils::tLog* log)
	:m_pLog(log)
{
	ChangeState(new tStateHalt(this, "the very start"));
	return;
}

void tCameraVC0706::operator()()
{
	while ((*m_pState)());
}

void tCameraVC0706::Start()
{
	m_Control_Operation = true;
}

void tCameraVC0706::Start(bool exitOnError)
{
	m_Control_ExitOnError = exitOnError;
	Start();
}

void tCameraVC0706::Restart()
{
	m_Control_Restart = true;
}

void tCameraVC0706::Halt()
{
	m_Control_Operation = false;
}

void tCameraVC0706::Exit()
{
	m_Control_Exit = true;
	m_Control_Operation = false;
}

//bool tCameraVC0706::StartUserTaskScript(const std::string& taskScriptID)
//{
//	//std::lock_guard<std::mutex> Lock(m_MtxState);
//
//	return m_pState->SetUserTaskScript(taskScriptID);
//}

tCameraStatus tCameraVC0706::GetStatus() const
{
	//std::lock_guard<std::mutex> Lock(m_MtxState);

	return m_pState->GetStatus();
}

std::string tCameraVC0706::GetLastErrorMsg() const
{
	//std::lock_guard<std::mutex> lock(m_mtxstate);
	
	return m_LastErrorMsg;
}

void tCameraVC0706::Board_OnReceived(utils::tVectorUInt8& data)
{
	std::lock_guard<std::mutex> Lock(m_MtxReceivedData);

	m_ReceivedData.push(data);
}

//bool tCameraVC0706::WaitForReceivedData(std::uint32_t wait_ms) const
//{
//	std::chrono::time_point<tClock> StartTime = tClock::now();
//
//	while (true)
//	{
//		{
//			std::lock_guard<std::mutex> Lock(m_MtxReceivedData);
//			if (m_ReceivedData.size() > 0)
//				return true;
//		}
//
//		std::this_thread::sleep_for(std::chrono::milliseconds(1));
//
//		const auto Duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(tClock::now() - StartTime).count();
//		if (wait_ms < Duration_ms)
//			return false;
//	}
//
//	return false;
//}

bool tCameraVC0706::IsReceivedData() const
{
	std::lock_guard<std::mutex> Lock(m_MtxReceivedData);

	return m_ReceivedData.size() > 0;
}

utils::tVectorUInt8 tCameraVC0706::GetReceivedDataChunk()
{
	std::lock_guard<std::mutex> Lock(m_MtxReceivedData);

	if (m_ReceivedData.size() > 0)
	{
		utils::tVectorUInt8 Data(std::forward<utils::tVectorUInt8>(m_ReceivedData.front()));

		m_ReceivedData.pop();

		return Data;
	}

	return {};
}




/*
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
*/

void tCameraVC0706::ClearReceivedData()
{
	std::lock_guard<std::mutex> Lock(m_MtxReceivedData);

	while (!m_ReceivedData.empty())
	{
		m_ReceivedData.pop();
	}
}

//void tGnssReceiver::SetStrTimePeriod(std::stringstream& stream, const std::chrono::time_point<tClock>& timePoint) const
//{
//	auto Time_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(tClock::now() - timePoint).count();//C++11
//	double Time_ms = static_cast<double>(Time_ns) / 1000000;//C++11
//
//	stream << Time_ms << " ms";
//}
//
//void tGnssReceiver::SetStrBaudrate(std::stringstream& stream, const std::chrono::time_point<tClock>& timePoint, std::size_t sizeBytes) const
//{
//	const auto Time_us = std::chrono::duration_cast<std::chrono::microseconds>(tClock::now() - timePoint).count();//C++11
//	const double Time_ms = static_cast<double>(Time_us) / 1000;//C++11
//
//	//stream << "; ";
//	stream << sizeBytes << ", ";
//	stream.setf(std::ios::fixed);
//	const int SizeFract = 2;
//	stream << std::setw(SizeFract + 5) << std::setprecision(SizeFract);
//	stream << Time_ms << " ms, ";
//
//	const double Time_s = Time_ms / 1000;
//	if (Time_s > 0)
//	{
//		stream.setf(std::ios::fixed);
//		stream << std::setw(SizeFract + 5) << std::setprecision(SizeFract);
//		stream << (static_cast<double>(sizeBytes) * 8 / Time_s) << " bps";
//	}
//	else
//	{
//		stream << "n/a";
//	}
//}

void tCameraVC0706::ChangeState(tState *state)
{
	tState* Prev = m_pState;

	m_pState = state;

	delete Prev;
}

}
