#include "lebbnativedb.h"
#include <Windows.h>
#include <cassert>
#include "G:\DevLibrary\UtilityFunc\CrossPlastformMutex.h"
#include "C:\DevLibrary\UtilityFunc\safemap.h"
#include "G:\DevLibrary\UtilityFunc\LbLog.h"
#include "C:\DevLibrary\UtilityFunc\MultiIDSetGetSameID.h"
#include "G:\DevLibrary\UtilityFunc\SplitString.h"
#include <regex>
#include <Shlwapi.h>
#include <regex>

using namespace std;


/*
id_name
_typestr
_value


_parent
_children

put
get
del


id_id

id_ids

name_ids
name_ids
name_ids
name_ids

find

uniquename_id


值索引:
if name have value_name:
value_name


多层json格式:
{
name:value
name:{
__value:value
name:value
name:value
name:{

}
}
}




搜索方式:
asdfsf,sdfdsaf,dsafdsaf,sadfdsaf
sdfs,sdfdsf,sadfdsaf,sdafdsaf>343
sdfs,sdfdsf,sadfdsaf,sdafdsaf<343
sdfs,sdfdsf,sadfdsaf,sdafdsaf regexmatch "sadfdsf"
父亲[name="张军"],小明; 年龄





搜索返回:
[id,id,id,id,id,id,id]

[{"_id":32432,"__name":"dsadsf","name":tid_value,"name":"tid_value"},{"_id":32432,"name":tid_value,"name":"tid_value"}]






__value:
    n1  idparentpa
    n2	idparentpa
__valtype:
    t1
    t2
__name:
    n1	idparentpa
    n2	idparentpa
__parent:
    p1
    p2
__children:
    c1 idparentpa
    c2 idparentpa
    c3 idparentpa

name: value
    child


    {"yanshe.id":1}
[
{
id:2344,
name:sadfdsaf,
children:{
    name:{
        "__value":value,
        "id":3243,
        "id":3243,
    }
    name:{
        "__value":value,
        "id":3243,
        "id":3243,
    }
}
}
]

[
{
id:2344,
name:sdkfds,
value:sdakfds
children:[
    {"name":234324,value:dsfdsafds,"id":324324,"id":32432},
    {"name":234324,value:dsfdsafds,"id":324324,"id":32432},
    {"name":234324,value:dsfdsafds,"id":324324,"id":32432},
]
}
]


*/


lebbnativedb::lebbnativedb(std::wstring kvpath)
{
    open(kvpath);
}
lebbnativedb::lebbnativedb(std::string kvpath)
{
    wchar_t wkvpath[1024]={0};
    MultiByteToWideChar(CP_UTF8,0,kvpath.c_str(),kvpath.size(),wkvpath,1024);
    open(wkvpath);
}
lebbnativedb::~lebbnativedb()
{
    if(isopen())close();

}

bool lebbnativedb::open(const std::wstring& nativedbpath)
{
    m_nativedbpath=nativedbpath;

    std::wstring nativedbpath2(nativedbpath.c_str(),nativedbpath.size());
    if(nativedbpath2.size()>0 && nativedbpath2[nativedbpath2.size()-1]!=L'\\')
        nativedbpath2+=L"\\";
    wstring findexpr=nativedbpath2+L"*.*";
    if(!PathFileExists(nativedbpath2.c_str()))
    {
        int fpos=0;
        while(true)
        {
            auto fndrl=nativedbpath2.find(L'\\',fpos);
            if(fndrl==std::wstring::npos)break;
            std::wstring subpa=nativedbpath2.substr(0,fndrl);
            CreateDirectory(subpa.c_str(),NULL);
            fpos=fndrl+1;
        }
    }

    id_name=new rocksdbkeyvalue(nativedbpath2+L"id_name.rdb");

    id_valformat=new rocksdbkeyvalue(nativedbpath2+L"id_valtype.rdb");
    id_value=new rocksdbkeyvalue(nativedbpath2+L"id_value.rdb");


    id_parent=new rocksdbkeyvalue(nativedbpath2+L"id_parent.rdb");
    id_children=new rocksdbkeyvalue(nativedbpath2+L"id_children.rdb");

    name_ids=new rocksdbkeyvalue(nativedbpath2+L"name_ids.rdb");//可以分成组


    uniquename_id=new rocksdbkeyvalue(nativedbpath2+L"uniquename_id.rdb");




    name_chainids=new rocksdbkeyvalue(nativedbpath2+L"name_chainids.rdb");
    chainid_ids=new rocksdbkeyvalue(nativedbpath2+L"chainid_ids.rdb");//用于查找,链要有顺序
    id_chainids=new rocksdbkeyvalue(nativedbpath2+L"id_chainids.rdb");//用于查找


    name_docids=new rocksdbkeyvalue(nativedbpath2+L"name_docids.rdb");//docid==添加条目时的顶对象ID



    WIN32_FIND_DATA fnddata;
    HANDLE hf=FindFirstFile(findexpr.c_str(),&fnddata);
    if(hf)
    {
        do
        {
            wstring filename=fnddata.cFileName;
            if(!(wstring(L".")==filename || wstring(L"..")==filename))
            {
                if(fnddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {

                    if(filename.find(L"_valu_ids.rdb")!=wstring::npos)
                    {
                        std::wstring name=filename.substr(0,filename.find(L"_"));
                        char u8name[1024]={0};
                        WideCharToMultiByte(CP_UTF8,0,name.c_str(),name.size(),u8name,1024,0,0);
                        if(strlen(u8name)>0)name_unitval_ids.insert(std::make_pair(u8name,new rocksdbkeyvalue(nativedbpath2+filename)));
                    }
                }
            }
        }while(FindNextFile(hf,&fnddata));
        FindClose(hf);
    }

    return true;
}
bool lebbnativedb::isopen()
{
    return true;
}
bool lebbnativedb::close()
{
    return false;
}

int lebbnativedb::findoutallchain(bson_iter_t *bsdociter,std::string parentpa, lb_id &out_topid, std::set<std::string>  &out_allword)
{
    /*
    sample data:
    {
        "title_docname":
        {
            "__value":324324,
            "dsafdsf":"jdslkafjdsf",
            "dsafdsf2":"jdslkafjdsf",
            "sdfdsf":{
                "__value":"sdfsafdsf",
                "dsafdsf":"[inttype]jdslkafjdsf",
                "dsafdsf2":"jdslkafjdsf",
                "sdfdsf":{
                    "__value":"sdfsafdsf",
                    "dsafdsf":"jdslkafjdsf",
                    "dsafdsf2":"jdslkafjdsf"
                }
            }
        }
    }
    */
    int childcnt=0;
    while(bson_iter_next(bsdociter))
    {
        string keyname=bson_iter_key(bsdociter);
        //加词
        if(keyname!="__value")out_allword.insert(keyname);
        //同一词索引
        //判断是否是世界唯一名
        lb_id keynameid=0;
        uniquename_id->get(keyname,&keynameid);
        if(keynameid>0)
        {
            //Get id from:name_ids
            //user id save to:id_ids
        }else{
            //set id = id
            //save to name_ids
            //save to id_name
        }

        if(bson_iter_type(bsdociter)==BSON_TYPE_DOCUMENT)
        {
            bson_iter_t childiter;
            bson_iter_recurse(bsdociter, &childiter);

            findoutallchain(&childiter,parentpa+"/"+keyname, out_topid,out_allword);

            //save data to id_name id_value id_valformat id_unit id_parent id_children


        }else{
            //get lastname to save
            string nameofvalue;
            if(keyname=="__value")
            {
                if(parentpa.rfind("/")!=std::string::npos)
                    nameofvalue=parentpa.substr(parentpa.rfind("/"));
                else
                    nameofvalue=parentpa;
            }else{
                nameofvalue=keyname;
            }

            if(bson_iter_type(bsdociter)==BSON_TYPE_UTF8)
            {
                //save data to id_name id_value id_valformat id_unit id_parent id_children
                uint32_t u8datalen=0;
                const char *u8data=bson_iter_utf8(bsdociter,&u8datalen);
                lb_id keyvalid=0;
                //get new id

                //use new id to save to id_*

                //
                //值索引
                std::string valfmt=getvalueformat(u8data, u8datalen);
                int addrl=addtoindex(valfmt,u8data,u8datalen,keyvalid,nameofvalue);
            }
            if(bson_iter_type(bsdociter)==BSON_TYPE_DOUBLE)
            {
                //save data to id_name id_value id_valformat id_unit id_parent id_children
                uint32_t u8datalen=0;
                const char *u8data=bson_iter_utf8(bsdociter,&u8datalen);
                lb_id keyvalid=0;
                //get new id

                //use new id to save to id_*

                //
                //值索引
                std::string valfmt=getvalueformat(u8data, u8datalen);
                int addrl=addtoindex(valfmt,u8data,u8datalen,keyvalid,nameofvalue);
            }
            if(bson_iter_type(bsdociter)==BSON_TYPE_INT32 || bson_iter_type(bsdociter)==BSON_TYPE_INT64)
            {
                //save data to id_name id_value id_valformat id_unit id_parent id_children
                uint32_t u8datalen=0;
                const char *u8data=bson_iter_utf8(bsdociter,&u8datalen);
                lb_id keyvalid=0;
                //get new id

                //use new id to save to id_*

                //
                //值索引
                std::string valfmt=getvalueformat(u8data, u8datalen);
                int addrl=addtoindex(valfmt,u8data,u8datalen,keyvalid,nameofvalue);
            }
            if(bson_iter_type(bsdociter)==BSON_TYPE_BINARY)
            {
                //save data to id_name id_value id_valformat id_unit id_parent id_children
                uint32_t u8datalen=0;
                const char *u8data=bson_iter_utf8(bsdociter,&u8datalen);
                lb_id keyvalid=0;
                //get new id

                //use new id to save to id_*

                //
                //值索引
                std::string valfmt=getvalueformat(u8data, u8datalen);
                int addrl=addtoindex(valfmt,u8data,u8datalen,keyvalid,nameofvalue);
            }
            if(bson_iter_type(bsdociter)==BSON_TYPE_ARRAY)
            {
                //get lastname to save
                string nameofvalue;
                if(keyname=="__value")
                {
                    if(parentpa.rfind("/")!=std::string::npos)
                        nameofvalue=parentpa.substr(parentpa.rfind("/"));
                    else
                        nameofvalue=parentpa;
                }else{
                    nameofvalue=keyname;
                }
                //save data to id_name id_value id_valformat id_unit id_parent id_children


                //值索引
                rocksdbkeyvalue *value_ids;
                if(name_unitval_ids.find(keyname)!=name_unitval_ids.end())
                {

                }else{

                }
                //save to value_ids

            }

            //链索引
            string maxchainpa=parentpa+"/"+keyname;
            //new chainid
            lb_id newchainid;
            //save to:chainid_ids name_chainids


        }

        childcnt++;
    }

    //处理id_children等
    if(childcnt==0 && parentpa!="")
    {

    }
    if(childcnt!=0 && parentpa!="")
    {

    }

    return 0;
}

int lebbnativedb::adddocdata(std::string bsondata)
{
    /*
    sample data:
    {
        "title_docname":
        {
            "__value":324324,
            "dsafdsf":"jdslkafjdsf",
            "dsafdsf2":"jdslkafjdsf",
            "sdfdsf":{
                "__value":"sdfsafdsf",
                "dsafdsf":"[inttype]jdslkafjdsf",
                "dsafdsf2":"jdslkafjdsf",
                "sdfdsf":{
                    "__value":"sdfsafdsf",
                    "dsafdsf":"jdslkafjdsf",
                    "dsafdsf2":"jdslkafjdsf"
                }
            }
        }
    }

    */

    //找出所有有用链
    bool bparseok=false;
    if(bsondata.size() && bsondata[0]=='{' && bsondata[bsondata.size()-1]=='}')
    {
        std::set<string>  docwordsset;
        bson_error_t bsonerr;
        bson_t *bdt=bson_new ();
        bson_init (bdt);
        bool brl=bson_init_from_json(bdt,(const char*)bsondata.c_str(),bsondata.size(),&bsonerr);
        if(brl)
        {
            bson_iter_t biter;
            bool bb=bson_iter_init(&biter,bdt);
            std::deque<string>  docchaindeq;
            if(bb)
            {
                lb_id out_topid=0;
                std::set<std::string>  out_allword;
                findoutallchain(&biter,"", out_topid, out_allword);
                if(out_allword.size())
                {
                    //
                }
                bparseok=true;
            }
        }
    }

    if(bparseok==false)
    {
        std::set<string>  docwordsset;
        bson_t bdt;
        bool brl=bson_init_static(&bdt,(const uint8_t*)bsondata.c_str(),bsondata.size());
        if(brl)
        {
            bson_iter_t biter;
            bool bb=bson_iter_init(&biter,&bdt);
            std::deque<string>  docchaindeq;
            if(bb)
            {
                lb_id out_topid=0;
                std::set<std::string>  out_allword;
                findoutallchain(&biter,"", out_topid, out_allword);
                if(out_allword.size())
                {
                    //
                }
            }
        }
    }

    return 0;
}


int lebbnativedb::adddata(std::string findjsonstr, std::string bsondata)
{
    return 0;
}

//设置子数据,通过找
int lebbnativedb::updatevaluedata(std::string findjsonstr, std::string bsondata)
{
    return 0;
}

//返回bson数组
int lebbnativedb::find(string findrulestr, string projjsonstr, string *out_jsondata,int skip,int count)
{
	//example:name,name,name
	//intro:每个名字表示一个条件,返回最后一个名的实ID
	//example:name[../sdfsa=3243],name,name
	//intro:../sdfsa=3243表示名字的父亲的子要求条件
	//example:name[sdfsa=3243],name,name
	//intro:sdfsa=3243表示名字的子要求条件
	//names like:铁,熔点
	//intro:
	//names like:百度百科,[__name re "sadfdsf"]
	//intro:
    //JSON:
    //{}

    if(findrulestr[0]=='{' && findrulestr[findrulestr.size()-1]=='}')
    {
        //与逗号查找方式一样,就是完了有绝对位置关系判断.定位次定义用"$locateword"表示, proj依赖定位词
        //有通路但是无边数据的结构
        //centworddata异边组合的

        //name find docid or centerwordid

        //use foundid to get doc or centerwordid

        //use mainid to get chain

        //locate returnword pos

        //use projdef get data

        //return

    }else{
        //能搜索chain:非树结构chain
        //获得小段条件
        std::deque<std::string> litesegdeq;
        int i=0;
        while(true)
        {
            if(findrulestr[i]=='[')
            {
                while(i<findrulestr.size() && findrulestr[i]!=']')i++;
            }else if(findrulestr[i]==',')
            {
                litesegdeq.push_back(findrulestr.substr(0,i));
                findrulestr=findrulestr.substr(i+1);
                i=0;
            }
        }
        if(findrulestr.size())
        {
            litesegdeq.push_back(findrulestr);
        }

        //now got all segexpre
        //use all segexpre name to get chainid
        std::map<string,string*> name_chainids_map;
        std::deque<string*>     chainids;
        std::set<string> nameset1,nameset2;
        for(int i=0;i<litesegdeq.size();i++)
        {
            std::string name=litesegdeq[0];
            if(name.find("[")!=std::string::npos)
            {
                name=name.substr(0,name.find("["));
            }
            nameset1.insert(name);
            std::string *ids=new string;
            name_chainids->get(name,ids);
            chainids.push_back(ids);
            name_chainids_map.insert(std::make_pair(name,ids));
        }

        std::string havewords_chainids_str;
        int rl=calccondids(chainids, &havewords_chainids_str);

        //use centerwordid to find all match chain
        std::deque<string> allrightchains;
        for(int i=0;i<havewords_chainids_str.size()/sizeof(lb_id);i+=1)
        {
            lb_id chainid=((lb_id*)havewords_chainids_str.c_str())[i];

            std::string idsofchainid;
            chainid_ids->get(chainid,&idsofchainid);
            allrightchains.push_back(idsofchainid);

        }

        //locate locateword
        //find projdef
        bson_t returnbs;
        string lastname;
        for(int i=0;i<allrightchains.size();i++)
        {
            string okchain=allrightchains[i];
            for(int ci=0;ci<okchain.size()/sizeof(lb_id);ci++)
            {
                lb_id ciid=((lb_id*)okchain.c_str())[ci];
                string ciname;
                id_name->get(ciid,&ciname);
                if(ciname==lastname)
                {
                    //parse projdef

                    //get projdef data

                    //add to bson array for return.
                }
            }
        }

        //return
        const uint8_t *docstr=bson_get_data(&returnbs);
        returnbs.len;

    }

	return 0;
}
int lebbnativedb::delname(string findjsonstr, string delprojname)
{
    return -1;
}


int lebbnativedb::idnamegetchildidname(lb_id id, std::string name, std::string *out_childidnamebson)
{
    return 0;
}
int lebbnativedb::idnamegetvalue(lb_id id, std::string name, std::string *out_idvaltype,std::string *out_value)
{
    return 0;
}
int lebbnativedb::idnamegetparentidname(lb_id id, std::string name, std::string *out_parenidnamebson)
{
    return 0;
}

int lebbnativedb::calccondids(std::deque<string*> &ids, std::string *out_ids)
{
    //
    //
    return MultiIDSetGetSameID(ids, out_ids,sizeof(lb_id));
}


int lebbnativedb::getfindsegids(std::string findseg, std::string *out_ids,std::deque<findsegcondinfo> *out_conddeq)
{
    std::string name;

    std::deque<findsegcondinfo> conddeq;
    name=findseg.substr(0,findseg.find("["));
    findseg=findseg.substr(findseg.find("["),findseg.rfind("]")-findseg.find("["));
    int i=0;
    while(true)
    {
        if(findseg[i]=='"')
        {
            i++;
            while(i<findseg.size() && findseg[i]!='"')i++;
        }else if(findseg[i]=='{')
        {
            i++;
            while(i<findseg.size() && findseg[i]!='}')i++;
        }else if(findseg[i]==',')
        {
            string condstr=findseg.substr(0,i);
            //regex1:name=32434
            std::smatch ma;
            if(std::regex_match(condstr,ma,std::regex("([_\\w])(+=)(\d+([.]\d*)?|\".*\")")))
            {
                findsegcondinfo info;
                info.condname=ma[1].str();
                info.condop=ma[2].str();
                info.condval=ma[3].str();
                conddeq.push_back(info);
            }
            //regex1:name<32434
            else if(std::regex_match(condstr,ma,std::regex("([_\\w]+)(<)(\d+([.]\d*)?)")))
            {
                findsegcondinfo info;
                info.condname=ma[1].str();
                info.condop=ma[2].str();
                info.condval=ma[3].str();
                conddeq.push_back(info);
            }
            //regex1:name>32434
            else if(std::regex_match(condstr,ma,std::regex("([_\\w]+)(>)(\d+([.]\d*)?)")))
            {
                findsegcondinfo info;
                info.condname=ma[1].str();
                info.condop=ma[2].str();
                info.condval=ma[3].str();
                conddeq.push_back(info);
            }
            //regex1:name<=32434
            else if(std::regex_match(condstr,ma,std::regex("([_\\w]+)(<=)(\d+([.]\d*)?)")))
            {
                findsegcondinfo info;
                info.condname=ma[1].str();
                info.condop=ma[2].str();
                info.condval=ma[3].str();
                conddeq.push_back(info);
            }
            //regex1:name>=32434
            else if(std::regex_match(condstr,ma,std::regex("([_\\w]+)(>=)(\d+([.]\d*)?)")))
            {
                findsegcondinfo info;
                info.condname=ma[1].str();
                info.condop=ma[2].str();
                info.condval=ma[3].str();
                conddeq.push_back(info);
            }
            //regex1:name match "regex"
            //regex1:__name match "regex"
            else if(std::regex_match(condstr,ma,std::regex("([_\\w]+)( regex )(\".*\")")))
            {
                findsegcondinfo info;
                info.condname=ma[1].str();
                info.condop=ma[2].str();
                info.condval=ma[3].str();
                conddeq.push_back(info);
            }

            //regex1:name like "re*gex"
            else if(std::regex_match(condstr,ma,std::regex("([_\\w]+)( like )(\".*\")")))
            {
                findsegcondinfo info;
                info.condname=ma[1].str();
                info.condop=ma[2].str();
                info.condval=ma[3].str();
                conddeq.push_back(info);
            }
            //regex1:name in {234,234,234}
            else if(std::regex_match(condstr,ma,std::regex("([_\\w]+)( in )({.*?})")))
            {
                findsegcondinfo info;
                info.condname=ma[1].str();
                info.condop=ma[2].str();
                info.condval=ma[3].str();
                conddeq.push_back(info);
            }
            else
            {
                return -1;
            }

            findseg=findseg.substr(i+1);
            i=0;
        }
    }
    //now we have name and conddeq
    bool bgetok=name_ids->get(name,out_ids);
    if(bgetok)
    {
        for(int i=0;i<conddeq.size();i++)
            out_conddeq->push_back(conddeq[i]);
    }
    return 0;
}

std::string lebbnativedb::getvalueformat(const char *data, uint32_t datalen)
{
    return "";
}

std::string lebbnativedb::getvalueunit(const char *data, uint32_t datalen)
{
    return "";
}


int lebbnativedb::addtoindex(std::string valfmt,const char *u8data, uint32_t u8datalen,lb_id id, std::string keyname)
{
    if(valfmt=="valunite")
    {
        //有单位值索引
        rocksdbkeyvalue *value_ids;
        std::string valunit=getvalueunit(u8data, u8datalen);
        if(name_unitval_ids.find(keyname)!=name_unitval_ids.end())
        {
            value_ids=name_unitval_ids[keyname];
        }else{
            wchar_t widstr[1024];
            int     widlen=0;
            MultiByteToWideChar(CP_ACP,0,keyname.c_str(),keyname.size(), widstr, widlen);
            value_ids=new rocksdbkeyvalue(m_nativedbpath+L"/"+widstr+L"_unitval_ids.rcb");
            name_unitval_ids.insert(std::make_pair(keyname,value_ids));
        }

    }else if(valfmt=="png" || valfmt=="jpg" || valfmt=="gif" || valfmt=="svg" || valfmt=="jpeg")
    {
        //图值索引

    }else if(valfmt=="name")
    {
        //有单位值索引
        std::string valunit=getvalueunit(u8data, u8datalen);
    }else if(valfmt=="c code")
    {
        //有单位值索引
        std::string valunit=getvalueunit(u8data, u8datalen);
    }else if(valfmt=="cpp code")
    {
        //有单位值索引
        std::string valunit=getvalueunit(u8data, u8datalen);
    }else if(valfmt=="python code")
    {
        //有单位值索引
        std::string valunit=getvalueunit(u8data, u8datalen);
    }else if(valfmt=="js code")
    {
        //有单位值索引
        std::string valunit=getvalueunit(u8data, u8datalen);
    }else if(valfmt=="vbs code")
    {
        //有单位值索引
        std::string valunit=getvalueunit(u8data, u8datalen);
    }else if(valfmt=="mp3" || valfmt=="ogg" || valfmt=="wav")
    {
        //有单位值索引
        std::string valunit=getvalueunit(u8data, u8datalen);
    }else if(valfmt=="exe")
    {
        //有单位值索引
        std::string valunit=getvalueunit(u8data, u8datalen);
    }else{
        //unknow value format
    }

    return 0;
}
