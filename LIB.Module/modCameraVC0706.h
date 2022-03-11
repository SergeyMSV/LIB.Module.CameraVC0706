///////////////////////////////////////////////////////////////////////////////////////////////////
// modCameraVC0706.h
// 2017-01-31
// Standard ISO/IEC 114882, C++20
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <devConfig.h>
#include <devDataSet.h>
#include <devLog.h>

#include <utilsBase.h>
#include <utilsChrono.h>
#include <utilsPacketCameraVC0706.h>

#include <chrono>
#include <queue>
//#include <source_location> GCC-11 is needed, now used gcc-9.3.0 from Ubuntu 20.04
#include <sstream>
#include <thread>

namespace mod
{

struct tCameraVC0706Settings
{
	utils::packet_CameraVC0706::tResolution Resolution = utils::packet_CameraVC0706::tResolution::VR640x480;
	std::uint32_t CheckPresencePeriod_ms = 0;
	std::uint32_t ImagePeriod_ms = 0;
	std::uint32_t ImageChunkSize = 0;
	std::uint32_t ImageChunkDelayFromReq_us = 0;

private:
	std::uint32_t PortCtrlBR = 0;
	std::uint32_t PortDataBR = 0;//if it is equal to 0 - image data is transferred through PortCtrl

public:
	void SetPortCtrlBR(std::uint32_t value) { PortCtrlBR = value; }
	void SetPortDataBR(std::uint32_t value) { PortDataBR = value; }
	std::uint32_t GetPortDataBR() const { return PortDataBR; }
	std::uint32_t GetImageDataBR() const
	{
		return PortDataBR != 0 ? PortDataBR : PortCtrlBR;
	}
};

class tCameraVC0706
{
	using tDevStatus = utils::tDevStatus;

	class tState
	{
	protected:
		tCameraVC0706* m_pObj = nullptr;

		utils::tVectorUInt8 m_ReceivedData;

		tState() = delete;

	public:
		explicit tState(tCameraVC0706* obj);
		tState(tCameraVC0706* obj, const std::string& taskScriptID);
		virtual ~tState();

		virtual void operator()() = 0;

		virtual bool Start() { return false; }
		virtual bool Halt();

		virtual tDevStatus GetStatus() = 0;

	protected:
		bool WaitForReceivedData(std::uint32_t wait_ms);

		template<typename T>
		bool HandleCmd(const utils::packet_CameraVC0706::tPacketCmd& packet, utils::packet_CameraVC0706::tMsgStatus& responseStatus, T& response, std::uint32_t wait_ms)
		{
			using namespace utils::packet_CameraVC0706;

			responseStatus = tMsgStatus::None;

			m_ReceivedData.clear();
			m_pObj->Board_SendCtrl(packet.ToVector());

			return HandleRsp(packet.GetMsgId(), responseStatus, response, wait_ms);
		}

		template<typename T>
		bool HandleCmd(const utils::packet_CameraVC0706::tPacketCmd& packet, utils::packet_CameraVC0706::tMsgStatus& responseStatus, T& response, std::uint32_t wait_ms, int repeatQty)
		{
			for (int i = 0; i < repeatQty; ++i)
			{
				if (HandleCmd(packet, responseStatus, response, wait_ms))
					return true;
			}
			return false;
		}

		bool HandleCmd(const utils::packet_CameraVC0706::tPacketCmd& packet, utils::packet_CameraVC0706::tMsgStatus& responseStatus, std::uint32_t wait_ms, int repeatQty);

	private:
		template<typename T>
		bool HandleRsp(const utils::packet_CameraVC0706::tMsgId msgId, utils::packet_CameraVC0706::tMsgStatus& responseStatus, T& response, std::uint32_t wait_ms)
		{
			using namespace utils::packet_CameraVC0706;

			const utils::tTimePoint TimeStart = utils::tClock::now();

			while (true)
			{
				const auto TimeElapsed = utils::GetDuration<utils::ttime_ms>(TimeStart, utils::tClock::now());
				if (wait_ms < TimeElapsed)
					return false;

				const std::uint32_t TimeLeft = wait_ms - TimeElapsed;

				if (!WaitForReceivedData(TimeLeft))
					return false;

				if (!m_pObj->IsControlOperation())
					return true;

				tPacketRet Rsp;
				if (tPacketRet::Find(m_ReceivedData, Rsp) > 0 && Rsp.GetMsgId() == msgId)
				{
					responseStatus = Rsp.GetMsgStatus();
					if (responseStatus != tMsgStatus::None)
						return true;

					tPacketRet::Parse(Rsp, response);
					return true;
				}

				if (m_ReceivedData.size() > ContainerCmdSize + ContainerPayloadSizeMax)
					return false;
			}
		}

	public:
		bool HandleRsp(const utils::packet_CameraVC0706::tMsgId msgId, utils::packet_CameraVC0706::tMsgStatus& responseStatus, std::uint32_t wait_ms);

	protected:
		bool IsChangeState_ToStop();//ChangeState
		void ChangeState(tState* state) { m_pObj->ChangeState(state); }
	};

	class tStateOperation : public tState
	{
	public:
		explicit tStateOperation(tCameraVC0706 *obj);

		void operator()() override;

		tDevStatus GetStatus() override { return tDevStatus::Operation; }
	};

	class tStateOperationImage : public tState
	{
		const tCameraVC0706Settings m_Settings;

		bool m_ImageReady = false;

	public:
		explicit tStateOperationImage(tCameraVC0706* obj);
		virtual ~tStateOperationImage();

		void operator()() override;

		tDevStatus GetStatus() override { return tDevStatus::Operation; }
	};

	class tStateError :public tState
	{
	public:
		tStateError(tCameraVC0706* obj, const std::string& value/*, const std::source_location loc = std::source_location::current()*/);

		void operator()() override;

		bool Halt() override { return false; }

		tDevStatus GetStatus() override { return tDevStatus::Error; }
	};

	class tStateHalt : public tState
	{
		const bool m_Error = false;
		bool m_Off = false;

	public:
		tStateHalt(tCameraVC0706* obj, const std::string& value);
		tStateHalt(tCameraVC0706* obj, const std::string& value, bool error);

		void operator()() override;

		bool Start() override { return false; }
		bool Halt() override { return true; }

		tDevStatus GetStatus() override { return tDevStatus::Halted; }
	};

	class tStateStart :public tState
	{
	public:
		explicit tStateStart(tCameraVC0706* obj);

		void operator()() override;

		tDevStatus GetStatus() override { return tDevStatus::Init; }
	};

	class tStateStop :public tState
	{
	public:
		explicit tStateStop(tCameraVC0706* obj);

		void operator()() override;

		bool Start() override { return false; }
		bool Halt() override { return true; }

		tDevStatus GetStatus() override { return tDevStatus::Deinit; }
	};

	utils::tLog* m_pLog = nullptr;

	tState* m_pState = nullptr;

	std::atomic_bool m_Control_Operation{ false };
	std::atomic_bool m_Control_Restart{ false };
	std::atomic_bool m_Control_Exit{ false };
	std::atomic_bool m_Control_ExitOnError{ false };

	std::atomic_bool m_Control_OnExit{ false };

	mutable std::mutex m_MtxReceivedData;
	std::queue<utils::tVectorUInt8> m_ReceivedData;

	const std::uint8_t m_SN = 0;

	utils::tTimePoint m_CheckLastTime{};//period for checking presence of the camera
	utils::tTimePoint m_ImageLastTime{};

	std::string m_LastErrorMsg;

public:
	tCameraVC0706() = delete;
	explicit tCameraVC0706(utils::tLog* log);
	tCameraVC0706(const tCameraVC0706&) = delete;
	tCameraVC0706(tCameraVC0706&&) = delete;
	virtual ~tCameraVC0706() {}
	
	void operator()();
	
	void Start();
	void Start(bool exitOnError);
	void Restart();
	void Halt();
	void Exit();

	tDevStatus GetStatus() const;
	std::string GetLastErrorMsg() const;

protected:
	virtual tCameraVC0706Settings GetSettings() = 0;

	//virtual void OnChanged(tCameraVC0706Property value) { }

	virtual void OnStart() = 0;
	virtual void OnReady() = 0;
	virtual void OnHalt() = 0;
	//virtual void OnRestart() = 0;
	//virtual void OnFailed(tCameraVC0706Error cerr) = 0;

	virtual void OnImageReady() = 0;
	virtual void OnImageChunk(utils::tVectorUInt8& data) = 0;
	virtual void OnImageComplete() = 0;

	virtual void Board_PowerSupply(bool state) = 0;
	virtual void Board_Reset(bool state) = 0;

	virtual bool Board_SendCtrl(const utils::tVectorUInt8& data) = 0;
	void Board_OnReceivedCtrl(utils::tVectorUInt8& data);

private:
	bool IsReceivedData() const;
	utils::tVectorUInt8 GetReceivedDataChunk();
	bool IsControlOperation() { return m_Control_Operation && !m_Control_Restart; }
	//bool IsControlStop() { return !m_Control_Operation && m_Control_Restart; }
	bool IsControlRestart() { return m_Control_Restart; }
	bool IsControlHalt() { return !m_Control_Operation; }

	void ClearReceivedData();

	void ChangeState(tState *state);
};

}
