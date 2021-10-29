///////////////////////////////////////////////////////////////////////////////////////////////////
// tCamera
//
// Created by Sergey Maslennikov
// Tel.:   +7-916-540-09-19
// E-mail: maslennikovserge@yandex.ru
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2017 01 31  |
// |            |               |
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef DEV_MODULE_CAMERA_H
#define DEV_MODULE_CAMERA_H
/*
#include <devConfig.h>

#include <devLog.h>

#include <modCameraVC0706.h>

#include <board_ModCamera.h>

namespace dev
{
///////////////////////////////////////////////////////////////////////////////////////////////////
class tCamera
{
///////////////////////////////////////////////////////////////////////////////////////////////////
	class tModCamera : public mod::tCameraVC0706
	{
///////////////////////////////////////////////////////////////////////////////////////////////////
		class tBoard : public board::tBoard_ModCamera
		{
			tModCamera* p_obj;

		public:
			tBoard(tModCamera* obj);
			virtual ~tBoard();

		protected:
			virtual void OnReceived(std::vector<char>& data);
		};
///////////////////////////////////////////////////////////////////////////////////////////////////
		tCamera* p_obj;

		tBoard* m_Board;

		//tModCamera() { }

	public:
		tModCamera(tCamera* obj, mod::tCameraVC0706Settings settings);
		virtual ~tModCamera();

		virtual void Control();

	protected:
		virtual void Board_PowerSupply(bool state);
		virtual void Board_Reset(bool state);

		virtual void Board_SetSerialPort(int baudrate);

		virtual bool Board_Send(std::vector<char>& data);

		void OnReceived(std::vector<char>& data);

		virtual void OnStart();
		virtual void OnReady();
		virtual void OnHalt();
		virtual void OnRestart();
		virtual void OnFailed(mod::tCameraVC0706Error cerr);

		virtual void OnImageReady();
		virtual void OnImageChunk(std::vector<char>& data);
		virtual void OnImageComplete();

#ifdef LIB_MODULE_CAMERA_VC0706_CONFIG
		virtual void OnGetConfig(std::vector<char>& data);
		virtual void OnSetConfig();
#endif//LIB_MODULE_CAMERA_VC0706_CONFIG
	};
///////////////////////////////////////////////////////////////////////////////////////////////////
	friend class tModCamera;

	utils::log::tLog *p_log;

	tModCamera *m_ModCamera;

public:
	tCamera();

	~tCamera();

	void Tick10ms();

	void Control();

	void Start();
	void Halt();

	//TEST
	bool GetImageReady() { return m_ModCamera->GetImageReady(); }//[srg]2017-02-02 TEST
	bool GetImageChunk(int chunkSize) { return m_ModCamera->GetImageChunk(chunkSize); }//[srg]2017-02-03 TEST

#ifdef LIB_MODULE_CAMERA_VC0706_CONFIG
	bool GetConfig(mod::CameraVC0706::Packet::tMemoryDevice memory, int address, int size) { return m_ModCamera->GetConfig(memory, address, size); }
	bool SetConfig(mod::CameraVC0706::Packet::tMemoryDevice memory, int address, std::vector<char>& data) { return m_ModCamera->SetConfig(memory, address, data); }
#endif//LIB_MODULE_CAMERA_VC0706_CONFIG
};
///////////////////////////////////////////////////////////////////////////////////////////////////
}
*/
#endif//DEV_MODULE_CAMERA_H
