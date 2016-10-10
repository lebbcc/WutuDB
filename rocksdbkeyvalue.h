#ifndef dbkeyvalue_h
#define dbkeyvalue_h

#include <rocksdb/db.h>
#include <string>


#define lw_max_keylen 1024
#define lw_max_keydatalen 8388608


class rocksdbkeyvalue
{
public:
	rocksdbkeyvalue(std::wstring kvpath);
	rocksdbkeyvalue(std::string kvpath);
	~rocksdbkeyvalue();

    bool open(const std::wstring& path);
    bool isopen();
    bool close();

    int put(const std::string &keydata, const std::string &keyvalue);
    int i32put(uint32_t idkey,std::string value);
    int i64put(uint64_t idkey, std::string value);
    int get(const std::string &key, std::string *out_value);
    int get(uint32_t idkey,uint32_t *out_value);
    int get(uint64_t idkey,uint64_t *out_value);
    int get(uint32_t idkey,std::string *out_value);
    int get(uint64_t idkey,std::string *out_value);
    int get(const std::string &key, uint32_t *out_id);
    int get(const std::string &key, uint64_t *out_id);
    int i32get(uint32_t idkey,std::string &value);
    int i64get(uint64_t idkey,std::string &value);

    std::string get(const std::string &key);
    std::string i32get(uint32_t idkey);
    std::string i64get(uint64_t idkey);

    int remove(std::string key);
    int i32remove(uint32_t idkey);
    int i64remove(uint64_t idkey);

    uint32_t nextid();
    int nextidreset(uint64_t idkey);
    std::string poptask(std::string value);
    int donetask(std::string key, std::string value);
    std::string nextkey();
    bool nextkeyreset();
    int atomic_value_remove(const std::string &keydata, const void *removeitem, int itemsize);
    int atomic_value_append(const std::string &keydata, const void* appenditem, int itemsize, int (*valcmpfunc)(const void*,const void*));
    int sync();


	//template <typename KeyName, typename ValueKey>
	//模板取值

private:
	std::wstring kvpath;

    rocksdb::DB      *mdb=NULL;
	rocksdb::Status openstatus;
    int putcnt = 0;
};




#endif


