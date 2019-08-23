///////////////////////////// Compilar g++ main.cpp -o trie_exe hash.o version.o allocators.o trieengine.o trie.o -lboost_system -lboost_thread -lssl -lcrypto  -std=c++11 -I.///////////////
// g++ -o trie.exe *.cpp -lssl -lboost_system -lboost_thread -lcrypto -I. -std=c++11 -DDEBUG_TRIE
#include <sqlite3.h>
#include <string>
#include <iostream>

#include "Host.hpp"

using namespace std;

#define MIN(X, Y) (X < Y ? X : Y)

evmc::address se_va_a_borrar;

struct account
{
    evmc::uint256be balance = {};
    size_t code_size = 0;
    evmc::bytes32 code_hash = {};
    std::map<evmc::bytes32, evmc::bytes32> storage;
};

void ExampleHost::Raiz(TrieNode *root){
	raiz = root;
}


bool ExampleHost::account_exists(const evmc::address & addr) noexcept
    {
    
    uint160_t addrc(addr);
    TrieNode *fnd = TrieEngine::Find(addrc, raiz,0);  /////// Se debe modificar para parsear dierecto la raiz del account y verificar que son los bits 
    	
    	if (fnd==0)
    	{
			return false;
    	}else{
    		return true;
    	}

    }

evmc::bytes32 ExampleHost::get_storage(const evmc::address& addr, const evmc::bytes32& key) noexcept 
    {
    	
    	if(account_exists(addr)){

			uint160_t addrc(addr);
	   		sqlite3 *db;
	   		sqlite3_stmt *ppStmt;
	   		char *zErrMsg = 0;
	   		int rc;
	   		string sql,name;
	   		evmc::bytes32 value;
	   		name= addrc.ToString()+".db";
	   		/* Open database */
	   		rc = sqlite3_open(name.c_str(), &db);
	   
	   		if( rc ) {
	      		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	      		return {};
	   		}
	   		sql = "Select COUNT(KEY), VALUE from STORAGE where KEY = ?";
	   		sqlite3_prepare_v2(db, sql.c_str(), sql.size()+1,&ppStmt,NULL);
	   		sqlite3_bind_blob(ppStmt, 1, (void *)&key, 32, SQLITE_STATIC);

	   		sqlite3_step(ppStmt);
	   		if(sqlite3_column_int(ppStmt,0)==1){
	   			evmc::bytes32 var = static_cast<evmc::bytes32>(*static_cast<const evmc::bytes32 *>(sqlite3_column_blob(ppStmt, 1)));	
	   			sqlite3_finalize(ppStmt);
	   			sqlite3_close(db);
	   			return var;
	   		}else{
	   			sqlite3_finalize(ppStmt);
	   			sqlite3_close(db);
	   			return {};
	   		}
    	}else { 
    		return {};} 	
    }

    evmc_storage_status ExampleHost::set_storage(const evmc::address& addr, const evmc::bytes32& key, const evmc::bytes32& value) noexcept 
    {
   		uint160_t addrc(addr);
   		sqlite3 *db;
   		sqlite3_stmt *ppStmt;
   		char *zErrMsg = 0;
   		int rc;
   		string sql,name;
   		evmc::bytes32 prevValue, vacio;
   		vacio={};
   		name= addrc.ToString()+".db";
   		prevValue = get_storage(addr, key);

   		if (account_exists(addr)){
   			/* Open database */
	   		rc = sqlite3_open(name.c_str(), &db);
	   		if( rc ) {
	      		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	      		sqlite3_close(db);
	      		return EVMC_STORAGE_UNCHANGED;
	   		} 
	   		if (prevValue==vacio)
	   		{
	   		    sql = "INSERT into STORAGE VALUES (?,?)";
	            rc = sqlite3_prepare_v2(db, sql.c_str(), sql.size()+1,&ppStmt,NULL);
	            if( rc != SQLITE_OK) {
	            	fprintf(stderr, "prepare: %s\n", sqlite3_errmsg(db));
	            	sqlite3_close(db);
	            	return EVMC_STORAGE_UNCHANGED;
	         	} 
	            rc = sqlite3_bind_blob(ppStmt, 1, (void *)&key, 32, SQLITE_STATIC);
	            if( rc != SQLITE_OK ) {
	            	fprintf(stderr, "bind: %s\n", sqlite3_errmsg(db));
	            	sqlite3_close(db);
	            	return EVMC_STORAGE_UNCHANGED;
	         	} 
	            rc = sqlite3_bind_blob(ppStmt, 2, (void *)&value, 32, SQLITE_STATIC);
	            if( rc != SQLITE_OK ) {
	            	fprintf(stderr, "bind 2: %s\n", sqlite3_errmsg(db));
	            	sqlite3_close(db);
	            	return EVMC_STORAGE_UNCHANGED;
	         	} 
	         	rc = sqlite3_step(ppStmt);
	         	if( rc != SQLITE_DONE ) {
	            	fprintf(stderr, "step: %s\n", sqlite3_errmsg(db));
	            	sqlite3_close(db);
	            	return EVMC_STORAGE_UNCHANGED;
	         	} 
	         	rc = sqlite3_finalize(ppStmt);
	         	if (rc == SQLITE_OK){
	            	fprintf(stderr, "finalize: %s\n", sqlite3_errmsg(db));
	            	sqlite3_close(db);
	               	return EVMC_STORAGE_ADDED;

	            }
	   		}
	   		if (prevValue==value)
	   		{
	   			sqlite3_close(db);
	            return EVMC_STORAGE_UNCHANGED;
	   		}else{

	   			sql = "UPDATE STORAGE set VALUE = ?  WHERE KEY= ?";
	   			sqlite3_prepare_v2(db, sql.c_str(), sql.size()+1,&ppStmt,NULL);
	   			sqlite3_bind_blob(ppStmt, 1, (void *)&value, 32, SQLITE_STATIC);
	   			sqlite3_bind_blob(ppStmt, 2, (void *)&key, 32, SQLITE_STATIC);
	   			sqlite3_step(ppStmt);
				rc = sqlite3_finalize(ppStmt);
				if (rc == SQLITE_OK){
					sqlite3_close(db);
					return EVMC_STORAGE_MODIFIED;
				}

			sqlite3_close(db);
	        return EVMC_STORAGE_UNCHANGED;
	   		}
   		}else{
   			return EVMC_STORAGE_UNCHANGED;
   		}
	   		
   		
    }

    evmc_uint256be ExampleHost::get_balance(const evmc::address& addr) noexcept 
    {
 	
 	uint160_t addrc(addr);
 	TrieNode *fnd = TrieEngine::Find(addrc, raiz,0);  /////// Se debe modificar para parsear dierecto la raiz del account y verificar que son los bits 
    	if (fnd==0)
    	{
			return {};
    	}else{
    		uint64_t balance = fnd->Balance();
    		uint256_t balance256 (balance);
    		evmc_uint256be balanceF(balance256);
    		return balanceF;
    	}
    }

    size_t ExampleHost::get_code_size(const evmc::address& addr) noexcept 
    {
 	
    	uint160_t addrc(addr);
 	TrieNode *fnd = TrieEngine::Find(addrc, raiz,0);  /////// Se debe modificar para parsear dierecto la raiz del account y verificar que son los bits 
    	if (fnd==0)
    	{
			return false;
    	}else{
    		if (fnd->ContractHash()==0)
    			return 0;
    		return ((fnd->Contract().size())/2);
    	}
    }

    evmc::bytes32 ExampleHost::get_code_hash(const evmc::address& addr) noexcept 
    {
    	uint160_t addrc(addr);
 		TrieNode *fnd = TrieEngine::Find(addrc, raiz,0);  /////// Se debe modificar para parsear dierecto la raiz del account y verificar que son los bits 
        if (fnd==0)
    	{
			return {};
    	}else{
    		if (fnd->ContractHash()==0)
    			return {};
    		evmc_uint256be evmchash = static_cast<evmc_uint256be>(fnd->ContractHash());
    		return evmchash;
    	}
    }

    size_t ExampleHost::copy_code(const evmc::address& addr,  size_t code_offset, uint8_t* buffer_data, size_t buffer_size) noexcept 
    {

		if (account_exists(addr))
		{
			uint160_t addrc(addr);
			TrieNode *fnd = TrieEngine::Find(addrc, raiz,0);
			string contract;  /////// Se debe modificar para parsear dierecto la raiz del account y verificar que son los bits 
			if (fnd==0)
			{
				return 0;
			} else {
				if (fnd->ContractHash()==0)
					return 0;
				string contract = fnd->Contract();
				size_t size = contract.size() / 2;
				if(code_offset>size)
					return 0;
				uint8_t * bytes = new uint8_t[size];
				char x[3];
				for (int i = 0, j = 0; (i < size) && (j < contract.size()); i++, j += 2) {
					
					memset(x, '\0', sizeof(char) * 3);
					x[0] = contract[j];
					x[1] = contract[j + 1];
					bytes[i] = static_cast<uint8_t>(strtol(x, NULL, 16));
				}
				memcpy(buffer_data, bytes +code_offset, sizeof(uint8_t) * MIN(buffer_size, size - code_offset));
				return MIN(buffer_size, size - code_offset);
				}
		}else{
			return 0;
		}

    }

    void ExampleHost::selfdestruct(const evmc::address& addr, const evmc::address& beneficiary) noexcept 
    {
       	
		if (account_exists(addr) && account_exists(beneficiary))
		{
	       	uint160_t addrc(addr), addrb(beneficiary);
	       	uint256_t hash_C;
	       	string hashS;
	       	TrieNode *fnd = TrieEngine::Find(addrc, raiz,0);
			TrieNode *bfc = TrieEngine::Find(addrb, raiz,0);
			hash_C = fnd->ContractHash();
			
			DB* db;
			Options options;
			options.create_if_missing = true;
			Status sT = DB::Open(options, "Contract.db", &db); ////////////// hay que agregar la direccion exacta del la base de datos 
			assert(sT.ok());
			sT = db->Delete(WriteOptions(), hash_C.ToString()); 
			delete db;

			bfc->SetBalance(fnd->Balance()+bfc->Balance());
			fnd->SetBalance(0x0);
		}else{
			return;
		}
    }

    evmc::result ExampleHost::call(const evmc_message& msg) noexcept 
    {
        return {EVMC_REVERT, msg.gas, msg.input_data, msg.input_size};
    }

    evmc_tx_context ExampleHost::get_tx_context() noexcept  { 
    	return {}; 
    }

    evmc::bytes32 ExampleHost::get_block_hash(int64_t number) noexcept 
    {

        const int64_t current_block_number = get_tx_context().block_number;

        //auto it = accounts.find(se_va_a_borrar);

        return{}; 
    }

    void ExampleHost::emit_log(const evmc::address& addr,
                  const uint8_t* data,
                  size_t data_size,
                  const evmc::bytes32 topics[],
                  size_t topics_count) noexcept 
    {
        (void)addr;
        (void)data;
        (void)data_size;
        (void)topics;
        (void)topics_count;
    }

/*extern "C" {

evmc_context* example_host_create_context()
{
    return new ExampleHost;
}

void example_host_destroy_context(evmc_context* context)
{
    delete static_cast<ExampleHost*>(context);
}
}*/










