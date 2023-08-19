#pragma once
#include<string>
#include<iostream>
using namespace std;

#define COL 48
#define ROW 48


typedef class _userinfo {
public:
	int id;
	string name;
	char pwd[20];
	int level_id;
}userinfo;

typedef class _levelinfo {
public:
	int id;
	int map_row;
	string name;
	int map_col;
	string map_data;
	int next_level_id;
}levelinfo;
bool fetch_user(userinfo& user);
bool fetch_level(levelinfo& level,int level_id);
bool transform_map(levelinfo& level,int map[ROW][COL]);
bool updata_user_level(userinfo& user, int next_level_id);
bool user_register(userinfo& user);
