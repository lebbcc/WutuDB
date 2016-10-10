#include "rocksdbkeyvalue.h"
#include <Windows.h>
#include <cassert>
#include "G:\DevLibrary\UtilityFunc\CrossPlastformMutex.h"
#include "C:\DevLibrary\UtilityFunc\safemap.h"
#include "G:\DevLibrary\UtilityFunc\LbLog.h"


using namespace std;


rocksdbkeyvalue::rocksdbkeyvalue(std::wstring kvpath)
{
    open(kvpath);
}

rocksdbkeyvalue::rocksdbkeyvalue(std::string kvpath)
{
    wchar_t wpath[1024]={0};
    MultiByteToWideChar(CP_ACP,0,kvpath.c_str(),kvpath.size(),wpath,1024);
    open(wpath);
}


rocksdbkeyvalue::~rocksdbkeyvalue()
{
    close();
}


bool rocksdbkeyvalue::open(const std::wstring& path)
    {
        if (path.empty())return false;
        close();
        rocksdb::Options  options;
        options.create_if_missing = true;
        options.write_buffer_size = 32 * 1024 * 1024;
        options.max_open_files = 10000;

        char cpath[1024] = { 0 };
        WideCharToMultiByte(CP_ACP, 0, path.c_str(), path.size(), cpath, 1024, 0, 0);

        // open
        openstatus = rocksdb::DB::Open(options, cpath, &mdb);
        assert(openstatus.ok());

        return openstatus.ok();
    }

    bool rocksdbkeyvalue::isopen(){ return mdb==NULL?false:openstatus.ok(); }
    bool rocksdbkeyvalue::close()
    {
        if (mdb)
        {
            delete mdb;
            mdb = NULL;
        }
        return true;
    }
    //只能打开时设置
    //bool setcache(int64_t cachesize)
    //{
    //	m_cachesize = cachesize;
    //	((DB*)m_bdbhand)->set_cachesize((DB*)m_bdbhand, 0, m_cachesize, 1);
    //	return true;
    //}
    int rocksdbkeyvalue::put(const std::string &key, const std::string &value)
    {
        // write
        rocksdb::WriteOptions wo;
        if (putcnt > 10000)
        {
            wo.sync = true;
            putcnt = 0;
        }
        auto status = mdb->Put(wo, key, value);
        //assert(status.ok());
        if (status.ok() == false)
        {
            auto str = status.ToString();
            int e = 34324;
        }
        else{
            putcnt++;
        }
        return status.ok()==true?0:1;
    }

    int rocksdbkeyvalue::i32put(uint32_t idkey,std::string value)
    {
        // write
        std::string key((char*)&idkey, sizeof(idkey));
        auto status = mdb->Put(rocksdb::WriteOptions(), key, value);
        assert(status.ok());
        return status.ok()==true?0:false;
    }

    int rocksdbkeyvalue::i64put(uint64_t idkey, std::string value)
    {
        // write
        std::string key((char*)&idkey, sizeof(idkey));
        auto status = mdb->Put(rocksdb::WriteOptions(), key, value);
        assert(status.ok());
        return status.ok()==true?0:false;
    }
    int rocksdbkeyvalue::get(const std::string &key, std::string *out_value)
    {
        auto status = mdb->Get(rocksdb::ReadOptions(), key, out_value);
        return status.ok()==true?0:1;
    }
    int rocksdbkeyvalue::get(uint32_t idkey,std::string *out_value)
    {
        std::string key((char*)&idkey, sizeof(idkey));
        auto status = mdb->Get(rocksdb::ReadOptions(), key, out_value);
        return status.ok()==true?0:1;
    }
    int rocksdbkeyvalue::get(uint64_t idkey,std::string *out_value)
    {
        std::string key((char*)&idkey, sizeof(idkey));
        auto status = mdb->Get(rocksdb::ReadOptions(), key, out_value);
        return status.ok()==true?0:1;
    }
    int rocksdbkeyvalue::get(uint32_t idkey,uint32_t *out_value)
    {
        std::string key((char*)&idkey, sizeof(idkey));
        std::string out_value2;
        auto status = mdb->Get(rocksdb::ReadOptions(), key, &out_value2);
        *out_value=((uint32_t*)out_value2.c_str())[0];
        return status.ok()==true?0:1;
    }
    int rocksdbkeyvalue::get(uint64_t idkey,uint64_t *out_value)
    {
        std::string key((char*)&idkey, sizeof(idkey));
        std::string out_value2;
        auto status = mdb->Get(rocksdb::ReadOptions(), key, &out_value2);
        *out_value=((uint64_t*)out_value2.c_str())[0];
        return status.ok()==true?0:1;
    }
    int rocksdbkeyvalue::get(const std::string &key, uint32_t *out_value)
    {
        std::string out_value2;
        auto status = mdb->Get(rocksdb::ReadOptions(), key, &out_value2);
        *out_value=((uint32_t*)out_value2.c_str())[0];
        return status.ok()==true?0:1;
    }

    int rocksdbkeyvalue::get(const std::string &key, uint64_t *out_value)
    {
        std::string out_value2;
        auto status = mdb->Get(rocksdb::ReadOptions(), key, &out_value2);
        *out_value=((uint64_t*)out_value2.c_str())[0];
        return status.ok()==true?0:1;
    }


    int rocksdbkeyvalue::i32get(uint32_t idkey,std::string &value)
    {
        std::string key((char*)&idkey, sizeof(idkey));
        auto status = mdb->Get(rocksdb::ReadOptions(), key, &value);
        return status.ok()==true?0:1;
    }
    int rocksdbkeyvalue::i64get(uint64_t idkey,std::string &value)
    {
        std::string key((char*)&idkey, sizeof(idkey));
        auto status = mdb->Get(rocksdb::ReadOptions(), key, &value);
        return status.ok()==true?0:1;
    }

    std::string rocksdbkeyvalue::get(const std::string &key)
    {
        std::string value;
        auto status = mdb->Get(rocksdb::ReadOptions(), key, &value);
        return value;
    }
    std::string rocksdbkeyvalue::i32get(uint32_t idkey)
    {
        std::string value;
        std::string key((char*)&idkey, sizeof(idkey));
        auto status = mdb->Get(rocksdb::ReadOptions(), key, &value);
        return value;
    }
    std::string rocksdbkeyvalue::i64get(uint64_t idkey)
    {
        std::string value;
        std::string key((char*)&idkey, sizeof(idkey));
        auto status = mdb->Get(rocksdb::ReadOptions(), key, &value);
        return value;
    }

    int rocksdbkeyvalue::remove(std::string key)
    {
        auto status = mdb->Delete(rocksdb::WriteOptions(), key);
        assert(status.ok());
        return status.ok()==true?0:1;
    }
    int rocksdbkeyvalue::i32remove(uint32_t idkey)
    {
        std::string key((char*)&idkey, sizeof(idkey));
        auto status = mdb->Delete(rocksdb::WriteOptions(), key);
        assert(status.ok());
        return status.ok()==true?0:1;
    }

    int rocksdbkeyvalue::i64remove(uint64_t idkey)
    {
        std::string key((char*)&idkey, sizeof(idkey));
        auto status = mdb->Delete(rocksdb::WriteOptions(), key);
        assert(status.ok());
        return status.ok()==true?0:1;
    }

    uint32_t rocksdbkeyvalue::nextid()
    {
        uint32_t newid = 0;
        std::string value;
        auto status = mdb->Get(rocksdb::ReadOptions(), "__pyberkeleydb_max_id", &value);
        if (status.ok())
        {
            if (value.size() == sizeof(uint32_t))
            {
                memcpy(&newid, value.c_str(), value.size());
                uint32_t nextnewid = newid + 1;
                put("__pyberkeleydb_max_id", std::string((char*)&nextnewid, sizeof(nextnewid)));
            }
            else{
                newid = 1;
                uint32_t nextnewid = 2;
                put("__pyberkeleydb_max_id", std::string((char*)&nextnewid, sizeof(nextnewid)));
            }
        }
        else{
            newid = 1;
            uint32_t nextnewid = 2;
            put("__pyberkeleydb_max_id", std::string((char*)&nextnewid, sizeof(nextnewid)));
        }
        return newid;
    }
    int rocksdbkeyvalue::nextidreset(uint64_t idkey)
    {
        uint32_t newid = 1;
        return put("__pyberkeleydb_max_id", std::string((char*)&newid, sizeof(newid)));
    }

    std::string rocksdbkeyvalue::poptask(std::string value)
    {
        if (value.empty())value = "1";
        rocksdb::Iterator* it = mdb->NewIterator(rocksdb::ReadOptions());
        std::string key;
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            if (it->value().ToString() == value)
            {
                key = it->key().ToString();
                delete it;
                return key;
            }
        }
        delete it;
        return key;
    }
    int rocksdbkeyvalue::donetask(std::string key, std::string value)
    {
        if (value.empty())value = "2";
        return put(key,value);
    }

    rocksdb::Iterator* it = NULL;
    std::string rocksdbkeyvalue::nextkey()
    {
        if (it == NULL)
        {
            it = mdb->NewIterator(rocksdb::ReadOptions());
            it->SeekToFirst();
        }
        std::string key;
        if (it->Valid())
        {
            key = it->key().ToString();
            it->Next();
        }
        if (key.empty())
        {
            int e = 324324;
        }
        return key;
    }
    bool rocksdbkeyvalue::nextkeyreset()
    {
        it->SeekToFirst();
        return true;
    }

safemap::map<std::string,MMutex*> lockdeq2;
int rocksdbkeyvalue::atomic_value_remove(const std::string &keydata, const void *removeitem, int itemsize)
{
    int dberr=0;
    if(lockdeq2.find_s(keydata)==lockdeq2.end_s())
    {
        lockdeq2.insert_s(keydata, new MMutex);
    }
    lockdeq2.find_s(keydata)->second->Lock();

    //get value
    int dbrl=0;
    std::string value;
    dbrl=get(keydata,&value);
    if(dbrl==0 && value.size())
    {
        //iterator value and find remove val
        assert(value.size()%itemsize==0);
        for(int i=value.size()/itemsize-1;i>=0;i--)
        {
            if(memcmp(value.c_str()+i*itemsize, removeitem, itemsize)==0)
            {
                value.replace(i*itemsize,itemsize,"");
            }
        }
        dbrl=put(keydata,value);
    }
    lockdeq2.find_s(keydata)->second->Unlock();
    delete lockdeq2.find_s(keydata)->second;
    lockdeq2.erase_s(lockdeq2.find_s(keydata));
    return dbrl==true?0:1;
}

int rocksdbkeyvalue::atomic_value_append(const std::string &keydata, const void* appenditem, int itemsize, int (*valcmpfunc)(const void*,const void*))
{
    int dberr=0;
    if(lockdeq2.find_s(keydata)==lockdeq2.end_s())
    {
        lockdeq2.insert_s(keydata, new MMutex);
    }
    lockdeq2.find_s(keydata)->second->Lock();

    //get value
    int dbrl=0;
    std::string value;
    dbrl=get(keydata,&value);
    //find exists
    if(value.size() && valcmpfunc!=NULL)
    {
        bool bexists=false;
        for(int i=value.size()/itemsize-1;i>=0;i--)
        {
            if(memcmp(value.c_str()+i*itemsize, appenditem, itemsize)==0)
            {
                bexists=true;
            }
        }
        if(bexists==false)
        {
            //append
            value.append((const char*)appenditem,itemsize);
            //sort
            qsort((void*)value.c_str(),value.size()/sizeof(itemsize),itemsize,valcmpfunc);
            //put
            dbrl=put(keydata,value);
        }
    }else{
        //put
        dbrl=put(keydata,value);
    }
    lockdeq2.find_s(keydata)->second->Unlock();
    delete lockdeq2.find_s(keydata)->second;
    lockdeq2.erase_s(lockdeq2.find_s(keydata));
    return dbrl==true?0:1;
}


int rocksdbkeyvalue::sync()
{
    int dberr=0;
    return dberr;
}
