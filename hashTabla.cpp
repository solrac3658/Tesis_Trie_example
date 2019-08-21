#include <stdio.h>  // compilar g++ hashTabla.cpp uint256.h -o hashtabla -lsqlite3 -std=c++11 -I . -I ./include -lboost_system -lboost_thread -lssl -lcrypto
#include <string.h> 
#include <string>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <sqlite3.h> 
#include <unistd.h> 
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>

#include "uint256.h"
#include "hash.h"




using namespace std;
using namespace boost::archive::iterators;

typedef base64_from_binary<transform_width<const char *, 6, 8>> base64_text;

int main()
{

	stringstream os;
	evmc::bytes32 key, value;
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	sqlite3_stmt *ppStmt;
	string sql, name;
	SHA256_CTX ctx;
	SHA256_Init(&ctx);
	uint256 hash1, f_hash;
	
	
//////////////////////////////////////////// Abrir Base de Datos ////////////////////////////////////////////
	name ="abbbccdd01eeff67abbbccdd01eeff67abbbccdd.db";
	rc = sqlite3_open(name.c_str(), &db);
	if( rc ) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	} else {
		fprintf(stderr, "Opened database successfully\n");
	}
//////////////////////////////////////////// Abrir Base de Datos ////////////////////////////////////////////

//////////////////////////////////////////// Realizando Query ///////////////////////////////////////////////

	sql = "SELECT key , value from STORAGE order by key";
	sqlite3_prepare_v2(db, sql.c_str(), sql.size()+1,&ppStmt,NULL);
	
	while (sqlite3_step(ppStmt) != SQLITE_DONE ) {
		key = static_cast<evmc::bytes32>(*static_cast<const evmc::bytes32 *>(sqlite3_column_blob(ppStmt, 0)));
		value = static_cast<evmc::bytes32>(*static_cast<const evmc::bytes32 *>(sqlite3_column_blob(ppStmt, 1)));	
		SHA256_Update(&ctx, &key, sizeof(key));	   		
		SHA256_Update(&ctx, &value, sizeof(value));
	}
//////////////////////////////////////////// Realizando Query ///////////////////////////////////////////////

//////////////////////////////////////////// Calculando Hash ///////////////////////////////////////////////
	SHA256_Final((unsigned char*)&hash1, &ctx);
    SHA256((unsigned char*)&hash1, sizeof(hash1), (unsigned char*)&f_hash);
    cout << f_hash.ToString() << endl;
//////////////////////////////////////////// Calculando Hash ///////////////////////////////////////////////
	sqlite3_finalize(ppStmt);
	sqlite3_close(db);
	return 0;
}

/*
typedef base64_from_binary<transform_width<const char *, 6, 8>> base64_text;

for (int i = 0; i < 32; i++) {
            printf("%02x", key.bytes[i]);
        }
        cout << endl;
        copy(base64_text(key.bytes), base64_text(key.bytes + 32),boost::archive::iterators::ostream_iterator<char>(os));
        cout << os.str()<<endl;
         os.str("");

    for (int i = 0; i < 32; i++) {
            printf("%02x", value.bytes[i]);
        }
        cout << endl;
    copy(base64_text(value.bytes), base64_text(value.bytes + 32),boost::archive::iterators::ostream_iterator<char>(os));
    cout << os.str()<<endl;
     os.str("");*/
