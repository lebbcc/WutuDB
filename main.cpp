#include <QCoreApplication>
#include "lebbnativedb.h"

using namespace std;

int main(int argc, char *argv[])
{
    lebbnativedb db("M:\\test_ledb");
    if(db.isopen())
    {
        //{"xiao1":{"__value":"33kuai","yuanse":"1jin"},"xiao2":{"__value":"43tiao","yueli":"3zhong"}}
        //yanse,xiao1; yanse
        db.adddocdata("{\"xiao1\":{\"__value\":\"33kuai\",\"yuanse\":\"1jin\"},\"xiao2\":{\"__value\":\"43tiao\",\"yueli\":\"3zhong\"}}");
        string outjsonresult1;
        db.find("yanse,xiao1","yanse",&outjsonresult1);


        //{"piaoliang":{"xiao1":{"__value":"33fu","yuanse":"1ge","size":"345ming"},"xiao2":{"__value":"43ren","size":"3ben"}}
        //piaoliang,yuanse;"..,./size"
        db.adddocdata("{\"piaoliang\":{\"xiao1\":{\"__value\":\"33fu\",\"yuanse\":\"1ge\",\"size\":\"345ming\"},\"xiao2\":{\"__value\":\"43ren\",\"size\":\"3ben\"}}");
        string outjsonresult2;
        db.find("piaoliang,yuanse","yanse",&outjsonresult2);


        //{"dongxi":{"yanse":"34tiao","daxiao":"13hao","size":"3mian"}}
        //daxiao,size;   ..
        db.adddocdata("{\"dongxi\":{\"yanse\":\"34tiao\",\"daxiao\":\"13hao\",\"size\":\"3mian\"}}");
        string outjsonresult3;
        db.find("daxiao,size","..",&outjsonresult3);

        //跨链
        //
        //daxiao,yueli; ..
        string outjsonresult4;
        db.find("daxiao,yueli","..",&outjsonresult4);

        //json 搜
        //

        db.close();
    }





    QCoreApplication a(argc, argv);

    return a.exec();
}

