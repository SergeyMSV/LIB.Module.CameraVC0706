#include "modCameraVC0706.h"
/*
namespace mod
{

tCameraVC0706::tStateGetImageIdle::tStateGetImageIdle(tCameraVC0706 *obj, char imageFrame, int fBufSize)
	:tState(obj)
{
#ifdef LIB_MODULE_LOG
	p_obj->p_log->WriteLine("tStateGetImageIdle");
#endif//LIB_MODULE_LOG

	m_ErrTimer.SetValue(utils::tTime(2, 0));//[#]

	m_ImageFrame = imageFrame;

	m_FBufSize = fBufSize;

	m_ChunkStartAddress = 0;
}

tCameraVC0706::tStateGetImageIdle::tStateGetImageIdle(tCameraVC0706 *obj, char imageFrame, int fBufSize, int chunkStartAddress)
	:tState(obj)
{
#ifdef LIB_MODULE_LOG
	p_obj->p_log->WriteLine("tStateGetImageIdle");
#endif//LIB_MODULE_LOG

	m_ErrTimer.SetValue(utils::tTime(2, 0));//[#]

	m_ImageFrame = imageFrame;

	m_FBufSize = fBufSize;

	m_ChunkStartAddress = chunkStartAddress;
}


void tCameraVC0706::tStateGetImageIdle::Control()
{
	if (!m_ErrTimer.GetState())
	{
		ChangeState(tStateGetImageStop::Instance(p_obj));
		return;
	}
}

bool tCameraVC0706::tStateGetImageIdle::GetImageChunk(int chunkSize)
{
	ChangeState(tStateGetImageChunk::Instance(p_obj, m_ImageFrame, m_FBufSize, m_ChunkStartAddress, chunkSize));
	return true;
}

void tCameraVC0706::tStateGetImageIdle::OnReceivedMsg(std::vector<char>& data)
{

}

}*/