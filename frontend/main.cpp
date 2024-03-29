#include <iostream>
#include "Network/BtChannel.h"
#include "Network/BtEndpoint.h"
#include "Network/BtEndpoint.h" 

#include "OPPRF/OPPRFReceiver.h"
#include "OPPRF/OPPRFSender.h"

#include <fstream>
using namespace osuCrypto;
#include "Tools/Tools.h"
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
#include <iostream>

using namespace std;
#include "Common/Defines.h"
using namespace osuCrypto;

#include "OtBinMain.h"

#include <numeric>
#include "Common/Log.h"

//#include "cuckoo/cuckooTests.h"

void usage(const char* argv0)
{
	std::cout << "Error! Please use:" << std::endl;
	std::cout << "\t 1. For unit test: " << argv0 << " -t" << std::endl;
	std::cout << "\t 2. For simulation (3 parties <=> 3 terminals): " << std::endl;;
	std::cout << "\t\t terminal: " << argv0 << " -p pIdx" << std::endl;
}
int main(int argc, char** argv)
{
	Cache_Test_Impl();
	return 0;

	//std::cout << "fffffffff\n";
	//myCuckooTest_stash();
	//Table_Based_Random_Test();
	//return 0;
	//OPPRF2_EmptrySet_Test_Main();
	//OPPRFn_EmptrySet_Test_Main();
	//Transpose_Test();
	//OPPRF3_EmptrySet_Test_Main();
	//return 0;
//	OPPRFnt_EmptrySet_Test_Main();
	//OPPRFnt_EmptrySet_Test_Main();
	//OPPRFn_Aug_EmptrySet_Test_Impl();
	//OPPRFnt_EmptrySet_Test_Impl();
	//OPPRF2_EmptrySet_Test_Main();
	//OPPRFn_Aug_EmptrySet_Test_Impl();
	//return 0;

	u64 trials = 1;

	std::vector<block> mSet;
	std::vector<u64> mPayload;
	
	u64 setSize = 1 << 16, psiSecParam = 40, bitSize = 128;

	u64 nParties, tParties, opt_basedOPPRF;
	u64 roundOPPRF;
	PRNG prng(_mm_set_epi32(4253465, 3434565, 234435, 23987045));
	mSet.resize(setSize);
	mPayload.resize(setSize);
	for (u64 i = 0; i < setSize; ++i)
	{
		mSet[i] = prng.get<block>();
		mPayload[i] = prng.get<u64>();

	}

	//getBinSizeDistribution(setSize, mSet, psiSecParam);
	//return 0;

	
//Augmented Model
#if 1
	opt_basedOPPRF = atoi(argv[1]);
	nParties = atoi(argv[2]);
	u64 pIdx = atoi(argv[3]);

	//TODO(remove this hack: unconditional zero - sharing);
	//only one time => very mirror effect on perfomance
	std::vector<std::vector<block>> mSeeds(nParties);
	std::vector<std::vector<PRNG>> mPRNGSeeds(nParties);
	for (u64 i = 0; i < nParties; ++i)
	{
		mSeeds[i].resize(nParties);
		for (u64 j = 0; j < nParties; ++j)
		{
			if (i <= j)
				mSeeds[i][j] = prng.get<block>();
			else
				mSeeds[i][j] = mSeeds[j][i];
		}
	}
	for (u64 i = 0; i < nParties; ++i)
	{
		mPRNGSeeds[i].resize(nParties);
		for (u64 j = 0; j < nParties; ++j)
		{
			mPRNGSeeds[i][j].SetSeed(mSeeds[i][j]);
		}
	}

	
	std::cout << "pIdx: " << pIdx << "\t";
	std::cout << "nParties: " << nParties << "\n";

	// First phase
	/*auto routine = [&](u64 user_repeat)
	{*/
		user_server(0, pIdx, nParties - 2, nParties - 1, mSet.size(), 1 << 8, mSet, mPayload, mPRNGSeeds[pIdx], TABLEb, 1);
	/*};

	std::vector<std::thread> thrds(2);
	for (u64 i = 0; i < thrds.size(); ++i)
	{
		thrds[i] = std::thread([=] {
			routine(i);
		});
	}

	for (auto& thrd : thrds)
		thrd.join();*/

	return 0;
#endif

//Standard Model
	if (argc == 7) {
		if (argv[1][0] == '-' && argv[1][1] == 'n')
			nParties = atoi(argv[2]);


		if (nParties == 3)
		{
			if (argv[3][0] == '-' && argv[3][1] == 'r')
				roundOPPRF = atoi(argv[4]);

			if (argv[5][0] == '-' && argv[5][1] == 'p') {
				u64 pIdx = atoi(argv[6]);
				if (roundOPPRF == 1)
				{
					 party3(pIdx, setSize, trials);
					//aug_party(pIdx, nParties, setSize, mSet, mPRNGSeeds[pIdx], opt_basedOPPRF, trials);
				}
				else
				{
					 tparty(pIdx, nParties, 2, setSize, trials);
				//	std::cout << "aug_party";
					//aug_party(pIdx, 2, mSet.size(), mSet, mPRNGSeeds[pIdx], opt_basedOPPRF, trials);
				}

			}
		}
		else
		{
			if (argv[3][0] == '-' && argv[3][1] == 't')
				tParties = atoi(argv[4]);

			if (argv[5][0] == '-' && argv[5][1] == 'p') {
				u64 pIdx = atoi(argv[6]);
				std::cout << "pIdx:  " << pIdx << "\n";
				tparty(pIdx, nParties, tParties, setSize, trials);
			}
		}

	}
	else if (argc == 2) {
		if (argv[1][0] == '-' && argv[1][1] == 'u')
		{
			OPPRFnt_EmptrySet_Test_Main();
		}
	}
	else {
		usage(argv[0]);
	}
	return 0;
}