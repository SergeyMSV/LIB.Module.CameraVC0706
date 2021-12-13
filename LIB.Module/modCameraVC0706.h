///////////////////////////////////////////////////////////////////////////////////////////////////
// modCameraVC0706.h
//
// Standard ISO/IEC 114882, C++11
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2017 01 31  |
// |            |               |
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <devConfig.h>
#include <devDataSet.h>
#include <devLog.h>

#include "modCamera.h"

#include <utilsBase.h>

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

	class tState
	{
		//std::string m_OnCmdTaskScriptIDLast;

		utils::tVectorUInt8 m_ReceivedData;
		bool m_ReceivedData_Parsed = false;

	protected:
		tCameraVC0706 * m_pObj = nullptr;

		std::chrono::time_point<tClock> m_StartTime;

		unsigned char m_Step = 0;

		tState() = delete;

	public:
		explicit tState(tCameraVC0706 *obj);
		tState(tCameraVC0706* obj, const std::string& taskScriptID);
		virtual ~tState();

		bool operator()();

		virtual bool Start() { return false; }
		virtual bool Halt();

		virtual tCameraStatus GetStatus() = 0;

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
//		virtual void OnReceivedMsg(std::vector<char>& data) { }//ChangeState

		virtual bool Go() { return true; }//ChangeState
		//virtual void OnReceived(const tPacketNMEA_Template& value);// {}//ChangeState

		void ChangeState(tState* state) { m_pObj->ChangeState(state); }
	};
///////////////////////////////////////////////////////////////////////////////////////////////////
/*	class tStateInitialize : public tState//and wait for the device as well
	{
#ifdef LIB_MODULE_CAMERA_VC0706_INITIALIZE_CONFIG
		CameraVC0706::Packet::tMemoryDevice m_Memory;
#endif//LIB_MODULE_CAMERA_VC0706_INITIALIZE_CONFIG

		utils::tTimer m_BaudrateTimer;
		unsigned char m_BaudrateStep;

		bool m_BaudrateRequired;

		bool m_ResetNeeded;

	public:
		tStateInitialize(tCameraVC0706 *obj);
		virtual ~tStateInitialize() { }

		static tState* Instance(tCameraVC0706 *obj) { return new tStateInitialize(obj); }

		virtual void Tick10ms();
		virtual void Tick100ms();

		virtual void Control();

	protected:
		virtual void OnReceivedMsg(std::vector<char>& data);
	};
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef LIB_MODULE_CAMERA_VC0706_CONFIG
	class tStateConfig : public tState//and wait for the device as well
	{
		CameraVC0706::Packet::tMemoryDevice m_Memory;

		int m_Address;
		int m_SizeToRead;
		std::vector<char> m_Data;

	public:
		tStateConfig(tCameraVC0706 *obj, CameraVC0706::Packet::tMemoryDevice memory, int address, int size);
		tStateConfig(tCameraVC0706 *obj, CameraVC0706::Packet::tMemoryDevice memory, int address, std::vector<char>& data);
		virtual ~tStateConfig() { }

		static tState* Instance(tCameraVC0706 *obj, CameraVC0706::Packet::tMemoryDevice memory, int address, int size) { return new tStateConfig(obj, memory, address, size); }
		static tState* Instance(tCameraVC0706 *obj, CameraVC0706::Packet::tMemoryDevice memory, int address, std::vector<char>& data) { return new tStateConfig(obj, memory, address, data); }

		virtual void Control();

	protected:
		virtual void OnReceivedMsg(std::vector<char>& data);
	};
#endif//LIB_MODULE_CAMERA_VC0706_CONFIG
///////////////////////////////////////////////////////////////////////////////////////////////////
	class tStateIdle : public tState
	{
		enum tTask
		{
			tTask_None,
			tTask_GetImageReady,
			tTask_GetConfig,
			tTask_SetConfig,
		};

		tTask m_Task;

		CameraVC0706::Packet::tMemoryDevice m_ConfigMemory;
		int m_ConfigAddress;
		int m_ConfigSize;
		std::vector<char> m_ConfigData;

		bool m_CheckConnect;

	public:
		tStateIdle(tCameraVC0706 *obj);
		virtual ~tStateIdle() { }

		static tState* Instance(tCameraVC0706 *obj) { return new tStateIdle(obj); }

		virtual void Control();

		virtual bool IsReady() { return true; }

		virtual bool GetImageReady();

#ifdef LIB_MODULE_CAMERA_VC0706_CONFIG
		virtual bool GetConfig(CameraVC0706::Packet::tMemoryDevice memory, int address, int size);
		virtual bool SetConfig(CameraVC0706::Packet::tMemoryDevice memory, int address, std::vector<char>& data);
#endif//LIB_MODULE_CAMERA_VC0706_CONFIG
		
	protected:
		virtual void OnReceivedMsg(std::vector<char>& data);
	};
///////////////////////////////////////////////////////////////////////////////////////////////////
	class tStateGetImageStart : public tState
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
	class tStateHalt : public tState
	{
		const bool m_Error = false;

	public:
		tStateHalt(tCameraVC0706* obj, const std::string& value);
		tStateHalt(tCameraVC0706* obj, const std::string& value, bool error);

		bool Start() override { return false; }
		bool Halt() override { return true; }

		tCameraStatus GetStatus() override { return tCameraStatus::Halted; }

	protected:
		bool Go() override;
	};

	class tStateStart :public tState
	{
		bool m_NextState_Stop = false;

	public:
		tStateStart(tCameraVC0706* obj);

		tCameraStatus GetStatus() override { return tCameraStatus::Init; }

	protected:
		//virtual void OnReceivedMsg(std::vector<char>& data); - get rid of it

		bool Go() override;
		//virtual void OnReceived(const tPacketNMEA_Template& value);// {}//ChangeState
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

	tCameraStatus GetStatus() const;
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
	virtual void Board_PowerSupply(bool state) = 0;
	virtual void Board_Reset(bool state) = 0;

	//virtual void Board_SetSerialPort(int baudrate) { }

	virtual bool Board_Send(const utils::tVectorUInt8& data) = 0;
	void Board_OnReceived(utils::tVectorUInt8& data);
/*
	virtual void OnChanged(tCameraVC0706Property value) { }

	virtual void OnStart() = 0;
	virtual void OnReady() = 0;
	virtual void OnHalt() = 0;
	virtual void OnRestart() = 0;
	virtual void OnFailed(tCameraVC0706Error cerr) = 0;

	virtual void OnImageReady() = 0;
	virtual void OnImageChunk(std::vector<char>& data) = 0;
	virtual void OnImageComplete() = 0;

#ifdef LIB_MODULE_CAMERA_VC0706_CONFIG
	virtual void OnGetConfig(std::vector<char>& data) = 0;
	virtual void OnSetConfig() = 0;
#endif//LIB_MODULE_CAMERA_VC0706_CONFIG
*/
private:
	bool IsReceivedData() const;
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
