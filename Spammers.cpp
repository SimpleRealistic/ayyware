#include "Spammers.h"

Spammers spammers;

void Spammers::run(CUserCmd* m_pcmd)
{
	clan_changer();
	name_spammer();
}

void Spammers::clan_changer()
{
	auto m_local = game::localdata.localplayer();

	if (miscconfig.bClanChanger)
	{
		/*int server_time = ((float)m_pGlobals->interval_per_tick * m_local->GetTickBase()) - m_pEngine->GetNetChannelInfo()->GetLatency(FLOW_INCOMING);

		const char* tag;

		int value = server_time % 32;
		switch (value) {
			case 0:	 tag = XorStr("yeti    "); break;
			case 1:	 tag = XorStr("yet i   "); break;
			case 2:	 tag = XorStr("yet  i  "); break;
			case 3:	 tag = XorStr("yet   i "); break;
			case 4:	 tag = XorStr("yet    i"); break;
			case 5:	 tag = XorStr("ye t   i"); break;
			case 6:	 tag = XorStr("ye  t  i"); break;
			case 7:	 tag = XorStr("ye   t i"); break;
			case 8:	 tag = XorStr("ye    ti"); break;
			case 9:	 tag = XorStr("y e   ti"); break;
			case 10: tag = XorStr("y  e  ti"); break;
			case 11: tag = XorStr("y   e ti"); break;
			case 12: tag = XorStr("y    eti"); break;
			case 13: tag = XorStr(" y   eti"); break;
			case 14: tag = XorStr("  y  eti"); break;
			case 15: tag = XorStr("   y eti"); break;
			case 16: tag = XorStr("    yeti"); break;
			case 17: tag = XorStr("   y eti"); break;
			case 18: tag = XorStr("  y  eti"); break;
			case 19: tag = XorStr(" y   eti"); break;
			case 20: tag = XorStr("y    eti"); break;
			case 21: tag = XorStr("y   e ti"); break;
			case 22: tag = XorStr("y  e  ti"); break;
			case 23: tag = XorStr("y e   ti"); break;
			case 24: tag = XorStr("ye    ti"); break;
			case 25: tag = XorStr("ye   t i"); break;
			case 26: tag = XorStr("ye  t  i"); break;
			case 27: tag = XorStr("ye t   i"); break;
			case 28: tag = XorStr("yet    i"); break;
			case 29: tag = XorStr("yet   i "); break;
			case 30: tag = XorStr("yet  i  "); break;
			case 31: tag = XorStr("yet i   "); break;
		}*/

		switch ( int( m_pGlobals->curtime * 2.4 ) % 27 )
		{
		case 0: game::functions.set_clan_tag( XorStr("Simple Realistic"), "gamesense" ); break;
		case 1: game::functions.set_clan_tag(XorStr("XenoConnor"), "gamesense"); break;
		}
	}
}

void Spammers::name_spammer()
{
	auto m_local = game::localdata.localplayer();

	if (!miscconfig.bNameChanger)
		return;

	if (!miscconfig.iNameChangerType)
		return;

	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < .5)
		return;

	const char* result;

	if (miscconfig.iNameChangerType == 1)
		result = XorStr(u8"�ANTI AYYWARE                                                ANTI AYYWARE ANTI AYYWARE ANTI AYYWARE ANTI AYYWARE ANTI AYYWARE ANTI AYYWARE ANTI AYYWARE ANTI AYYWARE ANTI ANTI AYYWARE ANTI AYYWARE ANTI AYYWARE ANTI AYYWARE�");
	else if (miscconfig.iNameChangerType == 2)
	{
		std::vector <std::string> names;

		for (int i = 1; i < m_pGlobals->maxClients; i++)
		{
			IClientEntity *entity = m_pEntityList->GetClientEntity(i);

			player_info_t pInfo;

			if (entity && m_local->GetTeamNum() == entity->GetTeamNum())
			{
				ClientClass* cClass = (ClientClass*)entity->GetClientClass();

				if (cClass->m_ClassID == (int)CSGOClassID::CCSPlayer)
				{
					if (m_pEngine->GetPlayerInfo(i, &pInfo))
					{
						if (!strstr(pInfo.name, "GOTV"))
							names.push_back(pInfo.name);
					}
				}
			}
		}

		int randomIndex = rand() % names.size();
		char buffer[128];
		sprintf_s(buffer, "%s ", names[randomIndex].c_str());
		result = buffer;
	}
	else if (miscconfig.iNameChangerType == 3)
		result = XorStr("\n\xAD\xAD\xAD");

	game::functions.set_name(result);
	start_t = clock();
}