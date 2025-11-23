#include <iostream>
#include <fstream>
#include <string>
#include <limits>
using namespace std;

#define LIST_INIT_SIZE 100 //存储空间的初始分配量
#define LISTINCREMENT  10  //存储空间的分配增量

typedef int Status;
#define OK 1
#define ERROR 0
#define OVERFLOW_ERR -2

struct Task  //定义任务结构体
{
    string name;
    string desc;
    int priority;
    string deadline;
};

struct SqList  //定义顺序表结构体
{
    Task *elem;  //存储空间基址
    int length;  //当前任务列表中任务个数
    int listsize; //顺序表的最大长度
};

bool valid_date_format(const string &s) {
    if (s.size() != 10) return false;
    if (s[4] != '-' || s[7] != '-') return false;
    for (int i = 0; i < 10; ++i) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(s[i])) return false;
    }
    return true;
}

Status InitList_Sq(SqList &L)  //构造空顺序表
{
    L.elem = new Task[LIST_INIT_SIZE]; //分配地址
    if (!L.elem) exit(OVERFLOW_ERR); //储存分配失败
    L.length = 0; //初始长度为0
    L.listsize = LIST_INIT_SIZE; //初始储存容量
    return OK;
}

Status add_task(SqList &L, Task t)  //添加任务
{
    if (L.length >= L.listsize)  //当前存储空间已满
    {
        Task *newbase = new Task[L.listsize + LISTINCREMENT];  //分配新的存储空间
        if (!newbase) exit(OVERFLOW_ERR); //存储空间分配失败
        for (int i = 0; i < L.length; i++) newbase[i] = L.elem[i]; //将原有数据拷贝到新地址
        delete[] L.elem; //释放原有空间
        L.elem = newbase;
        L.listsize += LISTINCREMENT;
    }
    
    L.elem[L.length++] = t;  //添加任务
    cout << "任务添加成功" << endl;
    return OK;
}

Status delete_task(SqList &L, string name)  //删除任务
{
    bool found = false;
    for (int i = 0; i < L.length; i++)  //遍历顺序表
    {
        if (L.elem[i].name == name)
        {
            for (int j = i; j < L.length - 1; j++)
            {
                L.elem[j] = L.elem[j + 1];  //将后续元素前移
            }
        L.length --;
        cout << "任务删除成功" << endl;
        found = true;
        }
    }
    if (! found)
    {
        cout << "该任务不存在" << endl;
        return ERROR;
    }
    
    return OK;
}

Status search_task(SqList &L, string key, int way)  //搜索任务
{
    bool found = false;
    if (way == 1)  //按照名称搜索
    {
        for (int i = 0; i < L.length; i++)
        {
            if (L.elem[i].name == key)
            {
                cout << "任务名称：" << L.elem[i].name << endl;
                cout << "任务描述：" << L.elem[i].desc << endl;
                cout << "任务优先级：" << L.elem[i].priority << endl;
                cout << "任务截止日期：" << L.elem[i].deadline << endl;
                found = true;
            }
        }
        if (! found)
        {
            cout << "未查询到该任务" << endl;
            return ERROR;
        }
    }
    else if (way == 2)  //按照截止日期搜索
    {
        if (! valid_date_format(key))
        {
            cout << "日期格式错误，请按 YYYY-MM-DD 输入！" << endl;
            return ERROR;
        }
        for (int i = 0; i < L.length; i++)
        {
            if (L.elem[i].deadline == key)
            {
                cout << "任务名称：" << L.elem[i].name << endl;
                cout << "任务描述：" << L.elem[i].desc << endl;
                cout << "任务优先级：" << L.elem[i].priority << endl;
                cout << "任务截止日期：" << L.elem[i].deadline << endl;
                found = true;
            }
        }
        if (! found)
        {
            cout << "未查询到该任务" << endl;
            return ERROR;
        }
    }
    else
    {
        cout << "搜索方式错误,请输入1或2" << endl;
    }
    return OK;
}

Status modify_task(SqList &L, string name)  //修改任务
{
    for (int i = 0; i < L.length; i++) 
    {
        if (L.elem[i].name == name)  //遍历顺序表，根据名称找到对应任务
        {
            cout << "输入新名称: "; 
            cin >> L.elem[i].name;
            cout << "输入新描述: "; 
            cin >> L.elem[i].desc;
            while (true) {
                cout << "输入新优先级(1-5): ";
                cin >> L.elem[i].priority;

                if (!cin) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "输入无效，请输入1-5的整数！" << endl;
                    continue;
                }

                if (L.elem[i].priority < 1 || L.elem[i].priority > 5) {
                    cout << "优先级必须是1-5之间的整数！" << endl;
                    continue;
                }
             break;
             }

            
            string deadline;
            while (true) {
                cout << "输入新截止日期 (YYYY-MM-DD): ";
                cin >> deadline;
                if (valid_date_format(deadline)) break;
                cout << "日期格式错误，请按 YYYY-MM-DD 输入！" << endl;
            }
            L.elem[i].deadline = deadline;
            cout << "修改任务成功" << endl;
            return OK;
        }
    }
    cout << "未找到该任务" << endl;
    return ERROR;
}

Status view_task(SqList &L)  //查看任务列表
{
    if (L.length == 0)  //任务列表为空
    {
        cout << "当前任务列表为空" << endl;
        return OK;
    }
    int way;
    cout << "请选择排序方式" << endl;
    cout << "按优先级排序查看,请输入1" << endl;
    cout << "按截止日期排序查看,请输入2" << endl;
    cout << "请选择排序方式" << endl;
    cin >> way;
    if (way == 1)  //按照优先级排序
    {
        for (int i = 0; i < L.length - 1; i++) //遍历顺序表
        {
            for (int j = i + 1; j < L.length; j++)  //遍历i号之后的元素
            {
                if (L.elem[i].priority < L.elem[j].priority)  //i优先级较低则交换
                {
                    Task t = L.elem[i];
                    L.elem[i] = L.elem[j];
                    L.elem[j] = t;
                }
            }
        }
        for (int i = 0; i < L.length; i++)
        {
            cout << i + 1 << "." << L.elem[i].name << "(优先级" << L.elem[i].priority << ")" << endl;
        }
    }
    else if (way == 2)  //按照截止日期排序
    {
        for (int i = 0; i < L.length - 1; i++)  //遍历顺序表
        {
            for (int j = i + 1; j < L.length; j++)  //遍历i号之后元素
            {
                if (L.elem[i].deadline > L.elem[j].deadline)  //i截止日期更远则交换
                {
                    Task t = L.elem[i];
                    L.elem[i] = L.elem[j];
                    L.elem[j] = t;
                }
            }
        }
        for (int i = 0; i < L.length; i++)
        {
            cout << i + 1 << "." << L.elem[i].name << "(" << "截止日期" << L.elem[i].deadline << ")" << endl;
        }
    }
    else
    {
        cout << "查看方式错误,请输入1或2" << endl;
        return ERROR;
    }
    return OK;
}

Status save_tasks(SqList &L, const string &filename)  //保存任务列表到文件
{
    ofstream fout(filename);
    for (int i = 0; i < L.length; i++) {
        fout << L.elem[i].name << " "
             << L.elem[i].desc << " "
             << L.elem[i].priority << " "
             << L.elem[i].deadline << endl;
    }
    fout.close();
    cout << "任务已保存到" << filename << endl;
    return OK;
}

Status load_tasks(SqList &L, const string &filename)  //从文件读取任务列表
{
    ifstream fin(filename);
    if (!fin) 
    {
        cout << "未找到文件: " << filename << endl;
        return ERROR;
    }
    L.length = 0;
    Task t;
    while (fin >> t.name >> t.desc >> t.priority >> t.deadline) {
        add_task(L, t);
    }
    fin.close();
    cout << "读取成功" << endl;
    return OK;
}


int main()
{
    SqList L;
    InitList_Sq(L); //初始化任务列表
    while(true)
    {
    int choice;
    cout << "欢迎使用任务管理系统" << endl;
    cout << "添加任务 请输入1" << endl;
    cout << "删除任务 请输入2" << endl;
    cout << "查询任务 请输入3" << endl;
    cout << "修改任务 请输入4" << endl;
    cout << "查看任务列表 请输入5" << endl;
    cout << "保存任务列表 请输入6" << endl;
    cout << "读取任务列表 请输入7" << endl;
    cout << "退出系统 请输入0" << endl;
    cin >> choice;
    if (!cin) 
    { // 输入失败
        cin.clear(); // 清除错误标志位
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 丢弃错误输入
        cout << "输入无效,请输入0-7的整数" << endl;
        continue; // 回到菜单
    }

    switch(choice)
    {
        case 1:  //添加任务
        {
            Task t;
            cout << "输入任务名称: "; cin >> t.name;
            cout << "输入任务描述: "; cin >> t.desc;
            
            while (true) {
                cout << "输入优先级(1-5): ";
                cin >> t.priority;
                if (!cin) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "输入无效,请输入1-5的整数!" << endl;
                continue;
                }
                if (t.priority < 1 || t.priority > 5) {
                cout << "优先级必须是1-5之间的整数!" << endl;
                continue;
                }
                break; // 输入合法，退出循环
            }
            
            string deadline;
            while (true) {
                cout << "输入截止日期(YYYY-MM-DD): ";
                cin >> deadline;
                if (valid_date_format(deadline)) break;
                cout << "日期格式错误，请按 YYYY-MM-DD 输入！" << endl;
            }
            t.deadline = deadline;
            
            add_task(L, t);
            break;
        }
        case 2:  //删除任务
        {
            string name;
            cout << "输入要删除的任务名称: ";
            cin >> name;
            delete_task(L, name);
            break;
        }
        case 3:  //查询任务
        {
            string key;
            int way;
            cout << "请选择查询方式,按名称查询请输入1,按截止日期查询请输入2" << endl;
            cin >> way;
            cout << "请输入查询内容(截止日期格式YYYY-MM-DD)" << endl;
            cin >> key;
            search_task(L, key, way);
            break;
        }
        case 4:  //修改任务
        {
            string name;
            cout << "请输入要修改的任务名称" << endl;
            cin >> name;
            modify_task(L, name);
            break;
        }
        case 5:  //查看任务列表
        {
            view_task(L);
            break;
        }
        case 6:  //保存任务列表
        {
            string filename;
            cout << "请输入文件名" << endl;
            cin >> filename;
            save_tasks(L, filename);
            break;
        }
        case 7:  //读取任务列表
        {
            string filename;
            cout << "请输入文件名" << endl;
            cin >> filename;
            load_tasks(L, filename);
            break;
        }
        case 0:  //退出程序
        {
            cout << "退出程序，再见！" << endl;
            return 0;
        }
        default:
        cout << "请输入0-7的整数" << endl;
    }
    }
}