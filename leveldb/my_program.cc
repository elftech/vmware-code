#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include "leveldb/db.h"

#include <iostream>
using namespace std;

/**
 * open a database connection to "./leveldb_dir"
 */

int main()
{
	leveldb::DB *db;
	leveldb::Options options;
	options.create_if_missing=true;
	leveldb::Status status=leveldb::DB::Open(options,"./leveldb_dir",&db);
	assert(status.ok());

	leveldb::Slice key1="CSC443/WINTER";
	leveldb::Slice value1=std::string("This is a course about database implementation.");
	//char *ptr=(char *)malloc(10);
	//leveldb::Slice key2(ptr,10);
	leveldb::Slice key2="haha";
	leveldb::Slice value2=std::string("One more value.");
	status = db->Put(leveldb::WriteOptions(), key1, value1);
	assert(status.ok());
	//status = db->Put(leveldb::WriteOptions(), key2, value2);
	assert(status.ok());

	std::string value_str;
	status = db->Get(leveldb::ReadOptions(),key1,&value_str);
	assert(status.ok());
	cout<<value_str<<endl;

	leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
	for (it->SeekToFirst(); it->Valid(); it->Next()) {
		leveldb::Slice key = it->key();
		leveldb::Slice value = it->value();
	 	std::string key_str = key.ToString();
		std::string val_str = value.ToString();
		cout << key_str << ": "  << val_str << endl;
	}
	assert(it->status().ok());  // Check for any errors found during the scan

	delete it;

	status=db->Delete(leveldb::WriteOptions(), key1);
	assert(status.ok());
	//status=db->Delete(leveldb::WriteOptions(), key2);
	//assert(status.ok());
	//delete db;
	return 0;
}
