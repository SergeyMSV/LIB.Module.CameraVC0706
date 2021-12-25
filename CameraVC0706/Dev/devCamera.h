///////////////////////////////////////////////////////////////////////////////////////////////////
// devCamera.h
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
#include <devLog.h>

#include <modCameraVC0706.h>

#include <utilsSerialPort.h>

#include <future>

#include <boost/asio.hpp>

namespace dev
{

class tCamera
{
///////////////////////////////////////////////////////////////////////////////////////////////////
	class tModCamera : public mod::tCameraVC0706
	{
		class tBoard : public utils::serial_port::tSerialPort<>
		{
			tModCamera* m_pObj = nullptr;

		public:
			tBoard(tModCamera* obj, boost::asio::io_context& io);
			virtual ~tBoard();

		protected:
			void OnReceived(utils::tVectorUInt8& data) override;
		};
///////////////////////////////////////////////////////////////////////////////////////////////////
		tCamera* m_pObj = nullptr;

		tBoard m_Board;

	public:
		explicit tModCamera(tCamera* obj);
		virtual ~tModCamera();

	protected:
		void OnStart() override;
		void OnReady() override;
		void OnHalt() override;
		//void OnRestart() override;
		//void OnFailed(mod::tCameraVC0706Error cerr) override;

		void OnImageReady() override;
		void OnImageChunk(utils::tVectorUInt8& data) override;
		void OnImageComplete() override;

		void Board_PowerSupply(bool state) override;
		void Board_Reset(bool state) override;

		bool Board_Send(const utils::tVectorUInt8& data) override;
		void OnReceived(utils::tVectorUInt8& data);
	};
///////////////////////////////////////////////////////////////////////////////////////////////////
	/*friend class tModCamera;

	utils::log::tLog *p_log;

	tModCamera *m_ModCamera;
*/

	utils::tLog* m_pLog = nullptr;

	boost::asio::io_context* m_pIO = nullptr;

	tModCamera* m_pModFSMachine = nullptr;

	bool m_StartAuto = true;

public:
	tCamera() = delete;
	tCamera(utils::tLog* log, boost::asio::io_context& io);
	tCamera(const tCamera&) = delete;
	tCamera(tCamera&&) = delete;
	~tCamera();

	tCamera& operator=(const tCamera&) = delete;
	tCamera& operator=(tCamera&&) = delete;
	void operator()();

	void Start();
	void Restart();
	void Halt();
	void Exit();

	//bool StartUserTaskScript(const std::string& taskScriptID);

	utils::tDevStatus GetStatus() const;
	std::string GetLastErrorMsg() const;

//	tCamera();
//	~tCamera();

/*	void Tick10ms();

	void Control();

	void Start();
	void Halt();

	//TEST
	bool GetImageReady() { return m_ModCamera->GetImageReady(); }//[srg]2017-02-02 TEST
	bool GetImageChunk(int chunkSize) { return m_ModCamera->GetImageChunk(chunkSize); }//[srg]2017-02-03 TEST

#ifdef LIB_MODULE_CAMERA_VC0706_CONFIG
	bool GetConfig(mod::CameraVC0706::Packet::tMemoryDevice memory, int address, int size) { return m_ModCamera->GetConfig(memory, address, size); }
	bool SetConfig(mod::CameraVC0706::Packet::tMemoryDevice memory, int address, std::vector<char>& data) { return m_ModCamera->SetConfig(memory, address, data); }
#endif//LIB_MODULE_CAMERA_VC0706_CONFIG*/
};

}
