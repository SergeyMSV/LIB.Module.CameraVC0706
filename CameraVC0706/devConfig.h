#pragma once

#include <utilsBase.h>

#include <atomic>
#include <mutex>
#include <string>

struct tDataSetMainControl
{
	enum class tStateCAM
	{
		Nothing,
		Halt,
		Start,
		Restart,
		Exit,
		UserTaskScriptStart,
	};

	std::atomic<tStateCAM> Thread_CAM_State{ tStateCAM::Halt };

	mutable std::mutex Thread_CAM_State_UserTaskScriptIDMtx;
	std::string Thread_CAM_State_UserTaskScriptID;
};