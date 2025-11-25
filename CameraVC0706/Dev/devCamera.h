///////////////////////////////////////////////////////////////////////////////////////////////////
// devCamera.h
// 2017-01-31
// Standard ISO/IEC 114882, C++=20
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <devConfig.h>
#include <devLog.h>

#include <modCameraVC0706.h>

#include <utilsPortSerial.h>

#include <fstream>
#include <future>

#include <boost/asio.hpp>

namespace dev
{

class tCam
{
	class tModCam : public mod::vc0706::tCamera
	{
		class tBoardCtrl : public utils::port::serial::tPortSerialAsync<>
		{
			tModCam* m_pObj = nullptr;

		public:
			tBoardCtrl(tModCam* obj, boost::asio::io_context& io);
			virtual ~tBoardCtrl();

		protected:
			void OnReceived(std::vector<std::uint8_t>& data) override;
		};

		class tBoardData : public utils::port::serial::tPortSerialAsync<4096>
		{
			tModCam* m_pObj = nullptr;

		public:
			tBoardData(tModCam* obj, boost::asio::io_context& io);
			virtual ~tBoardData();

		protected:
			void OnReceived(std::vector<std::uint8_t>& data) override;
		};

		tCam* m_pObj = nullptr;

		tBoardCtrl m_BoardCtrl;
		tBoardData* m_BoardData = nullptr;

		std::fstream m_File;
		std::string m_FileName;
		std::string m_FileNameTemp;

	public:
		explicit tModCam(tCam* obj);
		virtual ~tModCam();

	protected:
		mod::vc0706::tSettings GetSettings() override;

		void OnStart() override;
		void OnReady() override;
		void OnHalt() override;
		//void OnRestart() override;
		//void OnFailed(mod::vc0706::tCameraVC0706Error cerr) override;

		void OnImageReady() override;
		void OnImageChunk(std::vector<std::uint8_t>& data) override;
		void OnImageComplete() override;

		void Board_PowerSupply(bool state) override;
		void Board_Reset(bool state) override;

		bool Board_SendCtrl(const std::vector<std::uint8_t>& data) override;
		void OnReceivedCtrl(std::vector<std::uint8_t>& data);
	};

	utils::log::tLog* m_pLog = nullptr;

	boost::asio::io_context* m_pIO = nullptr;

	tModCam* m_pMod = nullptr;

	bool m_StartAuto = true;

public:
	tCam() = delete;
	tCam(utils::log::tLog* log, boost::asio::io_context& io);
	tCam(const tCam&) = delete;
	tCam(tCam&&) = delete;
	~tCam();

	tCam& operator=(const tCam&) = delete;
	tCam& operator=(tCam&&) = delete;
	void operator()();

	void Start();
	void Restart();
	void Halt();
	void Exit();

	mod::vc0706::tStatus GetStatus() const;
	std::string GetLastErrorMsg() const;
};

}
