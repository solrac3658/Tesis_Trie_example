#define BOOST_TEST_MODULE test host
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

#include "Host.hpp"

	
	string s;  
	SHA256_CTX ctx;
	uint256_t hash1, f_hash;
	uint160_t llave; 
	AccountData *cuenta = nullptr;
	uint160_t llavero;
	evmc::address addr,addrVacio;	
	evmc::bytes32 keyA, valueB, valueR;
	TrieNode * root = nullptr;
	//uint160_t key2(addr);
	TrieNode *nodo=nullptr;
	TrieNode * raiz=nullptr;
	ExampleHost* host = nullptr;

void init_vars() {
	s = "608060405234801561001057600080fd5b50336000806101000a81548173ffffffffffffffffffffffffffffffffffffffff021916908373ffffffffffffffffffffffffffffffffffffffff1602179055506000600181905550610246806100686000396000f300608060405260043610610057576000357c0100000000000000000000000000000000000000000000000000000000900463ffffffff16806311713df21461005c57806341c0e1b51461008757806393e84cd91461009e575b600080fd5b34801561006857600080fd5b506100716100a8565b6040518082815260200191505060405180910390f35b34801561009357600080fd5b5061009c6100b2565b005b6100a6610147565b005b6000600154905090565b6000809054906101000a900473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff163373ffffffffffffffffffffffffffffffffffffffff1614151561010d57600080fd5b6000809054906101000a900473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16ff5b670de0b6b3a76400003414151561015d57600080fd5b6000809054906101000a900473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff166108fc349081150290604051600060405180830381858888f193505050501580156101c4573d6000803e3d6000fd5b50600180600082825401925050819055503373ffffffffffffffffffffffffffffffffffffffff167fa75916785ed274ade409b333cb54e33f4d515761c89b45c54ba3d9d944b109c960405160405180910390a25600a165627a7a72305820cc407087701b513237584fca8eaef984eb2d5cdc4fb55ec2d7d33a1f83e9f5480029";
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, s.c_str(), (sizeof(char) * s.size()) - 1);
	SHA256_Final((unsigned char*)&hash1, &ctx);
	SHA256((unsigned char*)&hash1, sizeof(hash1), (unsigned char*)&f_hash);
	
	llave = 0x27EE05FF;

	if (cuenta != nullptr)
		delete cuenta;
	cuenta = new AccountData();
	cuenta->SetKey(llave);
	cuenta->SetAge(0x25E325E1E1);
	cuenta->SetBalance(0xFFFFFF);
	cuenta->SetLimit(0xFF);
	llavero = cuenta->Key();
	addrVacio = {};

	for (int i = 0; i < 20; i += 8) {
		addr.bytes[i] = 0xAB;
		addr.bytes[i + 1] = 0xBB;
		addr.bytes[i + 2] = 0xCC;
		addr.bytes[i + 3] = 0xDD;

		addr.bytes[i + 4] = 0x01;
		addr.bytes[i + 5] = 0xEE;
		addr.bytes[i + 6] = 0xFF;
		addr.bytes[i + 7] = 0x67;
	}
	for (int i = 0 ; i < 32; i += 8) {
		keyA.bytes[i] = 0x00;
		keyA.bytes[i + 1] = 0xBB;
		keyA.bytes[i + 2] = 0xCC;
		keyA.bytes[i + 3] = 0xDD;

		keyA.bytes[i + 4] = 0x11;
		keyA.bytes[i + 5] = 0xEE;
		keyA.bytes[i + 6] = 0xFF;
		keyA.bytes[i + 7] = 0x00;

		valueB.bytes[i] = 0x11;
		valueB.bytes[i + 1] = 0xFF;
		valueB.bytes[i + 2] = 0xE0;
		valueB.bytes[i + 3] = 0x11;

		valueB.bytes[i + 4] = 0xDD;
		valueB.bytes[i + 5] = 0xCC;
		valueB.bytes[i + 6] = 0xBB;
		valueB.bytes[i + 7] = 0x11;
	}

	if(nodo!=nullptr)
		delete nodo;
	nodo = new TrieNode(1);
	nodo->SetKey(uint160_t(addr));
	nodo->SetAge(0x25E325E1E1);
	nodo->SetBalance(0xFFFFFA);
	nodo->SetLimit(0xFF);
	nodo->SetContractHash(f_hash);
	llavero = nodo->Key();

	if (raiz!= nullptr)
		delete raiz;
	raiz = new TrieNode(3);
	raiz->SetKey(0xAAAA);
	raiz->SetAge(0x25E325E1E1);
	raiz->SetBalance(0xFFFFFF);
	raiz->SetLimit(0xFF);

	TrieEngine::Insert(&root, raiz, 0);
	TrieEngine::Insert(&raiz, nodo, 0);

	if (host != nullptr)
		delete host;
	host  = new ExampleHost();
	host->Raiz(raiz);


}

BOOST_AUTO_TEST_CASE(Host_account_exists){
	
	init_vars();
    BOOST_CHECK(host->account_exists(addr)==true);
    BOOST_CHECK(host->account_exists(addrVacio)==false);
}
