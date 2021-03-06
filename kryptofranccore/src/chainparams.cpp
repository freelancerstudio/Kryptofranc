// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Copyright (c) 2018-2019 The Kryptofranc Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <arith_uint256.h>

#include <chainparamsseeds.h>
#include <consensus/merkle.h>
#include <tinyformat.h>
#include <util/system.h>
#include <util/strencodings.h>
#include <versionbitsinfo.h>

#include <assert.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>


//Mining algorithm


const arith_uint256 maxUint = UintToArith256(
        uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));

static void MineGenesis(CBlockHeader &genesisBlock, const uint256 &powLimit, bool noProduction) {
    if (noProduction) genesisBlock.nTime = std::time(0);
    genesisBlock.nNonce = 0;

    printf("NOTE: Genesis nTime = %u \n", genesisBlock.nTime);
    printf("WARN: Genesis nNonce (BLANK!) = %u \n", genesisBlock.nNonce);

    arith_uint256 besthash;
    memset(&besthash, 0xFF, 32);
    arith_uint256 hashTarget = UintToArith256(powLimit);
    printf("Target: %s\n", hashTarget.GetHex().c_str());
    arith_uint256 newhash = UintToArith256(genesisBlock.GetHash());
    while (newhash > hashTarget) {
        genesisBlock.nNonce++;
        if (genesisBlock.nNonce == 0) {
            printf("NONCE WRAPPED, incrementing time\n");
            ++genesisBlock.nTime;
        }
        // If nothing found after trying for a while, print status
        if ((genesisBlock.nNonce & 0xffff) == 0)
            printf("nonce %08X: hash = %s \r",
                   genesisBlock.nNonce, newhash.ToString().c_str(),
                   hashTarget.ToString().c_str());

        if (newhash < besthash) {
            besthash = newhash;
            printf("New best: %s\n", newhash.GetHex().c_str());
        }
        newhash = UintToArith256(genesisBlock.GetHash());
    }
    printf("\nGenesis nTime = %u \n", genesisBlock.nTime);
    printf("Genesis nNonce = %u \n", genesisBlock.nNonce);
    printf("Genesis nBits: %08x\n", genesisBlock.nBits);
    printf("Genesis Hash = %s\n", newhash.ToString().c_str());
    printf("Genesis Hash Merkle Root = %s\n", genesisBlock.hashMerkleRoot.ToString().c_str());
    printf("Genesis Hash Merkle Root = %s\n", genesisBlock.hashMerkleRoot.ToString().c_str());
}


static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "30/05/2019 RT: Beijing has accused Washington of engaging in naked economic terrorism.";
    const CScript genesisOutputScript = CScript() << ParseHex("04bff16e305296ab93ee46fad2593b3fc13261aff978bd02732012956d4e6ba2cdb30e168adfef3fb3770cdd1857e3d1ba86a55c8072ec8d343767158a781a7472") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 525600/2.5;

        consensus.BIP16Exception = uint256S("0x00067ca5a4b9f4bb12eb66dbdbe8799d4089da90c65ac7b8db2aa475da7dc690");
        consensus.BIP34Height = 1; // optimization starting from 1;
        consensus.BIP34Hash = uint256S("0x00067ca5a4b9f4bb12eb66dbdbe8799d4089da90c65ac7b8db2aa475da7dc690");
        consensus.BIP65Height = 1; // optimization starting from 1; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.BIP65Height = 1; // optimization starting from 1; // 00000000000000000379eaa19dce8c9b722d46ae6a57c2f1a988119488b50931
        //consensus.powLimit = uint256S("0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.powLimit=uint256S("0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        //consensus.powLimit=uint256S("07ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

        consensus.nPowTargetTimespan =   2.5 * 60 * 10;
        consensus.nPowTargetSpacing = 2.5 * 60 ;


        assert(consensus.nPowTargetTimespan>= consensus.nPowTargetSpacing );

        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916*4; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016*4;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1462060800; // May 1st, 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1493596800; // May 1st, 2017

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1479168000; // November 15th, 2016.
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1510704000; // November 15th, 2017.

        // The best chain should have at least this much work.
        //consensus.nMinimumChainWork = uint256S("0x1d00fff0");
        consensus.nMinimumChainWork = uint256S("000000000000000000000000000000000000000000000000fa631fb237b72ada");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00"); //563378

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x4b;
        pchMessageStart[1] = 0x59;
        pchMessageStart[2] = 0x46;
        pchMessageStart[3] = 0x20;
        nDefaultPort = 1789;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 10; //10
        m_assumed_chain_state_size = 3; //3

        //genesis = CreateGenesisBlock(std::time(0), 0, 0x1d00ffff, 1, 50 * COIN);
        //MineGenesis(genesis, consensus.powLimit, true);
        //exit (0);
        genesis = CreateGenesisBlock(1559254695,2268597939,0x1d00ffff,1, 50*COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000000cb6124045a6aee1ffb4c5c42f43deddc4b3831a2f978c3f650871c05"));
        assert(genesis.hashMerkleRoot == uint256S("0xce040eb4a18c95973ee0ca618979b23a61fcbf46b65f2621fdf7aadf35e4f026"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they don't support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        vSeeds.emplace_back("seed1.kryptofranc.net"); // removed
        vSeeds.emplace_back("seed2.kryptofranc.net"); // removed
        vSeeds.emplace_back("seed3.kryptofranc.net"); // removed
        vSeeds.emplace_back("seed4.kryptofranc.net"); //

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,'K');
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,'k');
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,'+');
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "kf";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = {
                {
                        { 1, uint256S("000000009419e2f790c8798163414f63dcc2a2409368e7d34b95aa382146b4f4")},
                        { 10, uint256S("000000001f98f2feaeae6becc5725a9ad346c55c54041ec5f246618b6d5cbcef")},
                        { 30, uint256S("00000000b3a6eaf089b7aacc1eb87d7a8df1b6b4fd4d993b29ae6cc124d0a853")},
                        { 60, uint256S("000000002b08560496661acc5f1f269d4d563c9d8c011c54065bcbe8c02a0c16")},
                        { 90, uint256S("00000000e5339a910c0e6822d47ef7e056e4ea0051d9971104a5ca9b633cd50e")},
                        { 100, uint256S("00000000cb24c7def63bf8cd5de8a5135bcb24eebd3397d87c5b33d6d53360b4")},
                        { 500, uint256S("000000009ea4bc914dc63fc2b12ca177a7d29cb4c7c92ff13ce75d3b47cb08fd")},
                        { 1000, uint256S("0000000085b562f7360b2d9af027d8593ce24707e428b2112f5165afdbf0f9e8")},
                        { 1500, uint256S("00000000918ab6151aa5ab89920562991c297453200c43c27ea8a189cd238c39")},
                        { 2000, uint256S("0000000066176206ed353d844d33ff883d0b8655d23894db008266482ee00a84")},
                        { 3000, uint256S("0000000006db18634345b009cd5c9e4e3ed96f7d83a3400b80d7ef425a07a44f")},
                        { 4000, uint256S("00000000b5522b41dd1b4236243924641a51ac4a119790041f094fd8c8c198fd")},
                        { 5000, uint256S("00000000afccfe6886f3ae7a82f4083eaf2d5f4dcbaf82e4f0a65f87491a50a9")},
                        { 6000, uint256S("0000000079a1508a30370dcf29adce23662fca2e3b042fdef390296a1a5a992f")},
                        { 7000, uint256S("00000000a1ec00859e854269df331fc13f1d0f87379972bc75792016e8cbf743")},
                        { 8000, uint256S("0000000052863c9a2445a9960eef0b6aacf820c5ab5f9eedc8a472c46adf4379")},
                        { 9000, uint256S("00000000166f5435a956906efadbf06dafbc17272e38a45ff1b01da35f0fd44c")},
                        { 10000, uint256S("000000000c9ccbb284952629cb1d6447641d2d97494c7383d433b7e597130c98")},
                        { 11000, uint256S("0000000063c1ba4954c6632498f0b685ca725be2db37b1135f02f10ffb7c7eaf")},
                        { 12000, uint256S("000000007ab881f7c0963549938a8adde701a3efedfb096d36ff3e3070c278ed")},
                        { 13000, uint256S("000000003af450f804c253181cfbb27ce479e903b0dcd2b34aeb0600c8ba3532")},
                        { 14000, uint256S("0000000017f442c1f50bff5871aaa6984bee6cb118f7dc2725d622c6a05d4653")},
                        { 15000, uint256S("0000000092d074919ec5ddfb193445db8580480f8a4267396c3701ffb6dec6a8")},
                        { 16000, uint256S("0000000051fcded06f2d44113d30aec60e1aea8250ffa49ecfc1a5362b4aa410")},
                        { 17000, uint256S("000000004644ddafc94703bb42c9d9e67ad030950a92492d79f96e3a734ea6ec")},
                        { 18000, uint256S("0000000028cdd3380bee69a0d9ed043843ded2afe9a2cf37362285afd6d5b110")},
                        { 19000, uint256S("00000000378b244ec95ccd522faee1e6be63e3ad75dc7fdba5e744bedfea152d")},
                        { 20000, uint256S("00000000078ecf8008d6b92e4d7e27d6a3047094e8de97c523aac87fdbba89e9")},
                        { 21000, uint256S("000000005247605c6771c731d959aa1e06d70d8c44b66e7e96c66a36ddba3592")},
                        { 22000, uint256S("0000000027fb7b42251349a5ae12fe6a10e303be0111878a5bcea8e86f854f64")},
                        { 23000, uint256S("0000000034c382bc75e8465d73e56d352ab92d10b662ae4dedf9ac4b780963d0")},
                        { 24000, uint256S("0000000057b5b20bf6a7e3a7a914f0fef708ddefc8d74dd2e782fac7ece99294")},
                        { 25000, uint256S("000000005bd08be14511bf3de1d6a4bb2f8028fd269dbc2c278131d51862aa5a")},	
						{ 28000, uint256S("000000003bf12dc48d9752cd94850739f74d1665bcbbbcd1827c22c991bea209")},	
						{ 32000, uint256S("0000000000bd30dd8f47d1dfd18f96ca1211ef44ead039b6a8bd9c31337ef0fd")},	
						{ 34000, uint256S("0000000000d740301356b8f4f8bf528d82db7bae3309f8fb9c101522b15b6518")},	
						{ 36000, uint256S("000000003ddafd3b6add1655215d4ab8992a24fcecc28f5baf4381fc92e1371c")},	
						{ 38000, uint256S("000000000eab6ba4eed30e849e9b347b3351a25f9cd44dbd134f45e181f38d09")},	
						{ 40000, uint256S("00000000637bcce1c8b96afea8e7a243e3799f625920856998d364224a3165c5")},	
						{ 42000, uint256S("00000000029c74d9cae8073ccf287ee003eef1f96f97a86e0e0a47af0bb7897e")},							
						{ 44000, uint256S("000000002b99d84c1b3a51d9b9bfe330ab68b4cb61f6168fa0e0b5511296d8b4")},
						{ 46000, uint256S("000000003849df4e3d69d3a50306a9c1ea4dcb7bb98976911402b8fab1d7aaf4")},	
						{ 48000, uint256S("000000005ac12c74e8936d15a883a6a9b69b578cf0a2a0ff8c556b2b8ddb63f5")},
                        { 49000, uint256S("000000005ba0d22cd53f8704937b0f0e7ae3a969e97a6eebf2840ec3822cafac")},
                        { 50000, uint256S("00000000017119bf2dfb771357dbbfb67c9fe51a070547dc42dcf3fd76dc2dbe")},
                        { 60000, uint256S("0000000067b15a764ef6a814c57e3fad14b03dd6a815122de44a2d4c36e5737a")},
                        { 70000, uint256S("000000002eb816140fefaef01d90fe8ad1133262db4317c92257db7cb6a30783")},
                        { 80000, uint256S("000000001d0d683b67b270b79382506fb5088f0fb52cee9d9b749e6097cf823c")},
                        { 90000, uint256S("000000000322e68d565dbd074e9c52f28d37abd355aa6bea8991cb454eb1ed25")},
                        { 100000, uint256S("0000000003424d6e7dcae5a6dd15b53ef653fee3d8d6add66431c5a97820024c")},
                        { 110000, uint256S("0000000001d13bc3d4553fc374b7552806cb65355c3b8c5bb6e88d22d4333303")},
                        { 120000, uint256S("000000000cbaf9025f63b3e2a20fa687cac2a052e0b7994682d984f59c58733d")},
                        { 130000, uint256S("000000000a7fd6ff0beb353387f0c6c155672f71b83845e0d5039371436692e0")},
                        { 140000, uint256S("000000000bff9a801e5f212d6dacdea9e374429f09fcc99f841693b34bb6051b")},
                        { 150000, uint256S("00000000204bd021be5b26357b074e7fb592120f4ab9f65337a4a211143faa7b")},
                        { 160000, uint256S("00000000026039bb94933f2e721afc10eff5e0845a4ad00b1e7028d6b03861e6")},



                }
        };

        chainTxData = ChainTxData{
        };

        /* disable fallback fee on mainnet */
        m_fallback_fee_enabled = false;
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 525600/2.5;

        consensus.BIP16Exception = uint256S("0x00067ca5a4b9f4bb12eb66dbdbe8799d4089da90c65ac7b8db2aa475da7dc690");
        consensus.BIP34Height = 1; // optimization starting from 1;
        consensus.BIP34Hash = uint256S("0x00067ca5a4b9f4bb12eb66dbdbe8799d4089da90c65ac7b8db2aa475da7dc690");
        consensus.BIP65Height = 1; // optimization starting from 1; // 00000000007f6655f22f98e72ed80d8b06dc761d5da09df0fa1dc4be4f861eb6
        consensus.BIP65Height = 1; // optimization starting from 1; // 000000002104c8c45e99a8853285a3b592602a3ccde2b832481da85e9e4ba182
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        //consensus.powLimit("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetSpacing = 2.5 * 60 * 10; // retarget difficulty every 10 blocs or 1500s or 25 min
        consensus.nPowTargetTimespan =   2.5 * 60 ; // block generated every...
        assert(consensus.nPowTargetTimespan<= consensus.nPowTargetSpacing );
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1456790400; // March 1st, 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1493596800; // May 1st, 2017

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1462060800; // May 1st 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1493596800; // May 1st 2017

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00"); //1354312

        pchMessageStart[0] = 0x0b;
        pchMessageStart[1] = 0x11;
        pchMessageStart[2] = 0x09;
        pchMessageStart[3] = 0x07;
        nDefaultPort = 11789;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 10;
        m_assumed_chain_state_size = 2;

        genesis = CreateGenesisBlock(1559254695,2268597939,0x1d00ffff,1, 50*COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000000cb6124045a6aee1ffb4c5c42f43deddc4b3831a2f978c3f650871c05"));
        assert(genesis.hashMerkleRoot == uint256S("0xce040eb4a18c95973ee0ca618979b23a61fcbf46b65f2621fdf7aadf35e4f026"));


        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "tkf";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;


        checkpointData = {
                {
                        {0, uint256S("0x00067ca5a4b9f4bb12eb66dbdbe8799d4089da90c65ac7b8db2aa475da7dc690")},
                }
        };

        chainTxData = ChainTxData{
                // Data from rpc: getchaintxstats 4096 0000000000000037a8cd3e06cd5edbfe9dd1dbcc5dacab279376ef7cfc2b4c75
                /* nTime    */ 0,
                /* nTxCount */ 0,
                /* dTxRate  */ 0
        };

        /* enable fallback fee on testnet */
        m_fallback_fee_enabled = true;
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    explicit CRegTestParams(const ArgsManager& args) {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 1;
        consensus.BIP16Exception = uint256();
        consensus.BIP34Height = 1; // BIP34 activated on regtest (Used in functional tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1; // optimization starting from 1; // BIP65 activated on regtest (Used in functional tests)
        consensus.BIP65Height = 1; // optimization starting from 1; // BIP66 activated on regtest (Used in functional tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

        consensus.nPowTargetSpacing = 2.5 * 60 * 10; // retarget difficulty every 10 blocs or 1500s or 25 min
        consensus.nPowTargetTimespan =   2.5 * 60 ; // block generated every...
        assert(consensus.nPowTargetTimespan<= consensus.nPowTargetSpacing );
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 211789;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;

        UpdateVersionBitsParametersFromArgs(args);

        genesis = CreateGenesisBlock(1559254695,2268597939,0x1d00ffff,1, 50*COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000000cb6124045a6aee1ffb4c5c42f43deddc4b3831a2f978c3f650871c05"));
        assert(genesis.hashMerkleRoot == uint256S("0xce040eb4a18c95973ee0ca618979b23a61fcbf46b65f2621fdf7aadf35e4f026"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = {
                {
                        {0, uint256S("0x00067ca5a4b9f4bb12eb66dbdbe8799d4089da90c65ac7b8db2aa475da7dc690")},
                }
        };

        chainTxData = ChainTxData{
                0,
                0,
                0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "kfrt";

        /* enable fallback fee on regtest */
        m_fallback_fee_enabled = true;
    }

    /**
     * Allows modifying the Version Bits regtest parameters.
     */
    void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
    void UpdateVersionBitsParametersFromArgs(const ArgsManager& args);
};

void CRegTestParams::UpdateVersionBitsParametersFromArgs(const ArgsManager& args)
{
    if (!args.IsArgSet("-vbparams")) return;

    for (const std::string& strDeployment : args.GetArgs("-vbparams")) {
        std::vector<std::string> vDeploymentParams;
        boost::split(vDeploymentParams, strDeployment, boost::is_any_of(":"));
        if (vDeploymentParams.size() != 3) {
            throw std::runtime_error("Version bits parameters malformed, expecting deployment:start:end");
        }
        int64_t nStartTime, nTimeout;
        if (!ParseInt64(vDeploymentParams[1], &nStartTime)) {
            throw std::runtime_error(strprintf("Invalid nStartTime (%s)", vDeploymentParams[1]));
        }
        if (!ParseInt64(vDeploymentParams[2], &nTimeout)) {
            throw std::runtime_error(strprintf("Invalid nTimeout (%s)", vDeploymentParams[2]));
        }
        bool found = false;
        for (int j=0; j < (int)Consensus::MAX_VERSION_BITS_DEPLOYMENTS; ++j) {
            if (vDeploymentParams[0] == VersionBitsDeploymentInfo[j].name) {
                UpdateVersionBitsParameters(Consensus::DeploymentPos(j), nStartTime, nTimeout);
                found = true;
                LogPrintf("Setting version bits activation parameters for %s to start=%ld, timeout=%ld\n", vDeploymentParams[0], nStartTime, nTimeout);
                break;
            }
        }
        if (!found) {
            throw std::runtime_error(strprintf("Invalid deployment (%s)", vDeploymentParams[0]));
        }
    }
}

static std::unique_ptr<const CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<const CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams(gArgs));
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}
