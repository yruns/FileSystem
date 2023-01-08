#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cstring>
#include<time.h>

using namespace std;

// 用户组
struct user {
    string username;
    string password;
};

// 文件或目录
struct f_node {
    string filename;
    string content;
    string address;
    
    int is_dir;
    int is_open;
    int length;
    int child_num;

    tm* update_time;

    f_node* parent;
    f_node* child;
    f_node* next;
};

// 定义用户组
user users[2] = {
    "yruns", "root",
    "root", "root"
};

// 当前路径
string path;

// 当前目录   根目录
f_node* root;
f_node* current;

// 目录（文件）物理地址中的数字
int m = 0, n = 0;

// 登录校验函数
int login(string username, string password);

// 初始化文件（目录）
f_node* init_file_dir(string filename, int is_dir);

// 初始化根目录
void create_root_dir();

// 插入f_node节点
void insert_node(string filename, int is_dir);

// 创建目录函数
void mkdir();

// 创建文件函数
void create();

// 列出当前目录
void dir();

int run() // 运行文件系统
{
	char command[10];
	cout << "Linux:" << path << ">"; // 显示路径
	cin >> command;					 // 输入命令
	if (strcmp(command, "help") == 0)
        printf("help");
		// help();
	else if (strcmp(command, "mkdir") == 0)
		mkdir();
	else if (strcmp(command, "create") == 0)
		create();
	else if (strcmp(command, "dir") == 0)
		dir();
	// else if (strcmp(command, "write") == 0)
	// 	write();
	// else if (strcmp(command, "read") == 0)
	// 	read();
	// else if (strcmp(command, "cd") == 0)
	// 	cd();
	// else if (strcmp(command, "del") == 0)
	// 	del();
	// else if (strcmp(command, "open") == 0)
	// 	open();
	// else if (strcmp(command, "close") == 0)
	// 	close();
	else if (strcmp(command, "quit") == 0)
		return 0;
	else
		cout << "请参考help提供的命令列表" << endl;
}

int main()
{
	int flag; // 标记用户名密码是否正确
	char username[8], password[8];
	cout << "              -----------------------------------------------             " << endl;
	cout << "                           +++Linux文件系统+++                      " << endl;
	cout << "                   账号：user1-user4   密码：user1-user4                   " << endl;
	cout << "                                mkdir  目录名                                 " << endl;
	cout << "                                create 文件名                                 " << endl;
	cout << "                                write  文件名                                 " << endl;
	cout << "                                read   文件名                                 " << endl;
	cout << "                                cd     目录名                                 " << endl;
	cout << "                                open   文件名                                 " << endl;
	cout << "                                close  文件名                                 " << endl;
	cout << "                                del    文件名或目录名                     " << endl;
	cout << "                                dir                                 " << endl;
	cout << "                                quit                                 " << endl;
	cout << "                             输入help可获得帮助                             " << endl;
	cout << "              -----------------------------------------------             " << endl;

	create_root_dir();
	while (1)
	{
		if (!run()) // 退出系统
			break;
	}
}


// int main() {

//     cout << login("yrunss", "root") << endl;


//     cout << users[1].password << endl;

//     return 0;
// }

int login(string username, string password) {

    int user_num = sizeof(users) / sizeof(user);
    for (int i = 0; i < user_num; i++) {
        if (username == users[i].username &&
            password == users[i].password) {
            // 登录成功
            return 1;
        }
    }
    // 登录失败
    return 0;
}

f_node* init_file_dir(string filename, int is_dir) {

    f_node* t = new f_node;
    t->filename = filename;
    t->is_dir = is_dir;
    t->is_open = 0;
    t->parent = t->child = t->next = NULL;
    t->length = 0;

    time_t now_time = time(NULL);
    // 获取本地时间
    tm *t_tm = localtime(&now_time);
    t->update_time = t_tm;

    return t;
}

void create_root_dir() {
    string filename = "/";
    root = init_file_dir(filename, 1);
    current = root;
    path = "/";
}

void insert_node(string filename, int is_dir) {
    f_node* t = init_file_dir(filename, 1);

    string name = is_dir ? "目录": "文件";

    if (current->child == NULL) {
        // 当前目录无文件（目录），直接创建
        current->child = t;
        t->parent = current;
        m++;
        cout << name << "创建成功!" << endl;
    } else {
        // 有文件，防止重名；创建后挂到队尾。
        f_node* p = current->child;
        f_node* q = p;
        while (p != NULL) {
            if (p->filename == filename) {
                cout << name << "已存在";
                break;
            } else {
                p = p->next;
            }
        }
        p = t;
        p->parent = current;
        m++;
        cout << name << "创建成功!" << endl;
    }
}

void mkdir() {
    f_node* t;
    string filename;
    cin >> filename;

    insert_node(filename, 1);
}

void create() {
    string filename;
    cin >> filename;

    insert_node(filename, 0);
}

void dir() {
    int dir_num = 0, file_num = 0;
    int total_capacity = 0;
    if (current->child != NULL) {
        // 非空目录
        f_node* p = current->child;
        while (p != NULL) {
            if (p->is_dir) {
                dir_num++;
            } else {
                file_num++;
            }
            total_capacity += p->length;
            printf("-rw-r--r-- 4 %s %5d %s %s\n", p->filename, p->length, asctime(p->update_time), p->filename);
            p = p->next;
        }
    }
    printf("Total  %2d directorys  %2d files  %5d capacity\n", dir_num, file_num, total_capacity);
}