#include "PM_Tests.h"

#include "Common.h"
#include "Network/BtEndpoint.h"
#include "Common/Defines.h"
#include "OPPRF/OPPRFReceiver.h"
#include "OPPRF/OPPRFSender.h"
#include "OPPRF/binSet.h"
#include "Common/Log.h"
#include "Common/Log.h"
#include <memory>
#include "Common/Timer.h"
#include "Crypto/Curve.h"

#define PARALLEL

#include "Common/ByteStream.h"
#include "Common/BitVector.h"
#include "Crypto/sha1.h"

#include "TwoChooseOne/IknpOtExtReceiver.h"
#include "TwoChooseOne/IknpOtExtSender.h"

#include "TwoChooseOne/KosOtExtReceiver.h"
#include "TwoChooseOne/KosOtExtSender.h"

#include "TwoChooseOne/LzKosOtExtReceiver.h"
#include "TwoChooseOne/LzKosOtExtSender.h"

#include "NChooseOne/KkrtNcoOtReceiver.h"
#include "NChooseOne/KkrtNcoOtSender.h"


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

	u64 numOTs = 200, numBlkP=1, numBlkT=2, blkSize=128;
	u64 numP = numBlkP*blkSize, numT = numBlkT*blkSize;

	//1-o-o-3
	//   text
	//  --------
	//  --------
	std::vector<std::array<block, 3>> text(numBlkT);
	


	PRNG prng(_mm_set_epi32(4253465, 3434565, 234435, 23987045));
	PRNG prng0(_mm_set_epi32(4253465, 3434565, 234435, 23987045));
	PRNG prng1(_mm_set_epi32(4253233465, 334565, 0, 235));

	//t1, t1+s1
	std::vector<block> recvMsg1(numOTs), baseRecv1(128);
	std::vector<std::array<block, 2>> sendMsg1(numOTs), baseSend1(128);
	BitVector choices1(numOTs), baseChoice1(128);
	choices1.randomize(prng0);
	baseChoice1.randomize(prng0);
	
	//t2, t2+s2
	std::vector<block> recvMsg2(numOTs), baseRecv2(128);
	std::vector<std::array<block, 2>> sendMsg2(numOTs), baseSend2(128);
	BitVector choices2(numOTs), baseChoice2(128);
	choices2.randomize(prng0);
	baseChoice2.randomize(prng0);

	std::array<BitVector, 2> choices;
	choices[0] = choices1;
	choices[1] = choices2;

	//t1+t2, t1+t2+s1, t1+t2+s2, t1+t2+s1+s2
	std::vector<std::array<block, 4>> sendMsg(numOTs);
	std::vector<block> recvMsg(numOTs); //t1+t2
		
	IknpOtExtSender sender;
	IknpOtExtReceiver recv;
			

	std::thread thrd = std::thread([&]() {
		Log::setThreadName("receiver"); 
		
//Offline
		NaorPinkas base;
		base.send(baseSend1, prng, senderChannel, 2);
		base.send(baseSend2, prng, senderChannel, 2);

		recv.setBaseOts(baseSend1);
		recv.receive(choices1, recvMsg1, prng, recvChannel);
		recv.setBaseOts(baseSend2);
		recv.receive(choices2, recvMsg2, prng, recvChannel);

		for (u64 i = 0; i < numOTs; i++)
		{
			recvMsg[i] = recvMsg1[i] ^ recvMsg2[i];
		}

	



	});

	NaorPinkas base;
	base.receive(baseChoice1, baseRecv1, prng, recvChannel, 1);
	base.receive(baseChoice2, baseRecv2, prng, recvChannel, 1);

	sender.setBaseOts(baseRecv1, baseChoice1);
	sender.send(sendMsg1, prng1, senderChannel);

	sender.setBaseOts(baseRecv2, baseChoice2);
	sender.send(sendMsg2, prng1, senderChannel);
	
	
	for (u64 i = 0; i < numOTs; i++)
	{
		sendMsg[i][0] = sendMsg1[i][0] ^ sendMsg2[i][0];
		sendMsg[i][1] = sendMsg1[i][1] ^ sendMsg2[i][0];
		sendMsg[i][2] = sendMsg1[i][0] ^ sendMsg2[i][1];
		sendMsg[i][3] = sendMsg1[i][1] ^ sendMsg2[i][1];
	}

	thrd.join();

	std::cout << sendMsg[199][0] << std::endl;
	std::cout << sendMsg[199][1] << std::endl;
	std::cout << sendMsg[199][2] << std::endl;
	std::cout << sendMsg[199][3] << std::endl;
	
	std::cout << recvMsg[199] << std::endl;

	std::cout << choices[0][199] << std::endl;
	std::cout << choices[1][199] << std::endl;






#if 0
	auto& params = k233;
	auto seed = prng.get<block>();
	EllipticCurve curve(params, seed);

	EccNumber
		alpha(curve, prng),//(mainPk->get_rnd_num()),
						   //PKr(curve),//(mainPk->get_num()),
		tmp(curve);

	EccNumber beta(curve, prng);

	const EccPoint&
		g = curve.getGenerator();

	EccPoint pA(curve);
	EccPoint pB(curve);

	pA = g*alpha;

	u8 choice = choices1[0];

	if (!choice)
		pB = pA + g*beta;
	else
		pB = g*beta;

	EccPoint pPKr(curve);//(thrdPK->get_fe()),
	pPKr = pA*beta;

	EccPoint
		pPK0(curve),//(thrdPK->get_fe()),
		pPK1(curve);//(thrdPK->get_fe()),

	pPK0 = pB*alpha;
	pPK1 = (pB-pA)*alpha;

	std::cout << "pPK0: " << pPK0 << std::endl;
	std::cout << "pPK1: " << pPK1 << std::endl;
	std::cout << "pPKr: " << pPKr << std::endl;
#endif 


	senderChannel.close();
	recvChannel.close();

	ep1.stop();
	ep0.stop();

	ios.stop();
}


void IknpOtExt4_Test_Impl()
{
	setThreadName("Sender");

	BtIOService ios(0);
	BtEndpoint ep0(ios, "127.0.0.1", 1212, true, "ep");
	BtEndpoint ep1(ios, "127.0.0.1", 1212, false, "ep");
	Channel& senderChannel = ep1.addChannel("chl", "chl");
	Channel& recvChannel = ep0.addChannel("chl", "chl");

	PRNG prng0(_mm_set_epi32(4253465, 3434565, 234435, 23987045));
	PRNG prng1(_mm_set_epi32(4253233465, 334565, 0, 235));

	u64 numOTs = 200;

	std::vector<block> recvMsg(numOTs), baseRecv(128);
	std::vector<std::array<block, 2>> sendMsg(numOTs), baseSend(128);
	BitVector choices(numOTs), baseChoice(128);
	choices.randomize(prng0);
	baseChoice.randomize(prng0);

	prng0.get((u8*)baseSend.data()->data(), sizeof(block) * 2 * baseSend.size());
	for (u64 i = 0; i < 128; ++i)
	{
		baseRecv[i] = baseSend[i][baseChoice[i]];
	}

	IknpOtExtSender sender;
	IknpOtExtReceiver recv;

	std::thread thrd = std::thread([&]() {



		recv.setBaseOts(baseSend);
		recv.receive(choices, recvMsg, prng0, recvChannel);
	});



	//{
	//    std::lock_guard<std::mutex> lock(mMtx);
	//    for (u64 i = 0; i < baseOTs.receiver_outputs.size(); ++i)
	//    {
	//        std::cout << "i  " << baseOTs.receiver_outputs[i] << " " << (int)baseOTs.receiver_inputs[i] << std::endl;
	//    }
	//}
	sender.setBaseOts(baseRecv, baseChoice);
	sender.send(sendMsg, prng1, senderChannel);
	thrd.join();

	//for (u64 i = 0; i < baseOTs.receiver_outputs.size(); ++i)
	//{
	//    std::cout << sender.GetMessage(i, 0) << " " << sender.GetMessage(i, 1) << "\n" << recv.GetMessage(1) << "  " << recv.mChoiceBits[i] << std::endl;
	//}
//	OT_100Receive_Test(choices1, recvMsg1, sendMsg1);




	senderChannel.close();
	recvChannel.close();


	ep1.stop();
	ep0.stop();

	ios.stop();

	//senderNetMgr.Stop();
	//recvNetMg
}



