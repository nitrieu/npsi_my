
#include "SimpleCuckoo.h"
using namespace osuCrypto;
#include "CLP.h"
#include "Hashing/CuckooHasher1.h"
#include <fstream>

#define powNs  { 20}
#define powTrials {20/*,20*/}
//#define ee {1.1,1.2,1.5,2.0}
#define ee {/*1.05, 1.1,1.12,1.13,*/1.14,1.15,1.16,1.17,1.2}

//myCuckooTest_bin
#define Ns  { /*8,12,16,*/256/*,24 */}
#define powTrials_bin {20,25,30}
#define ee_bin {/*2.4	,3.0,	4.0,	5.0,	6.0,	7.0,	8.0,	9.0,	10.0,	20.0,*/	100.0,200.0}

void myCuckooTest_stash()
{
	
	for (auto e : ee)
	{
	for (auto powN : powNs)	{	
		//u64 avgMaxStashSize = 0;
		for (auto powTrial : powTrials)
		{
				u64 n = 1 << powN;
				u64 h = 3;
				u64 trials = 1 << powTrial;
				u64 numThrds = 64;
				std::vector<u64> maxStashSize(numThrds);

				auto routine = [trials, &maxStashSize, n, h, e, numThrds](u64 tIdx)
				{

					std::vector<u64> idxs(n);
					std::vector<u64> hashs(n * h + 1);
					MatrixView<u64> hashes(n, h);
					SimpleCuckoo::Workspace ws(n, h);
					maxStashSize[tIdx] = 0;

					u64 startIdx = tIdx * trials / numThrds;
					u64 endIdx = (tIdx + 1) * trials / numThrds;
					for (u64 i = 0; i < endIdx - startIdx; ++i)
					{
						PRNG prng(_mm_set1_epi64x(tIdx) ^ _mm_set1_epi64x(i));
						//prng.mAes.ecbEncCounterMode(prng.mBlockIdx, n * h / 2, (block*)hashs.data());
						//MatrixView<u64> hashes((u64*)hashs.data(), n, h, false);

						for (u64 i = 0; i < n; ++i)
						{
							idxs[i] = i;
							for (u64 j = 0; j < h; ++j)
							{
								hashes[i][j] = prng.get<u64>();
							}
						}

						SimpleCuckoo c;

						c.mParams.mBinScaler = e;
						c.mParams.mNumHashes = h;
						c.mParams.mStashSize = n;
						c.init(n, 40, false);

						c.insertBatch(idxs, hashes, ws);
						u64 stashSize = c.stashUtilization();
						//c.print();
						if (maxStashSize[tIdx] < stashSize && tIdx==0)
						{
							maxStashSize[tIdx] = stashSize;
						//	std::cout << "stashSize: " << i << "  " << maxStashSize[tIdx] << std::endl;
						}
					}

				};

				std::vector<std::thread> thrds(numThrds);

				for (u64 i = 0; i < numThrds; ++i)
				{
					thrds[i] = std::thread([&, i]() {routine(i); });
				}


				for (u64 i = 0; i < numThrds; ++i)
				{
					thrds[i].join();
				}

				u64 finalMaxStashSize = 0;

				for (u64 i = 0; i < numThrds; ++i)
				{
					if (finalMaxStashSize < maxStashSize[i])
						finalMaxStashSize = maxStashSize[i];
				}
				std::cout << "n=" << powN << "   trial=" << powTrial <<"   e=" <<e<<  "   maxStashSize=" << finalMaxStashSize << std::endl;
			//	if(avgMaxStashSize<finalMaxStashSize)
					//avgMaxStashSize=finalMaxStashSize
			}
		}
	}
	

}


void myCuckooTest_bin()
{
	for (auto n : Ns)
	{
		for (auto powTrial : powTrials_bin)
		{
			for (auto e : ee_bin)
			{
				//u64 n = 1 powN;
				u64 h = 3;
				u64 trials = 1 << powTrial;
				u64 numThrds = 32;
				std::vector<u64> maxStashSize(numThrds);

				auto routine = [trials, &maxStashSize, n, h, e, numThrds](u64 tIdx)
				{

					std::vector<u64> idxs(n);
					std::vector<u64> hashs(n * h + 1);
					MatrixView<u64> hashes(n, h);
					SimpleCuckoo::Workspace ws(n, h);
					maxStashSize[tIdx] = 0;

					u64 startIdx = tIdx * trials / numThrds;
					u64 endIdx = (tIdx + 1) * trials / numThrds;
					for (u64 i = 0; i < endIdx - startIdx; ++i)
					{
						PRNG prng(_mm_set1_epi64x(tIdx) ^ _mm_set1_epi64x(i));
						//prng.mAes.ecbEncCounterMode(prng.mBlockIdx, n * h / 2, (block*)hashs.data());
						//MatrixView<u64> hashes((u64*)hashs.data(), n, h, false);

						for (u64 i = 0; i < n; ++i)
						{
							idxs[i] = i;
							for (u64 j = 0; j < h; ++j)
							{
								hashes[i][j] = prng.get<u64>();
							}
						}

						SimpleCuckoo c;

						c.mParams.mBinScaler = e;
						c.mParams.mNumHashes = h;
						c.mParams.mStashSize = n;
						c.init(n, 40, false);

						c.insertBatch(idxs, hashes, ws);
						u64 stashSize = c.stashUtilization();
						//c.print();
						if (maxStashSize[tIdx] < stashSize)
							maxStashSize[tIdx] = stashSize;

						//	std::cout << "stashSize: " << i << "  " << stashSize << std::endl;
					}

				};

				std::vector<std::thread> thrds(numThrds);

				for (u64 i = 0; i < numThrds; ++i)
				{
					thrds[i] = std::thread([&, i]() {routine(i); });
				}


				for (u64 i = 0; i < numThrds; ++i)
				{
					thrds[i].join();
				}

				u64 finalMaxStashSize = 0;

				for (u64 i = 0; i < numThrds; ++i)
				{
					if (finalMaxStashSize < maxStashSize[i])
						finalMaxStashSize = maxStashSize[i];
				}
				std::cout << "n=" << n << "   trial=" << powTrial << "   e=" << e << "   maxStashSize=" << finalMaxStashSize << std::endl;

			}
		}
	}


}

void tt()
{
    u64 n = 10000;

    double e = 2;
    u64 h = 3;
    std::vector<u64> _hashes(n * h + 1);
    std::vector<u64> idx(n);
    MatrixView<u64> hashes(_hashes.begin(), _hashes.end(), h);
    PRNG prng(ZeroBlock);

	
    for (u64 i = 0; i < hashes.size()[0]; ++i)
    {
        idx[i] = i;
        for (u64 j = 0; j < h; ++j)
        {
            hashes[i][j] = prng.get<u64>();
        }
    }

    SimpleCuckoo hashMap1;
    SimpleCuckoo::Workspace w(n, h);
    hashMap1.mParams.mBinScaler = e;
    hashMap1.mParams.mNumHashes = h;
    hashMap1.mParams.mStashSize = 400;

    hashMap1.init(n, 40, true);

    hashMap1.insertBatch(idx, hashes, w);
	hashMap1.print();

    std::vector<u64> idxret(n);


    hashMap1.findBatch(hashes, idxret, w);
    for (u64 i = 0; i < hashes.size()[0]; ++i)
    {
        if (idxret[i] != i)
        {
            std::cout << i << std::endl;
            throw std::runtime_error("");
        }
    }
}

void simpleTest_find_e(int argc, char** argv)
{
    //tt();
    //return;

    std::fstream out;
    out.open("./stashSizes.txt", out.out | out.trunc);

    CLP cmd;
    cmd.parse(argc, argv);
    cmd.setDefault("n", "1000");
    cmd.setDefault("h", "3");
    cmd.setDefault("e", "1.35");
    cmd.setDefault("t", "12");
    cmd.setDefault("x", "3");
    cmd.setDefault("s", "0.05");
    cmd.setDefault("ss", "4");


    u64 stashSize = cmd.getInt("ss");;
    u64 N = cmd.getInt("n");
    u64 h = cmd.getInt("h");
    double e = cmd.getDouble("e");
    u64 t = cmd.getInt("t");

    cmd.setDefault("eEnd", ToString(e));
    double eEnd = cmd.getDouble("eEnd");
    u64 numThrds = cmd.getInt("x");
    double step = cmd.getDouble("s");

    std::cout << "N=" << N << "  h=" << h << "  e=" << e << "  t=" << t << "  s=" << step << std::endl;

    std::atomic<u64> max(0);

    while (step > 0 ? e <= eEnd : e >= eEnd)
    {

        //std::cout << "n=" << n << "  h=" << h << "  e=" << e << "  t=" << t << std::endl;

        std::vector<std::array<u64, 400>> counts(numThrds);
        memset(counts.data(), 0, sizeof(u64) * 400 * numThrds);

        u64 n = N / e;

        u64 tries = u64(1) << t;
        //std::atomic<u64> completed(0);
        max = 0;

        auto routine = [tries, &counts, n, h, e, numThrds](u64 tIdx)
        {

            PRNG prng(_mm_set1_epi64x(tIdx));

            std::vector<u64> hashs(n * h + 1);
            std::vector<u64> idxs(n);


            SimpleCuckoo::Workspace ws(n, h);

            u64 startIdx = tIdx * tries / numThrds;
            u64 endIdx = (tIdx + 1) * tries / numThrds;
            u64 count = endIdx - startIdx;

            for (u64 i = 0; i < count; ++i)
            {
                //if (i % step == 0)std::cout << "\r" << (i / step) << "%" << flush;


                prng.mAes.ecbEncCounterMode(prng.mBlockIdx, n * h / 2, (block*)hashs.data());
                prng.mBlockIdx += n;
                for (u64 i = 0; i < n; ++i)
                {
                    idxs[i] = i;
                }



                SimpleCuckoo c;

                c.mParams.mBinScaler = e;
                c.mParams.mNumHashes = h;
                c.mParams.mStashSize = 400;

                c.init(n, 40, false);


                MatrixView<u64> hashsView((u64*)hashs.data(), n, h, false);

                c.insertBatch(idxs, hashsView, ws);

                u64 stashSize = c.stashUtilization();
                ++counts[tIdx][stashSize];

                //completed.fetch_add(1, std::memory_order::memory_order_relaxed);
                //maxStash = std::max((u64)maxStash, stashSize);
                //std::cout << stashSize << std::endl;
            }
            //std::cout << "\r";

            //completed++;
        };

        std::vector<std::thread> thrds(numThrds);

        for (u64 i = 0; i < numThrds; ++i)
        {
            thrds[i] = std::thread([&, i]() {routine(i); });
        }

        u64 curTotal(0);
        u64 total = (u64(1) << t);
        while ((u64)curTotal != total)
        {
            std::array<u64, 400> count;
            for (u64 i = 0; i < count.size(); ++i)
                count[i] = 0;

            for(u64 t = 0; t < numThrds; ++t)
                for (u64 i = 0; i < count.size(); ++i)
                    count[i] += counts[t][i];


            curTotal = 0;
            for (u64 i = 0; i < count.size(); ++i)
            {
                curTotal += count[i];
            }



            double percent = curTotal * 10000 / tries / 100.0;

            std::cout << "\r " << std::setw(5) << percent << "%  t=" << t << "   e=" << e << "  n="<< n;
            auto p = std::setprecision(3);
            //auto w = std::setw(5);
            u64 good = 0;
            for (u64 i = 0; i < stashSize; ++i)
            {
                
                good += count[i];
                u64 bad = curTotal - good;
                double secLevel = std::log2(std::max(u64(1), good)) - std::log2(std::max(u64(1), bad));

                if (bad == 0)
                {
                    std::cout << "  >" << std::fixed << p << secLevel; 
                }
                else if (good == 0)
                {
                    std::cout << "  <" << std::fixed << p << secLevel; 
                }
                else
                {
                    std::cout << "  " << secLevel; 
                }

                //std::cout << "  "<< std::fixed <<p << secLevel << "  (" << good << " " << bad<<")";
                
            } 

            // first print spaces to clear what was on screen, then print the actual string.
            //std::cout   << flush;


            //stop = max;

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }


        std::array<u64, 400> count;
        for (u64 i = 0; i < count.size(); ++i)
            count[i] = 0;

        for (u64 t = 0; t < numThrds; ++t)
            for (u64 i = 0; i < count.size(); ++i)
                count[i] += counts[t][i];


        curTotal = 0;
        for (u64 i = 0; i < count.size(); ++i)
        {
            curTotal += count[i];
        }



        auto w = std::setw(5);

        std::cout << "\r" << e << "   n " << n;
        out << "e "<< e << "   n "<< n;
        u64 good =0;
        for (u64 i = 0; i < stashSize; ++i)
        {
            // the count of all good events;
            good += count[i];

            // the count of all bad events.
            u64 bad = curTotal - good;

            //std::cout << " (" << i<<" "<< good << ", " << bad << ") ";

            double secLevel = std::log2(std::max(u64(1), good)) - std::log2(std::max(u64(1), bad));

            if (bad == 0)
            {
                std::cout << "  >" << secLevel;
                out << "  >" << secLevel;
            }
            else if (good == 0)
            {
                std::cout << "  <" << secLevel;
                out << "  <" << secLevel;
            }
            else
            {
                std::cout << "  " << secLevel;
                out << "  " << secLevel;
            }

        }

        std::cout << "                                 " << std::endl;
        out << std::endl;



        for (u64 i = 0; i < counts.size(); ++i)
        {
            out << i << "  " << count[i] << std::endl;
        }


        e += step;

        for (u64 i = 0; i < numThrds; ++i)
        {
            thrds[i].join();
        }


    }

    std::cout  << std::endl;

}

void simpleTest(int argc, char** argv)
{
    simpleTest_find_e(argc, argv);
    return;

    std::fstream out;
    out.open("./stashSizes.txt", out.out | out.trunc);

    CLP cmd;
    cmd.parse(argc, argv);
    cmd.setDefault("n", "1000");
    cmd.setDefault("h", "2");
    cmd.setDefault("e", "2");
    cmd.setDefault("t", "16");
    cmd.setDefault("x", "1");
    cmd.setDefault("s", "1.1");

    u64 n = cmd.getInt("n");
    u64 h = cmd.getInt("h");
    double e = cmd.getDouble("e");
    u64 t = cmd.getInt("t");
    u64 numThrds = cmd.getInt("x");
    double step = cmd.getDouble("s");

    //std::cout << "n=" << n << "  h=" << h << "  e=" << e << "  t=" << t << std::endl;

    u64 max = 1;

    while (max)
    {

        std::cout << "n=" << n << "  h=" << h << "  e=" << e << "  t=" << t << std::endl;

        std::array<std::atomic<u64>, 400> counts;

        for (u64 i = 0; i < counts.size(); ++i)
            counts[i] = 0;

        u64 tries = u64(1) << t;
        std::atomic<u64> completed(0);


        auto routine = [tries, &completed, &counts, n, h, e, numThrds](u64 tIdx)
        {

            PRNG prng(_mm_set1_epi64x(tIdx));

            std::vector<u64> hashs(n * h + 1);
            std::vector<u64> idxs(n);


            SimpleCuckoo::Workspace ws(n, h);

            u64 startIdx = tIdx * tries / numThrds;
            u64 endIdx = (tIdx + 1) * tries / numThrds;
            u64 count = endIdx - startIdx;


            for (u64 i = 0; i < count; ++i)
            {
                //if (i % step == 0)std::cout << "\r" << (i / step) << "%" << flush;


                prng.mAes.ecbEncCounterMode(prng.mBlockIdx, n * h / 2, (block*)hashs.data());
                prng.mBlockIdx += n;
                for (u64 i = 0; i < n; ++i)
                {
                    idxs[i] = i;
                }



                SimpleCuckoo c;

                c.mParams.mBinScaler = e;
                c.mParams.mNumHashes = h;
                c.mParams.mStashSize = 400;

                c.init(n, 40, false);


                MatrixView<u64> hashsView((u64*)hashs.data(), n, h, false);

                c.insertBatch(idxs, hashsView, ws);

                u64 stashSize = c.stashUtilization();
                ++counts[stashSize];

                ++completed;
                //maxStash = std::max((u64)maxStash, stashSize);
                //std::cout << stashSize << std::endl;
            }
            //std::cout << "\r";

        };

        std::vector<std::thread> thrds(numThrds);

        for (u64 i = 0; i < numThrds; ++i)
        {
            thrds[i] = std::thread([&, i]() {routine(i); });
        }

        u64 stringLength = 0;
        while ((u64)completed != tries)
        {
            double percent = completed * 10000 / tries / 100.0;

            u64 max = 0;
            for (u64 i = counts.size() - 1; i != 0; --i)
            {
                if (counts[i])
                {
                    max = i;
                    break;
                }
            }

            std::stringstream ss;
            ss << "\r " << std::setw(5) << percent << "%  (" << completed << " / " << tries << ")   count[" << max << "] = " << counts[max];
            std::string str = ss.str();

            // first print spaces to clear what was on screen, then print the actual string.
            //std::cout << '\r' << std::string(' ', stringLength) << flush << str << flush;

            // update how long the string that we just printed is.
            stringLength = str.size();

            //stop = max;

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        max = 0;
        u64 min = 0;
        for (u64 i = counts.size() - 1; i != 0; --i)
        {
            if (counts[i])
            {
                max = i;
                break;
            }
        }
        for (u64 i = 0; i < counts.size(); ++i)
        {
            if (counts[i])
            {
                min = i;
                break;
            }
        }

        std::cout << "\r                                                " << std::endl;

        for (u64 i = min; i <= max; ++i)
        {
            std::cout << i << "  " << counts[i] << std::endl;
        }

        //if (!stop)
        //{

        //    std::cout << "\r" << "h=" << h << "  e=" << e << " passed                                                " << std::endl;
        //    out << "h=" << h << "  e=" << e << " passed" << std::endl;
        //}
        //else
        //{
        //    std::cout << "\r" << "h=" << h << "  e=" << e << " failed " << completed << " / " << tries << "                  "<< std::endl;
        //    out << "h=" << h << "  e=" << e << " failed " << completed << " / " << tries << std::endl;
        //}

        for (u64 i = 0; i < numThrds; ++i)
        {
            thrds[i].join();
        }

        e *= step;

    }
}

void simpleTest_var_h(int argc, char** argv)
{
    //tt();
    //return;

    std::fstream out;
    out.open("./stashSizes.txt", out.out | out.trunc);

    CLP cmd;
    cmd.parse(argc, argv);
    cmd.setDefault("n", "1000");
    cmd.setDefault("h", "2");
    cmd.setDefault("e", "2");
    cmd.setDefault("t", "16");
    cmd.setDefault("x", "1");

    u64 n = cmd.getInt("n");
    u64 h = cmd.getInt("h");
    double e = cmd.getDouble("e");
    u64 t = cmd.getInt("t");
    u64 numThrds = cmd.getInt("x");

    //std::cout << "n=" << n << "  h=" << h << "  e=" << e << "  t=" << t << std::endl;

    e = 1;
    double step = 0.05;

    for (u64 h = 6; h > 1; --h)
    {
        u64 max = 1;

        while (max)
        {

            std::cout << "n=" << n << "  h=" << h << "  e=" << e << "  t=" << t << std::endl;

            std::atomic<bool>  stop(false);
            std::array<std::atomic<u64>, 400> counts;

            for (u64 i = 0; i < counts.size(); ++i)
                counts[i] = 0;

            u64 tries = u64(1) << t;
            std::atomic<u64> completed(0);


            auto routine = [tries, &completed, &counts, &stop, n, h, e, numThrds](u64 tIdx)
            {

                PRNG prng(_mm_set1_epi64x(tIdx));

                std::vector<u64> hashs(n * h + 1);
                std::vector<u64> idxs(n);


                SimpleCuckoo::Workspace ws(n, h);

                u64 startIdx = tIdx * tries / numThrds;
                u64 endIdx = (tIdx + 1) * tries / numThrds;
                u64 count = endIdx - startIdx;


                for (u64 i = 0; i < count && !stop; ++i)
                {
                    //if (i % step == 0)std::cout << "\r" << (i / step) << "%" << flush;


                    prng.mAes.ecbEncCounterMode(prng.mBlockIdx, n * h / 2, (block*)hashs.data());
                    prng.mBlockIdx += n;
                    for (u64 i = 0; i < n; ++i)
                    {
                        idxs[i] = i;
                    }



                    SimpleCuckoo c;

                    c.mParams.mBinScaler = e;
                    c.mParams.mNumHashes = h;
                    c.mParams.mStashSize = 400;

                    c.init(n, 40, false);


                    MatrixView<u64> hashsView((u64*)hashs.data(), n, h, false);

                    c.insertBatch(idxs, hashsView, ws);

                    u64 stashSize = c.stashUtilization();
                    ++counts[stashSize];

                    ++completed;
                    //maxStash = std::max((u64)maxStash, stashSize);
                    //std::cout << stashSize << std::endl;
                }
                //std::cout << "\r";

            };

            std::vector<std::thread> thrds(numThrds);

            for (u64 i = 0; i < numThrds; ++i)
            {
                thrds[i] = std::thread([&, i]() {routine(i); });
            }

            u64 stringLength = 0;
            while ((u64)completed != tries && !stop)
            {
                double percent = completed * 10000 / tries / 100.0;

                u64 max = 0;
                for (u64 i = counts.size() - 1; i != 0; --i)
                {
                    if (counts[i])
                    {
                        max = i;
                        break;
                    }
                }

                std::stringstream ss;
                ss << "\r " << std::setw(5) << percent << "%  (" << completed << " / " << tries << ")   count[" << max << "] = " << counts[max];
                std::string str = ss.str();

                // first print spaces to clear what was on screen, then print the actual string.
                //std::cout << '\r' << std::string(' ', stringLength) << flush << str << flush;

                // update how long the string that we just printed is.
                stringLength = str.size();

                stop = max;

                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            std::cout << "\r" << std::endl;

            max = 0;
            for (u64 i = counts.size() - 1; i != 0; --i)
            {
                if (counts[i])
                {
                    max = i;
                    break;
                }
            }

            if (!stop)
            {

                out << "h=" << h << "  e=" << e << " passed" << std::endl;
            }
            else
            {
                out << "h=" << h << "  e=" << e << " failed " << completed << " / " << tries << std::endl;
            }

            for (u64 i = 0; i < numThrds; ++i)
            {
                thrds[i].join();
            }

            if (max) e += step;

        }
    }
}
