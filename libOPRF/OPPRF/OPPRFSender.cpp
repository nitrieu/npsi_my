#include "OPPRFSender.h"

#include "Crypto/Commit.h"
#include "Common/Log.h"
#include "Common/Log1.h"
#include "Common/Timer.h"
#include "Base/naor-pinkas.h"
#include "TwoChooseOne/IknpOtExtReceiver.h"
#include "TwoChooseOne/IknpOtExtSender.h"

//#define PRINT
namespace osuCrypto
{

    OPPRFSender::OPPRFSender()
    {
    }
    //const u64 OPPRFSender::hasherStepSize(128);


    OPPRFSender::~OPPRFSender()
    {
    }

    void OPPRFSender::init(u64 numParties, u64 setSize, u64 statSec, u64 inputBitSize,
        Channel & chl0, u64 otCounts,
        NcoOtExtSender&  otSend,
		NcoOtExtReceiver& otRecv,
        block seed, bool isOtherDirection)
    {
        init(numParties, setSize,  statSec, inputBitSize, { &chl0 }, otCounts, otSend, otRecv, seed, isOtherDirection);
    }

    void OPPRFSender::init(u64 numParties, u64 setSize,  u64 statSec, u64 inputBitSize,
        const std::vector<Channel*>& chls, u64 otCounts,
        NcoOtExtSender& otSend,
		NcoOtExtReceiver& otRecv,
        block seed, bool isOtherDirection)
	{
		mStatSecParam = statSec;
		mN = setSize;
		mParties = numParties;
		gTimer.setTimePoint("init.send.start");

		// must be a multiple of 128...
		u64 baseOtCount;// = 128 * CodeWordSize;
						//u64 plaintextBlkSize;

		u64 compSecParam = 128;

		otSend.getParams(
			false, // input, is malicious
			compSecParam, statSec, inputBitSize, mN, //  input
			mNcoInputBlkSize, baseOtCount); // output

		mOtMsgBlkSize = (baseOtCount + 127) / 128;


		mPrng.SetSeed(seed);
		auto myHashSeed = mPrng.get<block>();
		auto& chl0 = *chls[0];


		Commit comm(myHashSeed), theirComm;
		chl0.asyncSend(comm.data(), comm.size());
		chl0.recv(theirComm.data(), theirComm.size());


		chl0.asyncSend(&myHashSeed, sizeof(block));
		block theirHashingSeed;
		chl0.recv(&theirHashingSeed, sizeof(block));

		mHashingSeed = myHashSeed ^ theirHashingSeed;

		gTimer.setTimePoint("init.send.hashSeed");


	//	mSimpleBins.init(mN);
		//mCuckooBins.init(mN);

		//mPsis.resize(mBins.mBinCount);

		
		u64 otCountSend = otCounts;// mSimpleBins.mBins.size();
		u64 otCountRecv = otCounts; //mCuckooBins.mBins.size();


		gTimer.setTimePoint("init.send.baseStart");

		if (otSend.hasBaseOts() == false ||
			(otRecv.hasBaseOts() == false && isOtherDirection))
		{
			// first do 128 public key OTs (expensive)
			std::array<std::array<block, 2>, gOtExtBaseOtCount> baseMsg;
			NaorPinkas base;
			base.send(baseMsg, mPrng, chl0, 2);

			// now extend these to enough recv OTs to seed the send Kco and the send Kos ot extension
			u64 dualBaseOtCount = gOtExtBaseOtCount;

			if (!isOtherDirection) //if it is not dual, number extend OT is 128
				dualBaseOtCount = 0;

			BitVector recvChoice(baseOtCount + dualBaseOtCount); recvChoice.randomize(mPrng);
			std::vector<block> recvBaseMsg(baseOtCount + dualBaseOtCount);
			IknpOtExtReceiver kosRecv;
			kosRecv.setBaseOts(baseMsg);
			kosRecv.receive(recvChoice, recvBaseMsg, mPrng, chl0);


			// we now have a bunch of recv OTs, lets seed the NcoOtExtSender
			BitVector kcoSendBaseChoice;
			kcoSendBaseChoice.copy(recvChoice, 0, baseOtCount);
			ArrayView<block> kcoSendBase(
				recvBaseMsg.begin(),
				recvBaseMsg.begin() + baseOtCount);

			otSend.setBaseOts(kcoSendBase, kcoSendBaseChoice);

			if (isOtherDirection) {
				// now lets extend these recv OTs in the other direction
				BitVector kosSendBaseChoice;
				kosSendBaseChoice.copy(recvChoice, baseOtCount, dualBaseOtCount);
				ArrayView<block> kosSendBase(
					recvBaseMsg.begin() + baseOtCount,
					recvBaseMsg.end());
				IknpOtExtSender kos;
				kos.setBaseOts(kosSendBase, kosSendBaseChoice);

				// these send OTs will be stored here
				std::vector<std::array<block, 2>> sendBaseMsg(baseOtCount);
				kos.send(sendBaseMsg, mPrng, chl0);

				// now set these ~800 OTs as the base of our N choose 1 OTs NcoOtExtReceiver
				otRecv.setBaseOts(sendBaseMsg);
			}
		}

		gTimer.setTimePoint("init.send.extStart");

		
		auto sendRoutine = [&](u64 tIdx, u64 total, NcoOtExtSender& ots, Channel& chl)
		{
			auto start = (tIdx     * otCountSend / total) ;
			auto end = ((tIdx + 1) * otCountSend / total) ;

			ots.init(end - start);
		};

		
		auto recvOtRountine = [&](u64 tIdx, u64 total, NcoOtExtReceiver& ots, Channel& chl)
		{
			auto start = (tIdx     * otCountRecv / total) ;
			auto end = ((tIdx + 1) * otCountRecv / total) ;

			ots.init(end - start);
		};

		u64 numThreads = chls.size() - 1;
		u64 numSendThreads, numRecvThreads;

		if (isOtherDirection) {
			 numSendThreads = numThreads / 2;
			 numRecvThreads = numThreads - numSendThreads;
		}
		else
		{
			numSendThreads = numThreads;
			numRecvThreads = 0;
		}


		std::vector<std::thread> thrds(numThreads);
		auto thrdIter = thrds.begin();
		auto chlIter = chls.begin() + 1;

		mOtSends.resize(chls.size());

		for (u64 i = 0; i < numSendThreads; ++i)
		{
			mOtSends[i] = std::move(otSend.split());

			*thrdIter++ = std::thread([&, i, chlIter]()
			{
				//std::cout << IoStream::lock << "s sendOt " << l << "  " << (**chlIter).getName() << std::endl << IoStream::unlock;
				sendRoutine(i + 1, numSendThreads + 1, *mOtSends[i], **chlIter);
			});
			++chlIter;
		}
		mOtSends[0] = std::move(otSend.split());
		sendRoutine(0, numSendThreads + 1, *mOtSends[0], chl0);

		mOtRecvs.resize(chls.size());
		for (u64 i = 0; i < numRecvThreads; ++i)
		{
			mOtRecvs[i] = std::move(otRecv.split());

			*thrdIter++ = std::thread([&, i, chlIter]()
			{
				//std::cout << IoStream::lock << "s recvOt " << l << "  " << (**chlIter).getName() << std::endl << IoStream::unlock;
				recvOtRountine(i, numRecvThreads, *mOtRecvs[i], **chlIter);
			});
			++chlIter;
		}
		

		if (numRecvThreads == 0 && isOtherDirection)
		{
			mOtRecvs[0] = std::move(otRecv.split());

			recvOtRountine(0, 1, *mOtRecvs[0], chl0);
		}


		for (auto& thrd : thrds)
			thrd.join();

		gTimer.setTimePoint("init.send.done");

	}


	void OPPRFSender::getOPRFKeys(u64 IdxParty, binSet& bins, Channel & chl, bool isOtherDirectionGetOPRF)
	{
		getOPRFKeys(IdxParty, bins,{ &chl }, isOtherDirectionGetOPRF);
	}

	void Bit_Position_Random_Test_Impl11()
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
			for (u64 k = 0; k <mSimpleBins.mNumHashes[0]; ++k)
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

		for (u64 bIdx = 0; bIdx < mSimpleBins.mBinCount[1]; ++bIdx)
		{
			auto& bin = mSimpleBins.mBins[mSimpleBins.mBinCount[0] + bIdx];
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

		//mSimpleBins.print(0, true, true, true, true);
		/*BitPosition b;

		std::set<int> rs;
		b.init(4);
		Timer mTimer;
		auto start = mTimer.setTimePoint("getPos1.start");
		b.getPos1(testSet, 128);
		auto end = mTimer.setTimePoint("getPos1.done");
		double time= std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		Log::out << "time= " << time << "\n";

		Log::out << "    getPos= ";
		for (u64 j = 0; j < b.mPos.size(); ++j)
		{
		Log::out << static_cast<int16_t>(b.mPos[j]) << " ";
		}
		Log::out << Log::endl;

		for (u64 j = 0; j < b.mMaps.size(); ++j)
		{
		Log::out << testSet[j] << " " << static_cast<int16_t>(b.mMaps[j]) << " " << Log::endl;
		}*/
		Log::out << Log::endl;

	}

	void  OPPRFSender::getOPRFKeys(u64 IdxP, binSet& bins, const std::vector<Channel*>& chls, bool isOtherDirectionGetOPRF)
	{
		
		//std::vector<std::thread>  thrds(chls.size());
		std::vector<std::thread>  thrds(1);

		gTimer.setTimePoint("online.send.spaw");


		for (u64 tIdx = 0; tIdx < thrds.size(); ++tIdx)
		{
			auto seed = mPrng.get<block>();
			thrds[tIdx] = std::thread([&, tIdx, seed]() {

				PRNG prng(seed);

				if (tIdx == 0) gTimer.setTimePoint("online.send.thrdStart");


				auto& chl = *chls[tIdx];

				if (tIdx == 0) gTimer.setTimePoint("online.send.insert");
				const u64 stepSize = 16;
				
				std::vector<block> ncoInput(bins.mNcoInputBlkSize);

#if 1
#pragma region compute Send Bark-OPRF				
				//####################
				//#######Sender role
				//####################
				auto& otSend = *mOtSends[tIdx];
				auto otCountSend = bins.mSimpleBins.mBins.size();

				auto binStart = tIdx       * otCountSend / thrds.size();
				auto binEnd = (tIdx + 1) * otCountSend / thrds.size();

				if (tIdx == 0) gTimer.setTimePoint("online.send.OT");

				for (u64 bIdx = binStart; bIdx < binEnd;)
				{
					u64 currentStepSize = std::min(stepSize, binEnd - bIdx);
					otSend.recvCorrection(chl, currentStepSize);				

					for (u64 stepIdx = 0; stepIdx < currentStepSize; ++bIdx, ++stepIdx)
					{

						auto& bin = bins.mSimpleBins.mBins[bIdx];

						if (bin.mIdx.size() > 0)
						{
							bin.mValOPRF[IdxP].resize(bin.mIdx.size());
						//std::cout << "s-" << bIdx << ", ";
						for (u64 i = 0; i < bin.mIdx.size(); ++i)
						{

							u64 inputIdx = bin.mIdx[i];

							for (u64 j = 0; j < bins.mNcoInputBlkSize; ++j)
							{
								ncoInput[j] = bins.mNcoInputBuff[j][inputIdx];
							}

							//    block sendMask;

							otSend.encode(
								bIdx, //each bin has 1 OT
								ncoInput,
								bin.mValOPRF[IdxP][i]);
								//mmOPRF[bIdx][i]);
							/*if (bIdx < 3 || (bIdx < mN && bIdx > mN - 2))
							std::cout << "s-"<<bIdx <<", "<< inputIdx << ": " << sendMask << std::endl;*/
						}

						//#####################
						//######Finding bit locations
						//#####################

						//std::cout << bin.mValOPRF[IdxP][0];

						//diff max bin size for first mSimpleBins.mBinCount and 
						// mSimpleBins.mBinStashCount
						if (bIdx < bins.mSimpleBins.mBinCount[0])
							bin.mBits[IdxP].init(/*bin.mIdx.size(), */bins.mSimpleBins.mNumBits[0]);
						else
							bin.mBits[IdxP].init(/*bin.mIdx.size(), */bins.mSimpleBins.mNumBits[1]);

						auto start = mTimer.setTimePoint("getPos1.start");
						bin.mBits[IdxP].getPos1(bin.mValOPRF[IdxP], 128);
						auto end = mTimer.setTimePoint("getPos1.done");
						
						mPosBitsTime += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
					
						//bin.mBits[IdxP].getMasks(bin.mValOPRF[IdxP]);
						//std::cout << ", "
						//	<< static_cast<int16_t>(bin.mBits[IdxP].mMaps[0]) << std::endl;
					}
					}
				}
		

				if (tIdx == 0) gTimer.setTimePoint("online.send.otSend.finalOPRF");

#ifdef PRINT
				std::cout << "getPosTime" << IdxP << ": " << mPosBitsTime / pow(10, 6) << std::endl;
#endif // PRINT

				
#pragma endregion
#endif

#if 1
#pragma region compute Recv Bark-OPRF

				//####################
				//#######Receiver role
				//####################
				if (isOtherDirectionGetOPRF) {
					auto& otRecv = *mOtRecvs[tIdx];
					auto otCountRecv = bins.mCuckooBins.mBins.size();
					// get the region of the base OTs that this thread should do.
					binStart = tIdx       * otCountRecv / thrds.size();
					binEnd = (tIdx + 1) * otCountRecv / thrds.size();

					for (u64 bIdx = binStart; bIdx < binEnd;)
					{
						u64 currentStepSize = std::min(stepSize, binEnd - bIdx);

						for (u64 stepIdx = 0; stepIdx < currentStepSize; ++bIdx, ++stepIdx)
						{

							auto& bin = bins.mCuckooBins.mBins[bIdx];

							if (!bin.isEmpty())
							{
								u64 inputIdx = bin.idx();

								for (u64 j = 0; j < ncoInput.size(); ++j)
									ncoInput[j] = bins.mNcoInputBuff[j][inputIdx];

								otRecv.encode(
									bIdx,      // input
									ncoInput,             // input
									bin.mValOPRF[IdxP]); // output

														 /*if (bIdx < 3 || (bIdx < mN && bIdx > mN-2))
														 std::cout << "r-" << bIdx << ", " << inputIdx << ": " << valOPRF[inputIdx] << std::endl;*/
							}
							else
							{
								otRecv.zeroEncode(bIdx);
							}
						}
						otRecv.sendCorrection(chl, currentStepSize);
					}

					if (tIdx == 0) gTimer.setTimePoint("online.send.otRecv.finalOPRF");

					otRecv.check(chl);
				}
#pragma endregion
#endif
				otSend.check(chl);

			});
		}

		for (auto& thrd : thrds)
			thrd.join();
	}
	


	void OPPRFSender::sendSecretSharing(u64 IdxParty, binSet& bins, std::vector<block>& plaintexts, Channel & chl)
	{
		sendSecretSharing(IdxParty, bins,plaintexts,{ &chl });
	}

	void OPPRFSender::sendSecretSharing(u64 IdxP, binSet& bins, std::vector<block>& plaintexts,const std::vector<Channel*>& chls)
	{
		if (plaintexts.size() != mN)
			throw std::runtime_error(LOCATION);


		//TODO: double check
	//	u64 maskSize = sizeof(block);//roundUpTo(mStatSecParam + 2 * std::log(mN) - 1, 8) / 8;
		u64 maskSize =roundUpTo(mStatSecParam + 2 * std::log(mN) - 1 + bins.mSimpleBins.mNumBits[1], 8) / 8;
		//u64 maskSize = 7;
		if (maskSize > sizeof(block))
			throw std::runtime_error("masked are stored in blocks, so they can exceed that size");



		std::vector<std::thread>  thrds(chls.size());
		// std::vector<std::thread>  thrds(1);        

		std::mutex mtx;


		gTimer.setTimePoint("online.send.spaw");

		for (u64 tIdx = 0; tIdx < thrds.size(); ++tIdx)
		{
			auto seed = mPrng.get<block>();
			thrds[tIdx] = std::thread([&, tIdx, seed]() {

				PRNG prng(seed);

				if (tIdx == 0) gTimer.setTimePoint("online.send.thrdStart");

				auto& chl = *chls[tIdx];
				const u64 stepSize = 16;

#pragma region sendShare
#if 1
				if (tIdx == 0) gTimer.setTimePoint("online.send.sendShare");

				//2 type of bins: normal bin in inital step + stash bin
				for (auto bIdxType = 0; bIdxType < 2; bIdxType++)
				{
					auto binCountSend = bins.mSimpleBins.mBinCount[bIdxType];
					u64 binStart, binEnd;
					if (bIdxType == 0)
					{
						 binStart = tIdx       * binCountSend / thrds.size();
						 binEnd = (tIdx + 1) * binCountSend / thrds.size();
					}
					else
					{
						 binStart = tIdx       * binCountSend / thrds.size()+ bins.mSimpleBins.mBinCount[0];
						 binEnd = (tIdx + 1) * binCountSend / thrds.size() + bins.mSimpleBins.mBinCount[0];
					}

					if (tIdx == 0) gTimer.setTimePoint("online.send.masks.init.step");

					for (u64 bIdx = binStart; bIdx < binEnd;)
					{
						u64 currentStepSize = std::min(stepSize, binEnd - bIdx);
						uPtr<Buff> sendMaskBuff(new Buff);
						sendMaskBuff->resize(currentStepSize * (bins.mSimpleBins.mMaxBinSize[bIdxType] * maskSize + bins.mSimpleBins.mNumBits[bIdxType] * sizeof(u8)));
						auto maskView = sendMaskBuff->getMatrixView<u8>(bins.mSimpleBins.mMaxBinSize[bIdxType] * maskSize + bins.mSimpleBins.mNumBits[bIdxType] * sizeof(u8));

						for (u64 stepIdx = 0; stepIdx < currentStepSize; ++bIdx, ++stepIdx)
						{
							//Log::out << "sBin #" << bIdx << Log::endl;

							auto& bin = bins.mSimpleBins.mBins[bIdx];
							u64 baseMaskIdx = stepIdx;
							int MaskIdx = 0;

							if (bin.mIdx.size() > 0)
							{
								//copy bit locations in which all OPRF values are distinct

							//	Log::out << "    c_mPos= ";

								if (bin.mBits[IdxP].mPos.size() != bins.mSimpleBins.mNumBits[bIdxType])
								{
#ifdef PRINT
									Log::out << "bin.mBits[IdxP].mPos.size() != bins.mSimpleBins.mNumBits[bIdxType]" << Log::endl;
									Log::out << "Party: " << IdxP << Log::endl;
									Log::out << "bIdx: " << bIdx << Log::endl;
									Log::out << "bin.mBits[IdxP].mPos.size(): " << bin.mBits[IdxP].mPos.size() << Log::endl;
									Log::out << "mSimpleBins.mNumBits[bIdxType]: " << bins.mSimpleBins.mNumBits[bIdxType] << Log::endl;
#endif // PRINT
									throw std::runtime_error("bin.mBits.mPos.size()!= mBins.mNumBits");

								}

								//copy bit positions
								for (u64 idxPos = 0; idxPos < bin.mBits[IdxP].mPos.size(); idxPos++)
								{
								//	Log::out << static_cast<int16_t>(bin.mBits[IdxP].mPos[idxPos]) << " ";
									memcpy(
										maskView[baseMaskIdx].data() + idxPos,
										(u8*)&bin.mBits[IdxP].mPos[idxPos], sizeof(u8));
								}
								//Log::out << Log::endl;


								for (u64 i = 0; i < bin.mIdx.size(); ++i)
								{
									u64 inputIdx = bin.mIdx[i];
									block encr = bin.mValOPRF[IdxP][i] ^ plaintexts[inputIdx];

									//Log::out << "    c_idx=" << inputIdx;
									//Log::out << "    c_OPRF=" << encr;
									//Log::out << "    c_Map=" << static_cast<int16_t>(bin.mBits.mMaps[i]);

									MaskIdx = bin.mBits[IdxP].mMaps[i] * maskSize + bins.mSimpleBins.mNumBits[bIdxType];

									memcpy(
										maskView[baseMaskIdx].data() + MaskIdx,
										(u8*)&encr,
										maskSize);

									//	Log::out << Log::endl;
								}

								//#####################
								//######Filling dummy mask
								//#####################

								for (u64 i = 0; i < bins.mSimpleBins.mMaxBinSize[bIdxType]; ++i)
								{
									if (std::find(bin.mBits[IdxP].mMaps.begin(), bin.mBits[IdxP].mMaps.end(), i) == bin.mBits[IdxP].mMaps.end())
									{
										MaskIdx = i* maskSize + bins.mSimpleBins.mNumBits[bIdxType];
										//	Log::out << "    cc_Map=" << i << Log::endl;
										memcpy(
											maskView[baseMaskIdx].data() + MaskIdx,
											(u8*)&ZeroBlock,  //make randome
											maskSize);
									}
								}
							}
							else //pad all dummy
							{
								//bit positions
								std::vector<u8> dummyPos;
								auto idxDummyPos = 0;
								while (dummyPos.size()<bins.mSimpleBins.mNumBits[bIdxType])
								{
									u64 rand = std::rand() % 128; //choose randome bit location
									if (std::find(dummyPos.begin(), dummyPos.end(), rand) == dummyPos.end())
									{
										dummyPos.push_back(rand);
										memcpy(
											maskView[baseMaskIdx].data() + idxDummyPos,
											(u8*)&rand, sizeof(u8));
										idxDummyPos++;
									}
								}

								for (u64 i = 0; i < bins.mSimpleBins.mMaxBinSize[bIdxType]; ++i)
								{
										MaskIdx = i* maskSize + bins.mSimpleBins.mNumBits[bIdxType];
										//	Log::out << "    cc_Map=" << i << Log::endl;
										memcpy(
											maskView[baseMaskIdx].data() + MaskIdx,
											(u8*)&ZeroBlock,  //make randome
											maskSize);
		
								}
							
							}


						}

#ifdef PRINT
						Log::out << "maskSize: ";
						for (size_t i = 0; i < maskView.size()[0]; i++)
						{
							for (size_t j = 0; j <  mSimpleBins.mNumBits[bIdxType]; j++)
							{
								Log::out << static_cast<int16_t>(maskView[i][j]) << " ";
							}
							Log::out << Log::endl;

							for (size_t j = 0; j < mSimpleBins.mMaxBinSize[bIdxType]; j++) {
								auto theirMask = ZeroBlock;
								memcpy(&theirMask, maskView[i].data() + j*maskSize + mSimpleBins.mNumBits[bIdxType], maskSize);
								if (theirMask != ZeroBlock)
								{
									Log::out << theirMask << " " << Log::endl;
								}
							}
						}
#endif
						chl.asyncSend(std::move(sendMaskBuff));

					}
				}
				if (tIdx == 0) gTimer.setTimePoint("online.send.sendMask");

			//	otSend.check(chl);

				

				/* if (tIdx == 0)
				chl.asyncSend(std::move(sendMaskBuff));*/

				if (tIdx == 0) gTimer.setTimePoint("online.send.finalMask");
#endif
#pragma endregion

			});
		}

		for (auto& thrd : thrds)
			thrd.join();

		//    permThrd.join();



	}

	void OPPRFSender::revSecretSharing(u64 IdxParty, binSet& bins, std::vector<block>& plaintexts, Channel & chl)
	{
		revSecretSharing(IdxParty, bins, plaintexts, { &chl });
	}

	void OPPRFSender::revSecretSharing(u64 IdxP, binSet& bins, std::vector<block>& plaintexts, const std::vector<Channel*>& chls)
	{

		// this is the online phase.
		gTimer.setTimePoint("online.recv.start");

		//u64 maskSize = sizeof(block);// roundUpTo(mStatSecParam + 2 * std::log(mN) - 1, 8) / 8;
		u64 maskSize = roundUpTo(mStatSecParam + 2 * std::log(mN) - 1 + bins.mSimpleBins.mNumBits[1], 8) / 8;
		if (maskSize > sizeof(block))
			throw std::runtime_error("masked are stored in blocks, so they can exceed that size");


		std::vector<std::thread>  thrds(chls.size());
		// this mutex is used to guard inserting things into the intersection vector.
		std::mutex mInsertMtx;

		// fr each thread, spawn it.
		for (u64 tIdx = 0; tIdx < thrds.size(); ++tIdx)
		{
			auto seed = mPrng.get<block>();
			thrds[tIdx] = std::thread([&, tIdx, seed]()
			{
				if (tIdx == 0) gTimer.setTimePoint("online.recv.thrdStart");

				auto& chl = *chls[tIdx];
				const u64 stepSize = 16;

				if (tIdx == 0) gTimer.setTimePoint("online.recv.recvShare");

				//2 type of bins: normal bin in inital step + stash bin
				for (auto bIdxType = 0; bIdxType < 2; bIdxType++)
				{
					auto binCountRecv = bins.mCuckooBins.mBinCount[bIdxType];

					u64 binStart, binEnd;
					if (bIdxType == 0)
					{
						binStart = tIdx       * binCountRecv / thrds.size();
						binEnd = (tIdx + 1) * binCountRecv / thrds.size();
					}
					else
					{
						binStart = tIdx       * binCountRecv / thrds.size() + bins.mCuckooBins.mBinCount[0];
						binEnd = (tIdx + 1) * binCountRecv / thrds.size() + bins.mCuckooBins.mBinCount[0];
					}



					//use the params of the simple hashing as their params
					u64 mTheirBins_mMaxBinSize = bins.mSimpleBins.mMaxBinSize[bIdxType];
					u64 mTheirBins_mNumBits = bins.mSimpleBins.mNumBits[bIdxType];
					for (u64 bIdx = binStart; bIdx < binEnd;)
					{
						u64 curStepSize = std::min(stepSize, binEnd - bIdx);

						MatrixView<u8> maskView;
						ByteStream maskBuffer;
						chl.recv(maskBuffer);
						//maskView = maskBuffer.getMatrixView<u8>(mTheirBins_mMaxBinSize * maskSize + mTheirBins_mNumBits * sizeof(u8));
						maskView = maskBuffer.getMatrixView<u8>(mTheirBins_mMaxBinSize * maskSize + mTheirBins_mNumBits * sizeof(u8));
						if (maskView.size()[0] != curStepSize)
							throw std::runtime_error("size not expedted");

						for (u64 stepIdx = 0; stepIdx < curStepSize; ++bIdx, ++stepIdx)
						{

							auto& bin = bins.mCuckooBins.mBins[bIdx];
							if (!bin.isEmpty())
							{
								u64 baseMaskIdx = stepIdx;
								auto mask = maskView[baseMaskIdx];
								BitPosition b;
								b.mMaxBitSize = mTheirBins_mNumBits;
								for (u64 i = 0; i < b.mMaxBitSize; i++)
								{
									int idxPos = 0;
									memcpy(&idxPos, maskView[baseMaskIdx].data() + i, sizeof(u8));
									b.mPos.push_back(idxPos);
								}
#ifdef PRINT
								Log::out << "RBin #" << bIdx << Log::endl;
								Log::out << "    cc_mPos= ";
								for (u64 idxPos = 0; idxPos < b.mPos.size(); idxPos++)
								{
									Log::out << static_cast<int16_t>(b.mPos[idxPos]) << " ";
								}
								Log::out << Log::endl;
#endif
								u64 inputIdx = bin.idx();
								auto myMask = bin.mValOPRF[IdxP];
								//	u8 myMaskPos = 0;
								b.getMask(myMask, bin.mValMap[IdxP]);

								u64	MaskIdx = bin.mValMap[IdxP] * maskSize + mTheirBins_mNumBits;

								auto theirMask = ZeroBlock;
								memcpy(&theirMask, maskView[baseMaskIdx].data() + MaskIdx, maskSize);

								//if (!memcmp((u8*)&myMask, &theirMask, maskSize))
								//{
								//Log::out << "inputIdx: " << inputIdx << Log::endl;
								//	Log::out << "myMask: " << myMask << Log::endl;
								//Log::out << "theirMask: " << theirMask << " " << Log::endl;


								plaintexts[inputIdx] = myMask^theirMask;


								//}
							}
						}
					}
				}


			});
			//	if (tIdx == 0) gTimer.setTimePoint("online.recv.done");
		}
		// join the threads.
		for (auto& thrd : thrds)
			thrd.join();

		// check that the number of inputs is as expected.
		if (plaintexts.size() != mN)
			throw std::runtime_error(LOCATION);



	}

#if 0

	void OPPRFSender::revSecretSharing(u64 IdxP, binSet& bins, std::vector<block>& plaintexts, const std::vector<Channel*>& chls)
	{

		// this is the online phase.
		gTimer.setTimePoint("online.recv.start");

		u64 maskSize = sizeof(block);// roundUpTo(mStatSecParam + 2 * std::log(mN) - 1, 8) / 8;

		if (maskSize > sizeof(block))
			throw std::runtime_error("masked are stored in blocks, so they can exceed that size");


		std::vector<std::thread>  thrds(chls.size());
		// this mutex is used to guard inserting things into the intersection vector.
		std::mutex mInsertMtx;

		// fr each thread, spawn it.
		for (u64 tIdx = 0; tIdx < thrds.size(); ++tIdx)
		{
			auto seed = mPrng.get<block>();
			thrds[tIdx] = std::thread([&, tIdx, seed]()
			{
				if (tIdx == 0) gTimer.setTimePoint("online.recv.thrdStart");

				auto& chl = *chls[tIdx];
				const u64 stepSize = 16;

				if (tIdx == 0) gTimer.setTimePoint("online.recv.recvShare");

				//2 type of bins: normal bin in inital step + stash bin
				for (auto bIdxType = 0; bIdxType < 2; bIdxType++)
				{
					auto binCountRecv = bins.mCuckooBins.mBinCount[bIdxType];

					u64 binStart, binEnd;
					if (bIdxType == 0)
					{
						binStart = tIdx       * binCountRecv / thrds.size();
						binEnd = (tIdx + 1) * binCountRecv / thrds.size();
					}
					else
					{
						binStart = tIdx       * binCountRecv / thrds.size() + bins.mCuckooBins.mBinCount[0];
						binEnd = (tIdx + 1) * binCountRecv / thrds.size() + bins.mCuckooBins.mBinCount[0];
					}



					//use the params of the simple hashing as their params
					u64 mTheirBins_mMaxBinSize = bins.mSimpleBins.mMaxBinSize[bIdxType];
					u64 mTheirBins_mNumBits = bins.mSimpleBins.mNumBits[bIdxType];
					for (u64 bIdx = binStart; bIdx < binEnd;)
					{
						u64 curStepSize = std::min(stepSize, binEnd - bIdx);

						MatrixView<u8> maskView;
						ByteStream maskBuffer;
						chl.recv(maskBuffer);
						//maskView = maskBuffer.getMatrixView<u8>(mTheirBins_mMaxBinSize * maskSize + mTheirBins_mNumBits * sizeof(u8));
						maskView = maskBuffer.getMatrixView<u8>(mTheirBins_mMaxBinSize * maskSize + mTheirBins_mNumBits * sizeof(u8));
						if (maskView.size()[0] != curStepSize)
							throw std::runtime_error("size not expedted");

						for (u64 stepIdx = 0; stepIdx < curStepSize; ++bIdx, ++stepIdx)
						{

							auto& bin = bins.mCuckooBins.mBins[bIdx];
							if (!bin.isEmpty())
							{
								u64 baseMaskIdx = stepIdx;
								auto mask = maskView[baseMaskIdx];
								BitPosition b;
								b.mMaxBitSize = mTheirBins_mNumBits;
								for (u64 i = 0; i < b.mMaxBitSize; i++)
								{
									int idxPos = 0;
									memcpy(&idxPos, maskView[baseMaskIdx].data() + i, sizeof(u8));
									b.mPos.push_back(idxPos);
								}
#ifdef PRINT
								Log::out << "RBin #" << bIdx << Log::endl;
								Log::out << "    cc_mPos= ";
								for (u64 idxPos = 0; idxPos < b.mPos.size(); idxPos++)
								{
									Log::out << static_cast<int16_t>(b.mPos[idxPos]) << " ";
								}
								Log::out << Log::endl;
#endif
								u64 inputIdx = bin.idx();
								auto myMask = bin.mValOPRF[IdxP];
								//	u8 myMaskPos = 0;
								b.getMask(myMask, bin.mValMap[IdxP]);

								u64	MaskIdx = bin.mValMap[IdxP] * maskSize + mTheirBins_mNumBits;

								auto theirMask = ZeroBlock;
								memcpy(&theirMask, maskView[baseMaskIdx].data() + MaskIdx, maskSize);

								//if (!memcmp((u8*)&myMask, &theirMask, maskSize))
								//{
								//Log::out << "inputIdx: " << inputIdx << Log::endl;
								//	Log::out << "myMask: " << myMask << Log::endl;
								//Log::out << "theirMask: " << theirMask << " " << Log::endl;
								plaintexts[inputIdx] = myMask^theirMask;


								//}
							}
						}
					}
				}


			});
			//	if (tIdx == 0) gTimer.setTimePoint("online.recv.done");
		}
		// join the threads.
		for (auto& thrd : thrds)
			thrd.join();

		// check that the number of inputs is as expected.
		if (plaintexts.size() != mN)
			throw std::runtime_error(LOCATION);



	}

	void OPPRFSender::hash2Bins(std::vector<block>& inputs, Channel & chl)
	{
		hash2Bins(inputs, { &chl });
	}
	void  OPPRFSender::hash2Bins(std::vector<block>& inputs, const std::vector<Channel*>& chls)
	{

		if (inputs.size() != mN)
			throw std::runtime_error(LOCATION);

		//TODO: double check
		u64 maskSize = roundUpTo(mStatSecParam + 2 * std::log(mN) - 1, 8) / 8;

		if (maskSize > sizeof(block))
			throw std::runtime_error("masked are stored in blocks, so they can exceed that size");



		std::vector<std::thread>  thrds(chls.size());
		//std::vector<std::thread>  thrds(1);        

		std::atomic<u32> remaining((u32)thrds.size());
		std::promise<void> doneProm;
		std::shared_future<void>
			doneFuture(doneProm.get_future());

		std::mutex mtx;

		mNcoInputBuff.resize(mNcoInputBlkSize);

		for (u64 hashIdx = 0; hashIdx < mNcoInputBuff.size(); ++hashIdx)
			mNcoInputBuff[hashIdx].resize(mN);


		auto permSeed = mPrng.get<block>();


		gTimer.setTimePoint("online.send.spaw");

		for (u64 tIdx = 0; tIdx < thrds.size(); ++tIdx)
		{
			auto seed = mPrng.get<block>();
			thrds[tIdx] = std::thread([&, tIdx, seed]() {

				PRNG prng(seed);

				if (tIdx == 0) gTimer.setTimePoint("online.send.thrdStart");

				auto& otSend = *mOtSends[tIdx];
				auto startIdx = tIdx       * mN / thrds.size();
				auto endIdx = (tIdx + 1) * mN / thrds.size();

				// compute the region of inputs this thread should insert.
				//ArrayView<block> itemRange(
				//    inputs.begin() + startIdx,
				//    inputs.begin() + endIdx);
#pragma region Hashing				
				std::vector<AES> ncoInputHasher(mNcoInputBlkSize);
				//Log::out << "mHashingSeed: " << mHashingSeed << Log::endl;
				for (u64 i = 0; i < ncoInputHasher.size(); ++i)
					ncoInputHasher[i].setKey(_mm_set1_epi64x(i) ^ mHashingSeed);



				for (u64 i = startIdx; i < endIdx; i += 128)
				{
					auto currentStepSize = std::min(u64(128), mN - i);

					for (u64 hashIdx = 0; hashIdx < ncoInputHasher.size(); ++hashIdx)
					{
						ncoInputHasher[hashIdx].ecbEncBlocks(
							inputs.data() + i,
							currentStepSize,
							mNcoInputBuff[hashIdx].data() + i);
					}

					// since we are using random codes, lets just use the first part of the code 
					// as where each item should be hashed.
					std::vector<block> tempMaskBuff(currentStepSize);
					std::vector<u64> tempIdxBuff(currentStepSize);
					CuckooHasher1::Workspace w(tempMaskBuff.size());
					MatrixView<u64> hashes(currentStepSize, mCuckooBins.mParams.mNumHashes[0]);

					for (u64 j = 0; j < currentStepSize; ++j)
					{
						tempIdxBuff[j] = i + j;
						for (u64 k = 0; k <mCuckooBins.mParams.mNumHashes[0]; ++k)
						{
							hashes[j][k] = *(u64*)&mNcoInputBuff[k][i + j];
						}
					}
					mSimpleBins.insertBatch(tempIdxBuff, hashes);
					mCuckooBins.insertBatch(tempIdxBuff, hashes, w);
				}

				CuckooHasher1::Workspace stashW(mCuckooBins.mStashIdxs.size());
				MatrixView<u64> stashHashes(mCuckooBins.mStashIdxs.size(), mCuckooBins.mParams.mNumHashes[1]);

				for (u64 j = 0; j < mCuckooBins.mStashIdxs.size(); ++j)
				{
					for (u64 k = 0; k <mCuckooBins.mParams.mNumHashes[1]; ++k)
					{
						stashHashes[j][k] = *(u64*)&mNcoInputBuff[k][mCuckooBins.mStashIdxs[j]];
					}
				}
				mCuckooBins.insertStashBatch(mCuckooBins.mStashIdxs, stashHashes, stashW);

				//<< IoStream::lock << "Sender"<< std::endl;
				//mBins.insertItemsWithPhasing(range, mStatSecParam, inputs.size());


				// block until all items have been inserted. the last to finish will set the promise...
				if (--remaining)
					doneFuture.get();
				else
					doneProm.set_value();

				/*for (u64 i = 0; i < mBins.mBinCount; ++i)
				{
				if (i < 3 || (i < mN && i > mN - 2)) {
				std::cout << "s-Bin" << i << ": ";
				for (u64 k = 0; k < mBins.mBins[i].size(); ++k)
				{
				std::cout << mBins.mBins[i][k] << " ";
				}
				std::cout << std::endl;
				}
				}*/

#pragma endregion
				if (tIdx == 0) gTimer.setTimePoint("online.recv.thrdStart");

#pragma region Init Bin
#if 1
				if (tIdx == 0) gTimer.setTimePoint("online.recv.insertDone");


				if (mCuckooBins.mBins.size() != mSimpleBins.mBins.size())
					throw std::runtime_error("mCuckooBins.mBins.size()!= mSimpleBins.mBins.size()");

				auto binCount = mCuckooBins.mBins.size();
				// get the region of the base OTs that this thread should do.
				auto binStart = tIdx       * binCount / thrds.size();
				auto binEnd = (tIdx + 1) * binCount / thrds.size();

				const u64 stepSize = 16;

				for (u64 bIdx = binStart; bIdx < binEnd;)
				{
					u64 currentStepSize = std::min(stepSize, binEnd - bIdx);

					for (u64 stepIdx = 0; stepIdx < currentStepSize; ++bIdx, ++stepIdx)
					{
						auto& cbin = mCuckooBins.mBins[bIdx];
						if (!cbin.isEmpty())
						{
							cbin.mValOPRF.resize(mParties);
							cbin.mValMap.resize(mParties);
						}
						auto& sbin = mSimpleBins.mBins[bIdx];

						if (sbin.mIdx.size()>0)
						{
							sbin.mValOPRF.resize(mParties);
							sbin.mBits.resize(mParties);
						}

					}

				}
#endif
#pragma endregion


			});


		}

		for (auto& thrd : thrds)
			thrd.join();
			}

#endif
}


