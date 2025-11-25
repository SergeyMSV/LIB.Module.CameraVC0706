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

class tCamera
{
	class tModCamera : public mod::vc0706::tCameraVC0706
	{
		class tBoardCtrl : public utils::port::serial::tPortSerialAsync<>
		{
			tModCamera* m_pObj = nullptr;

		public:
			tBoardCtrl(tModCamera* obj, boost::asio::io_context& io);
			virtual ~tBoardCtrl();

		protected:
			void OnReceived(std::vector<std::uint8_t>& data) override;
		};

		class tBoardData : public utils::port::serial::tPortSerialAsync<4096>
		{
			tModCamera* m_pObj = nullptr;

		public:
			tBoardData(tModCamera* obj, boost::asio::io_context& io);
			virtual ~tBoardData();

		protected:
			void OnReceived(std::vector<std::uint8_t>& data) override;
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

	tModCamera* m_pMod = nullptr;

	bool m_StartAuto = true;

public:
	tCamera() = delete;
	tCamera(utils::log::tLog* log, boost::asio::io_context& io);
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

	mod::vc0706::tStatus GetStatus() const;
	std::string GetLastErrorMsg() const;
};

}
