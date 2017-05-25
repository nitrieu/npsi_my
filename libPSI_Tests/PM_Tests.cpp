#include "PM_Tests.h"

#include "Common.h"
#include "Network/BtEndpoint.h"
#include "Common/Defines.h"
#include "OPPRF/OPPRFReceiver.h"
#include "OPPRF/OPPRFSender.h"
#include "OPPRF/binSet.h"
#include "Common/Log.h"

#include "NChooseOne/KkrtNcoOtReceiver.h"
#include "NChooseOne/KkrtNcoOtSender.h"


#include "NChooseOne/Oos/OosNcoOtReceiver.h"
#include "NChooseOne/Oos/OosNcoOtSender.h"

#include "Hashing/CuckooHash.h"
#include "Hashing/Hints.h"
#include "Common/Log.h"
#include "Common/Log1.h"
#include <array>

#include "NTL/GF2EX.h"
#include "NTL/GF2XFactoring.h"
#include <NTL/GF2E.h>
#include "NTL/GF2EX.h"
#include <NTL/ZZ_pE.h>
#include <NTL/vec_ZZ_pE.h>
#include "NTL/GF2EX.h"
#include "NTL/ZZ_p.h"
#include "NTL/GF2EX.h" 
#include "NTL/GF2XFactoring.h"
#include "Common/Log.h"
#include "Tools/Tools.h"

using namespace osuCrypto;
#define PRINT
//#define BIN_PRINT


void PM_Test_Impl()
{

	std::string name("psi");
	BtIOService ios(0);

	BtEndpoint ep0(ios, "127.0.0.1", 1212, true, "ep");
	BtEndpoint ep1(ios, "127.0.0.1", 1212, false, "ep");
	Channel& senderChannel = ep1.addChannel("chl", "chl");
	Channel& recvChannel = ep0.addChannel("chl", "chl");

	PRNG prng(_mm_set_epi32(4253465, 3434565, 234435, 23987045));
	std::array<block, gOtExtBaseOtCount> kosSendBase;
	BitVector choices(gOtExtBaseOtCount); choices.randomize(prng);

	std::array<std::array<block, 2>, gOtExtBaseOtCount> baseMsg;
	

	std::thread thrd = std::thread([&]() {
		Log::setThreadName("receiver"); 
		NaorPinkas base;
		base.receive(choices, kosSendBase, prng, recvChannel, 1);
	});

	NaorPinkas base;
	base.send(baseMsg, prng, senderChannel, 2);

	thrd.join();

	std::cout << baseMsg[0][0] << std::endl;
	std::cout << baseMsg[0][1] << std::endl;
	std::cout << kosSendBase[0] << std::endl;


	senderChannel.close();
	recvChannel.close();

	ep1.stop();
	ep0.stop();

	ios.stop();
}