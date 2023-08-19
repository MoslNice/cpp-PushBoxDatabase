#include "database.h"
#include<mysql.h>
static bool connect_db(MYSQL& mysql);

bool fetch_user(userinfo& user)
{
    MYSQL mysql;
    MYSQL_RES* res; //结果集
    MYSQL_ROW row;
    char sql[256];
    //连接到数据库
    if (!connect_db(mysql)) {
        return false;
    }
    //查询
    snprintf(sql, 256, "select id,level_id from users where username = '%s' and pass = md5('%s');", user.name.c_str(), user.pwd);
    bool ret = mysql_query(&mysql, sql);  // 成功返回0
    if (ret) {
        cout << "错误原因：" << mysql_error(&mysql) << endl;
        mysql_close(&mysql);
        return false;
    }
    //获取结果
    res = mysql_store_result(&mysql);
    row = mysql_fetch_row(res);
    //如果没查到数据就结束 释放空间
    if (!row) {
        mysql_free_result(res);
        mysql_close(&mysql);
        return false;
    }
    //取出用户id和关卡id
    user.id = atoi(row[0]);
    user.level_id = atoi(row[1]);

    cout << endl << "用户ID:" << user.id;

    mysql_free_result(res);
    mysql_close(&mysql);


    return true;
}

bool fetch_level(levelinfo& level, int level_id)
{
    MYSQL mysql;
    MYSQL_RES* res; //结果集
    MYSQL_ROW row;
    char sql[256];
    //连接到数据库
    if (!connect_db(mysql)) {
        return false;
    }
    //根据用户的level_id 去取出关卡id 也就是第几关
    snprintf(sql, 256, "select mapname,map_row,map_col,map_data,next_level_id from levels where id = %d;", level_id);
    bool ret = mysql_query(&mysql, sql);  // 成功返回0
    if (ret) {
        cout << "错误原因：" << mysql_error(&mysql) << endl;
        mysql_close(&mysql);
        return false;
    }
    //获取结果
    res = mysql_store_result(&mysql);
    row = mysql_fetch_row(res);

    level.id = level_id;
    level.name = row[0];
    level.map_row = atoi(row[1]);
    level.map_col = atoi(row[2]);
    level.map_data = row[3];
    level.next_level_id = atoi(row[4]);
    cout << "用户关卡ID:" << level.id << endl;
    cout << level.name << endl;
    //cout << level.map_data << endl;
    //释放结果集
    mysql_free_result(res);
    //关闭数据库
    mysql_close(&mysql);

    return true;
}

bool transform_map(levelinfo& level, int map[ROW][COL])
{
    if (level.map_row > ROW || level.map_col > COL) {   //如果超出范围 直接结束
        cout << "地图太大" << endl;
        return false;
    }
    if (level.map_data.length() < 1) {   //如果超出范围 直接结束
        cout << "地图太小" << endl;
        return false;
    }
    int start = 0, end = 0;
    int row = 0, col = 0;
    do {
        end = level.map_data.find('|', start);   //从start的下标开始 碰到一个|说明某一行已经取完 
        if (end < 0) {   //因为最后一行没有|  所以需要特判
            end = level.map_data.length();
        }
        if (start >= end) {          //数据读取完毕 结束
            break;
        }

        string line = level.map_data.substr(start, end - start);
        //对行数据进行解析
        char* next_token = NULL;
        //一个一个用逗号分开成单个字符
        char* item = strtok_s((char*)line.c_str(), ",", &next_token);
        col = 0;
        while (item&&col<level.map_col) {
            cout << item <<" ";
            map[row][col] = atoi(item);   //取出来的是字符串 转为int
            col++;
            item=strtok_s(NULL, ",", &next_token);  //转到逗号后面的那个字符  循环取出
        }
        if (col > level.map_col) {
            cout << "地图出错" << endl;
            return false;
        }
        if (row >= level.map_row) {
            break;
        }
        cout << endl;
        row++;

        start = end + 1;  //准备下一次循环
    } while (true);
    if (row < level.map_row) {  //最后地图的行数和列数应该和数据库里的一样 如果不满足 直接结束   
        cout << "地图不全" << endl;
        return false;
    }
    return true;
}

bool updata_user_level(userinfo& user, int next_level_id)
{
    MYSQL mysql;
    char sql[256];
    //连接到数据库
    if (!connect_db(mysql)) {
        return false;
    }
    //根据用户id更新next_level_id
    snprintf(sql, 256, "update users set level_id=%d where id=%d;", next_level_id, user.id);
    bool ret = mysql_query(&mysql,sql);  //成功返回0
    if (ret) {
        cout << "错误：" << mysql_error(&mysql) << endl;
        mysql_close(&mysql);
        return false;
    }
    mysql_close(&mysql);
    return true;
}
//注册用户
bool user_register(userinfo& user)
{
    MYSQL mysql;
    char sql[256];
    //连接到数据库
    if (!connect_db(mysql)) {
        return false;
    }
    //在数据库添加信息
    snprintf(sql, 256, "insert into users values(null,'%s',md5('%s'),1);",user.name.c_str(),user.pwd);
    bool ret = mysql_query(&mysql, sql);  //成功返回0
    if (ret) {
        cout << "错误：" << mysql_error(&mysql) << endl;
        mysql_close(&mysql);
        exit(-1);
    }
    mysql_close(&mysql);
    return true;
}


bool connect_db(MYSQL& mysql)
{
    //初始化数据库
    mysql_init(&mysql);
    //设置字符编码
    mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
    //连接数据库
    if (!mysql_real_connect(&mysql, "127.0.0.1", "root", "0302", "box_man", 13306, NULL, 0)) {
        cout << "连接出错" << " " << mysql_error(&mysql) << endl;
        return false;
    }
    return true;
}

