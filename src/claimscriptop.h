// Copyright (c) 2018 The LBRY developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef CLAIMSCRIPTOP_H
#define CLAIMSCRIPTOP_H

#include "amount.h"
#include "claimtrie.h"
#include "hash.h"
#include "primitives/transaction.h"
#include "script/script.h"
#include "uint256.h"
#include "util.h"

#include <string>
#include <vector>

/**
 * Claim script operation base class
 */
class CClaimScriptOp
{
public:
    virtual ~CClaimScriptOp() {}
    /**
     * Pure virtual, OP_CLAIM_NAME handler
     * @param[in] trieCache     trie to operate on
     * @param[in] name          name of the claim
     */
    virtual bool claimName(CClaimTrieCache& trieCache, const std::string& name) = 0;
    /**
     * Pure virtual, OP_UPDATE_CLAIM handler
     * @param[in] trieCache     trie to operate on
     * @param[in] name          name of the claim
     * @param[in] claimId       id of the claim
     */
    virtual bool updateClaim(CClaimTrieCache& trieCache, const std::string& name, const uint160& claimId) = 0;
    /**
     * Pure virtual, OP_SUPPORT_CLAIM handler
     * @param[in] trieCache     trie to operate on
     * @param[in] name          name of the claim
     * @param[in] claimId       id of the claim
     */
    virtual bool supportClaim(CClaimTrieCache& trieCache, const std::string& name, const uint160& claimId) = 0;
};

/**
 * Class to add claim in trie
 */
class CClaimScriptAddOp : public CClaimScriptOp
{
public:
    /**
     * Constructor
     * @param[in] point     pair of transaction hash and its index
     * @param[in] nValue    value of the claim
     * @param[in] nHeight   entry height of the claim
     */
    CClaimScriptAddOp(const COutPoint& point, CAmount nValue, int nHeight);
    /**
     * Implamention of OP_CLAIM_NAME handler
     * @see CClaimScriptOp::claimName
     */
    bool claimName(CClaimTrieCache& trieCache, const std::string& name);
    /**
     * Implamention of OP_UPDATE_CLAIM handler
     * @see CClaimScriptOp::updateClaim
     */
    bool updateClaim(CClaimTrieCache& trieCache, const std::string& name, const uint160& claimId);
    /**
     * Implamention of OP_SUPPORT_CLAIM handler
     * @see CClaimScriptOp::supportClaim
     */
    bool supportClaim(CClaimTrieCache& trieCache, const std::string& name, const uint160& claimId);

protected:
    /**
     * Reimplement to handle OP_CLAIM_NAME and OP_UPDATE_CLAIM at once
     * @param[in] trieCache     trie to operate on
     * @param[in] name          name of the claim
     * @param[in] claimId       id of the claim
     */
    virtual bool addClaim(CClaimTrieCache& trieCache, const std::string& name, const uint160& claimId);
    const COutPoint point;
    const CAmount nValue;
    const int nHeight;
};

/**
 * Class to undo added claim in trie
 */
class CClaimScriptUndoAddOp : public CClaimScriptOp
{
public:
    /**
     * Constructor
     * @param[in] point     pair of transaction hash and its index
     * @param[in] nHeight   entry height of the claim
     */
    CClaimScriptUndoAddOp(const COutPoint& point, int nHeight);
    /**
     * Implamention of OP_CLAIM_NAME handler
     * @see CClaimScriptOp::claimName
     */
    bool claimName(CClaimTrieCache& trieCache, const std::string& name);
    /**
     * Implamention of OP_UPDATE_CLAIM handler
     * @see CClaimScriptOp::updateClaim
     */
    bool updateClaim(CClaimTrieCache& trieCache, const std::string& name, const uint160& claimId);
    /**
     * Implamention of OP_SUPPORT_CLAIM handler
     * @see CClaimScriptOp::supportClaim
     */
    bool supportClaim(CClaimTrieCache& trieCache, const std::string& name, const uint160& claimId);

protected:
    /**
     * Reimplement to handle OP_CLAIM_NAME and OP_UPDATE_CLAIM at once
     * @param[in] trieCache     trie to operate on
     * @param[in] name          name of the claim
     * @param[in] claimId       id of the claim
     */
    virtual bool undoAddClaim(CClaimTrieCache& trieCache, const std::string& name, const uint160& claimId);
    const COutPoint point;
    const int nHeight;
};

/**
 * Class to spend claim from trie
 */
class CClaimScriptSpendOp : public CClaimScriptOp
{
public:
    /**
     * Constructor
     * @param[in]  point        pair of transaction hash and its index
     * @param[in]  nHeight      entry height of the claim
     * @param[out] nValidHeight valid height of the claim
     */
    CClaimScriptSpendOp(const COutPoint& point, int nHeight, int& nValidHeight);
    /**
     * Implamention of OP_CLAIM_NAME handler
     * @see CClaimScriptOp::claimName
     */
    bool claimName(CClaimTrieCache& trieCache, const std::string& name);
    /**
     * Implamention of OP_UPDATE_CLAIM handler
     * @see CClaimScriptOp::updateClaim
     */
    bool updateClaim(CClaimTrieCache& trieCache, const std::string& name, const uint160& claimId);
    /**
     * Implamention of OP_SUPPORT_CLAIM handler
     * @see CClaimScriptOp::supportClaim
     */
    bool supportClaim(CClaimTrieCache& trieCache, const std::string& name, const uint160& claimId);

protected:
    /**
     * Reimplement to handle OP_CLAIM_NAME and OP_UPDATE_CLAIM at once
     * @param[in] trieCache     trie to operate on
     * @param[in] name          name of the claim
     * @param[in] claimId       id of the claim
     */
    virtual bool spendClaim(CClaimTrieCache& trieCache, const std::string& name, const uint160& claimId);
    const COutPoint point;
    const int nHeight;
    int& nValidHeight;
};

/**
 * Class to undo spent claim from trie
 */
class CClaimScriptUndoSpentOp : public CClaimScriptOp
{
public:
    /**
     * Constructor
     * @param[in] point         pair of transaction hash and its index
     * @param[in] nValue        value of the claim
     * @param[in] nHeight       entry height of the claim
     * @param[in] nValidHeight  valid height of the claim
     */
    CClaimScriptUndoSpentOp(const COutPoint& point, CAmount nValue, int nHeight, int nValidHeight);
    /**
     * Implamention of OP_CLAIM_NAME handler
     * @see CClaimScriptOp::claimName
     */
    bool claimName(CClaimTrieCache& trieCache, const std::string& name);
    /**
     * Implamention of OP_UPDATE_CLAIM handler
     * @see CClaimScriptOp::updateClaim
     */
    bool updateClaim(CClaimTrieCache& trieCache, const std::string& name, const uint160& claimId);
    /**
     * Implamention of OP_SUPPORT_CLAIM handler
     * @see CClaimScriptOp::supportClaim
     */
    bool supportClaim(CClaimTrieCache& trieCache, const std::string& name, const uint160& claimId);

protected:
    /**
     * Reimplement to handle OP_CLAIM_NAME and OP_UPDATE_CLAIM at once
     * @param[in] trieCache     trie to operate on
     * @param[in] name          name of the claim
     * @param[in] claimId       id of the claim
     */
    virtual bool undoSpendClaim(CClaimTrieCache& trieCache, const std::string& name, const uint160& claimId);
    const COutPoint point;
    const CAmount nValue;
    const int nHeight;
    const int nValidHeight;
};

bool ProcessClaim(CClaimScriptOp& operation, CClaimTrieCache& trieCache, const CScript& scriptPubKey);

typedef std::pair<std::string, uint160> spentClaimType;

typedef std::vector<spentClaimType> spentClaimsType;

/**
 * Function to spend claim
 * @param[in]  trieCache        trie to operate on
 * @param[in]  scriptPubKey     claim script
 * @param[in]  point            pair of transaction hash and its index
 * @param[in]  nHeight          entry height of the claim
 * @param[out] nValidHeight     valid height of the claim
 * @param[out] spentClaims      inserts successfully spent claim
 */
bool SpendClaim(CClaimTrieCache& trieCache, const CScript& scriptPubKey, const COutPoint& point, int nHeight, int& nValidHeight, spentClaimsType& spentClaims);

/**
 * Function to add spent claim
 * @param[in]  trieCache        trie to operate on
 * @param[in]  scriptPubKey     claim script
 * @param[in]  point            pair of transaction hash and its index
 * @param[in]  nValue        `  value of the claim
 * @param[in]  nHeight          entry height of the claim
 * @param[out] spentClaims      erases successfully added claim
 */
bool AddSpentClaim(CClaimTrieCache& trieCache, const CScript& scriptPubKey, const COutPoint& point, CAmount nValue, int nHeight, spentClaimsType& spentClaims);

#endif // CLAIMSCRIPTOP_H
