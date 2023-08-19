#include "database.h"
#include<mysql.h>
static bool connect_db(MYSQL& mysql);

bool fetch_user(userinfo& user)
{
    MYSQL mysql;
    MYSQL_RES* res; //�����
    MYSQL_ROW row;
    char sql[256];
    //���ӵ����ݿ�
    if (!connect_db(mysql)) {
        return false;
    }
    //��ѯ
    snprintf(sql, 256, "select id,level_id from users where username = '%s' and pass = md5('%s');", user.name.c_str(), user.pwd);
    bool ret = mysql_query(&mysql, sql);  // �ɹ�����0
    if (ret) {
        cout << "����ԭ��" << mysql_error(&mysql) << endl;
        mysql_close(&mysql);
        return false;
    }
    //��ȡ���
    res = mysql_store_result(&mysql);
    row = mysql_fetch_row(res);
    //���û�鵽���ݾͽ��� �ͷſռ�
    if (!row) {
        mysql_free_result(res);
        mysql_close(&mysql);
        return false;
    }
    //ȡ���û�id�͹ؿ�id
    user.id = atoi(row[0]);
    user.level_id = atoi(row[1]);

    cout << endl << "�û�ID:" << user.id;

    mysql_free_result(res);
    mysql_close(&mysql);


    return true;
}

bool fetch_level(levelinfo& level, int level_id)
{
    MYSQL mysql;
    MYSQL_RES* res; //�����
    MYSQL_ROW row;
    char sql[256];
    //���ӵ����ݿ�
    if (!connect_db(mysql)) {
        return false;
    }
    //�����û���level_id ȥȡ���ؿ�id Ҳ���ǵڼ���
    snprintf(sql, 256, "select mapname,map_row,map_col,map_data,next_level_id from levels where id = %d;", level_id);
    bool ret = mysql_query(&mysql, sql);  // �ɹ�����0
    if (ret) {
        cout << "����ԭ��" << mysql_error(&mysql) << endl;
        mysql_close(&mysql);
        return false;
    }
    //��ȡ���
    res = mysql_store_result(&mysql);
    row = mysql_fetch_row(res);

    level.id = level_id;
    level.name = row[0];
    level.map_row = atoi(row[1]);
    level.map_col = atoi(row[2]);
    level.map_data = row[3];
    level.next_level_id = atoi(row[4]);
    cout << "�û��ؿ�ID:" << level.id << endl;
    cout << level.name << endl;
    //cout << level.map_data << endl;
    //�ͷŽ����
    mysql_free_result(res);
    //�ر����ݿ�
    mysql_close(&mysql);

    return true;
}

bool transform_map(levelinfo& level, int map[ROW][COL])
{
    if (level.map_row > ROW || level.map_col > COL) {   //���������Χ ֱ�ӽ���
        cout << "��ͼ̫��" << endl;
        return false;
    }
    if (level.map_data.length() < 1) {   //���������Χ ֱ�ӽ���
        cout << "��ͼ̫С" << endl;
        return false;
    }
    int start = 0, end = 0;
    int row = 0, col = 0;
    do {
        end = level.map_data.find('|', start);   //��start���±꿪ʼ ����һ��|˵��ĳһ���Ѿ�ȡ�� 
        if (end < 0) {   //��Ϊ���һ��û��|  ������Ҫ����
            end = level.map_data.length();
        }
        if (start >= end) {          //���ݶ�ȡ��� ����
            break;
        }

        string line = level.map_data.substr(start, end - start);
        //�������ݽ��н���
        char* next_token = NULL;
        //һ��һ���ö��ŷֿ��ɵ����ַ�
        char* item = strtok_s((char*)line.c_str(), ",", &next_token);
        col = 0;
        while (item&&col<level.map_col) {
            cout << item <<" ";
            map[row][col] = atoi(item);   //ȡ���������ַ��� תΪint
            col++;
            item=strtok_s(NULL, ",", &next_token);  //ת�����ź�����Ǹ��ַ�  ѭ��ȡ��
        }
        if (col > level.map_col) {
            cout << "��ͼ����" << endl;
            return false;
        }
        if (row >= level.map_row) {
            break;
        }
        cout << endl;
        row++;

        start = end + 1;  //׼����һ��ѭ��
    } while (true);
    if (row < level.map_row) {  //����ͼ������������Ӧ�ú����ݿ����һ�� ��������� ֱ�ӽ���   
        cout << "��ͼ��ȫ" << endl;
        return false;
    }
    return true;
}

bool updata_user_level(userinfo& user, int next_level_id)
{
    MYSQL mysql;
    char sql[256];
    //���ӵ����ݿ�
    if (!connect_db(mysql)) {
        return false;
    }
    //�����û�id����next_level_id
    snprintf(sql, 256, "update users set level_id=%d where id=%d;", next_level_id, user.id);
    bool ret = mysql_query(&mysql,sql);  //�ɹ�����0
    if (ret) {
        cout << "����" << mysql_error(&mysql) << endl;
        mysql_close(&mysql);
        return false;
    }
    mysql_close(&mysql);
    return true;
}
//ע���û�
bool user_register(userinfo& user)
{
    MYSQL mysql;
    char sql[256];
    //���ӵ����ݿ�
    if (!connect_db(mysql)) {
        return false;
    }
    //�����ݿ������Ϣ
    snprintf(sql, 256, "insert into users values(null,'%s',md5('%s'),1);",user.name.c_str(),user.pwd);
    bool ret = mysql_query(&mysql, sql);  //�ɹ�����0
    if (ret) {
        cout << "����" << mysql_error(&mysql) << endl;
        mysql_close(&mysql);
        exit(-1);
    }
    mysql_close(&mysql);
    return true;
}


bool connect_db(MYSQL& mysql)
{
    //��ʼ�����ݿ�
    mysql_init(&mysql);
    //�����ַ�����
    mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
    //�������ݿ�
    if (!mysql_real_connect(&mysql, "127.0.0.1", "root", "0302", "box_man", 13306, NULL, 0)) {
        cout << "���ӳ���" << " " << mysql_error(&mysql) << endl;
        return false;
    }
    return true;
}

