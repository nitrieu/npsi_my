#include "SimpleCuckoo.h"
#include "Crypto/sha1.h"
#include "Crypto/PRNG.h"
#include <random>
#include "Common/Log.h"
#include <numeric>

namespace osuCrypto
{


    SimpleCuckoo::SimpleCuckoo()
        :mTotalTries(0)
    {
    }

    SimpleCuckoo::~SimpleCuckoo()
    {

        mHashes = std::vector<u64>();
        mHashesView   = MatrixView<u64>();

        mBins = std::vector<Bin>();
        mStash = std::vector<Bin>();

    }

    bool SimpleCuckoo::operator==(const SimpleCuckoo & cmp) const
    {
        if (mBins.size() != cmp.mBins.size())
            throw std::runtime_error("");

        if (mStash.size() != cmp.mStash.size())
            throw std::runtime_error("");



        for (u64 i = 0; i < mBins.size(); ++i)
        {
            if (mBins[i].mVal != cmp.mBins[i].mVal)
            {
                return false;
            }
        }

        for (u64 i = 0; i < mStash.size(); ++i)
        {
            if (mStash[i].mVal != cmp.mStash[i].mVal)
            {
                return false;
            }
        }

        return true;
    }

    bool SimpleCuckoo::operator!=(const SimpleCuckoo & cmp) const
    {
        return !(*this == cmp);
    }

    void SimpleCuckoo::print() const
    {

        std::cout << "Cuckoo Hasher  " << std::endl;


        for (u64 i = 0; i < mBins.size(); ++i)
        {
            std::cout << "Bin #" << i;

            if (mBins[i].isEmpty())
            {
                std::cout << " - " << std::endl;
            }
            else
            {
                std::cout << "    c_idx=" << mBins[i].idx() << "  hIdx=" << mBins[i].hashIdx() << std::endl;

            }

        }
        for (u64 i = 0; i < mStash.size() && mStash[i].isEmpty() == false; ++i)
        {
            std::cout << "SBin #" << i;

            if (mStash[i].isEmpty())
            {
                std::cout << " - " << std::endl;
            }
            else
            {
                std::cout << "    c_idx=" << mStash[i].idx() << "  hIdx=" << mStash[i].hashIdx() << std::endl;

            }

        }
        std::cout << std::endl;

    }

    void SimpleCuckoo::init(u64 n, u64 statSecParam, bool multiThreaded)
    {
        if (statSecParam != 40) throw std::runtime_error("not implemented");

        mHashes.resize(n * mParams.mNumHashes, 0);
        mHashesView = MatrixView<u64>(mHashes.begin(), mHashes.end(), mParams.mNumHashes);
        u64 binCount = mParams.mBinScaler * n;
        mBins.resize(binCount);
        mStash.resize(mParams.mStashSize);
    }


    void SimpleCuckoo::insertBatch(
        ArrayView<u64> inputIdxs,
        MatrixView<u64> hashs,
        Workspace& w)
    {

        u64 width = mHashesView.size()[1];
        u64 remaining = inputIdxs.size();
        u64 tryCount = 0;
        //u64 evists = 0;

#ifndef  NDEBUG
        if (hashs.size()[1] != width)
            throw std::runtime_error("" LOCATION);
#endif // ! NDEBUG


        for (u64 i = 0; i < inputIdxs.size(); ++i)
        {
            //std::cout << inputIdxs[i] << " hs ";

            for (u64 j = 0; j < mParams.mNumHashes; ++j)
            {
#ifndef  NDEBUG
                mHashesView[inputIdxs[i]][j] = hashs[i][j];
#else
                (mHashesView.data() + inputIdxs[i] * width)[j] = (hashs.data() + i * width)[j];
#endif // ! NDEBUG

                //std::cout << hashs[i][j] << "   ";

            }

            //std::cout << std::endl;

            w.curHashIdxs[i] = 0;
        }


        while (remaining && tryCount++ <100)
        {

            // this data fetch can be slow (after the first loop). 
            // As such, lets do several fetches in parallel.
            for (u64 i = 0; i < remaining; ++i)
            {
#ifndef  NDEBUG
                w.curAddrs[i] = mHashesView[inputIdxs[i]][w.curHashIdxs[i]] % mBins.size();
#else
                w.curAddrs[i] = (mHashesView.data() + inputIdxs[i] * width)[w.curHashIdxs[i]] % mBins.size();
#endif
                //std::cout <<  i << "   idx " << inputIdxs[i]  <<  "  addr "<< w.curAddrs[i] << std::endl;
            }
            //std::cout << std::endl;

            // same thing here, this fetch is slow. Do them in parallel.
            for (u64 i = 0; i < remaining; ++i)
            {
                u64 newVal = inputIdxs[i] | (w.curHashIdxs[i] << 56);
#ifdef THREAD_SAFE_CUCKOO
                w.oldVals[i] = mBins[w.curAddrs[i]].mVal.exchange(newVal, std::memory_order_relaxed);
#else
                w.oldVals[i] = mBins[w.curAddrs[i]].mVal;
                mBins[w.curAddrs[i]].mVal = newVal;
#endif

                //u64 oldIdx = w.oldVals[i] & (u64(-1) >> 8);
                //u64 oldHash = (w.oldVals[i] >> 56);
                //std::cout 
                //    << i << "   bin[" << w.curAddrs[i] << "]  " 
                //    << " gets (" << inputIdxs[i] << ", "<< w.curHashIdxs[i]<< "),"
                //    << " evicts ("<< oldIdx << ", "<< oldHash<< ")" << std::endl;

            }

            // this loop will update the items that were just evicted. The main
            // idea of that our array looks like
            //     |XW__Y____Z __|
            // For X and W, which failed to be placed, lets write over them
            // with the vaues that they evicted.
            u64 putIdx = 0, getIdx = 0;
            while (putIdx < remaining && w.oldVals[putIdx] != u64(-1))
            {
                inputIdxs[putIdx] = w.oldVals[putIdx] & (u64(-1) >> 8);
                w.curHashIdxs[putIdx] = (1 + (w.oldVals[putIdx] >> 56)) % mParams.mNumHashes;
                ++putIdx;
            }

            getIdx = putIdx + 1;

            // Now we want an array that looks like 
            //  |ABCD___________| but currently have 
            //  |AB__Y_____Z____| so lets move them 
            // forward and replace Y, Z with the values
            // they evicted.
            while (getIdx < remaining)
            {
                while (getIdx < remaining &&
                    w.oldVals[getIdx] == u64(-1))
                    ++getIdx;

                if (getIdx >= remaining) break;

                inputIdxs[putIdx] = w.oldVals[getIdx] & (u64(-1) >> 8);
                w.curHashIdxs[putIdx] = (1 + (w.oldVals[getIdx] >> 56)) % mParams.mNumHashes;

                // not needed. debug only
                //std::swap(w.oldVals[putIdx], w.oldVals[getIdx]);

                ++putIdx;
                ++getIdx;
            }

            remaining = putIdx;
            //evists += remaining;

            //std::cout << std::endl;
            //for (u64 i = 0; i < remaining; ++i)
            //    std::cout<< "evicted[" << i << "]'  " << inputIdxs[i] << "  " << w.curHashIdxs[i] << std::endl;
            //std::cout << std::endl;

        }

        // put any that remain in the stash.

		ArrayView<u64> inputIdxs2(inputIdxs.begin(), inputIdxs.begin() + remaining, false);
		MatrixView<u64> hashs2(hashs.data(), remaining, mParams.mNumHashes, false);

		/*for (u64 i = 0; i < remaining; ++i)
		{
			std::cout << "inputIdxs2 " << inputIdxs2[i] << std::endl;
		}

		for (u64 i = 0; i < remaining; ++i)
		{
			std::cout << "hashs2 " << hashs2[i][w.curHashIdxs[i]] << std::endl;
		}*/

		for (u64 i = 0, j = 0; i < remaining; ++j)
		{
		/*	std::cout << "inputIdxs[i] " << inputIdxs[i] << std::endl;
			std::cout << "w.curHashIdxs[i] " << hashs[i][w.curHashIdxs[i]] << std::endl;*/

			mStash[j].swap(inputIdxs[i], w.curHashIdxs[i]);

			if (inputIdxs[i] == -1)
			{
				//std::cout << "total evicts " << evists << std::endl;
				++i;
			}
		}

        //std::cout << "total evicts "<< evists << std::endl;
    }



    u64 SimpleCuckoo::findBatch(
        MatrixView<u64> hashes,
        ArrayView<u64> idxs,
        Workspace& w)
    {

        if (mParams.mNumHashes == 2)
        {
            std::array<u64, 2>  addr;

            for (u64 i = 0; i < hashes.size()[0]; ++i)
            {
                idxs[i] = -1;

                addr[0] = (hashes[i][0]) % mBins.size();
                addr[1] = (hashes[i][1]) % mBins.size();

#ifdef THREAD_SAFE_CUCKOO
                w.findVal[i][0] = mBins[addr[0]].mVal.load(std::memory_order::memory_order_relaxed);
                w.findVal[i][1] = mBins[addr[1]].mVal.load(std::memory_order::memory_order_relaxed);
#else
                w.findVal[i][0] = mBins[addr[0]].mVal;
                w.findVal[i][1] = mBins[addr[1]].mVal;
#endif
            }

            for (u64 i = 0; i < hashes.size()[0]; ++i)
            {
                if (w.findVal[i][0] != -1)
                {
                    u64 itemIdx = w.findVal[i][0] & (u64(-1) >> 8);

                    bool match =
                        (mHashesView[itemIdx][0] == hashes[i][0]) &&
                        (mHashesView[itemIdx][1] == hashes[i][1]);

                    if (match) idxs[i] = itemIdx;
                }

                if (w.findVal[i][1] != -1)
                {
                    u64 itemIdx = w.findVal[i][1] & (u64(-1) >> 8);

                    bool match =
                        (mHashesView[itemIdx][0] == hashes[i][0]) &&
                        (mHashesView[itemIdx][1] == hashes[i][1]);

                    if (match) idxs[i] = itemIdx;
                }

                // stash
                if (idxs[i] == -1)
                {
                    u64 j = 0;
                    while (j < mStash.size() && mStash[j].isEmpty() == false)
                    {
#ifdef THREAD_SAFE_CUCKOO
                        u64 val = mStash[j].mVal.load(std::memory_order::memory_order_relaxed);
#else
                        u64 val = mStash[j].mVal;
#endif
                        if (val != u64(-1))
                        {
                            u64 itemIdx = val & (u64(-1) >> 8);
                             

                            bool match =
                                (mHashesView[itemIdx][0] == hashes[i][0]) &&
                                (mHashesView[itemIdx][1] == hashes[i][1]);

                            if (match)
                            {
                                idxs[i] = itemIdx;
                            }
                            
                        }

                        ++j;
                    }
                }

            } 


        }
        else
        {
            std::vector<u64> addr(hashes.size()[1]);

            for (u64 i = 0; i < hashes.size()[0]; ++i)
            {
                idxs[i] = -1;

                for(u64 j =0; j < hashes.size()[1]; ++j)
                    addr[j] = hashes[i][j] % mBins.size();

#ifdef THREAD_SAFE_CUCKOO
                for (u64 j = 0; j < hashes.size()[1]; ++j)
                    w.findVal[i][j] = mBins[addr[j]].mVal.load(std::memory_order::memory_order_relaxed);
#else
                for (u64 j = 0; j < hashes.size()[1]; ++j)
                    w.findVal[i][j] = mBins[addr[j]].mVal;
#endif
            }

            for (u64 i = 0; i < hashes.size()[0]; ++i)
            {
                for (u64 j = 0; j < hashes.size()[1] && idxs[i] == -1; ++j)
                {

                    if (w.findVal[i][j] != -1)
                    {
                        u64 itemIdx = w.findVal[i][j] & (u64(-1) >> 8);

                        bool match = true;

                        for (u64 k = 0; k < hashes.size()[1]; ++k)
                        {
                            match &= (mHashesView[itemIdx][k] == hashes[i][k]);
                        }

                        if (match) idxs[i] = itemIdx;
                    }
                }

                // stash
                if (idxs[i] == -1)
                {
                    u64 j = 0;
                    while (j < mStash.size() && mStash[j].isEmpty() == false)
                    {
#ifdef THREAD_SAFE_CUCKOO
                        u64 val = mStash[j].mVal.load(std::memory_order::memory_order_relaxed);
#else
                        u64 val = mStash[j].mVal;
#endif
                        if (val != u64(-1))
                        {
                            u64 itemIdx = val & (u64(-1) >> 8);
                            bool match = true;

                            for (u64 k = 0; k < hashes.size()[1]; ++k)
                            {
                                match &= (mHashesView[itemIdx][k] == hashes[i][k]);
                            }

                            if (match) idxs[i] = itemIdx;

                        }

                        ++j;
                    }
                }


            }

        }
        return u64(-1);
    }

    u64 SimpleCuckoo::stashUtilization()
    {

        i64 i = 0;
        while (i < mStash.size() && mStash[i].isEmpty() == false)
        {
            ++i;
        }

        return i;
    }




    bool SimpleCuckoo::Bin::isEmpty() const
    {
        return mVal == u64(-1);
    }

    u64 SimpleCuckoo::Bin::idx() const
    {
        return mVal  & (u64(-1) >> 8);
    }

    u64 SimpleCuckoo::Bin::hashIdx() const
    {
        return mVal >> 56;
    }

    void SimpleCuckoo::Bin::swap(u64 & idx, u64 & hashIdx)
    {
		
        u64 newVal = idx | (hashIdx << 56);
#ifdef THREAD_SAFE_CUCKOO
        u64 oldVal = mVal.exchange(newVal, std::memory_order_relaxed);
#else
        u64 oldVal = mVal;
        mVal = newVal;
#endif
        if (oldVal == u64(-1))
        {
            idx = hashIdx = u64(-1);
        }
        else
        {
            idx = oldVal & (u64(-1) >> 8);
			std::cout << "iidx " << idx << std::endl;
            hashIdx = oldVal >> 56;
        }
    }
}
