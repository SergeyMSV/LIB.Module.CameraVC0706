#include "modCameraVC0706.h"

namespace mod
{

tCameraVC0706::tState::tState(tCameraVC0706* obj)
	:m_pObj(obj)
{
	m_pObj->ClearReceivedData();
}

tCameraVC0706::tState::tState(tCameraVC0706* obj, const std::string& taskScriptID)
	:tState(obj)
{
	//m_TaskScript = m_pObj->GetTaskScript(taskScriptID, false);
}

tCameraVC0706::tState::~tState()
{
	//delete m_pCmd;
}

bool tCameraVC0706::tState::operator()()
{
/*	if (m_pCmd && (*m_pCmd)())
		return true;

	if (m_pObj->IsReceivedData())
	{
		const utils::tVectorUInt8 DataChunk = m_pObj->GetReceivedDataChunk();

		if (DataChunk.size() > 0)
		{
			m_pObj->m_pLog->WriteHex(true, utils::tLogColour::LightRed, "Received", DataChunk);

			m_ReceivedData.insert(m_ReceivedData.end(), DataChunk.cbegin(), DataChunk.cend());//C++14

			m_ReceivedData_Parsed = false;
		}		
	}

	if (!m_ReceivedData_Parsed)
	{
		tPacketNMEA_Template Packet;

		const std::size_t PacketSize = tPacketNMEA_Template::Find(m_ReceivedData, Packet);

		//if something is parsed it's needed to try to parse the rest data because there can be one more packet
		m_ReceivedData_Parsed = PacketSize == 0;

		if (PacketSize)
		{
			if (m_pCmd && m_pCmd->OnReceived(Packet))//ChangeState
				return true;

			OnReceived(Packet);//ChangeState
			return true;
		}
	}

	TaskScript();*/

	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	return Go();//ChangeState
}

bool tCameraVC0706::tState::Halt()
{
	//ChangeState(new tStateStop(m_pObj, "halt"));
	return true;
}

//void tCameraVC0706::tState::TaskScript()
//{
//	if (m_pCmd == nullptr && !m_TaskScript.empty())
//	{
//		const tGnssTaskScriptCmd* Ptr = m_TaskScript.front().get();
//
//		if (Ptr != nullptr)
//		{
//			switch (Ptr->GetID())
//			{
//			case tGnssTaskScriptCmd::tID::GPI:
//			{
//				m_pCmd = new tCmdGPI(this, std::move(m_TaskScript.front()));
//				break;
//			}
//			case tGnssTaskScriptCmd::tID::GPO:
//			{
//				m_pCmd = new tCmdGPO(this, std::move(m_TaskScript.front()));
//				break;
//			}
//			case tGnssTaskScriptCmd::tID::REQ:
//			{
//				m_pCmd = new tCmdREQ(this, std::move(m_TaskScript.front()));
//				break;
//			}
//			default://ERROR
//			{
//				m_pObj->m_pLog->WriteLine(true, utils::tLogColour::LightRed, "ERR: unknown task");
//
//				throw std::invalid_argument("Unknown task");
//				//[TBD] throw an exception or do nothing
//
//				break;
//			}
//			}
//
//			m_TaskScript.pop_front();
//		}
//	}
//}

//bool tCameraVC0706::tState::OnCmdDone()
//{
//	ResetCmd();
//
//	if (m_TaskScript.empty())
//	{
//		OnTaskScriptDone();
//		return true;
//	}
//	
//	return true;
//}

//void tCameraVC0706::tState::OnCmdTaskScript(std::unique_ptr<tGnssTaskScriptCmd> cmd, const std::string& taskScriptID)
//{
//	m_TaskScript.push_front(std::move(cmd));
//
//	if (m_OnCmdTaskScriptIDLast != taskScriptID)
//	{
//		m_OnCmdTaskScriptIDLast = taskScriptID;
//
//		tGnssTaskScript Script = m_pObj->GetTaskScript(taskScriptID, false);
//
//		for (tGnssTaskScript::reverse_iterator i = Script.rbegin(); i != Script.rend(); ++i)//C++11
//		{
//			m_TaskScript.push_front(std::move(*i));
//		}
//	}
//	else
//	{
//		OnTaskScriptFailed(m_OnCmdTaskScriptIDLast);//ChangeState
//		return;
//	}
//}

//void tCameraVC0706::tState::ResetCmd()
//{
//	tCmd* Cmd = m_pCmd;
//
//	m_pCmd = nullptr;
//
//	delete Cmd;
//}

//bool tCameraVC0706::tState::SetTaskScript(const std::string& taskScriptID, bool userTaskScript)
//{
//	tGnssTaskScript Script = m_pObj->GetTaskScript(taskScriptID, userTaskScript);
//
//	if (Script.empty())
//	{
//		m_pObj->m_pLog->WriteLine(false, utils::tLogColour::LightRed, "SetTaskScript - NO SCRIPT LOADED: " + taskScriptID);
//		return false;
//	}
//
//	for (auto& i : Script)
//	{
//		m_TaskScript.push_back(std::move(i));
//	}
//
//	return true;
//}

//bool tCameraVC0706::tState::OnCmdFailed()
//{
//	ChangeState(new tStateError(m_pObj, "OnCmdFailed"));
//	return true;
//}

//void tCameraVC0706::tState::OnReceived(const tPacketNMEA_Template& value)
//{
//	m_pObj->m_pLog->WriteLine(false, utils::tLogColour::LightYellow, "OnReceived: " + value.GetPayload());
//}

}


/*
namespace mod
{

tCameraVC0706::tState::tState(tCameraVC0706 *obj)
	:p_obj(obj)
{
}

void tCameraVC0706::tState::Halt()
{
	ChangeState(tStateHalt::Instance(p_obj));
	return;
}

void tCameraVC0706::tState::Receive(std::vector<char>& data)
{
	if (m_ReceivedData.size() > 128)//[#]
	{
		m_ReceivedData.clear();
	}

	m_ReceivedData.insert(m_ReceivedData.end(), data.begin(), data.end());

	for (int i = 0; i < 10; ++i)//[#]
	{
		std::vector<char> Packet = CameraVC0706::Packet::FindPacketRet(m_ReceivedData);

		if (Packet.size() != 0)
		{
#ifdef LIB_MODULE_LOG_LIBTEST
			p_obj->p_log->WriteHex("Rcvd Packet", Packet);
#endif//LIB_MODULE_LOG_LIBTEST

			OnReceivedMsg(Packet);//ChangeState
			return;
		}
	}
}

void tCameraVC0706::tState::ChangeState(tState *state)
{
	p_obj->ChangeState(state);
}

}*/