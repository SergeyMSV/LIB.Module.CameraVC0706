///////////////////////////////////////////////////////////////////////////////////////////////////
// modCameraVC0706_Attribute
//
// Created by Sergey Maslennikov
// Tel.:   +7-916-540-09-19
// E-mail: maslennikovserge@yandex.ru
//
// |   version  |    release    | Description
// |------------|---------------|---------------------------------
// |      1     |   2018 04 07  |
// |            |               |
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <libConfig.h>

namespace mod
{

union tCameraVC0706Settings
{
	struct
	{
		uint32_t VideoResolution : 2;//0 - don't change, 1 - 160x120, 2 - 320x240, 3 - 640x480
		uint32_t : 30;
	}Field;

	uint32_t Value;
};

struct tCameraVC0706Property
{
	//...

	//tCameraVC0706Property()
	//{
	//}
};

enum tCameraVC0706Error
//enum class tCameraVC0706Error// : uint8_t
{
	tCameraVC0706Error_StateInitialize_ErrTimer,
	tCameraVC0706Error_StateInitialize_BaudrateStep,
	tCameraVC0706Error_StateInitialize_GetVersion_WrongAnswer,
	tCameraVC0706Error_StateInitialize_GetVersion_WrongAnswer2,
	tCameraVC0706Error_StateInitialize_SetPort_WrongAnswer,
	//tCameraVC0706Error_StateInitialize_OsdAddChar_WrongAnswer,
	tCameraVC0706Error_StateInitialize_Read_Config_VideoResolution_WrongAnswer,
	tCameraVC0706Error_StateInitialize_Write_Config_VideoResolution_WrongAnswer,
	tCameraVC0706Error_StateInitialize_Read_Config_SerialPort_WrongAnswer,
	tCameraVC0706Error_StateInitialize_Write_Config_SerialPort_WrongAnswer,

	tCameraVC0706Error_StateConfig_ErrTimer,
	tCameraVC0706Error_StateConfig_Read_WrongAnswer,
	tCameraVC0706Error_StateConfig_Write_WrongAnswer,

	tCameraVC0706Error_StateGetImageStart_ErrTimer,
	tCameraVC0706Error_StateGetImageStart_WrongSize,
	tCameraVC0706Error_StateGetImageStart_FBufCtrl_WrongAnswer,
	tCameraVC0706Error_StateGetImageStart_GetFBufLength_WrongAnswer,

	tCameraVC0706Error_StateGetImageChunk_ErrTimer,
	tCameraVC0706Error_StateGetImageChunk_WrongSize,
	tCameraVC0706Error_StateGetImageChunk_ReadFBuf_WrongAnswer,

	tCameraVC0706Error_StateIdle_ErrTimer,

	tCameraVC0706Error_StateGetImageStop_ErrTimer,
	tCameraVC0706Error_StateGetImageStop_FBufCtrl_WrongAnswer,

	tCameraVC0706Error_StateReset_ErrTimer,
	tCameraVC0706Error_StateReset_WrongAnswer,

	//tCameraVC0706Error_StateStart_ErrTimer,

	//tCameraVC0706Error_StateRestart_ErrTimer,

	tCameraVC0706Error_Unknown = 0xFF,
};

}
