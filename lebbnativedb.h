#ifndef lebbnativedb_h
#define lebbnativedb_h

#include <rocksdbkeyvalue.h>
#include <string>
#include <map>
#include <set>
#include <deque>
#include <cstdint>
#include <bson.h>


typedef uint32_t lb_id;


class lebbnativedb
{
public:
	lebbnativedb(std::wstring kvpath);
	lebbnativedb(std::string kvpath);
	~lebbnativedb();

    bool open(const std::wstring& nativedbpath);
    bool isopen();
    bool close();

    //id have type:name_value_id, chainid, realid

	//names ","分开,或"/"分开,相当于查找
    int adddocdata(std::string bsondata);
    //添加子数据,有同的也能添加上
    int adddata(std::string findjsonstr, std::string bsondata);
    //设置子数据,通过找
    int updatevaluedata(std::string findjsonstr, std::string bsondata);

    //返回bson数组,names ","分开表示无序路径条件,JSON可找有序路径条件"同级加父亲形式"等,JSON找用$centerword定义中间词
    //返回:
    int find(std::string findjsonstr, std::string projjsonstr, std::string *out_jsondata,int skip=0,int count=10);
    //删除通过defprojname
	int delname(std::string findjsonstr, std::string delprojname);

    //from name_value_id get data
    int idnamegetchildidname(lb_id id, std::string name, std::string *out_childidnamebson);
    int idnamegetvalue(lb_id id, std::string name, std::string *out_idvaltype,std::string *out_value);
    int idnamegetparentidname(lb_id id, std::string name, std::string *out_parenidnamebson);

private:
    std::string getvalueformat(const char *data, uint32_t datalen);
    std::string getvalueunit(const char *data, uint32_t datalen);
    int findoutallchain(bson_iter_t *bsdociter,std::string parentpa, lb_id &out_topid, std::set<std::string>  &out_allword);
    int calccondids(std::deque<std::string*> &ids, std::string *out_ids);
    int addtoindex(std::string valfmt,const char *data, uint32_t datalen,lb_id id, std::string keyname);
    //find seg like:name[aa=bb]
    struct findsegcondinfo
    {
        std::string condname;
        std::string condop;
        std::string condval;
    };
    int getfindsegids(std::string findseg, std::string *out_ids,std::deque<findsegcondinfo> *out_conddeq);

    std::wstring m_nativedbpath;

    
	rocksdbkeyvalue *id_name;
    rocksdbkeyvalue *id_valformat;
	rocksdbkeyvalue *id_value;


	rocksdbkeyvalue *id_parent;
	rocksdbkeyvalue *id_children;

    //rocksdbkeyvalue *id_realid;
    //rocksdbkeyvalue *realid_ids;
    //rocksdbkeyvalue *name_realids;
    //rocksdbkeyvalue *realid_name;
    rocksdbkeyvalue *name_ids;//可以分成组


    rocksdbkeyvalue *uniquename_id;


    //rocksdbkeyvalue *realid_checkrule;//用于重名时判断是否是这个realid的名



    //centerwordid=realid;链就是一个有查找意义的小段
    //-多路径搜索方法能搜索非链词组的词,有:步骤找,
    rocksdbkeyvalue *name_chainids;
    rocksdbkeyvalue *chainid_ids;//用于查找,链要有顺序
    rocksdbkeyvalue *id_chainids;//用于查找
    /*
    rocksdbkeyvalue *centerwordid_names;//用于添加是快速分析
    rocksdbkeyvalue *idshash_centerwordid;//用于查找
    rocksdbkeyvalue *idshash_usetimes;//用于删除索引
    rocksdbkeyvalue *idshash_lastuse;//用于删除索引
    */

    //找方法:
    //统计事件人物: 事件找到所有词组, 分析出是人物的组,返回

    //
    rocksdbkeyvalue *name_docids;//docid==添加条目时的顶对象ID


    //值索引
    //有单位值索引
    std::map<std::string, rocksdbkeyvalue *> name_unitval_ids;
    //文本值索引
    rocksdbkeyvalue *textval_ids;
    //图值索引
    rocksdbkeyvalue *imageval_ids;

};




#endif


