///////////////////////////////////////////////////////////////////////////////////////////////////
// modCameraVC0706.h
// 2017-01-31
// Standard ISO/IEC 114882, C++20
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <devConfig.h>
#include <devDataSet.h>
#include <devLog.h>

#include "modCamera.h"

#include <utilsBase.h>
#include <utilsPacketCameraVC0706.h>

//#include <atomic>
#include <chrono>
//#include <mutex>
#include <queue>
#include <sstream>
//#include <thread>

/*
//"ProjectName.[MAIN].[ATTRIBUTE]"
#define LIB_MODULE_CAMERA_VC0706_VERSION "modCameraVC0706.5.1"

#include <libConfig.h>

#include "modCameraVC0706_Attribute.h"
#include "modCameraVC0706_Packet.h"

#include <utilsLog.h>

#include <string>
#include <vector>
*/
namespace mod
{

class tCameraVC0706
{
	using tClock = std::chrono::high_resolution_clock;
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

		virtual bool operator()();

		virtual bool Start() { return false; }
		virtual bool Halt();

		virtual tDevStatus GetStatus() = 0;

		//		virtual bool GetImageReady() { return false; }//[srg]2017-02-02 Start
		//		virtual bool GetImageChunk(int chunkSize) { return false; }//[srg]2017-02-03
		//
		////#ifdef LIB_MODULE_CAMERA_VC0706_CONFIG
		////		virtual bool GetConfig(CameraVC0706::Packet::tMemoryDevice memory, int address, int size) { return false; }
		////		virtual bool SetConfig(CameraVC0706::Packet::tMemoryDevice memory, int address, std::vector<char>& data) { return false; }
		////#endif//LIB_MODULE_CAMERA_VC0706_CONFIG
		//
		//		virtual void Receive(std::vector<char>& data);
		//
	protected:
		bool WaitForReceivedData(std::uint32_t wait_ms);

		template<typename T>
		bool HandleCmd(const utils::packet_CameraVC0706::tPacketCmd& packet, utils::packet_CameraVC0706::tMsgStatus& responseStatus, T& response, std::uint32_t wait_ms)
		{
			using namespace utils::packet_CameraVC0706;

			responseStatus = tMsgStatus::None;

			m_ReceivedData.clear();
			m_pObj->Board_Send(packet.ToVector());
			while (true)
			{
				if (!WaitForReceivedData(wait_ms))//[TBD] calculate according to BR and waiting for camera response
					return false;

				tPacketRet Rsp;
				if (tPacketRet::Find(m_ReceivedData, Rsp) > 0 && Rsp.GetMsgId() == packet.GetMsgId())
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

		bool HandleCmd(const utils::packet_CameraVC0706::tPacketCmd& packet, utils::packet_CameraVC0706::tMsgStatus& responseStatus, std::uint32_t wait_ms);

	//private: [TBD] -- acute - is to be uncommented
		bool HandleRsp(const utils::packet_CameraVC0706::tMsgId msgId, utils::packet_CameraVC0706::tMsgStatus& responseStatus, std::uint32_t wait_ms);

	protected:
		virtual bool Go() { return true; }//ChangeState

		void ChangeState(tState* state) { m_pObj->ChangeState(state); }
	};
///////////////////////////////////////////////////////////////////////////////////////////////////
	class tStateOperation : public tState
	{
		std::chrono::time_point<tClock> m_CheckLastTime = tClock::now();

	public:
		explicit tStateOperation(tCameraVC0706 *obj);

		static tState* Instance(tCameraVC0706 *obj) { return new tStateOperation(obj); }

		tDevStatus GetStatus() override { return tDevStatus::Operation; }

	protected:
		bool Go() override;

	private:
		bool GetImage();
	};
///////////////////////////////////////////////////////////////////////////////////////////////////
/*	class tStateGetImageStart : public tState
	{
		char m_ImageFrame;
		int m_FBufSize;

	public:
		tStateGetImageStart(tCameraVC0706 *obj);
		virtual ~tStateGetImageStart() { }

		static tState* Instance(tCameraVC0706 *obj) { return new tStateGetImageStart(obj); }

		virtual void Control();

	protected:
		virtual void OnReceivedMsg(std::vector<char>& data);
	};
///////////////////////////////////////////////////////////////////////////////////////////////////
	class tStateGetImageIdle : public tState
	{
		char m_ImageFrame;
		int m_FBufSize;

		int m_ChunkStartAddress;

	public:
		tStateGetImageIdle(tCameraVC0706 *obj, char imageFrame, int fBufSize);
		tStateGetImageIdle(tCameraVC0706 *obj, char imageFrame, int fBufSize, int chunkStartAddress);
		virtual ~tStateGetImageIdle() { }

		static tState* Instance(tCameraVC0706 *obj, char imageFrame, int fBufSize) { return new tStateGetImageIdle(obj, imageFrame, fBufSize); }
		static tState* Instance(tCameraVC0706 *obj, char imageFrame, int fBufSize, int chunkStartAddress) { return new tStateGetImageIdle(obj, imageFrame, fBufSize, chunkStartAddress); }

		virtual void Control();

		virtual bool GetImageChunk(int chunkSize);

	protected:
		virtual void OnReceivedMsg(std::vector<char>& data);
	};
///////////////////////////////////////////////////////////////////////////////////////////////////
	class tStateGetImageChunk : public tState
	{
		char m_ImageFrame;
		int m_FBufSize;

		int m_ChunkStartAddress;
		int m_ChunkSize;

		unsigned char m_ChunkTransferStep;

		std::vector<char> m_Chunk;
		unsigned int m_ChunkIndex;

	public:
		tStateGetImageChunk(tCameraVC0706 *obj, char imageFrame, int fBufSize, int chunkStartAddress, int chunkSize);
		virtual ~tStateGetImageChunk() { }

		static tState* Instance(tCameraVC0706 *obj, char imageFrame, int fBufSize, int chunkStartAddress, int chunkSize)
		{ return new tStateGetImageChunk(obj, imageFrame, fBufSize, chunkStartAddress, chunkSize); }

		virtual void Control();

		virtual void Receive(std::vector<char>& data);

	protected:
		virtual void OnReceivedMsg(std::vector<char>& data);

	private:
		void HandleReceived(std::vector<char>& data);
	};
///////////////////////////////////////////////////////////////////////////////////////////////////
	class tStateGetImageStop : public tState
	{
		char m_ImageFrame;
		int m_FBufSize;

	public:
		tStateGetImageStop(tCameraVC0706 *obj);
		virtual ~tStateGetImageStop() { }

		static tState* Instance(tCameraVC0706 *obj) { return new tStateGetImageStop(obj); }

		virtual void Control();

	protected:
		virtual void OnReceivedMsg(std::vector<char>& data);
	};
///////////////////////////////////////////////////////////////////////////////////////////////////
	class tStateReset : public tState//and wait for the device as well
	{
	public:
		tStateReset(tCameraVC0706 *obj);
		virtual ~tStateReset() { }

		static tState* Instance(tCameraVC0706 *obj) { return new tStateReset(obj); }

		virtual void Control();

	protected:
		virtual void OnReceivedMsg(std::vector<char>& data);
	};
///////////////////////////////////////////////////////////////////////////////////////////////////
	class tStateStart : public tState//and wait for the device as well
	{
	public:
		tStateStart(tCameraVC0706 *obj);
		virtual ~tStateStart() { }

		static tState* Instance(tCameraVC0706 *obj) { return new tStateStart(obj); }

		virtual void Control();

	protected:
		virtual void OnReceivedMsg(std::vector<char>& data);
	};
///////////////////////////////////////////////////////////////////////////////////////////////////
	class tStateRestart : public tState//Restarts the device with PWR
	{
	public:
		tStateRestart(tCameraVC0706 *obj);
		virtual ~tStateRestart() { }

		static tState* Instance(tCameraVC0706 *obj) { return new tStateRestart(obj); }

		virtual void Control();

	protected:
		virtual void OnReceivedMsg(std::vector<char>& data);
	};*/
///////////////////////////////////////////////////////////////////////////////////////////////////
	class tStateError :public tState
	{
	public:
		tStateError(tCameraVC0706* obj, const std::string& value);

		bool operator()() override;

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

		bool operator()() override;

		bool Start() override { return false; }
		bool Halt() override { return true; }

		tDevStatus GetStatus() override { return tDevStatus::Halted; }
	};

	class tStateStart :public tState
	{
	public:
		explicit tStateStart(tCameraVC0706* obj);

		tDevStatus GetStatus() override { return tDevStatus::Init; }

	protected:
		bool Go() override;
	};

	class tStateStop :public tState
	{
	public:
		explicit tStateStop(tCameraVC0706* obj);

		bool Start() override { return false; }
		bool Halt() override { return true; }

		tDevStatus GetStatus() override { return tDevStatus::Deinit; }

	protected:
		bool Go() override;
	};

/*	friend class tState;

	utils::log::tLog* p_log;

	tState *p_State;

	tCameraVC0706Settings m_Settings;
*/
	utils::tLog* m_pLog = nullptr;

	tState* m_pState = nullptr;

	std::atomic_bool m_Control_Operation{ false };
	std::atomic_bool m_Control_Restart{ false };
	std::atomic_bool m_Control_Exit{ false };
	std::atomic_bool m_Control_ExitOnError{ false };

	mutable std::mutex m_MtxReceivedData;
	std::queue<utils::tVectorUInt8> m_ReceivedData;

	const std::uint8_t m_SN = 0;

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

/*
	tCameraVC0706() { }

public:
	tCameraVC0706(utils::log::tLog *log, tCameraVC0706Settings settings);
	virtual ~tCameraVC0706();

	void Tick10ms();
	void Tick100ms();

	void Control();

	void Start();
	void Halt();

	bool IsReady();

	bool GetImageReady();
	bool GetImageChunk(int chunkSize);

#ifdef LIB_MODULE_CAMERA_VC0706_CONFIG
	bool GetConfig(CameraVC0706::Packet::tMemoryDevice memory, int address, int size);
	bool SetConfig(CameraVC0706::Packet::tMemoryDevice memory, int address, std::vector<char>& data);
#endif//LIB_MODULE_CAMERA_VC0706_CONFIG
*/
protected:
	/*
	virtual void OnChanged(tCameraVC0706Property value) { }
*/
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

	virtual bool Board_Send(const utils::tVectorUInt8& data) = 0;
	void Board_OnReceived(utils::tVectorUInt8& data);

private:
	//bool WaitForReceivedData(std::uint32_t wait_ms) const;
	bool IsReceivedData() const;//[TBD] deprecated might be
	utils::tVectorUInt8 GetReceivedDataChunk();
	bool IsControlOperation() { return m_Control_Operation && !m_Control_Restart; }
	//bool IsControlStop() { return !m_Control_Operation && m_Control_Restart; }
	bool IsControlRestart() { return m_Control_Restart; }
	bool IsControlHalt() { return !m_Control_Operation; }

	void ClearReceivedData();

//	void SetStrTimePeriod(std::stringstream& stream, const std::chrono::time_point<tClock>& timePoint) const;
//	void SetStrBaudrate(std::stringstream& stream, const std::chrono::time_point<tClock>& timePoint, std::size_t sizeBytes) const;

	void ChangeState(tState *state);
};

}
