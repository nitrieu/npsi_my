#include "AknBfMPsiSender.h"
#include "AknBfMPsiSender.h"
#include "TwoChooseOne/KosOtExtSender.h"
#include "Crypto/PRNG.h"
#include "Crypto/Commit.h"
#include "Common/Log.h"
#include <set>

namespace osuCrypto {

    AknBfMPsiSender::AknBfMPsiSender()
    {
    }


    AknBfMPsiSender::~AknBfMPsiSender()
    {
    }
    void AknBfMPsiSender::init(u64 n, u64 statSecParam, OtExtSender & otExt, Channel & chl, block seed)
    {
        std::vector<Channel*> chls{ &chl };
        init(n, statSecParam, otExt,chls , seed);
    }
    void AknBfMPsiSender::init(u64 n, u64 statSecParam, OtExtSender& otExt, std::vector<Channel*>& chls, block seed)
    {
        gTimer.setTimePoint("sender.init.start");
        mN = n;
        mStatSecParam = statSecParam;

        PRNG prng(seed);
        mSeed = prng.get<block>();
        auto myHashSeed = prng.get<block>();

        Commit comm(myHashSeed), theirComm;

        auto& chl0 = *chls[0];
        chl0.asyncSend(comm.data(), comm.size());
        auto theirCommFutre = chl0.asyncRecv(theirComm.data(), theirComm.size());

        //u64 statSecParam(40);
        u64 totalOtCount, cncOnesThreshold, totalOnesCount;
        double cncProb;

        computeAknBfParams(n, statSecParam, totalOtCount, totalOnesCount, cncOnesThreshold, cncProb, mNumHashFunctions, mBfBitCount);


        //mHashs.resize(numHashFunctions);


        gTimer.setTimePoint("sender.init.aknOT.start");

        mAknOt.init(totalOtCount, cncOnesThreshold, cncProb, otExt, chls, prng);
        gTimer.setTimePoint("sender.init.aknOT.done");

        if (mBfBitCount > mAknOt.mMessages.size())
            throw std::runtime_error(LOCATION);

        //mAknOt.init(m, mBinSize * mNumHashFunctions, mNumBins, mNumHashFunctions, otExt, chl, prng);

        theirCommFutre.get();
        chl0.asyncSend(&myHashSeed, sizeof(block));
        block theirHashingSeed;
        chl0.recv(&theirHashingSeed, sizeof(block));

        mHashingSeed = myHashSeed ^ theirHashingSeed;


        gTimer.setTimePoint("sender.init.done");

    }


    void AknBfMPsiSender::sendInput(std::vector<block>& inputs, Channel & chl)
    {
        std::vector<Channel*> cc{ &chl };

        sendInput(inputs, cc);
    }

    void AknBfMPsiSender::sendInput(std::vector<block>& inputs, std::vector<Channel*> & chls)
    {

        if (inputs.size() != mN)
            throw std::runtime_error(LOCATION);

        gTimer.setTimePoint("sender.online.start");

        //TODO("real seed");
        PRNG prng(mSeed);

        auto& chl0 = *chls[0];

        //std::cout << "mBfBitCount " << mBfBitCount << std::endl;

        //u8 dummy[1];
        //chl.recv(dummy, 1);
        //gTimer.setTimePoint("sender.online.dummyRecv");




        std::promise<bool> isValidPerm;
        std::shared_future<bool> isValidPermFuture(isValidPerm.get_future());


        //std::vector<u8> hashBuff(roundUpTo(mNumHashFunctions * sizeof(u64), sizeof(block)));
        ArrayView<block>bv((mNumHashFunctions + 1) / 2);

        ByteStream piBuff;
        piBuff.resize(mBfBitCount * sizeof(LogOtCount_t));

        //u64 blockSize = 4096 * 128 * 20;
        ////std::cout << "blockSize " << blockSize << std::endl;
        //for (i64 i = 0; i < (i64)piBuff.size();i += blockSize)
        //{
        //    auto ss = std::min(blockSize, piBuff.size() - i);

        //    chl0.recv(piBuff.data() + i, ss);
        //    gTimer.setTimePoint("sender.online.permRecv("+std::to_string(i/ blockSize) + "/" + std::to_string(piBuff.size()/ blockSize) +")");

        //}
        chl0.recv(piBuff);

        gTimer.setTimePoint("sender.online.permRecv");
        //TODO("make perm item size smaller");


        auto permutes = piBuff.getArrayView<LogOtCount_t>();

        if (permutes.size() != mBfBitCount)
            throw std::runtime_error(LOCATION);

        auto routine = [&](u64 t)
        {
            auto & chl = *chls[t];
            auto start = inputs.size() * t / chls.size();
            auto end = inputs.size() * (t + 1) / chls.size();
            std::set<u64> idxs;

            std::unique_ptr<ByteStream> myMasksBuff(new ByteStream((end - start) * sizeof(block)));
            myMasksBuff->setp(myMasksBuff->capacity());
            auto myMasks = myMasksBuff->getArrayView<block>();
            SHA1 hash;
            u8 hashOut[SHA1::HashSize];

            if (t == 0)
                gTimer.setTimePoint("sender.online.masksStart");


            //std::cout << IoStream::lock;

            for (u64 i = start, k = 0; i < end; ++i, ++k)
            {
                myMasks[k] = ZeroBlock;
                //auto hash = mHashs[0];

                hash.Reset();
                hash.Update(mHashingSeed);
                hash.Update(inputs[i]);
                hash.Final(hashOut); 
                //std::cout << "s " << (u64)hashOut << std::endl;

                //PRNG hasher( *(block*)hashOut);

                AES hasher(toBlock(hashOut));

                hasher.ecbEncCounterMode(0, bv.size(), bv.data());
                ArrayView<u64>iv((u64*)bv.data(), mNumHashFunctions);

                //std::cout << "S inputs[" << i << "] " << inputs[i]  << " h -> " 
                //    << toBlock(hashOut) << " = H("<< mHashingSeed <<" || "<< inputs[i]<<")"<< std::endl;

                for (u64 j = 0; j < mNumHashFunctions; ++j)
                {
                    auto idx = iv[j] % mBfBitCount;

                    auto pIdx = permutes[idx];

                    //std::cout << "send " << i << "  " << j << "  " << pIdx  <<"   ("<<idx<<")"<< std::endl;

                    myMasks[k] = myMasks[k] ^ mAknOt.mMessages[pIdx][1];
                }
            }

            //std::cout << IoStream::unlock;


            if (t == 0)
                gTimer.setTimePoint("sender.online.masksComputed");


            bool result = isValidPermFuture.get();

            if (result)
            {
                //chl.asyncSend(std::move(myMasksBuff));
                chl.send(*myMasksBuff);
            }


            if (t == 0)
                gTimer.setTimePoint("sender.online.masksSent");
        };


        std::vector< std::thread> thrds(chls.size());


        for (u64 i = 0; i < chls.size(); ++i)
        {
            //gTimer.setTimePoint("sender.online.thrd_" + std::to_string(i) + "_spawned");

            thrds[i] = std::thread([&, i]()
            {
                //gTimer.setTimePoint("sender.online.thrd_" + std::to_string(i) + "_started");
                routine(i);
            });

        }

        BitVector& usedBits = mAknOt.mSampled;
        for (u64 i = 0; i < mBfBitCount; ++i)
        {
            if (usedBits[permutes[i]])
            {
                isValidPerm.set_value(false);
                break;
            }

            usedBits[permutes[i]] = 1;
        }

        isValidPerm.set_value(true);

        for (auto& thrd : thrds)
            thrd.join();

        //    //std::cout << IoStream::lock << "s" << std::endl;;
        //    for (u64 i = 0; i < inputs.size(); ++i)
        //    {
        //        myMasks[i] = ZeroBlock;
        //        for (u64 j = 0; j < mNumHashFunctions; ++j)
        //        {
        //            // copy the hash since its stateful
        //            auto hash = mHashs[j];

        //            hash.Update(inputs[i]);
        //            hash.Final(hashOut);
        //            u64& idx = *(u64*)hashOut;

        //            idx %= mBfBitCount;

        //            auto pIdx = permutes[idx];

        //            myMasks[i] = myMasks[i] ^ mAknOt.mMessages[pIdx][1];

        //            //if (i == 0)
        //            //{
        //            //    std::cout << mAknOt.mMessages[pIdx][1] << "  " << pIdx << "  " << mAknOt.mMessages[pIdx][0] << "  " << std::endl;
        //            //}

        //        }

        //        //if (i == 0)
        //        //{
        //        //    std::cout << myMasks[i] << std::endl;
        //        //}
        //    }
        //    //std::cout << IoStream::unlock;

        //    chl.asyncSend(std::move(myMasksBuff));
        //}
    }
}