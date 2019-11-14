#pragma once

#include "Crypto/PRNG.h"



//void OPPRFRecv();
//void OPPRFSend();
//void BarkOPRFRecv();
//void BarkOPRSend();
void Channel_test();
void OPPRF3_EmptrySet_Test_Main();
void OPPRFn_EmptrySet_Test_Main();
void OPPRF2_EmptrySet_Test_Main();
void Table_Based_Random_Test();
void OPPRFnt_EmptrySet_Test_Main();
void party3(u64 myIdx, u64 setSize, u64 nTrials);
void party2(u64 myIdx, u64 setSize, u64 nTrials);
void party(u64 myIdx, u64 nParties, u64 setSize, std::vector<block>& mSet);
void tparty(u64 myIdx, u64 nParties, u64 tParties, u64 setSize, u64 nTrials);
void aug_party(u64 myIdx, u64 nParties, u64 setSize, std::vector<block>& mSet, std::vector<PRNG>& mSeedPrng,u64 opt, u64 nTrials);
void user_server(u64 user_repeat, u64 myIdx, u64 leaderIdx, u64 nParties, u64 setSize, u64 k_prior, std::vector<block>& mSet, std::vector<u64>& mPayload, std::vector<PRNG>& mSeedPrng, u64 opt, u64 nTrials);

void OPPRFn_Aug_EmptrySet_Test_Impl();
void OPPRFnt_EmptrySet_Test_Impl();
void getBinSizeDistribution(u64 setSize, std::vector<block> set, u64 psiSecParam);
//void OPPRFn_EmptrySet_Test();
void Transpose_Test();
void Cache_Test_Impl();