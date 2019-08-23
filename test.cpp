#define BOOST_TEST_MODULE test host
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

#include "Host.hpp"
#include <sqlite3.h>
#include <string>
#include <string.h>
#include <stdio.h>

#define MIN(X, Y) (X < Y ? X : Y)

	string s;  
	SHA256_CTX ctx;
	uint256_t hash1, f_hash;
	uint160_t llave; 
	AccountData *cuenta = nullptr;
	uint160_t llavero;
	evmc::address addr,addrVacio, addr2;	
	evmc::bytes32 keyA, valueA,keyB, valueB;
	TrieNode * root = nullptr;
	//uint160_t key2(addr);
	TrieNode *nodo=nullptr;
	TrieNode * raiz=nullptr;
	TrieNode * nodo2 =nullptr;
	ExampleHost* host = nullptr;

	using namespace leveldb;

void CrearLeveldDB(){

	DB* db;
	Options options;
	options.create_if_missing = true;
	Status sT = DB::Open(options, "Contract.db", &db);
	assert(sT.ok());
	sT = db->Put(WriteOptions(), f_hash.ToString(), s);
	delete db;
	//sT = db->Get(ReadOptions(), f_hash.ToString(), &w);
	//cout << w << endl;
	//sT = db->Delete(WriteOptions(), f_hash.ToString());

	//sT = db->Get(ReadOptions(), f_hash.ToString(), &v);
}



void CrearBd(string name){

	sqlite3 *db;
	sqlite3_stmt *ppStmt;
	char *zErrMsg = 0;
	int rc;
	string sql;

	rc = sqlite3_open(("/home/administrador/Desktop/EVMC-host/TrieExample/"+name+".db").c_str(), &db);
	if( rc ) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return;
	} else {
		fprintf(stderr, "Opened database successfully\n");
	}
	/* Create SQL statement */
	sql = "CREATE TABLE STORAGE( KEY BLOB(32) PRIMARY KEY NOT NULL, VALUE BLOB(32));";

	rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &zErrMsg);

	if( rc != SQLITE_OK ){
	    fprintf(stderr, "SQL error: %s\n", zErrMsg);
	    sqlite3_free(zErrMsg);
	} else {
	    fprintf(stdout, "Table created successfully\n");
	   }
	sqlite3_close(db);
}

void init_vars() {
	string name = "/home/administrador/Desktop/EVMC-host/TrieExample/aabbccdd01eeff67aabbccdd01eeff67aabbccdd.db";
	s = "608060405234801561001057600080fd5b50336000806101000a81548173ffffffffffffffffffffffffffffffffffffffff021916908373ffffffffffffffffffffffffffffffffffffffff1602179055506000600181905550610246806100686000396000f300608060405260043610610057576000357c0100000000000000000000000000000000000000000000000000000000900463ffffffff16806311713df21461005c57806341c0e1b51461008757806393e84cd91461009e575b600080fd5b34801561006857600080fd5b506100716100a8565b6040518082815260200191505060405180910390f35b34801561009357600080fd5b5061009c6100b2565b005b6100a6610147565b005b6000600154905090565b6000809054906101000a900473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff163373ffffffffffffffffffffffffffffffffffffffff1614151561010d57600080fd5b6000809054906101000a900473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff16ff5b670de0b6b3a76400003414151561015d57600080fd5b6000809054906101000a900473ffffffffffffffffffffffffffffffffffffffff1673ffffffffffffffffffffffffffffffffffffffff166108fc349081150290604051600060405180830381858888f193505050501580156101c4573d6000803e3d6000fd5b50600180600082825401925050819055503373ffffffffffffffffffffffffffffffffffffffff167fa75916785ed274ade409b333cb54e33f4d515761c89b45c54ba3d9d944b109c960405160405180910390a25600a165627a7a72305820cc407087701b513237584fca8eaef984eb2d5cdc4fb55ec2d7d33a1f83e9f5480029";
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, s.c_str(), (sizeof(char) * s.size()) - 1);
	SHA256_Final((unsigned char*)&hash1, &ctx);
	SHA256((unsigned char*)&hash1, sizeof(hash1), (unsigned char*)&f_hash);
	
	llave = 0x27EE05FF;


	addrVacio = {};

	for (int i = 0; i < 20; i += 8) {
		addr.bytes[i] = 0xAA;
		addr.bytes[i + 1] = 0xBB;
		addr.bytes[i + 2] = 0xCC;
		addr.bytes[i + 3] = 0xDD;

		addr.bytes[i + 4] = 0x01;
		addr.bytes[i + 5] = 0xEE;
		addr.bytes[i + 6] = 0xFF;
		addr.bytes[i + 7] = 0x67;

		addr2.bytes[i] = 0x67;
		addr2.bytes[i + 1] = 0xFF;
		addr2.bytes[i + 2] = 0xEE;
		addr2.bytes[i + 3] = 0x01;

		addr2.bytes[i + 4] = 0xDD;
		addr2.bytes[i + 5] = 0xCC;
		addr2.bytes[i + 6] = 0xBB;
		addr2.bytes[i + 7] = 0xAA;


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

		valueA.bytes[i] = 0x11;
		valueA.bytes[i + 1] = 0xFF;
		valueA.bytes[i + 2] = 0xE0;
		valueA.bytes[i + 3] = 0x11;

		valueA.bytes[i + 4] = 0xDD;
		valueA.bytes[i + 5] = 0xCC;
		valueA.bytes[i + 6] = 0xBB;
		valueA.bytes[i + 7] = 0x11;
	}

	valueB = {};
	for (int i = 0 ; i < 32; i += 8) {
		keyB.bytes[i] = 0x00;
		keyB.bytes[i + 1] = 0xBB;
		keyB.bytes[i + 2] = 0xCC;
		keyB.bytes[i + 3] = 0xDD;

		keyB.bytes[i + 4] = 0x11;
		keyB.bytes[i + 5] = 0xEE;
		keyB.bytes[i + 6] = 0xFF;
		keyB.bytes[i + 7] = 0x01;
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

	if (nodo2 != nullptr)
		delete nodo2;
	nodo2 = new TrieNode(1);
	nodo2->SetKey(uint160_t(addr2));
	nodo2->SetAge(0x25E325E1E1);
	nodo2->SetBalance(0xFFFFFF);
	nodo2->SetLimit(0xFF);
	//llavero = cuenta->Key();

	if (TrieEngine::Size(raiz)==2){
		TrieEngine::Remove(&raiz, nodo);
		TrieEngine::Remove(&raiz, nodo2);
		TrieEngine::Remove(&raiz, raiz);
		TrieEngine::Remove(&root, raiz);
	}
	TrieEngine::Insert(&root, raiz, 0);
	TrieEngine::Insert(&raiz, nodo, 0);
	TrieEngine::Insert(&raiz, nodo2, 0);

	if (host != nullptr)
		delete host;
	host  = new ExampleHost();
	host->Raiz(raiz);

	int status;
	status=remove(name.c_str());
	CrearBd("aabbccdd01eeff67aabbccdd01eeff67aabbccdd");
	CrearLeveldDB();

}

inline bool CheckCopi(uint8_t* buffer_data , uint8_t* buffer_dataR,  size_t size ){
	for (int i = 0; i < size; ++i) {
		if (buffer_data[i]!=buffer_dataR[i])
			return false;
	}
	return true;
}


BOOST_AUTO_TEST_CASE(Host_account_exists){
	
	init_vars();
    BOOST_CHECK(host->account_exists(addr)==true);
    BOOST_CHECK(host->account_exists(addrVacio)==false);
}

BOOST_AUTO_TEST_CASE(Host_set_storage){

	
	BOOST_CHECK(host->set_storage(addr, keyA, valueA)==3); // CreAr Nueva Entrada 3 =  agregado Nuevo
	BOOST_CHECK(host->set_storage(addr, keyA, valueA)==0); // Modificar una Entrada con el mismo valor que poseia antes 0 = no modificado
	BOOST_CHECK(host->set_storage(addr, keyA, keyA)==1); // Modificar una entrada con un nuevo valor 1 = ACtualizado
	BOOST_CHECK(host->set_storage(addr, keyA, valueA)==1); // volver a Colocar el valor Inicial
	BOOST_CHECK(host->set_storage(addrVacio, keyA, valueA)==0); // Agregar a una cuenta Inexistente

}
	
BOOST_AUTO_TEST_CASE(Host_get_storage){
	
    BOOST_CHECK(host->get_storage(addr, keyA)==valueA); // Cuenta y clave existentes
    BOOST_CHECK(host->get_storage(addr, keyB)==valueB); // Cuenta existente y clave no valueB es avcio
    BOOST_CHECK(host->get_storage(addrVacio, keyA)==valueB); // Cuenta Inexistente y clave si
    BOOST_CHECK(host->get_storage(addrVacio, keyB)==valueB); // Cuenta y clave Inexistente
} 

BOOST_AUTO_TEST_CASE(Host_get_balance){

	bool flag = true;
	uint64_t balance = nodo->Balance();
	uint256_t balance256 (balance);
    evmc_uint256be balanceF(balance256);
	evmc_uint256be balanceCuenta = host->get_balance(addr);
	for (int i = 0; i < 32; ++i)
	{
		if(balanceCuenta.bytes[i]!= balanceF.bytes[i])
			flag = false;
	}
    
    BOOST_CHECK(flag); // cuenta existente
	BOOST_CHECK(host->get_balance(addrVacio)== valueB); // cuenta vacio debe dar igual a vacio

}

BOOST_AUTO_TEST_CASE(Host_get_code_size){

	size_t size = s.size()/2;

	BOOST_CHECK(host->get_code_size(addr)==size);
	BOOST_CHECK(host->get_code_size(addrVacio)== 0); // Cuenta vacia no posee codigo
	BOOST_CHECK(host->get_code_size(addr2)== 0); // Cuenta existente sin contrato
}

BOOST_AUTO_TEST_CASE(get_code_hash){

	evmc_uint256be evmchash = static_cast<evmc_uint256be>(nodo->ContractHash());

	BOOST_CHECK(host->get_code_hash(addr)==evmchash);	
	BOOST_CHECK(host->get_code_hash(addrVacio)==valueB); // hash de cuenta inexistente es vacio
	BOOST_CHECK(host->get_code_hash(addr2)==valueB);// // Cuenta existente sin contrato

}

BOOST_AUTO_TEST_CASE(selfdestruct){

	bool flag = true;
	uint64_t balance = nodo->Balance()+nodo2->Balance();
	uint256_t balance256 (balance);
    evmc_uint256be balanceF(balance256);
    string contractV="Nada";
    string contractAntes="Nada";

    DB* db;
	Options options;
	Status sT = DB::Open(options, "Contract.db", &db);
	assert(sT.ok());
	sT = db->Get(ReadOptions(), f_hash.ToString(), &contractAntes);
	delete db;

    host->selfdestruct(addr, addr2);
    evmc_uint256be balanceCuenta = host->get_balance(addr2);
    for (int i = 0; i < 32; ++i)
	{
		if(balanceCuenta.bytes[i]!= balanceF.bytes[i])
			flag = false;
	}


	sT = DB::Open(options, "Contract.db", &db);
	assert(sT.ok());
	sT = db->Get(ReadOptions(), f_hash.ToString(), &contractV);
	delete db;
	host->selfdestruct(addr, addrVacio); // no realiza nada si el primer argumento es vacio
	host->selfdestruct(addrVacio, addr); // no realiza nada si el segundo argumento es vacio
	
	BOOST_CHECK(flag); // Probando que el balance del addr se le sume al addr2 
	BOOST_CHECK(host->get_balance(addr)== valueB); // Probando que el addr posea balance en cero
	BOOST_CHECK(contractAntes!="Nada"); // Verificando que Antes de realizar el SelfDestruct exista un contrato
	BOOST_CHECK(contractAntes==s); // Verificando que el contrato obtenido efectivamente es el contrato guardado
	BOOST_CHECK(contractV!=s); // Verificando que el resultado de la Consulta en la base de datos sea distinto al contrato existente para la cuenta
	BOOST_CHECK(contractV=="Nada"); // Verificando que no haya devuelto nada la consulta a la base de datos  

}

BOOST_AUTO_TEST_CASE(copy_code){

	CrearLeveldDB();
	bool flag = true;
	size_t code_offset = 20;
	size_t buffer_size = 10;
	size_t final;

	uint8_t buffer_data [buffer_size];
	uint8_t buffer_dataR [buffer_size];

	char x[3];
	size_t size = s.size() / 2;
	uint8_t * bytes = new uint8_t[size];
	uint8_t buffer_dataComplete [size];


	for (int i = 0, j = 0; (i < size) && (j < s.size()); i++, j += 2) {
					
		memset(x, '\0', sizeof(char) * 3);
		x[0] = s[j];
		x[1] = s[j + 1];
		bytes[i] = static_cast<uint8_t>(strtol(x, NULL, 16));
	}

	memcpy(buffer_dataR, bytes +code_offset, sizeof(uint8_t) * MIN(buffer_size, size - code_offset));
	final = MIN(buffer_size, size - code_offset);

	
	BOOST_CHECK(host->copy_code(addrVacio,code_offset,buffer_data, buffer_size)== 0); // Pasarle una direccion vacia debe retornar 0
	BOOST_CHECK(host->copy_code(addr2,code_offset,buffer_data, buffer_size)== 0); // Pasarle un Direccion sin contrato debe retornar 0
	BOOST_CHECK(host->copy_code(addr,code_offset,buffer_data, buffer_size)==final); // Retornar el size Indicado
	BOOST_CHECK(host->copy_code(addr,code_offset,buffer_data, 0)==0); // Size 0 debe retornar 0
	BOOST_CHECK(host->copy_code(addr,-1,buffer_data,buffer_size)==0);
	BOOST_CHECK(host->copy_code(addr,0,buffer_dataComplete, size)==size);
	BOOST_CHECK(CheckCopi(buffer_dataComplete,bytes,size)); // Se copia desde cero debe retornar el Contrato completo
	BOOST_CHECK(CheckCopi(buffer_data,buffer_dataR,buffer_size)); // Se copia desde code_offset debe retornar el Contrato completo
}
