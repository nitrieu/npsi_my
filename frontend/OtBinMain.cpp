#include "bloomFilterMain.h"
#include "Network/BtEndpoint.h" 

#include "OPPRF/OPPRFReceiver.h"
#include "OPPRF/OPPRFSender.h"
#include "OPPRF/BarkOPRFReceiver.h"
#include "OPPRF/BarkOPRFSender.h"

#include <fstream>
using namespace osuCrypto;
#include "util.h"

#include "Common/Defines.h"
#include "NChooseOne/KkrtNcoOtReceiver.h"
#include "NChooseOne/KkrtNcoOtSender.h"

#include "NChooseOne/Oos/OosNcoOtReceiver.h"
#include "NChooseOne/Oos/OosNcoOtSender.h"
#include "Common/Log.h"
#include "Common/Log1.h"
#include "Common/Timer.h"
#include "Crypto/PRNG.h"
#include <numeric>

//#define OOS
//#define PRINT
#define pows  { 16/*8,12,,20*/ }
#define threadss {1/*1,4,16,64*/}
#define  numTrial 2
std::vector<block> sendSet;
std::vector<block> mSet;
u64 nParties(3);

#if 0
void BarkOPRSend()
{
	Log::out << "dsfds" << Log::endl;

	setThreadName("CP_Test_Thread");
	u64 numThreads(1);

	std::fstream online, offline;
	online.open("./online.txt", online.trunc | online.out);
	offline.open("./offline.txt", offline.trunc | offline.out);


	std::cout << "role  = sender (" << numThreads << ") otBin" << std::endl;

	std::string name("psi");

	BtIOService ios(0);
	BtEndpoint sendEP(ios, "localhost", 1213, true, name);

	std::vector<Channel*> sendChls_(numThreads);

	for (u64 i = 0; i < numThreads; ++i)
	{
		sendChls_[i] = &sendEP.addChannel("chl" + std::to_string(i), "chl" + std::to_string(i));
	}
	u8 dummy[1];

	senderGetLatency(*sendChls_[0]);
	sendChls_[0]->resetStats();

	LinearCode code;
	// code.loadBinFile(SOLUTION_DIR "/../libOTe/libOTe/Tools/bch511.bin");

	 //for (auto pow : {/* 8,12,*/ 16/*, 20 */ })
	for (auto pow : pows)
	{

		for (auto cc : threadss)
		{
			std::vector<Channel*> sendChls;

			if (pow == 8)
				cc = std::min(8, cc);

			//std::cout << "numTHreads = " << cc << std::endl;

			sendChls.insert(sendChls.begin(), sendChls_.begin(), sendChls_.begin() + cc);

			u64 offlineTimeTot(0);
			u64 onlineTimeTot(0);
			//for (u64 numThreads = 1; numThreads < 129; numThreads *= 2)
			for (u64 jj = 0; jj < numTrial; jj++)
			{

				//u64 repeatCount = 1;
				u64 setSize = (1 << pow), psiSecParam = 40;
				PRNG prng(_mm_set_epi32(4253465, 3434565, 234435, 23987045));



				sendSet.resize(setSize);

				for (u64 i = 0; i < setSize; ++i)
				{
					sendSet[i] = prng.get<block>();
				}

				std::cout << "s\n";
				std::cout << sendSet[5] << std::endl;

#ifdef OOS
				OosNcoOtReceiver otRecv(code);
				OosNcoOtSender otSend(code);
#else
				OPPRFReceiver otRecv;
				KkrtNcoOtSender otSend;
#endif
				BarkOPRFSender sendPSIs;

				//gTimer.reset();

				sendChls[0]->asyncSend(dummy, 1);
				sendChls[0]->recv(dummy, 1);
				u64 otIdx = 0;
				//std::cout << "sender init" << std::endl;
				sendPSIs.init(setSize, psiSecParam, 128, sendChls, otSend, prng.get<block>());
				//std::cout << "s\n";
			//	std::cout << otSend.mGens[5].mSeed << std::endl;


				//return;
				sendChls[0]->asyncSend(dummy, 1);
				sendChls[0]->recv(dummy, 1);
				//std::cout << "sender init done" << std::endl;

				sendPSIs.sendInput(sendSet, sendChls);

				//  sendPSIs.mBins.print();

				u64 dataSent = 0;
				for (u64 g = 0; g < sendChls.size(); ++g)
				{
					dataSent += sendChls[g]->getTotalDataSent();
				}

				//std::accumulate(sendChls[0]->getTotalDataSent())

				//std::cout << setSize << "    " << dataSent / std::pow(2, 20) << " byte  " << std::endl;
				for (u64 g = 0; g < sendChls.size(); ++g)
					sendChls[g]->resetStats();

				//std::cout << gTimer << std::endl;
			}

		}


	}
	for (u64 i = 0; i < numThreads; ++i)
	{
		sendChls_[i]->close();// = &sendEP.addChannel("chl" + std::to_string(i), "chl" + std::to_string(i));
	}
	//sendChl.close();
	//recvChl.close();

	sendEP.stop();

	ios.stop();
}

void BarkOPRFRecv()
{

	setThreadName("CP_Test_Thread");
	u64 numThreads(1);

	std::fstream online, offline;
	online.open("./online.txt", online.trunc | online.out);
	offline.open("./offline.txt", offline.trunc | offline.out);


	std::string name("psi");

	BtIOService ios(0);
	BtEndpoint recvEP(ios, "localhost", 1213, false, name);

	LinearCode code;

	//   code.loadBinFile(SOLUTION_DIR "/../libOTe/libOTe/Tools/bch511.bin");

	std::vector<Channel*> recvChls_(numThreads);
	for (u64 i = 0; i < numThreads; ++i)
	{
		recvChls_[i] = &recvEP.addChannel("chl" + std::to_string(i), "chl" + std::to_string(i));
	}

	std::cout << "role  = recv(" << numThreads << ") otBin" << std::endl;
	u8 dummy[1];
	recverGetLatency(*recvChls_[0]);

	//for (auto pow : {/* 8,12,*/16/*,20*/ })
	for (auto pow : pows)
	{
		for (auto cc : threadss)
		{
			std::vector<Channel*> recvChls;

			if (pow == 8)
				cc = std::min(8, cc);

			u64 setSize = (1 << pow), psiSecParam = 40;

			std::cout << "numTHreads = " << cc << "  n=" << setSize << std::endl;

			recvChls.insert(recvChls.begin(), recvChls_.begin(), recvChls_.begin() + cc);

			u64 offlineTimeTot(0);
			u64 onlineTimeTot(0);
			//for (u64 numThreads = 1; numThreads < 129; numThreads *= 2)
			for (u64 jj = 0; jj < numTrial; jj++)
			{

				//u64 repeatCount = 1;
				PRNG prng(_mm_set_epi32(42553465, 343452565, 2364435, 23923587));


				std::vector<block> recvSet(setSize);




				for (u64 i = 0; i < setSize; ++i)
				{
					recvSet[i] = prng.get<block>();
					// sendSet[i];// = prng.get<block>();
				}
				for (u64 i = 1; i < 3; ++i)
				{
					recvSet[i] = sendSet[i];
				}

				for (u64 i = setSize - 3; i < setSize; ++i)
				{
					recvSet[i] = sendSet[i];
				}

				std::cout << "s\n";
				std::cout << recvSet[5] << std::endl;
#ifdef OOS
				OosNcoOtReceiver otRecv(code);
				OosNcoOtSender otSend(code);
#else
				KkrtNcoOtReceiver otRecv;
#endif
				BarkOPRFReceiver recvPSIs;


				recvChls[0]->recv(dummy, 1);
				gTimer.reset();
				recvChls[0]->asyncSend(dummy, 1);

				u64 otIdx = 0;


				Timer timer;
				auto start = timer.setTimePoint("start");
				recvPSIs.init(setSize, psiSecParam, 128, recvChls, otRecv, ZeroBlock);

				/*std::cout << "r\n";
				std::cout << otRecv.mGens[5][0].mSeed << std::endl;
				std::cout << otRecv.mGens[5][1].mSeed << std::endl;*/

				//return;


				//std::vector<u64> sss(recvChls.size());
				//for (u64 g = 0; g < recvChls.size(); ++g)
				//{
				//    sss[g] =  recvChls[g]->getTotalDataSent();
				//}

				recvChls[0]->asyncSend(dummy, 1);
				recvChls[0]->recv(dummy, 1);
				auto mid = timer.setTimePoint("init");


				recvPSIs.sendInput(recvSet, recvChls);
				//recvPSIs.mBins.print();


				auto end = timer.setTimePoint("done");

				auto offlineTime = std::chrono::duration_cast<std::chrono::milliseconds>(mid - start).count();
				auto onlineTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - mid).count();


				offlineTimeTot += offlineTime;
				onlineTimeTot += onlineTime;
				//auto byteSent = recvChls[0]->getTotalDataSent() *recvChls.size();

				u64 dataSent = 0;
				for (u64 g = 0; g < recvChls.size(); ++g)
				{
					dataSent += recvChls[g]->getTotalDataSent();
					//std::cout << "chl[" << g << "] " << recvChls[g]->getTotalDataSent() << "   " << sss[g] << std::endl;
				}

				double time = offlineTime + onlineTime;
				time /= 1000;
				auto Mbps = dataSent * 8 / time / (1 << 20);

				std::cout << setSize << "  " << offlineTime << "  " << onlineTime << "        " << Mbps << " Mbps      " << (dataSent / std::pow(2.0, 20)) << " MB" << std::endl;

				for (u64 g = 0; g < recvChls.size(); ++g)
					recvChls[g]->resetStats();

				//std::cout << "threads =  " << numThreads << std::endl << timer << std::endl << std::endl << std::endl;


				//std::cout << numThreads << std::endl;
				//std::cout << timer << std::endl;

			 //   std::cout << gTimer << std::endl;

				//if (recv.mIntersection.size() != setSize)
				//    throw std::runtime_error("");







			}



			online << onlineTimeTot / numTrial << "-";
			offline << offlineTimeTot / numTrial << "-";

		}
	}

	for (u64 i = 0; i < numThreads; ++i)
	{
		recvChls_[i]->close();// = &recvEP.addChannel("chl" + std::to_string(i), "chl" + std::to_string(i));
	}
	//sendChl.close();
	//recvChl.close();

	recvEP.stop();

	ios.stop();
}

void OPPRFSend()
{
	Log::out << "dsfds" << Log::endl;

	setThreadName("CP_Test_Thread");
	u64 numThreads(1);

	std::fstream online, offline;
	online.open("./online.txt", online.trunc | online.out);
	offline.open("./offline.txt", offline.trunc | offline.out);



	std::cout << "role  = sender (" << numThreads << ") otBin" << std::endl;

	std::string name("psi");

	BtIOService ios(0);
	BtEndpoint sendEP(ios, "localhost", 1213, true, name);

	std::vector<Channel*> sendChls_(numThreads);

	for (u64 i = 0; i < numThreads; ++i)
	{
		sendChls_[i] = &sendEP.addChannel("chl" + std::to_string(i), "chl" + std::to_string(i));
	}
	u8 dummy[1];

	senderGetLatency(*sendChls_[0]);
	sendChls_[0]->resetStats();

	LinearCode code;
	// code.loadBinFile(SOLUTION_DIR "/../libOTe/libOTe/Tools/bch511.bin");

	//for (auto pow : {/* 8,12,*/ 16/*, 20 */ })
	for (auto pow : pows)
	{

		for (auto cc : threadss)
		{
			std::vector<Channel*> sendChls;

			if (pow == 8)
				cc = std::min(8, cc);

			//std::cout << "numTHreads = " << cc << std::endl;

			sendChls.insert(sendChls.begin(), sendChls_.begin(), sendChls_.begin() + cc);

			u64 offlineTimeTot(0);
			u64 onlineTimeTot(0);
			//for (u64 numThreads = 1; numThreads < 129; numThreads *= 2)
			for (u64 jj = 0; jj < numTrial; jj++)
			{

				//u64 repeatCount = 1;
				u64 setSize = (1 << pow), psiSecParam = 40;
				PRNG prng(_mm_set_epi32(4253465, 3434565, 234435, 23987045));



				sendSet.resize(setSize);

				for (u64 i = 0; i < setSize; ++i)
				{
					sendSet[i] = prng.get<block>();
				}

				std::cout << "s\n";
				std::cout << sendSet[5] << std::endl;

#ifdef OOS
				OosNcoOtReceiver otRecv(code);
				OosNcoOtSender otSend(code);
#else
				OPPRFReceiver otRecv;
				KkrtNcoOtSender otSend;
#endif
				OPPRFSender sendPSIs;

				//gTimer.reset();

				sendChls[0]->asyncSend(dummy, 1);
				sendChls[0]->recv(dummy, 1);
				u64 otIdx = 0;
				//std::cout << "sender init" << std::endl;
			//	sendPSIs.init(setSize, psiSecParam, 128, sendChls, otSend, prng.get<block>());
				//std::cout << "s\n";
				//	std::cout << otSend.mGens[5].mSeed << std::endl;


				//return;
				sendChls[0]->asyncSend(dummy, 1);
				sendChls[0]->recv(dummy, 1);
				//std::cout << "sender init done" << std::endl;

				//sendPSIs.sendInput(sendSet.data(), sendSet.size(), sendChls);
			//	sendPSIs.sendInput(sendSet, sendChls);

				//  sendPSIs.mBins.print();

				u64 dataSent = 0;
				for (u64 g = 0; g < sendChls.size(); ++g)
				{
					dataSent += sendChls[g]->getTotalDataSent();
				}

				//std::accumulate(sendChls[0]->getTotalDataSent())

				//std::cout << setSize << "    " << dataSent / std::pow(2, 20) << " byte  " << std::endl;
				for (u64 g = 0; g < sendChls.size(); ++g)
					sendChls[g]->resetStats();

				//std::cout << gTimer << std::endl;
			}

		}


	}
	for (u64 i = 0; i < numThreads; ++i)
	{
		sendChls_[i]->close();// = &sendEP.addChannel("chl" + std::to_string(i), "chl" + std::to_string(i));
	}
	//sendChl.close();
	//recvChl.close();

	sendEP.stop();

	ios.stop();
}

void OPPRFRecv()
{

	setThreadName("CP_Test_Thread");
	u64 numThreads(1);

	std::fstream online, offline;
	online.open("./online.txt", online.trunc | online.out);
	offline.open("./offline.txt", offline.trunc | offline.out);


	std::string name("psi");

	BtIOService ios(0);
	BtEndpoint recvEP(ios, "localhost", 1213, false, name);

	LinearCode code;

	//   code.loadBinFile(SOLUTION_DIR "/../libOTe/libOTe/Tools/bch511.bin");

	std::vector<Channel*> recvChls_(numThreads);
	for (u64 i = 0; i < numThreads; ++i)
	{
		recvChls_[i] = &recvEP.addChannel("chl" + std::to_string(i), "chl" + std::to_string(i));
	}

	std::cout << "role  = recv(" << numThreads << ") otBin" << std::endl;
	u8 dummy[1];
	recverGetLatency(*recvChls_[0]);

	//for (auto pow : {/* 8,12,*/16/*,20*/ })
	for (auto pow : pows)
	{
		for (auto cc : threadss)
		{
			std::vector<Channel*> recvChls;

			if (pow == 8)
				cc = std::min(8, cc);

			u64 setSize = (1 << pow), psiSecParam = 40;

			std::cout << "numTHreads = " << cc << "  n=" << setSize << std::endl;

			recvChls.insert(recvChls.begin(), recvChls_.begin(), recvChls_.begin() + cc);

			u64 offlineTimeTot(0);
			u64 onlineTimeTot(0);
			//for (u64 numThreads = 1; numThreads < 129; numThreads *= 2)
			for (u64 jj = 0; jj < numTrial; jj++)
			{

				//u64 repeatCount = 1;
				PRNG prng(_mm_set_epi32(42553465, 343452565, 2364435, 23923587));


				std::vector<block> recvSet(setSize);




				for (u64 i = 0; i < setSize; ++i)
				{
					recvSet[i] = prng.get<block>();
					// sendSet[i];// = prng.get<block>();
				}
				for (u64 i = 1; i < 3; ++i)
				{
					recvSet[i] = sendSet[i];
				}

				for (u64 i = setSize - 3; i < setSize; ++i)
				{
					recvSet[i] = sendSet[i];
				}

				std::cout << "s\n";
				std::cout << recvSet[5] << std::endl;
#ifdef OOS
				OosNcoOtReceiver otRecv(code);
				OosNcoOtSender otSend(code);
#else
				KkrtNcoOtReceiver otRecv;
#endif
				OPPRFReceiver recvPSIs;


				recvChls[0]->recv(dummy, 1);
				gTimer.reset();
				recvChls[0]->asyncSend(dummy, 1);

				u64 otIdx = 0;


				Timer timer;
				auto start = timer.setTimePoint("start");
				//	recvPSIs.init(setSize, psiSecParam, 128, recvChls, otRecv, ZeroBlock);

					/*std::cout << "r\n";
					std::cout << otRecv.mGens[5][0].mSeed << std::endl;
					std::cout << otRecv.mGens[5][1].mSeed << std::endl;*/

					//return;


					//std::vector<u64> sss(recvChls.size());
					//for (u64 g = 0; g < recvChls.size(); ++g)
					//{
					//    sss[g] =  recvChls[g]->getTotalDataSent();
					//}

				recvChls[0]->asyncSend(dummy, 1);
				recvChls[0]->recv(dummy, 1);
				auto mid = timer.setTimePoint("init");


				//	recvPSIs.sendInput(recvSet.data(), recvSet.size(), recvChls);
				//	recvPSIs.sendInput(recvSet, recvChls);
					//recvPSIs.mBins.print();


				auto end = timer.setTimePoint("done");

				auto offlineTime = std::chrono::duration_cast<std::chrono::milliseconds>(mid - start).count();
				auto onlineTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - mid).count();


				offlineTimeTot += offlineTime;
				onlineTimeTot += onlineTime;
				//auto byteSent = recvChls[0]->getTotalDataSent() *recvChls.size();

				u64 dataSent = 0;
				for (u64 g = 0; g < recvChls.size(); ++g)
				{
					dataSent += recvChls[g]->getTotalDataSent();
					//std::cout << "chl[" << g << "] " << recvChls[g]->getTotalDataSent() << "   " << sss[g] << std::endl;
				}

				double time = offlineTime + onlineTime;
				time /= 1000;
				auto Mbps = dataSent * 8 / time / (1 << 20);

				std::cout << setSize << "  " << offlineTime << "  " << onlineTime << "        " << Mbps << " Mbps      " << (dataSent / std::pow(2.0, 20)) << " MB" << std::endl;

				for (u64 g = 0; g < recvChls.size(); ++g)
					recvChls[g]->resetStats();

				//std::cout << "threads =  " << numThreads << std::endl << timer << std::endl << std::endl << std::endl;


				//std::cout << numThreads << std::endl;
				//std::cout << timer << std::endl;

				//   std::cout << gTimer << std::endl;

				//if (recv.mIntersection.size() != setSize)
				//    throw std::runtime_error("");







			}



			online << onlineTimeTot / numTrial << "-";
			offline << offlineTimeTot / numTrial << "-";

		}
	}

	for (u64 i = 0; i < numThreads; ++i)
	{
		recvChls_[i]->close();// = &recvEP.addChannel("chl" + std::to_string(i), "chl" + std::to_string(i));
	}
	//sendChl.close();
	//recvChl.close();

	recvEP.stop();

	ios.stop();
}


void OPPRF2_EmptrySet_Test()
{
	u64 setSize = 1 << 20, psiSecParam = 40, bitSize = 128, numParties = 2;
	PRNG prng(_mm_set_epi32(4253465, 3434565, 234435, 23987045));

	std::vector<block> sendSet(setSize), recvSet(setSize);
	std::vector<std::vector<block>> sendPayLoads(numParties), recvPayLoads(numParties);



	for (u64 i = 0; i < setSize; ++i)
	{
		sendSet[i] = prng.get<block>();
		recvSet[i] = sendSet[i];
	}

	for (u64 j = 0; j < numParties; ++j) {
		sendPayLoads[j].resize(setSize);
		recvPayLoads[j].resize(setSize);
		for (u64 i = 0; i < setSize; ++i)
		{
			sendPayLoads[j][i] = prng.get<block>();
		}
	}

	for (u64 i = 1; i < 3; ++i)
	{
		recvSet[i] = sendSet[i];
	}

	std::string name("psi");

	BtIOService ios(0);
	BtEndpoint ep0(ios, "localhost", 1212, true, name);
	BtEndpoint ep1(ios, "localhost", 1212, false, name);


	std::vector<Channel*> recvChl{ &ep1.addChannel(name, name) };
	std::vector<Channel*> sendChl{ &ep0.addChannel(name, name) };

	KkrtNcoOtReceiver otRecv0, otRecv1;
	KkrtNcoOtSender otSend0, otSend1;



	OPPRFSender send;
	OPPRFReceiver recv;
	std::thread thrd([&]() {


		send.init(numParties, setSize, psiSecParam, bitSize, sendChl, otSend0, otRecv1, prng.get<block>());
		//send.hash2Bins(sendSet, sendChl);
		//send.getOPRFKeys(1, sendChl);
		//send.sendSecretSharing(1, sendPayLoads[1], sendChl);
		//send.revSecretSharing(1, recvPayLoads[1], sendChl);
		//Log::out << "send.mSimpleBins.print(true, false, false,false);" << Log::endl;
		//send.mSimpleBins.print(1, true, true, true, true);
		//Log::out << "send.mCuckooBins.print(true, false, false);" << Log::endl;
		//send.mCuckooBins.print(1,true, true, false);



	});
	Timer timer;
	auto start = timer.setTimePoint("start");
	recv.init(numParties, setSize, psiSecParam, bitSize, recvChl, otRecv0, otSend1, ZeroBlock);

	auto mid = timer.setTimePoint("init");
	//recv.hash2Bins(recvSet, recvChl);
	auto mid_hashing = timer.setTimePoint("hashing");
	//recv.getOPRFkeys(0, recvChl);
	//recv.revSecretSharing(0, recvPayLoads[0], recvChl);
	//recv.sendSecretSharing(0, sendPayLoads[0], recvChl);
	auto end = timer.setTimePoint("done");
	//Log::out << "recv.mCuckooBins.print(true, false, false);" << Log::endl;
	//recv.mCuckooBins.print(0, true, true, false);
	//Log::out << "recv.mSimpleBins.print(true, false, false,false);" << Log::endl;
	//recv.mSimpleBins.print(0,true, true, true, true);

	auto offlineTime = std::chrono::duration_cast<std::chrono::milliseconds>(mid - start).count();
	auto hashingTime = std::chrono::duration_cast<std::chrono::milliseconds>(mid_hashing - mid).count();
	auto onlineTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - mid).count();

	double time = offlineTime + onlineTime;
	time /= 1000;

	std::cout << setSize << "  " << offlineTime << "  " << onlineTime
		<< " " << hashingTime
		<< " " << time << std::endl;

	for (u64 i = 1; i < recvPayLoads[0].size(); ++i)
	{
		if (memcmp((u8*)&recvPayLoads[0][i], &sendPayLoads[1][i], sizeof(block)))
		{
			Log::out << "recvPayLoads[i] != sendPayLoads[i]" << Log::endl;
			Log::out << recvSet[i] << Log::endl;
			Log::out << sendSet[i] << Log::endl;
			Log::out << i << Log::endl;
		}
	}

	for (u64 i = 1; i < recvPayLoads[1].size(); ++i)
	{
		if (memcmp((u8*)&recvPayLoads[1][i], &sendPayLoads[0][i], sizeof(block)))
		{
			Log::out << "recvPayLoads[i] != sendPayLoads[i]" << Log::endl;
			Log::out << recvSet[i] << Log::endl;
			Log::out << sendSet[i] << Log::endl;
			Log::out << i << Log::endl;
		}
	}

#ifdef PRINT
	std::cout << IoStream::lock;
	for (u64 i = 1; i < recvPayLoads.size(); ++i)
	{
		Log::out << recvPayLoads[i] << Log::endl;
		Log::out << sendPayLoads[i] << Log::endl;
		if (memcmp((u8*)&recvPayLoads[i], &sendPayLoads[i], sizeof(block)))
		{
			Log::out << "recvPayLoads[i] != sendPayLoads[i]" << Log::endl;
			Log::out << recvSet[i] << Log::endl;
			Log::out << sendSet[i] << Log::endl;
			Log::out << i << Log::endl;
		}

	}

	std::cout << IoStream::unlock;

	std::cout << IoStream::lock;
	Log::out << otSend0.mT.size()[1] << Log::endl;
	Log::out << otSend1.mT.size()[1] << Log::endl;
	Log::out << otSend0.mGens[0].get<block>() << Log::endl;
	Log::out << otRecv0.mGens[0][0].get<block>() << Log::endl;
	Log::out << otRecv0.mGens[0][1].get<block>() << Log::endl;
	Log::out << "------------" << Log::endl;
	Log::out << otSend1.mGens[0].get<block>() << Log::endl;
	Log::out << otRecv1.mGens[0][0].get<block>() << Log::endl;
	Log::out << otRecv1.mGens[0][1].get<block>() << Log::endl;
	std::cout << IoStream::unlock;

#endif

	thrd.join();







	sendChl[0]->close();
	recvChl[0]->close();

	ep0.stop();
	ep1.stop();
	ios.stop();
}
#endif

void Channel_test()
{
	std::string name("psi");

	BtIOService ios(0);
	BtEndpoint ep0(ios, "localhost", 1212, false, name);
	BtEndpoint ep1(ios, "localhost", 1212, true, name);
	u8 dummy = 1;
	u8 revDummy;
	std::vector<Channel*> recvChl{ &ep0.addChannel(name, name) };
	std::vector<Channel*> sendChl{ &ep1.addChannel(name, name) };

	std::thread thrd([&]() {
		sendChl[0]->asyncSend(&dummy, 1);
	});


	recvChl[0]->recv(&revDummy, 1);
	std::cout << static_cast<int16_t>(revDummy) << std::endl;

	sendChl[0]->close();
	recvChl[0]->close();

	ep0.stop();
	ep1.stop();
	ios.stop();
}
void Channel_party_test(u64 myIdx)
{
	u64 setSize = 1 << 5, psiSecParam = 40, bitSize = 128, numThreads = 1;
	PRNG prng(_mm_set_epi32(4253465, 3434565, 234435, 23987045));


	std::vector<u8> dummy(nParties);
	std::vector<u8> revDummy(nParties);


	std::string name("psi");
	BtIOService ios(0);

	int btCount = nParties;
	std::vector<BtEndpoint> ep(nParties);

	for (u64 i = 0; i < nParties; ++i)
	{
		dummy[i] = myIdx * 10 + i;
		if (i < myIdx)
		{
			u32 port = i * 10 + myIdx;//get the same port; i=1 & pIdx=2 =>port=102
			ep[i].start(ios, "localhost", port, false, name); //channel bwt i and pIdx, where i is sender
		}
		else if (i > myIdx)
		{
			u32 port = myIdx * 10 + i;//get the same port; i=2 & pIdx=1 =>port=102
			ep[i].start(ios, "localhost", port, true, name); //channel bwt i and pIdx, where i is receiver
		}
	}


	std::vector<std::vector<Channel*>> chls(nParties);

	for (u64 i = 0; i < nParties; ++i)
	{
		if (i != myIdx) {
			chls[i].resize(numThreads);
			for (u64 j = 0; j < numThreads; ++j)
			{
				//chls[i][j] = &ep[i].addChannel("chl" + std::to_string(j), "chl" + std::to_string(j));
				chls[i][j] = &ep[i].addChannel(name, name);
			}
		}
	}



	std::mutex printMtx1, printMtx2;
	std::vector<std::thread>  pThrds(nParties);
	for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx)
	{
		pThrds[pIdx] = std::thread([&, pIdx]() {
			if (pIdx < myIdx) {


				chls[pIdx][0]->asyncSend(&dummy[pIdx], 1);
				std::lock_guard<std::mutex> lock(printMtx1);
				std::cout << "s: " << myIdx << " -> " << pIdx << " : " << static_cast<int16_t>(dummy[pIdx]) << std::endl;

			}
			else if (pIdx > myIdx) {

				chls[pIdx][0]->recv(&revDummy[pIdx], 1);
				std::lock_guard<std::mutex> lock(printMtx2);
				std::cout << "r: " << myIdx << " <- " << pIdx << " : " << static_cast<int16_t>(revDummy[pIdx]) << std::endl;

			}
		});
	}


	for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx)
	{
		//	if(pIdx!=myIdx)
		pThrds[pIdx].join();
	}




	for (u64 i = 0; i < nParties; ++i)
	{
		if (i != myIdx)
		{
			for (u64 j = 0; j < numThreads; ++j)
			{
				chls[i][j]->close();
			}
		}
	}

	for (u64 i = 0; i < nParties; ++i)
	{
		if (i != myIdx)
			ep[i].stop();
	}


	ios.stop();
}


void party(u64 myIdx, u64 nParties, u64 setSize, std::vector<block>& mSet)
{
	//nParties = 4;
	std::fstream runtime;
	if (myIdx == 0)
		runtime.open("./runtime" + nParties, runtime.trunc | runtime.out);

	u64 offlineAvgTime(0), hashingAvgTime(0), getOPRFAvgTime(0),
		ss2DirAvgTime(0), ssRoundAvgTime(0), intersectionAvgTime(0), onlineAvgTime(0);

	u64  psiSecParam = 40, bitSize = 128, numThreads = 1;
	PRNG prng(_mm_set_epi32(4253465, 3434565, myIdx, myIdx));

	std::string name("psi");
	BtIOService ios(0);

	int btCount = nParties;
	std::vector<BtEndpoint> ep(nParties);

	for (u64 i = 0; i < nParties; ++i)
	{
		if (i < myIdx)
		{
			u32 port = 1120 + i * 100 + myIdx;;//get the same port; i=1 & pIdx=2 =>port=102
			ep[i].start(ios, "localhost", port, false, name); //channel bwt i and pIdx, where i is sender
		}
		else if (i > myIdx)
		{
			u32 port = 1120 + myIdx * 100 + i;//get the same port; i=2 & pIdx=1 =>port=102
			ep[i].start(ios, "localhost", port, true, name); //channel bwt i and pIdx, where i is receiver
		}
	}

	std::vector<std::vector<Channel*>> chls(nParties);

	for (u64 i = 0; i < nParties; ++i)
	{
		if (i != myIdx) {
			chls[i].resize(numThreads);
			for (u64 j = 0; j < numThreads; ++j)
			{
				//chls[i][j] = &ep[i].addChannel("chl" + std::to_string(j), "chl" + std::to_string(j));
				chls[i][j] = &ep[i].addChannel(name, name);
			}
		}
	}

	u64 maskSize = roundUpTo(psiSecParam + 2 * std::log(setSize) - 1, 8) / 8;

	for (u64 idxTrial = 0; idxTrial < numTrial; idxTrial++)
	{
		std::vector<block> set(setSize);
		std::vector<std::vector<block>> sendPayLoads(nParties), recvPayLoads(nParties);

		for (u64 i = 0; i < setSize; ++i)
		{
			set[i] = mSet[i];
		}
		PRNG prng1(_mm_set_epi32(4253465, 3434565, 234435, myIdx));
		set[0] = prng1.get<block>();;
		for (u64 idxP = 0; idxP < nParties; ++idxP)
		{
			sendPayLoads[idxP].resize(setSize);
			recvPayLoads[idxP].resize(setSize);
			for (u64 i = 0; i < setSize; ++i)
				sendPayLoads[idxP][i] = prng.get<block>();
		}
		u64 nextNeighbor = (myIdx + 1) % nParties;
		u64 prevNeighbor = (myIdx - 1 + nParties) % nParties;
		//sum share of other party =0 => compute the share to his neighbor = sum of other shares
		if (myIdx != 0) {
			for (u64 i = 0; i < setSize; ++i)
			{
				block sum = ZeroBlock;
				for (u64 idxP = 0; idxP < nParties; ++idxP)
				{
					if ((idxP != myIdx && idxP != nextNeighbor))
						sum = sum ^ sendPayLoads[idxP][i];
				}
				sendPayLoads[nextNeighbor][i] = sum;

			}
		}
		else
			for (u64 i = 0; i < setSize; ++i)
			{
				sendPayLoads[myIdx][i] = ZeroBlock;
				for (u64 idxP = 0; idxP < nParties; ++idxP)
				{
					if (idxP != myIdx)
						sendPayLoads[myIdx][i] = sendPayLoads[myIdx][i] ^ sendPayLoads[idxP][i];
				}
			}

#ifdef PRINT
		std::cout << IoStream::lock;
		if (myIdx != 0) {
			for (u64 i = 0; i < setSize; ++i)
			{
				block check = ZeroBlock;
				for (u64 idxP = 0; idxP < nParties; ++idxP)
				{
					if (idxP != myIdx)
						check = check ^ sendPayLoads[idxP][i];
				}
				if (memcmp((u8*)&check, &ZeroBlock, sizeof(block)))
					std::cout << "Error ss values: myIdx: " << myIdx
					<< " value: " << check << std::endl;
			}
		}
		else
			for (u64 i = 0; i < setSize; ++i)
			{
				block check = ZeroBlock;
				for (u64 idxP = 0; idxP < nParties; ++idxP)
				{
					check = check ^ sendPayLoads[idxP][i];
				}
				if (memcmp((u8*)&check, &ZeroBlock, sizeof(block)))
					std::cout << "Error ss values: myIdx: " << myIdx
					<< " value: " << check << std::endl;
			}
		std::cout << IoStream::unlock;
#endif


		std::vector<KkrtNcoOtReceiver> otRecv(nParties);
		std::vector<KkrtNcoOtSender> otSend(nParties);

		std::vector<OPPRFSender> send(nParties - myIdx - 1);
		std::vector<OPPRFReceiver> recv(myIdx);
		binSet bins;

		std::vector<std::thread>  pThrds(nParties);

		//##########################
		//### Offline Phasing
		//##########################
		Timer timer;
		auto start = timer.setTimePoint("start");
		bins.init(myIdx, nParties, setSize, psiSecParam);
		u64 otCountSend = bins.mSimpleBins.mBins.size();
		u64 otCountRecv = bins.mCuckooBins.mBins.size();

		for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx)
		{
			pThrds[pIdx] = std::thread([&, pIdx]() {
				if (pIdx < myIdx) {
					//I am a receiver if other party idx < mine
					recv[pIdx].init(nParties, setSize, psiSecParam, bitSize, chls[pIdx], otCountRecv, otRecv[pIdx], otSend[pIdx], ZeroBlock, true);
				}
				else if (pIdx > myIdx) {
					send[pIdx - myIdx - 1].init(nParties, setSize, psiSecParam, bitSize, chls[pIdx], otCountSend, otSend[pIdx], otRecv[pIdx], prng.get<block>(), true);
				}
			});
		}

		for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx)
			pThrds[pIdx].join();

		auto initDone = timer.setTimePoint("initDone");

#ifdef PRINT
		std::cout << IoStream::lock;
		if (myIdx == 0)
		{
			Log::out << otSend[2].mGens[0].get<block>() << Log::endl;
			if (otRecv[2].hasBaseOts())
			{
				Log::out << otRecv[2].mGens[0][0].get<block>() << Log::endl;
				Log::out << otRecv[2].mGens[0][1].get<block>() << Log::endl;
			}
			Log::out << "------------" << Log::endl;
		}
		if (myIdx == 2)
		{
			if (otSend[0].hasBaseOts())
				Log::out << otSend[0].mGens[0].get<block>() << Log::endl;

			Log::out << otRecv[0].mGens[0][0].get<block>() << Log::endl;
			Log::out << otRecv[0].mGens[0][1].get<block>() << Log::endl;
		}
		std::cout << IoStream::unlock;
#endif

		//##########################
		//### Hashing
		//##########################
		bins.hashing2Bins(set, 1);

		//if(myIdx==0)
		//	bins.mSimpleBins.print(myIdx, true, false, false, false);
		//if (myIdx == 2)
		//	bins.mCuckooBins.print(myIdx, true, false, false);

		auto hashingDone = timer.setTimePoint("hashingDone");
		//##########################
		//### Online Phasing - compute OPRF
		//##########################

		pThrds.clear();
		pThrds.resize(nParties);
		for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx)
		{
			pThrds[pIdx] = std::thread([&, pIdx]() {
				if (pIdx < myIdx) {
					//I am a receiver if other party idx < mine
					recv[pIdx].getOPRFkeys(pIdx, bins, chls[pIdx], true);
				}
				else if (pIdx > myIdx) {
					send[pIdx - myIdx - 1].getOPRFKeys(pIdx, bins, chls[pIdx], true);
				}
			});
		}

		for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx)
			pThrds[pIdx].join();

		//if (myIdx == 0)
		//{
		//	bins.mSimpleBins.print(2, true, true, false, false);
		//	//bins.mCuckooBins.print(2, true, false, false);
		//	Log::out << "------------" << Log::endl;
		//}
		//if (myIdx == 2)
		//{
		//	//bins.mSimpleBins.print(myIdx, true, false, false, false);
		//	bins.mCuckooBins.print(0, true, true, false);
		//}

		auto getOPRFDone = timer.setTimePoint("getOPRFDone");

		//##########################
		//### online phasing - secretsharing
		//##########################
		pThrds.clear();
		pThrds.resize(nParties);

		for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx)
		{
			pThrds[pIdx] = std::thread([&, pIdx]() {
				if ((pIdx < myIdx && pIdx != prevNeighbor)) {
					//I am a receiver if other party idx < mine				
					recv[pIdx].revSecretSharing(pIdx, bins, recvPayLoads[pIdx], chls[pIdx]);
					recv[pIdx].sendSecretSharing(pIdx, bins, sendPayLoads[pIdx], chls[pIdx]);
				}
				else if (pIdx > myIdx && pIdx != nextNeighbor) {
					send[pIdx - myIdx - 1].sendSecretSharing(pIdx, bins, sendPayLoads[pIdx], chls[pIdx]);
					send[pIdx - myIdx - 1].revSecretSharing(pIdx, bins, recvPayLoads[pIdx], chls[pIdx]);
				}

				else if (pIdx == prevNeighbor && myIdx != 0) {
					recv[pIdx].sendSecretSharing(pIdx, bins, sendPayLoads[pIdx], chls[pIdx]);
				}
				else if (pIdx == nextNeighbor && myIdx != nParties - 1)
				{
					send[pIdx - myIdx - 1].revSecretSharing(pIdx, bins, recvPayLoads[pIdx], chls[pIdx]);
				}

				else if (pIdx == nParties - 1 && myIdx == 0) {
					send[pIdx - myIdx - 1].sendSecretSharing(pIdx, bins, sendPayLoads[pIdx], chls[pIdx]);
				}

				else if (pIdx == 0 && myIdx == nParties - 1)
				{
					recv[pIdx].revSecretSharing(pIdx, bins, recvPayLoads[pIdx], chls[pIdx]);
				}

			});
		}

		for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx)
			pThrds[pIdx].join();

		auto getSSDone2Dir = timer.setTimePoint("getSSDone2Dir");

#ifdef PRINT
		std::cout << IoStream::lock;
		if (myIdx == 0)
		{
			for (int i = 0; i < 3; i++)
			{
				block temp = ZeroBlock;
				memcpy((u8*)&temp, (u8*)&sendPayLoads[2][i], maskSize);
				Log::out << "s " << myIdx << " - 2: Idx" << i << " - " << temp << Log::endl;

				block temp1 = ZeroBlock;
				memcpy((u8*)&temp1, (u8*)&recvPayLoads[2][i], maskSize);
				Log::out << "r " << myIdx << " - 2: Idx" << i << " - " << temp1 << Log::endl;
			}
			Log::out << "------------" << Log::endl;
		}
		if (myIdx == 2)
		{
			for (int i = 0; i < 3; i++)
			{
				block temp = ZeroBlock;
				memcpy((u8*)&temp, (u8*)&recvPayLoads[0][i], maskSize);
				Log::out << "r " << myIdx << " - 0: Idx" << i << " - " << temp << Log::endl;

				block temp1 = ZeroBlock;
				memcpy((u8*)&temp1, (u8*)&sendPayLoads[0][i], maskSize);
				Log::out << "s " << myIdx << " - 0: Idx" << i << " - " << temp1 << Log::endl;
			}
			Log::out << "------------" << Log::endl;
		}
		std::cout << IoStream::unlock;
#endif
		//##########################
		//### online phasing - secretsharing - round
		//##########################

		if (myIdx == 0)
		{
			// Xor the received shares
			for (u64 i = 0; i < setSize; ++i)
			{
				for (u64 idxP = 0; idxP < nParties; ++idxP)
				{
					if (idxP != myIdx && idxP != prevNeighbor)
						sendPayLoads[nextNeighbor][i] = sendPayLoads[nextNeighbor][i] ^ recvPayLoads[idxP][i];
				}
			}

			send[nextNeighbor].sendSecretSharing(nextNeighbor, bins, sendPayLoads[nextNeighbor], chls[nextNeighbor]);
			send[nextNeighbor - myIdx - 1].revSecretSharing(prevNeighbor, bins, recvPayLoads[prevNeighbor], chls[prevNeighbor]);

		}
		else if (myIdx == nParties - 1)
		{
			recv[prevNeighbor].revSecretSharing(prevNeighbor, bins, recvPayLoads[prevNeighbor], chls[prevNeighbor]);

			//Xor the received shares 	
			for (u64 i = 0; i < setSize; ++i)
			{
				sendPayLoads[nextNeighbor][i] = sendPayLoads[nextNeighbor][i] ^ recvPayLoads[prevNeighbor][i];
				for (u64 idxP = 0; idxP < nParties; ++idxP)
				{
					if (idxP != myIdx && idxP != prevNeighbor)
						sendPayLoads[nextNeighbor][i] = sendPayLoads[nextNeighbor][i] ^ recvPayLoads[idxP][i];
				}
			}

			recv[nextNeighbor].sendSecretSharing(nextNeighbor, bins, sendPayLoads[nextNeighbor], chls[nextNeighbor]);

		}
		else
		{
			recv[prevNeighbor].revSecretSharing(prevNeighbor, bins, recvPayLoads[prevNeighbor], chls[prevNeighbor]);
			//Xor the received shares 	
			for (u64 i = 0; i < setSize; ++i)
			{
				sendPayLoads[nextNeighbor][i] = sendPayLoads[nextNeighbor][i] ^ recvPayLoads[prevNeighbor][i];
				for (u64 idxP = 0; idxP < nParties; ++idxP)
				{
					if (idxP != myIdx && idxP != prevNeighbor)
						sendPayLoads[nextNeighbor][i] = sendPayLoads[nextNeighbor][i] ^ recvPayLoads[idxP][i];
				}
			}
			send[nextNeighbor - myIdx - 1].sendSecretSharing(nextNeighbor, bins, sendPayLoads[nextNeighbor], chls[nextNeighbor]);
		}

		auto getSSDoneRound = timer.setTimePoint("getSSDoneRound");


#ifdef PRINT
		std::cout << IoStream::lock;
		if (myIdx == 0)
		{
			for (int i = 0; i < 5; i++)
			{
				block temp = ZeroBlock;
				memcpy((u8*)&temp, (u8*)&sendPayLoads[1][i], maskSize);
				Log::out << myIdx << " - " << temp << Log::endl;
				//Log::out << recvPayLoads[2][i] << Log::endl;
			}
			Log::out << "------------" << Log::endl;
		}
		if (myIdx == 1)
		{
			for (int i = 0; i < 5; i++)
			{
				block temp = ZeroBlock;
				memcpy((u8*)&temp, (u8*)&recvPayLoads[0][i], maskSize);
				Log::out << myIdx << " - " << temp << Log::endl;
				//Log::out << sendPayLoads[0][i] << Log::endl;
			}
		}
		std::cout << IoStream::unlock;
#endif

		//##########################
		//### online phasing - compute intersection
		//##########################

		if (myIdx == 0) {
			std::vector<u64> mIntersection;
			u64 maskSize = roundUpTo(psiSecParam + 2 * std::log(setSize) - 1, 8) / 8;
			for (u64 i = 0; i < setSize; ++i)
			{
				if (!memcmp((u8*)&sendPayLoads[myIdx][i], &recvPayLoads[prevNeighbor][i], maskSize))
				{
					mIntersection.push_back(i);
				}
			}
			Log::out << "mIntersection.size(): " << mIntersection.size() << Log::endl;
		}
		auto getIntersection = timer.setTimePoint("getIntersection");


		if (myIdx == 0) {
			auto offlineTime = std::chrono::duration_cast<std::chrono::milliseconds>(initDone - start).count();
			auto hashingTime = std::chrono::duration_cast<std::chrono::milliseconds>(hashingDone - initDone).count();
			auto getOPRFTime = std::chrono::duration_cast<std::chrono::milliseconds>(getOPRFDone - hashingDone).count();
			auto ss2DirTime = std::chrono::duration_cast<std::chrono::milliseconds>(getSSDone2Dir - getOPRFDone).count();
			auto ssRoundTime = std::chrono::duration_cast<std::chrono::milliseconds>(getSSDoneRound - getSSDone2Dir).count();
			auto intersectionTime = std::chrono::duration_cast<std::chrono::milliseconds>(getIntersection - getSSDoneRound).count();

			double onlineTime = hashingTime + getOPRFTime + ss2DirTime + ssRoundTime + intersectionTime;

			double time = offlineTime + onlineTime;
			time /= 1000;

			std::cout << "setSize: " << setSize << "\n"
				<< "offlineTime:  " << offlineTime << " ms\n"
				<< "hashingTime:  " << hashingTime << " ms\n"
				<< "getOPRFTime:  " << getOPRFTime << " ms\n"
				<< "ss2DirTime:  " << ss2DirTime << " ms\n"
				<< "ssRoundTime:  " << ssRoundTime << " ms\n"
				<< "intersection:  " << intersectionTime << " ms\n"
				<< "onlineTime:  " << onlineTime << " ms\n"
				<< "Total time: " << time << " s\n"
				<< "------------------\n";


			offlineAvgTime += offlineTime;
			hashingAvgTime += hashingTime;
			getOPRFAvgTime += getOPRFTime;
			ss2DirAvgTime += ss2DirTime;
			ssRoundAvgTime += ssRoundTime;
			intersectionAvgTime += intersectionTime;
			onlineAvgTime += onlineTime;

		}
		
	}

	if (myIdx == 0) {
		double avgTime = (offlineAvgTime + onlineAvgTime);
		avgTime /= 1000;
		std::cout << "=========avg==========\n"
			<< "setSize: " << setSize << "\n"
			<< "offlineTime:  " << offlineAvgTime / numTrial << " ms\n"
			<< "hashingTime:  " << hashingAvgTime / numTrial << " ms\n"
			<< "getOPRFTime:  " << getOPRFAvgTime / numTrial << " ms\n"
			<< "ss2DirTime:  " << ss2DirAvgTime << " ms\n"
			<< "ssRoundTime:  " << ssRoundAvgTime << " ms\n"
			<< "intersection:  " << intersectionAvgTime / numTrial << " ms\n"
			<< "onlineTime:  " << onlineAvgTime / numTrial << " ms\n"
			<< "Total time: " << avgTime / numTrial << " s\n";
		runtime << "setSize: " << setSize << "\n"
			<< "offlineTime:  " << offlineAvgTime / numTrial << " ms\n"
			<< "hashingTime:  " << hashingAvgTime / numTrial << " ms\n"
			<< "getOPRFTime:  " << getOPRFAvgTime / numTrial << " ms\n"
			<< "ss2DirTime:  " << ss2DirAvgTime << " ms\n"
			<< "ssRoundTime:  " << ssRoundAvgTime << " ms\n"
			<< "intersection:  " << intersectionAvgTime / numTrial << " ms\n"
			<< "onlineTime:  " << onlineAvgTime / numTrial << " ms\n"
			<< "Total time: " << avgTime / numTrial << " s\n";
		runtime.close();
	}

	for (u64 i = 0; i < nParties; ++i)
	{
		if (i != myIdx)
		{
			for (u64 j = 0; j < numThreads; ++j)
			{
				chls[i][j]->close();
			}
		}
	}

	for (u64 i = 0; i < nParties; ++i)
	{
		if (i != myIdx)
			ep[i].stop();
	}


	ios.stop();
}

void party3(u64 myIdx, u64 setSize, std::vector<block>& mSet)
{
	std::fstream runtime;
	if (myIdx == 0)
		runtime.open("./runtime3.txt", runtime.trunc | runtime.out);

	u64 offlineAvgTime(0), hashingAvgTime(0), getOPRFAvgTime(0),
		secretSharingAvgTime(0), intersectionAvgTime(0), onlineAvgTime(0);

	u64  psiSecParam = 40, bitSize = 128, numThreads = 1;
	PRNG prng(_mm_set_epi32(4253465, 3434565, 234435, 23987045));

	std::string name("psi");
	BtIOService ios(0);

	int btCount = nParties;
	std::vector<BtEndpoint> ep(nParties);

	u64 offlineTimeTot(0);
	u64 onlineTimeTot(0);
	Timer timer;

	for (u64 i = 0; i < nParties; ++i)
	{
		if (i < myIdx)
		{
			u32 port = 1120 + i * 100 + myIdx;//get the same port; i=1 & pIdx=2 =>port=102
			ep[i].start(ios, "localhost", port, false, name); //channel bwt i and pIdx, where i is sender
		}
		else if (i > myIdx)
		{
			u32 port = 1120 + myIdx * 100 + i;//get the same port; i=2 & pIdx=1 =>port=102
			ep[i].start(ios, "localhost", port, true, name); //channel bwt i and pIdx, where i is receiver
		}
	}


	std::vector<std::vector<Channel*>> chls(nParties);

	for (u64 i = 0; i < nParties; ++i)
	{
		if (i != myIdx) {
			chls[i].resize(numThreads);
			for (u64 j = 0; j < numThreads; ++j)
			{
				//chls[i][j] = &ep[i].addChannel("chl" + std::to_string(j), "chl" + std::to_string(j));
				chls[i][j] = &ep[i].addChannel(name, name);
			}
		}
	}

	for (u64 idxTrial = 0; idxTrial < numTrial; idxTrial++)
	{
		std::vector<block> set(setSize);
		for (u64 i = 0; i < setSize; ++i)
			set[i] = mSet[i];

		PRNG prng1(_mm_set_epi32(4253465, myIdx, myIdx, myIdx)); //for test
		set[0] = prng1.get<block>();;

		std::vector<block> sendPayLoads(setSize);
		std::vector<block> recvPayLoads(setSize);

		//only P0 genaretes secret sharing
		if (myIdx == 0)
		{
			for (u64 i = 0; i < setSize; ++i)
				sendPayLoads[i] = prng.get<block>();
		}

		std::vector<KkrtNcoOtReceiver> otRecv(nParties);
		std::vector<KkrtNcoOtSender> otSend(nParties);

		OPPRFSender send;
		OPPRFReceiver recv;
		binSet bins;

		std::vector<std::thread>  pThrds(nParties);

		//##########################
		//### Offline Phasing
		//##########################

		auto start = timer.setTimePoint("start");

		bins.init(myIdx, nParties, setSize, psiSecParam);
		u64 otCountSend = bins.mSimpleBins.mBins.size();
		u64 otCountRecv = bins.mCuckooBins.mBins.size();

		u64 nextNeibough = (myIdx + 1) % nParties;
		u64 prevNeibough = (myIdx - 1 + nParties) % nParties;

		for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx)
		{
			pThrds[pIdx] = std::thread([&, pIdx]() {
				if (pIdx == nextNeibough) {
					//I am a sender to my next neigbour
					send.init(nParties, setSize, psiSecParam, bitSize, chls[pIdx], otCountSend, otSend[pIdx], otRecv[pIdx], prng.get<block>(), false);

				}
				else if (pIdx == prevNeibough) {
					//I am a recv to my previous neigbour
					recv.init(nParties, setSize, psiSecParam, bitSize, chls[pIdx], otCountRecv, otRecv[pIdx], otSend[pIdx], ZeroBlock, false);
				}
			});
		}

		for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx)
			pThrds[pIdx].join();

#ifdef PRINT
		std::cout << IoStream::lock;
		if (myIdx == 0)
		{
			Log::out << "------0------" << Log::endl;
			Log::out << otSend[1].mGens[0].get<block>() << Log::endl;
			Log::out << otRecv[2].mGens[0][0].get<block>() << Log::endl;
			Log::out << otRecv[2].mGens[0][1].get<block>() << Log::endl;
		}
		if (myIdx == 1)
		{
			Log::out << "------1------" << Log::endl;
			Log::out << otRecv[0].mGens[0][0].get<block>() << Log::endl;
			Log::out << otRecv[0].mGens[0][1].get<block>() << Log::endl;
			Log::out << otSend[2].mGens[0].get<block>() << Log::endl;
		}

		if (myIdx == 2)
		{
			Log::out << "------2------" << Log::endl;
			Log::out << otRecv[1].mGens[0][0].get<block>() << Log::endl;
			Log::out << otRecv[1].mGens[0][1].get<block>() << Log::endl;
			Log::out << otSend[0].mGens[0].get<block>() << Log::endl;
		}
		std::cout << IoStream::unlock;
#endif

		auto initDone = timer.setTimePoint("initDone");

		//##########################
		//### Hashing
		//##########################
		bins.hashing2Bins(set, nParties);
		//bins.mSimpleBins.print(myIdx, true, false, false, false);
		//bins.mCuckooBins.print(myIdx, true, false, false);

		auto hashingDone = timer.setTimePoint("hashingDone");

		//##########################
		//### Online Phasing - compute OPRF
		//##########################

		pThrds.clear();
		pThrds.resize(nParties);
		for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx)
		{
			pThrds[pIdx] = std::thread([&, pIdx]() {

				if (pIdx == nextNeibough) {
					//I am a sender to my next neigbour
					send.getOPRFKeys(pIdx, bins, chls[pIdx], false);
				}
				else if (pIdx == prevNeibough) {
					//I am a recv to my previous neigbour
					recv.getOPRFkeys(pIdx, bins, chls[pIdx], false);
				}
			});
		}

		for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx)
			pThrds[pIdx].join();

		//if (myIdx == 2)
		//{
		//	//bins.mSimpleBins.print(2, true, true, false, false);
		//	bins.mCuckooBins.print(1, true, true, false);
		//	Log::out << "------------" << Log::endl;
		//}
		//if (myIdx == 1)
		//{
		//	bins.mSimpleBins.print(2, true, true, false, false);
		//	//bins.mCuckooBins.print(0, true, true, false);
		//}

		auto getOPRFDone = timer.setTimePoint("getOPRFDone");


		//##########################
		//### online phasing - secretsharing
		//##########################

		pThrds.clear();
		pThrds.resize(nParties - 1);

		if (myIdx == 0)
		{
			//for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx)
			//{
			//	pThrds[pIdx] = std::thread([&, pIdx]() {
			//		if (pIdx == 0) {
			//			send.sendSecretSharing(nextNeibough, bins, sendPayLoads, chls[nextNeibough]);
			//		}
			//		else if (pIdx == 1) {
			//			recv.revSecretSharing(prevNeibough, bins, recvPayLoads, chls[prevNeibough]);
			//		}
			//	});
			//}
			send.sendSecretSharing(nextNeibough, bins, sendPayLoads, chls[nextNeibough]);
			recv.revSecretSharing(prevNeibough, bins, recvPayLoads, chls[prevNeibough]);
		}
		else
		{
			/*for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx)
			{
				pThrds[pIdx] = std::thread([&, pIdx]() {
					if (pIdx == 0) {
						recv.revSecretSharing(prevNeibough, bins, recvPayLoads, chls[prevNeibough]);
					}
					else if (pIdx == 1) {
						send.sendSecretSharing(nextNeibough, bins, recvPayLoads, chls[nextNeibough]);
					}
				});
			}	*/
			recv.revSecretSharing(prevNeibough, bins, recvPayLoads, chls[prevNeibough]);
			//sendPayLoads = recvPayLoads;
			send.sendSecretSharing(nextNeibough, bins, recvPayLoads, chls[nextNeibough]);

		}

		/*for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx)
			pThrds[pIdx].join();*/

		auto getSSDone = timer.setTimePoint("getSSDone");

#ifdef PRINT
		std::cout << IoStream::lock;
		if (myIdx == 0)
		{
			for (int i = 0; i < 5; i++)
			{
				Log::out << sendPayLoads[i] << Log::endl;
				//Log::out << recvPayLoads[2][i] << Log::endl;
			}
			Log::out << "------------" << Log::endl;
		}
		if (myIdx == 1)
		{
			for (int i = 0; i < 5; i++)
			{
				//Log::out << recvPayLoads[i] << Log::endl;
				Log::out << sendPayLoads[i] << Log::endl;
			}
		}
		if (myIdx == 2)
		{
			for (int i = 0; i < 5; i++)
			{
				Log::out << sendPayLoads[i] << Log::endl;
			}
		}
		std::cout << IoStream::unlock;
#endif

		//##########################
		//### online phasing - compute intersection
		//##########################

		if (myIdx == 0) {
			std::vector<u64> mIntersection;
			u64 maskSize = roundUpTo(psiSecParam + 2 * std::log(setSize) - 1, 8) / 8;
			for (u64 i = 0; i < setSize; ++i)
			{
				if (!memcmp((u8*)&sendPayLoads[i], &recvPayLoads[i], maskSize))
				{
					mIntersection.push_back(i);
				}
			}
			Log::out << "mIntersection.size(): " << mIntersection.size() << Log::endl;
		}
		auto getIntersection = timer.setTimePoint("getIntersection");



		if (myIdx == 0) {
			auto offlineTime = std::chrono::duration_cast<std::chrono::milliseconds>(initDone - start).count();
			auto hashingTime = std::chrono::duration_cast<std::chrono::milliseconds>(hashingDone - initDone).count();
			auto getOPRFTime = std::chrono::duration_cast<std::chrono::milliseconds>(getOPRFDone - hashingDone).count();
			auto secretSharingTime = std::chrono::duration_cast<std::chrono::milliseconds>(getSSDone - getOPRFDone).count();
			auto intersectionTime = std::chrono::duration_cast<std::chrono::milliseconds>(getIntersection - getSSDone).count();

			double onlineTime = hashingTime + getOPRFTime + secretSharingTime + intersectionTime;

			double time = offlineTime + onlineTime;
			time /= 1000;

			std::cout << "setSize: " << setSize << "\n"
				<< "offlineTime:  " << offlineTime << " ms\n"
				<< "hashingTime:  " << hashingTime << " ms\n"
				<< "getOPRFTime:  " << getOPRFTime << " ms\n"
				<< "secretSharing:  " << secretSharingTime << " ms\n"
				<< "intersection:  " << intersectionTime << " ms\n"
				<< "onlineTime:  " << onlineTime << " ms\n"
				<< "Total time: " << time << " s\n"
				<< "------------------\n";

			offlineAvgTime += offlineTime;
			hashingAvgTime += hashingTime;
			getOPRFAvgTime += getOPRFTime;
			secretSharingAvgTime += secretSharingTime;
			intersectionAvgTime += intersectionTime;
			onlineAvgTime += onlineTime;
		}
	}

	if (myIdx == 0) {
		double avgTime = (offlineAvgTime + onlineAvgTime);
		avgTime /= 1000;
		std::cout << "=========avg==========\n"
			<< "setSize: " << setSize << "\n"
			<< "offlineTime:  " << offlineAvgTime / numTrial << " ms\n"
			<< "hashingTime:  " << hashingAvgTime / numTrial << " ms\n"
			<< "getOPRFTime:  " << getOPRFAvgTime / numTrial << " ms\n"
			<< "secretSharing:  " << secretSharingAvgTime / numTrial << " ms\n"
			<< "intersection:  " << intersectionAvgTime / numTrial << " ms\n"
			<< "onlineTime:  " << onlineAvgTime / numTrial << " ms\n"
			<< "Total time: " << avgTime / numTrial << " s\n";
		runtime << "setSize: " << setSize << "\n"
			<< "offlineTime:  " << offlineAvgTime / numTrial << " ms\n"
			<< "hashingTime:  " << hashingAvgTime / numTrial << " ms\n"
			<< "getOPRFTime:  " << getOPRFAvgTime / numTrial << " ms\n"
			<< "secretSharing:  " << secretSharingAvgTime / numTrial << " ms\n"
			<< "intersection:  " << intersectionAvgTime / numTrial << " ms\n"
			<< "onlineTime:  " << onlineAvgTime / numTrial << " ms\n"
			<< "Total time: " << avgTime / numTrial << " s\n";
		runtime.close();
	}

	for (u64 i = 0; i < nParties; ++i)
	{
		if (i != myIdx)
		{
			for (u64 j = 0; j < numThreads; ++j)
			{
				chls[i][j]->close();
			}
		}
	}

	for (u64 i = 0; i < nParties; ++i)
	{
		if (i != myIdx)
			ep[i].stop();
	}


	ios.stop();
}

void party2(u64 myIdx, u64 setSize)
{
	nParties = 2;
	u64  psiSecParam = 40, bitSize = 128, numThreads = 1;
	PRNG prng(_mm_set_epi32(4253465, 3434565, 234435, 23987045));

	std::vector<block> set(setSize);
	for (u64 i = 0; i < setSize; ++i)
		set[i] = mSet[i];

	PRNG prng1(_mm_set_epi32(4253465, myIdx, myIdx, myIdx)); //for test
	set[0] = prng1.get<block>();;

	std::vector<block> sendPayLoads(setSize);
	std::vector<block> recvPayLoads(setSize);

	//only P0 genaretes secret sharing
	if (myIdx == 0)
	{
		for (u64 i = 0; i < setSize; ++i)
			sendPayLoads[i] = prng.get<block>();
	}


	std::string name("psi");
	BtIOService ios(0);

	int btCount = nParties;
	std::vector<BtEndpoint> ep(nParties);

	u64 offlineTimeTot(0);
	u64 onlineTimeTot(0);
	Timer timer;

	for (u64 i = 0; i < nParties; ++i)
	{
		if (i < myIdx)
		{
			u32 port = 1210 + i * 10 + myIdx;//get the same port; i=1 & pIdx=2 =>port=102
			ep[i].start(ios, "localhost", port, false, name); //channel bwt i and pIdx, where i is sender
		}
		else if (i > myIdx)
		{
			u32 port = 1210 + myIdx * 10 + i;//get the same port; i=2 & pIdx=1 =>port=102
			ep[i].start(ios, "localhost", port, true, name); //channel bwt i and pIdx, where i is receiver
		}
	}


	std::vector<std::vector<Channel*>> chls(nParties);

	for (u64 i = 0; i < nParties; ++i)
	{
		if (i != myIdx) {
			chls[i].resize(numThreads);
			for (u64 j = 0; j < numThreads; ++j)
			{
				//chls[i][j] = &ep[i].addChannel("chl" + std::to_string(j), "chl" + std::to_string(j));
				chls[i][j] = &ep[i].addChannel(name, name);
			}
		}
	}

	std::vector<KkrtNcoOtReceiver> otRecv(nParties);
	std::vector<KkrtNcoOtSender> otSend(nParties);

	OPPRFSender send;
	OPPRFReceiver recv;
	binSet bins;

	std::vector<std::thread>  pThrds(nParties);

	//##########################
	//### Offline Phasing
	//##########################

	auto start = timer.setTimePoint("start");

	bins.init(myIdx, nParties, setSize, psiSecParam);
	u64 otCountSend = bins.mSimpleBins.mBins.size();
	u64 otCountRecv = bins.mCuckooBins.mBins.size();


	if (myIdx == 0) {
		//I am a sender to my next neigbour
		send.init(nParties, setSize, psiSecParam, bitSize, chls[1], otCountSend, otSend[1], otRecv[1], prng.get<block>(), false);

	}
	else if (myIdx == 1) {
		//I am a recv to my previous neigbour
		recv.init(nParties, setSize, psiSecParam, bitSize, chls[0], otCountRecv, otRecv[0], otSend[0], ZeroBlock, false);
	}


#ifdef PRINT
	std::cout << IoStream::lock;
	if (myIdx == 0)
	{
		Log::out << "------0------" << Log::endl;
		Log::out << otSend[1].mGens[0].get<block>() << Log::endl;
		Log::out << otRecv[2].mGens[0][0].get<block>() << Log::endl;
		Log::out << otRecv[2].mGens[0][1].get<block>() << Log::endl;
	}
	if (myIdx == 1)
	{
		Log::out << "------1------" << Log::endl;
		Log::out << otRecv[0].mGens[0][0].get<block>() << Log::endl;
		Log::out << otRecv[0].mGens[0][1].get<block>() << Log::endl;
		Log::out << otSend[2].mGens[0].get<block>() << Log::endl;
	}

	if (myIdx == 2)
	{
		Log::out << "------2------" << Log::endl;
		Log::out << otRecv[1].mGens[0][0].get<block>() << Log::endl;
		Log::out << otRecv[1].mGens[0][1].get<block>() << Log::endl;
		Log::out << otSend[0].mGens[0].get<block>() << Log::endl;
	}
	std::cout << IoStream::unlock;
#endif

	auto initDone = timer.setTimePoint("initDone");

	//##########################
	//### Hashing
	//##########################
	bins.hashing2Bins(set, 1);
	//bins.mSimpleBins.print(myIdx, true, false, false, false);
	//bins.mCuckooBins.print(myIdx, true, false, false);

	auto hashingDone = timer.setTimePoint("hashingDone");

	//##########################
	//### Online Phasing - compute OPRF
	//##########################


	if (myIdx == 0) {
		//I am a sender to my next neigbour
		send.getOPRFKeys(1, bins, chls[1], false);
	}
	else if (myIdx == 1) {
		//I am a recv to my previous neigbour
		recv.getOPRFkeys(0, bins, chls[0], false);
	}


	//if (myIdx == 2)
	//{
	//	//bins.mSimpleBins.print(2, true, true, false, false);
	//	bins.mCuckooBins.print(1, true, true, false);
	//	Log::out << "------------" << Log::endl;
	//}
	//if (myIdx == 1)
	//{
	//	bins.mSimpleBins.print(2, true, true, false, false);
	//	//bins.mCuckooBins.print(0, true, true, false);
	//}

	auto getOPRFDone = timer.setTimePoint("getOPRFDone");


	//##########################
	//### online phasing - secretsharing
	//##########################
	if (myIdx == 0)
	{
		//	send.sendSecretSharing(nextNeibough, bins, sendPayLoads, chls[nextNeibough]);
		//	recv.revSecretSharing(prevNeibough, bins, recvPayLoads, chls[prevNeibough]);

	}
	else
	{
		//	recv.revSecretSharing(prevNeibough, bins, recvPayLoads, chls[prevNeibough]);
		//sendPayLoads = recvPayLoads;
		//	send.sendSecretSharing(nextNeibough, bins, recvPayLoads, chls[nextNeibough]);
	}


#ifdef PRINT
	std::cout << IoStream::lock;
	if (myIdx == 0)
	{
		for (int i = 0; i < 5; i++)
		{
			Log::out << sendPayLoads[i] << Log::endl;
			//Log::out << recvPayLoads[2][i] << Log::endl;
		}
		Log::out << "------------" << Log::endl;
	}
	if (myIdx == 1)
	{
		for (int i = 0; i < 5; i++)
		{
			//Log::out << recvPayLoads[i] << Log::endl;
			Log::out << sendPayLoads[i] << Log::endl;
		}
	}
	if (myIdx == 2)
	{
		for (int i = 0; i < 5; i++)
		{
			Log::out << sendPayLoads[i] << Log::endl;
		}
	}
	std::cout << IoStream::unlock;
#endif

	//##########################
	//### online phasing - compute intersection
	//##########################

	if (myIdx == 0) {
		std::vector<u64> mIntersection;
		u64 maskSize = roundUpTo(psiSecParam + 2 * std::log(setSize) - 1, 8) / 8;
		for (u64 i = 0; i < setSize; ++i)
		{
			if (!memcmp((u8*)&sendPayLoads[i], &recvPayLoads[i], maskSize))
			{
				//	mIntersection.push_back(i);
			}
		}
		Log::out << "mIntersection.size(): " << mIntersection.size() << Log::endl;
	}
	auto end = timer.setTimePoint("getOPRFDone");


	if (myIdx == 0) {
		auto offlineTime = std::chrono::duration_cast<std::chrono::milliseconds>(initDone - start).count();
		auto hashingTime = std::chrono::duration_cast<std::chrono::milliseconds>(hashingDone - initDone).count();
		auto getOPRFTime = std::chrono::duration_cast<std::chrono::milliseconds>(getOPRFDone - hashingDone).count();
		auto endTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - getOPRFDone).count();

		double time = offlineTime + hashingTime + getOPRFTime + endTime;
		time /= 1000;

		std::cout << "setSize: " << setSize << "\n"
			<< "offlineTime:  " << offlineTime << "\n"
			<< "hashingTime:  " << hashingTime << "\n"
			<< "getOPRFTime:  " << getOPRFTime << "\n"
			<< "secretSharing:  " << endTime << "\n"
			<< "onlineTime:  " << hashingTime + getOPRFTime + endTime << "\n"
			<< "time: " << time << std::endl;
	}

	for (u64 i = 0; i < nParties; ++i)
	{
		if (i != myIdx)
		{
			for (u64 j = 0; j < numThreads; ++j)
			{
				chls[i][j]->close();
			}
		}
	}

	for (u64 i = 0; i < nParties; ++i)
	{
		if (i != myIdx)
			ep[i].stop();
	}


	ios.stop();
}
bool is_in_dual_area(u64 startIdx, u64 endIdx, u64 numIdx, u64 checkIdx) {
	bool res = false;
	if (startIdx <= endIdx)
	{
		if (startIdx <= checkIdx && checkIdx <= endIdx)
			res = true;
	}
	else //crosing 0, e.i, areas: startIdx....n-1, 0...endIdx
	{
		if ((0 <= checkIdx && checkIdx <= endIdx) //0...endIdx
			|| (startIdx <= checkIdx && checkIdx <= numIdx))
			//startIdx...n-1
			res = true;
	}
	return res;
}

//leader is n-1
void tparty(u64 myIdx, u64 nParties, u64 tParties, u64 setSize, std::vector<block>& mSet, u64 nTrials)
{

#pragma region setup



	//nParties = 4;
	/*std::fstream runtime;
	if (myIdx == 0)
	runtime.open("./runtime" + nParties, runtime.trunc | runtime.out);*/

	u64 leaderIdx = nParties - 1; //leader party
	u64 nSS = nParties - 1; //n-2 parties joinly operated secrete sharing
	int tSS = tParties; //ss with t next  parties, and last for leader => t+1  


	u64 offlineAvgTime(0), hashingAvgTime(0), getOPRFAvgTime(0),
		ss2DirAvgTime(0), ssRoundAvgTime(0), intersectionAvgTime(0), onlineAvgTime(0);

	u64  psiSecParam = 40, bitSize = 128, numThreads = 1;
	PRNG prng(_mm_set_epi32(4253465, 3434565, myIdx, myIdx));

	std::string name("psi");
	BtIOService ios(0);


	std::vector<BtEndpoint> ep(nParties);

	for (u64 i = 0; i < nParties; ++i)
	{
		if (i < myIdx)
		{
			u32 port = 1120 + i * 100 + myIdx;;//get the same port; i=1 & pIdx=2 =>port=102
			ep[i].start(ios, "localhost", port, false, name); //channel bwt i and pIdx, where i is sender
		}
		else if (i > myIdx)
		{
			u32 port = 1120 + myIdx * 100 + i;//get the same port; i=2 & pIdx=1 =>port=102
			ep[i].start(ios, "localhost", port, true, name); //channel bwt i and pIdx, where i is receiver
		}
	}

	std::vector<std::vector<Channel*>> chls(nParties);
	std::vector<u8> dummy(nParties);
	std::vector<u8> revDummy(nParties);

	for (u64 i = 0; i < nParties; ++i)
	{
		dummy[i] = myIdx * 10 + i;

		if (i != myIdx) {
			chls[i].resize(numThreads);
			for (u64 j = 0; j < numThreads; ++j)
			{
				//chls[i][j] = &ep[i].addChannel("chl" + std::to_string(j), "chl" + std::to_string(j));
				chls[i][j] = &ep[i].addChannel(name, name);
				//chls[i][j].mEndpoint;



			}
		}
	}


	u64 maskSize = roundUpTo(psiSecParam + 2 * std::log(setSize) - 1, 8) / 8;
	u64 nextNeighbor = (myIdx + 1) % nParties;
	u64 prevNeighbor = (myIdx - 1 + nParties) % nParties;

#pragma endregion

	for (u64 idxTrial = 0; idxTrial < nTrials; idxTrial++)
	{
#pragma region input
		std::vector<block> set(setSize);

		std::vector<std::vector<block>>
			sendPayLoads(tParties + 1), //include the last PayLoads to leader
			recvPayLoads(tParties); //received form clients

		for (u64 i = 0; i < setSize; ++i)
		{
			set[i] = mSet[i];
		}
		PRNG prng1(_mm_set_epi32(4253465, 3434565, 234435, myIdx));
		set[0] = prng1.get<block>();;


		if (myIdx != leaderIdx) {//generate share of zero for leader myIDx!=n-1		
			for (u64 idxP = 0; idxP < tParties; ++idxP)
			{
				sendPayLoads[idxP].resize(setSize);
				for (u64 i = 0; i < setSize; ++i)
				{
					sendPayLoads[idxP][i] = prng.get<block>();
				}
			}

			sendPayLoads[tParties].resize(setSize); //share to leader at second phase
			for (u64 i = 0; i < setSize; ++i)
			{
				sendPayLoads[tParties][i] = ZeroBlock;
				for (u64 idxP = 0; idxP < tParties; ++idxP)
				{
					sendPayLoads[tParties][i] =
						sendPayLoads[tParties][i] ^ sendPayLoads[idxP][i];
				}
			}
			for (u64 idxP = 0; idxP < recvPayLoads.size(); ++idxP)
			{
				recvPayLoads[idxP].resize(setSize);
			}

		}
		else
		{
			//leader: dont send; only receive ss from clients
			sendPayLoads.resize(0);//
			recvPayLoads.resize(nParties - 1);
			for (u64 idxP = 0; idxP < recvPayLoads.size(); ++idxP)
			{
				recvPayLoads[idxP].resize(setSize);
			}

		}


#ifdef PRINT	
		std::cout << IoStream::lock;
		if (myIdx != leaderIdx) {
			for (u64 i = 0; i < setSize; ++i)
			{
				block check = ZeroBlock;
				for (u64 idxP = 0; idxP < tParties + 1; ++idxP)
				{
					//if (idxP != myIdx)
					check = check ^ sendPayLoads[idxP][i];
				}
				if (memcmp((u8*)&check, &ZeroBlock, sizeof(block)))
					std::cout << "Error ss values: myIdx: " << myIdx
					<< " value: " << check << std::endl;
			}
		}
		std::cout << IoStream::unlock;
#endif
#pragma endregion
		u64 num_threads = nParties - 1; //except P0, and my
		bool isDual = true;
		u64 idx_start_dual = 0;
		u64 idx_end_dual = 0;
		u64 t_prev_shift = tSS;

		if (myIdx != leaderIdx) {
			if (2 * tSS < nSS)
			{
				num_threads = 2 * tSS + 1;
				isDual = false;
			}
			else {
				idx_start_dual = (myIdx - tSS + nSS) % nSS;
				idx_end_dual = (myIdx + tSS) % nSS;
			}

			/*std::cout << IoStream::lock;
			std::cout << myIdx << "| " << idx_start_dual << " " << idx_end_dual << "\n";
			std::cout << IoStream::unlock;*/
		}
		std::vector<std::thread>  pThrds(num_threads);

		std::vector<KkrtNcoOtReceiver> otRecv(nParties);
		std::vector<KkrtNcoOtSender> otSend(nParties);
		std::vector<OPPRFSender> send(nParties);
		std::vector<OPPRFReceiver> recv(nParties);

		if (myIdx == leaderIdx)
		{
			/*otRecv.resize(nParties - 1);
			otSend.resize(nParties - 1);
			send.resize(nParties - 1);
			recv.resize(nParties - 1);*/
			pThrds.resize(nParties - 1);
		}



		binSet bins;

		//##########################
		//### Offline Phasing
		//##########################
		Timer timer;
		auto start = timer.setTimePoint("start");
		bins.init(myIdx, nParties, setSize, psiSecParam);
		u64 otCountSend = bins.mSimpleBins.mBins.size();
		u64 otCountRecv = bins.mCuckooBins.mBins.size();


#pragma region base OT
		//##########################
		//### Base OT
		//##########################

		if (myIdx != leaderIdx)
		{
			for (u64 pIdx = 0; pIdx < tSS; ++pIdx)
			{
				u64 prevIdx = (myIdx - pIdx - 1 + nSS) % nSS;

				if (!(isDual && is_in_dual_area(idx_start_dual, idx_end_dual, nSS, prevIdx)))
				{
					u64 thr = t_prev_shift + pIdx;

					pThrds[thr] = std::thread([&, prevIdx, thr]() {

						//chls[prevIdx][0]->recv(&revDummy[prevIdx], 1);
						//std::cout << IoStream::lock;
						//std::cout << myIdx << "| : " << "| thr[" << thr << "]:" << prevIdx << " --> " << myIdx << ": " << static_cast<int16_t>(revDummy[prevIdx]) << "\n";
						//std::cout << IoStream::unlock;


						//prevIdx << " --> " << myIdx
						recv[prevIdx].init(nParties, setSize, psiSecParam, bitSize, chls[prevIdx], otCountRecv, otRecv[prevIdx], otSend[prevIdx], ZeroBlock, false);

					});



				}
			}

			for (u64 pIdx = 0; pIdx < tSS; ++pIdx)
			{
				u64 nextIdx = (myIdx + pIdx + 1) % nSS;

				if ((isDual && is_in_dual_area(idx_start_dual, idx_end_dual, nSS, nextIdx))) {

					pThrds[pIdx] = std::thread([&, nextIdx, pIdx]() {


						//dual myIdx << " <-> " << nextIdx 
						if (myIdx < nextIdx)
						{
							//chls[nextIdx][0]->asyncSend(&dummy[nextIdx], 1);
							//std::cout << IoStream::lock;
							//std::cout << myIdx << "| d: " << "| thr[" << pIdx << "]:" << myIdx << " <->> " << nextIdx << ": " << static_cast<int16_t>(dummy[nextIdx]) << "\n";
							//std::cout << IoStream::unlock;

							send[nextIdx].init(nParties, setSize, psiSecParam, bitSize, chls[nextIdx], otCountSend, otSend[nextIdx], otRecv[nextIdx], prng.get<block>(), true);
						}
						else if (myIdx > nextIdx) //by index
						{
	/*						chls[nextIdx][0]->recv(&revDummy[nextIdx], 1);

							std::cout << IoStream::lock;
							std::cout << myIdx << "| d: " << "| thr[" << pIdx << "]:" << myIdx << " <<-> " << nextIdx << ": " << static_cast<int16_t>(revDummy[nextIdx]) << "\n";
							std::cout << IoStream::unlock;*/

							recv[nextIdx].init(nParties, setSize, psiSecParam, bitSize, chls[nextIdx], otCountRecv, otRecv[nextIdx], otSend[nextIdx], ZeroBlock, true);
						}
					});

				}
				else
				{
					pThrds[pIdx] = std::thread([&, nextIdx, pIdx]() {

						//chls[nextIdx][0]->asyncSend(&dummy[nextIdx], 1);
						//std::cout << IoStream::lock;
						//std::cout << myIdx << "| : " << "| thr[" << pIdx << "]:" << myIdx << " -> " << nextIdx << ": " << static_cast<int16_t>(dummy[nextIdx]) << "\n";
						//std::cout << IoStream::unlock;
						send[nextIdx].init(nParties, setSize, psiSecParam, bitSize, chls[nextIdx], otCountSend, otSend[nextIdx], otRecv[nextIdx], prng.get<block>(), false);
					});
				}
			}

			//last thread for connecting with leader
			u64 tLeaderIdx = pThrds.size() - 1;
			pThrds[pThrds.size() - 1] = std::thread([&, leaderIdx]() {

			//	chls[leaderIdx][0]->asyncSend(&dummy[leaderIdx], 1);

				//std::cout << IoStream::lock;
				//std::cout << myIdx << "| : " << "| thr[" << pThrds.size() - 1 << "]:" << myIdx << " --> " << leaderIdx << ": " << static_cast<int16_t>(dummy[leaderIdx]) << "\n";
				//std::cout << IoStream::unlock;

				send[leaderIdx].init(nParties, setSize, psiSecParam, bitSize, chls[leaderIdx], otCountSend, otSend[leaderIdx], otRecv[leaderIdx], prng.get<block>(), false);
			});

		}
		else
		{ //leader party 

			for (u64 pIdx = 0; pIdx < nSS; ++pIdx)
			{
				pThrds[pIdx] = std::thread([&, pIdx]() {
	/*				chls[pIdx][0]->recv(&revDummy[pIdx], 1);
					std::cout << IoStream::lock;
					std::cout << myIdx << "| : " << "| thr[" << pIdx << "]:" << pIdx << " --> " << myIdx << ": " << static_cast<int16_t>(revDummy[pIdx]) << "\n";
					std::cout << IoStream::unlock;*/

					recv[pIdx].init(nParties, setSize, psiSecParam, bitSize, chls[pIdx], otCountRecv, otRecv[pIdx], otSend[pIdx], ZeroBlock, false);
				});

			}
		}

		for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx)
			pThrds[pIdx].join();

		auto initDone = timer.setTimePoint("initDone");


#ifdef PRINT
		std::cout << IoStream::lock;
		if (myIdx == 0)
		{
			Log::out << myIdx << "| -> " << otSend[1].mGens[0].get<block>() << Log::endl;
			if (otRecv[1].hasBaseOts())
			{
				Log::out << myIdx << "| <- " << otRecv[1].mGens[0][0].get<block>() << Log::endl;
				Log::out << myIdx << "| <- " << otRecv[1].mGens[0][1].get<block>() << Log::endl;
			}
			Log::out << "------------" << Log::endl;
		}
		if (myIdx == 1)
		{
			if (otSend[0].hasBaseOts())
				Log::out << myIdx << "| -> " << otSend[0].mGens[0].get<block>() << Log::endl;

			Log::out << myIdx << "| <- " << otRecv[0].mGens[0][0].get<block>() << Log::endl;
			Log::out << myIdx << "| <- " << otRecv[0].mGens[0][1].get<block>() << Log::endl;
		}

		if (isDual)
		{
			if (myIdx == 0)
			{
				Log::out << myIdx << "| <->> " << otSend[tSS].mGens[0].get<block>() << Log::endl;
				if (otRecv[tSS].hasBaseOts())
				{
					Log::out << myIdx << "| <<-> " << otRecv[tSS].mGens[0][0].get<block>() << Log::endl;
					Log::out << myIdx << "| <<-> " << otRecv[tSS].mGens[0][1].get<block>() << Log::endl;
				}
				Log::out << "------------" << Log::endl;
			}
			if (myIdx == tSS)
			{
				if (otSend[0].hasBaseOts())
					Log::out << myIdx << "| <->> " << otSend[0].mGens[0].get<block>() << Log::endl;

				Log::out << myIdx << "| <<-> " << otRecv[0].mGens[0][0].get<block>() << Log::endl;
				Log::out << myIdx << "| <<-> " << otRecv[0].mGens[0][1].get<block>() << Log::endl;
			}
		}
		std::cout << IoStream::unlock;
#endif

#pragma endregion


		//##########################
		//### Hashing
		//##########################
		bins.hashing2Bins(set, 1);

		/*if(myIdx==0)
		bins.mSimpleBins.print(myIdx, true, false, false, false);
		if (myIdx == 1)
		bins.mCuckooBins.print(myIdx, true, false, false);*/

		auto hashingDone = timer.setTimePoint("hashingDone");

#pragma region compute OPRF

		//##########################
		//### Online Phasing - compute OPRF
		//##########################

		pThrds.clear();
		pThrds.resize(num_threads);
		if (myIdx == leaderIdx)
		{
			pThrds.resize(nParties - 1);
		}

		if (myIdx != leaderIdx)
		{
			for (u64 pIdx = 0; pIdx < tSS; ++pIdx)
			{
				u64 prevIdx = (myIdx - pIdx - 1 + nSS) % nSS;

				if (!(isDual && is_in_dual_area(idx_start_dual, idx_end_dual, nSS, prevIdx)))
				{
					u64 thr = t_prev_shift + pIdx;

					pThrds[thr] = std::thread([&, prevIdx]() {

						//prevIdx << " --> " << myIdx
						recv[prevIdx].getOPRFkeys(prevIdx, bins, chls[prevIdx], false);

					});
				}
			}

			for (u64 pIdx = 0; pIdx < tSS; ++pIdx)
			{
				u64 nextIdx = (myIdx + pIdx + 1) % nSS;

				if ((isDual && is_in_dual_area(idx_start_dual, idx_end_dual, nSS, nextIdx))) {

					pThrds[pIdx] = std::thread([&, nextIdx]() {
						//dual myIdx << " <-> " << nextIdx 
						if (myIdx < nextIdx)
						{
							send[nextIdx].getOPRFKeys(nextIdx, bins, chls[nextIdx], true);
						}
						else if (myIdx > nextIdx) //by index
						{
							recv[nextIdx].getOPRFkeys(nextIdx, bins, chls[nextIdx], true);
						}
					});

				}
				else
				{
					pThrds[pIdx] = std::thread([&, nextIdx]() {
						send[nextIdx].getOPRFKeys(nextIdx, bins, chls[nextIdx], false);
					});
				}
			}

			//last thread for connecting with leader
			pThrds[pThrds.size() - 1] = std::thread([&, leaderIdx]() {
				send[leaderIdx].getOPRFKeys(leaderIdx, bins, chls[leaderIdx], false);
			});

		}
		else
		{ //leader party 
			for (u64 pIdx = 0; pIdx < nSS; ++pIdx)
			{
				pThrds[pIdx] = std::thread([&, pIdx]() {
					recv[pIdx].getOPRFkeys(pIdx, bins, chls[pIdx], false);
				});
			}
		}

		for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx)
			pThrds[pIdx].join();

		auto getOPRFDone = timer.setTimePoint("getOPRFDone");


#ifdef BIN_PRINT

		if (myIdx == 0)
		{
			bins.mSimpleBins.print(1, true, true, false, false);
		}
		if (myIdx == 1)
		{
			bins.mCuckooBins.print(0, true, true, false);
		}

		if (isDual)
		{
			if (myIdx == 0)
			{
				bins.mCuckooBins.print(tSS, true, true, false);
			}
			if (myIdx == tSS)
			{
				bins.mSimpleBins.print(0, true, true, false, false);
			}
		}

#endif
#pragma endregion

#pragma region SS

		//##########################
		//### online phasing - secretsharing
		//##########################

		pThrds.clear();

		if (myIdx != leaderIdx)
		{
			pThrds.resize(num_threads);
			for (u64 pIdx = 0; pIdx < tSS; ++pIdx)
			{
				u64 prevIdx = (myIdx - pIdx - 1 + nSS) % nSS;

				if (!(isDual && is_in_dual_area(idx_start_dual, idx_end_dual, nSS, prevIdx)))
				{
					u64 thr = t_prev_shift + pIdx;

					pThrds[thr] = std::thread([&, prevIdx, pIdx]() {

						//prevIdx << " --> " << myIdx
						recv[prevIdx].revSecretSharing(prevIdx, bins, recvPayLoads[pIdx], chls[prevIdx]);

					});
				}
			}

			for (u64 pIdx = 0; pIdx < tSS; ++pIdx)
			{
				u64 nextIdx = (myIdx + pIdx + 1) % nSS;

				if ((isDual && is_in_dual_area(idx_start_dual, idx_end_dual, nSS, nextIdx))) {

					pThrds[pIdx] = std::thread([&, nextIdx, pIdx]() {
						//dual myIdx << " <-> " << nextIdx 
						//send OPRF can receive payload
						if (myIdx < nextIdx)
						{
							send[nextIdx].sendSecretSharing(nextIdx, bins, sendPayLoads[pIdx], chls[nextIdx]);

							send[nextIdx].revSecretSharing(nextIdx, bins, recvPayLoads[pIdx], chls[nextIdx]);
						}
						else if (myIdx > nextIdx) //by index
						{
							recv[nextIdx].revSecretSharing(nextIdx, bins, recvPayLoads[pIdx], chls[nextIdx]);

							recv[nextIdx].sendSecretSharing(nextIdx, bins, sendPayLoads[pIdx], chls[nextIdx]);

						}
					});

				}
				else
				{
					pThrds[pIdx] = std::thread([&, nextIdx, pIdx]() {
						send[nextIdx].sendSecretSharing(nextIdx, bins, sendPayLoads[pIdx], chls[nextIdx]);
					});
				}
			}

			//last thread for connecting with leader
			pThrds[pThrds.size() - 1] = std::thread([&, leaderIdx]() {
				//send[leaderIdx].getOPRFKeys(leaderIdx, bins, chls[leaderIdx], false);
			});

			for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx)
				pThrds[pIdx].join();
		}

		auto getSSDone2Dir = timer.setTimePoint("secretsharingDone");


#ifdef PRINT
		std::cout << IoStream::lock;
		if (myIdx == 0)
		{
			for (int i = 0; i < 3; i++)
			{
				block temp = ZeroBlock;
				memcpy((u8*)&temp, (u8*)&sendPayLoads[0][i], maskSize);
				Log::out << myIdx << "| -> 1: (" << i << ", " << temp << ")" << Log::endl;
			}
			Log::out << "------------" << Log::endl;
		}
		if (myIdx == 1)
		{
			for (int i = 0; i < 3; i++)
			{
				block temp = ZeroBlock;
				memcpy((u8*)&temp, (u8*)&recvPayLoads[0][i], maskSize);
				Log::out << myIdx << "| <- 0: (" << i << ", " << temp << ")" << Log::endl;
			}
			Log::out << "------------" << Log::endl;
		}

		if (isDual)
		{
			/*if (myIdx == 0)
			{
			for (int i = 0; i < 3; i++)
			{
			block temp = ZeroBlock;
			memcpy((u8*)&temp, (u8*)&recvPayLoads[tSS][i], maskSize);
			Log::out << myIdx << "| <- "<< tSS<<": (" << i << ", " << temp << ")" << Log::endl;
			}
			Log::out << "------------" << Log::endl;
			}
			if (myIdx == tSS)
			{
			for (int i = 0; i < 3; i++)
			{
			block temp = ZeroBlock;
			memcpy((u8*)&temp, (u8*)&sendPayLoads[0][i], maskSize);
			Log::out << myIdx << "| -> 0: (" << i << ", " << temp << ")" << Log::endl;
			}
			Log::out << "------------" << Log::endl;
			}*/
		}

		std::cout << IoStream::unlock;
#endif
#pragma endregion

		//##########################
		//### online phasing - send XOR of zero share to leader
		//##########################
		pThrds.clear();

		if (myIdx != leaderIdx)
		{

			for (u64 i = 0; i < setSize; ++i)
			{
				//xor all received share
				for (u64 idxP = 0; idxP < tParties; ++idxP)
				{
					sendPayLoads[tParties][i] = sendPayLoads[tParties][i] ^ recvPayLoads[idxP][i];
				}
			}
			//send to leader
			send[leaderIdx].sendSecretSharing(leaderIdx, bins, sendPayLoads[tParties], chls[leaderIdx]);
		}
		else
		{
			pThrds.resize(nParties - 1);

			for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx) {
				pThrds[pIdx] = std::thread([&, pIdx]() {
					recv[pIdx].revSecretSharing(pIdx, bins, recvPayLoads[pIdx], chls[pIdx]);
				});
			}

			for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx)
				pThrds[pIdx].join();
		}


		auto getSSDoneRound = timer.setTimePoint("leaderGetXorDone");


		//##########################
		//### online phasing - compute intersection
		//##########################

		if (myIdx == leaderIdx) {
			std::vector<u64> mIntersection;
			u64 maskSize = roundUpTo(psiSecParam + 2 * std::log(setSize) - 1, 8) / 8;

			for (u64 i = 0; i < setSize; ++i)
			{

				//xor all received share
				block sum = ZeroBlock;
				for (u64 idxP = 0; idxP < nParties - 1; ++idxP)
				{
					sum = sum ^ recvPayLoads[idxP][i];
				}

				if (!memcmp((u8*)&ZeroBlock, &sum, maskSize))
				{
					mIntersection.push_back(i);
				}
			}
			Log::out << "mIntersection.size(): " << mIntersection.size() << Log::endl;
		}
		auto getIntersection = timer.setTimePoint("getIntersection");


		if (myIdx == 0) {
			auto offlineTime = std::chrono::duration_cast<std::chrono::milliseconds>(initDone - start).count();
			auto hashingTime = std::chrono::duration_cast<std::chrono::milliseconds>(hashingDone - initDone).count();
			auto getOPRFTime = std::chrono::duration_cast<std::chrono::milliseconds>(getOPRFDone - hashingDone).count();
			auto ss2DirTime = std::chrono::duration_cast<std::chrono::milliseconds>(getSSDone2Dir - getOPRFDone).count();
			auto ssRoundTime = std::chrono::duration_cast<std::chrono::milliseconds>(getSSDoneRound - getSSDone2Dir).count();
			auto intersectionTime = std::chrono::duration_cast<std::chrono::milliseconds>(getIntersection - getSSDoneRound).count();

			double onlineTime = hashingTime + getOPRFTime + ss2DirTime + ssRoundTime + intersectionTime;

			double time = offlineTime + onlineTime;
			time /= 1000;

			std::cout << "setSize: " << setSize << "\n"
				<< "offlineTime:  " << offlineTime << " ms\n"
				<< "hashingTime:  " << hashingTime << " ms\n"
				<< "getOPRFTime:  " << getOPRFTime << " ms\n"
				<< "ss2DirTime:  " << ss2DirTime << " ms\n"
				<< "ssRoundTime:  " << ssRoundTime << " ms\n"
				<< "intersection:  " << intersectionTime << " ms\n"
				<< "onlineTime:  " << onlineTime << " ms\n"
				<< "Total time: " << time << " s\n"
				<< "------------------\n";


			offlineAvgTime += offlineTime;
			hashingAvgTime += hashingTime;
			getOPRFAvgTime += getOPRFTime;
			ss2DirAvgTime += ss2DirTime;
			ssRoundAvgTime += ssRoundTime;
			intersectionAvgTime += intersectionTime;
			onlineAvgTime += onlineTime;

		}

	}


	/*if (myIdx == 0) {
	double avgTime = (offlineAvgTime + onlineAvgTime);
	avgTime /= 1000;
	std::cout << "=========avg==========\n"
	<< "setSize: " << setSize << "\n"
	<< "offlineTime:  " << offlineAvgTime / numTrial << " ms\n"
	<< "hashingTime:  " << hashingAvgTime / numTrial << " ms\n"
	<< "getOPRFTime:  " << getOPRFAvgTime / numTrial << " ms\n"
	<< "ss2DirTime:  " << ss2DirAvgTime << " ms\n"
	<< "ssRoundTime:  " << ssRoundAvgTime << " ms\n"
	<< "intersection:  " << intersectionAvgTime / numTrial << " ms\n"
	<< "onlineTime:  " << onlineAvgTime / numTrial << " ms\n"
	<< "Total time: " << avgTime / numTrial << " s\n";
	runtime << "setSize: " << setSize << "\n"
	<< "offlineTime:  " << offlineAvgTime / numTrial << " ms\n"
	<< "hashingTime:  " << hashingAvgTime / numTrial << " ms\n"
	<< "getOPRFTime:  " << getOPRFAvgTime / numTrial << " ms\n"
	<< "ss2DirTime:  " << ss2DirAvgTime << " ms\n"
	<< "ssRoundTime:  " << ssRoundAvgTime << " ms\n"
	<< "intersection:  " << intersectionAvgTime / numTrial << " ms\n"
	<< "onlineTime:  " << onlineAvgTime / numTrial << " ms\n"
	<< "Total time: " << avgTime / numTrial << " s\n";
	runtime.close();
	}
	*/
	for (u64 i = 0; i < nParties; ++i)
	{
		if (i != myIdx)
		{
			for (u64 j = 0; j < numThreads; ++j)
			{
				chls[i][j]->close();
			}
		}
	}

	for (u64 i = 0; i < nParties; ++i)
	{
		if (i != myIdx)
			ep[i].stop();
	}


	ios.stop();
}
void OPPRFnt_EmptrySet_Test_Main()
{
	u64 setSize = 1 << 5, psiSecParam = 40, bitSize = 128;
	PRNG prng(_mm_set_epi32(4253465, 3434565, 234435, 23987045));
	mSet.resize(setSize);
	for (u64 i = 0; i < setSize; ++i)
	{
		mSet[i] = prng.get<block>();
	}
	nParties = 5;
	u64 tParties = 2;

	if (tParties == nParties - 1)//max ss = n-1
		tParties--;
	else if (tParties < 1) //make sure to do ss with at least one client
		tParties = 1;
	std::vector<std::thread>  pThrds(nParties);
	for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx)
	{
		//if (pIdx == 0)
		//{
		//	//tparty0(pIdx, nParties, 1, setSize, mSet);
		//}
		//else
		{
			pThrds[pIdx] = std::thread([&, pIdx]() {
				//	Channel_party_test(pIdx);
				tparty(pIdx, nParties, tParties, mSet.size(), mSet, 1);
			});
		}
	}
	for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx)
		pThrds[pIdx].join();


}

void OPPRFn_EmptrySet_Test_Main()
{
	u64 setSize = 1 << 5, psiSecParam = 40, bitSize = 128;
	u64 nParties = 4;
	PRNG prng(_mm_set_epi32(4253465, 3434565, 234435, 23987045));
	mSet.resize(setSize);
	for (u64 i = 0; i < setSize; ++i)
	{
		mSet[i] = prng.get<block>();
	}
	std::vector<std::thread>  pThrds(nParties);
	for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx)
	{
		pThrds[pIdx] = std::thread([&, pIdx]() {
			//	Channel_party_test(pIdx);
			party(pIdx, nParties, setSize, mSet);
		});
	}
	for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx)
		pThrds[pIdx].join();


}

void OPPRF3_EmptrySet_Test_Main()
{
	u64 setSize = 1 << 5, psiSecParam = 40, bitSize = 128;
	PRNG prng(_mm_set_epi32(4253465, 3434565, 234435, 23987045));
	mSet.resize(setSize);
	for (u64 i = 0; i < setSize; ++i)
	{
		mSet[i] = prng.get<block>();
	}
	nParties = 3;
	std::vector<std::thread>  pThrds(nParties);
	for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx)
	{
		pThrds[pIdx] = std::thread([&, pIdx]() {
			//	Channel_party_test(pIdx);
			party3(pIdx, setSize, mSet);
		});
	}
	for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx)
		pThrds[pIdx].join();


}

void OPPRF2_EmptrySet_Test_Main()
{
	u64 setSize = 1 << 20, psiSecParam = 40, bitSize = 128;
	PRNG prng(_mm_set_epi32(4253465, 3434565, 234435, 23987045));
	mSet.resize(setSize);
	for (u64 i = 0; i < setSize; ++i)
	{
		mSet[i] = prng.get<block>();
	}
	std::vector<std::thread>  pThrds(2);
	for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx)
	{
		pThrds[pIdx] = std::thread([&, pIdx]() {
			//	Channel_party_test(pIdx);
			party2(pIdx, setSize);
		});
	}
	for (u64 pIdx = 0; pIdx < pThrds.size(); ++pIdx)
		pThrds[pIdx].join();


}

void Bit_Position_Random_Test()
{
	u64 power = 20;
	u64 setSize = 1 << power;

	PRNG prng(_mm_set_epi32(4253465, 3434565, 234435, 23987045));


	SimpleHasher1 mSimpleBins;
	mSimpleBins.init(setSize);
	std::vector<u64> tempIdxBuff(setSize);
	MatrixView<u64> hashes(setSize, mSimpleBins.mNumHashes[0]);

	for (u64 j = 0; j < setSize; ++j)
	{
		tempIdxBuff[j] = j;
		for (u64 k = 0; k < mSimpleBins.mNumHashes[0]; ++k)
		{
			block a = prng.get<block>();
			hashes[j][k] = *(u64*)&a;
		}
	}

	mSimpleBins.insertBatch(tempIdxBuff, hashes);

	for (u64 bIdx = 0; bIdx < mSimpleBins.mBins.size(); ++bIdx)
	{
		auto& bin = mSimpleBins.mBins[bIdx];
		if (bin.mIdx.size() > 0)
		{
			bin.mValOPRF.resize(1);
			bin.mBits.resize(1);
			bin.mValOPRF[0].resize(bin.mIdx.size());

			for (u64 i = 0; i < bin.mIdx.size(); ++i)
			{
				bin.mValOPRF[0][i] = prng.get<block>();
			}
		}
	}

	Timer mTimer;
	double mTime = 0;

	auto start = mTimer.setTimePoint("getPos1.start");

	for (u64 bIdx = 0; bIdx < mSimpleBins.mBinCount[0]; ++bIdx)
	{
		auto& bin = mSimpleBins.mBins[bIdx];
		if (bin.mIdx.size() > 0)
		{
			bin.mBits[0].init(mSimpleBins.mNumBits[0]);
			bin.mBits[0].getPos1(bin.mValOPRF[0], 128);
		}

	}
	auto mid = mTimer.setTimePoint("getPos1.mid");

	for (u64 bIdx = mSimpleBins.mBinCount[0]; bIdx < mSimpleBins.mBins.size(); ++bIdx)
	{
		auto& bin = mSimpleBins.mBins[bIdx];
		if (bin.mIdx.size() > 0)
		{
			bin.mBits[0].init(mSimpleBins.mNumBits[1]);
			bin.mBits[0].getPos1(bin.mValOPRF[0], 128);
		}
	}

	auto end = mTimer.setTimePoint("getPos1.done");
	double time1 = std::chrono::duration_cast<std::chrono::milliseconds>(mid - start).count();
	double time2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - mid).count();
	double time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	Log::out << "time1= " << time1 << "\n";
	Log::out << "time2= " << time2 << "\n";
	Log::out << "total= " << time << "\n";
	Log::out << "mSimpleBins.mBins.size()= " << mSimpleBins.mBins.size() << "\n";

	Log::out << Log::endl;

}



//void OPPRF_EmptrySet_Test_Impl1()
//{
//	u64 setSize = 2 << 8, psiSecParam = 40, bitSize = 128;
//	PRNG prng(_mm_set_epi32(4253465, 3434565, 234435, 23987045));
//
//	std::vector<block> sendSet(setSize), recvSet(setSize);
//	for (u64 i = 0; i < setSize; ++i)
//	{
//		sendSet[i] = prng.get<block>();
//		recvSet[i] = prng.get<block>();
//		//recvSet[i] = sendSet[i];
//	}
//	for (u64 i = 1; i < 3; ++i)
//	{
//		recvSet[i] = sendSet[i];
//	}
//
//	std::string name("psi");
//
//	BtIOService ios(0);
//	BtEndpoint ep0(ios, "localhost", 1212, true, name);
//	BtEndpoint ep1(ios, "localhost", 1212, false, name);
//
//
//	std::vector<Channel*> recvChl{ &ep1.addChannel(name, name) };
//	std::vector<Channel*> sendChl{ &ep0.addChannel(name, name) };
//
//	KkrtNcoOtReceiver otRecv;
//	KkrtNcoOtSender otSend;
//
//
//	//u64 baseCount = 128 * 4;
//	//std::vector<std::array<block, 2>> sendBlks(baseCount);
//	//std::vector<block> recvBlks(baseCount);
//	//BitVector choices(baseCount);
//	//choices.randomize(prng);
//
//	//for (u64 i = 0; i < baseCount; ++i)
//	//{
//	//    sendBlks[i][0] = prng.get<block>();
//	//    sendBlks[i][1] = prng.get<block>();
//	//    recvBlks[i] = sendBlks[i][choices[i]];
//	//}
//
//	//otRecv.setBaseOts(sendBlks);
//	//otSend.setBaseOts(recvBlks, choices);
//
//	//for (u64 i = 0; i < baseCount; ++i)
//	//{
//	//    sendBlks[i][0] = prng.get<block>();
//	//    sendBlks[i][1] = prng.get<block>();
//	//    recvBlks[i] = sendBlks[i][choices[i]];
//	//}
//
//
//	OPPRFSender send;
//	OPPRFReceiver recv;
//	std::thread thrd([&]() {
//
//
//		send.init(setSize, psiSecParam, bitSize, sendChl, otSend, prng.get<block>());
//		send.sendInput(sendSet, sendChl);
//		//Log::out << sendSet[0] << Log::endl;
//		//	send.mBins.print();
//
//
//	});
//
//	recv.init(setSize, psiSecParam, bitSize, recvChl, otRecv, ZeroBlock);
//	recv.sendInput(recvSet, recvChl);
//	//Log::out << recvSet[0] << Log::endl;
//	//recv.mBins.print();
//
//
//	thrd.join();
//
//	sendChl[0]->close();
//	recvChl[0]->close();
//
//	ep0.stop();
//	ep1.stop();
//	ios.stop();
//}

