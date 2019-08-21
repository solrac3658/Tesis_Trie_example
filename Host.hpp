#ifndef HOST_HPP
#define HOST_HPP

#include "uint256.h"
#include "hash.h"
#include "trie.h"
#include <evmc/evmc.hpp>


class ExampleHost : public evmc::Host{
private:
    TrieNode *raiz;

public:

void Raiz(TrieNode *root);

bool account_exists(const evmc::address & addr) noexcept;

evmc::bytes32 get_storage(const evmc::address& addr, const evmc::bytes32& key) noexcept final;

evmc_storage_status set_storage(const evmc::address& addr, const evmc::bytes32& key, const evmc::bytes32& value) noexcept final;

evmc_uint256be get_balance(const evmc::address& addr) noexcept final;

size_t get_code_size(const evmc::address& addr) noexcept final;

 evmc::bytes32 get_code_hash(const evmc::address& addr) noexcept final;

size_t copy_code(const evmc::address& addr,  size_t code_offset, uint8_t* buffer_data, size_t buffer_size) noexcept final;

void selfdestruct(const evmc::address& addr, const evmc::address& beneficiary) noexcept final;

 evmc::result call(const evmc_message& msg) noexcept final;

 evmc_tx_context get_tx_context() noexcept final;

 evmc::bytes32 get_block_hash(int64_t number) noexcept final;

 void emit_log(const evmc::address& addr,const uint8_t* data,size_t data_size,const evmc::bytes32 topics[],size_t topics_count) noexcept final;

};

#endif
 