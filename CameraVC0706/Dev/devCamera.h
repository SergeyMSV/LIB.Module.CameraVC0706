///////////////////////////////////////////////////////////////////////////////////////////////////
// devCamera.h
// 2017-01-31
// Standard ISO/IEC 114882, C++=20
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <devConfig.h>
#include <devLog.h>

#include <modCameraVC0706.h>

#include <utilsSerialPort.h>

#include <fstream>
#include <future>

#include <boost/asio.hpp>

namespace dev
{

class tCamera
{
	class tModCamera : public mod::tCameraVC0706
	{
		class tBoardCtrl : public utils::serial_port::tSerialPort<>
		{
			tModCamera* m_pObj = nullptr;

		public:
			tBoardCtrl(tModCamera* obj, boost::asio::io_context& io);
			virtual ~tBoardCtrl();

		protected:
			void OnReceived(utils::tVectorUInt8& data) override;
		};

		class tBoardData : public utils::serial_port::tSerialPort<4096>
		{
			tModCamera* m_pObj = nullptr;

		public:
			tBoardData(tModCamera* obj, boost::asio::io_context& io);
			virtual ~tBoardData();

		protected:
			void OnReceived(utils::tVectorUInt8& data) override;
		};

		tCamera* m_pObj = nullptr;

		tBoardCtrl m_BoardCtrl;
		tBoardData* m_BoardData = nullptr;

		std::fstream m_File;
		std::string m_FileName;
		std::string m_FileNameTemp;

	public:
		explicit tModCamera(tCamera* obj);
		virtual ~tModCamera();

	protected:
		mod::tCameraVC0706Settings GetSettings() override;

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

		bool Board_SendCtrl(const utils::tVectorUInt8& data) override;
		void OnReceivedCtrl(utils::tVectorUInt8& data);

		void OnReceivedData(utils::tVectorUInt8& data);
	};

	utils::tLog* m_pLog = nullptr;

	boost::asio::io_context* m_pIO = nullptr;

	tModCamera* m_pMod = nullptr;

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

	utils::tDevStatus GetStatus() const;
	std::string GetLastErrorMsg() const;
};

}
