#include <iostream>
#include <fstream>
#include <string>
using namespace std;

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

typedef struct LNode  //定义链表结构体
{
     Task data;  //数据域
     struct LNode  *next;  //指针域
} LNode, *LinkList;  //LinkList为LNode的指针   

bool valid_date_format(const string &s) 
{
    if (s.size() != 10) return false;
    if (s[4] != '-' || s[7] != '-') return false;
    for (int i = 0; i < 10; ++i) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(s[i])) return false;
    }
    return true;
}

Status InitList_Link(LinkList &L)  //构造空链表
{
    L = new LNode;  //头结点
    L->next = nullptr;  //头结点指针置为空
    return OK;
}

Status add_task(LinkList &L, Task t)  //添加任务
{
    LNode *s = new LNode;  //创建新结点
    s->data = t; //将输入的任务存放在新结点的data中
    s->next = L->next;  //插入到头结点后
    L->next = s;
    cout << "任务添加成功" << endl;
    return OK;
}

Status delete_task(LinkList &L, string name)
{
    LinkList p = L;
    bool found = false;
    while (p->next != nullptr)
    {
        LinkList r = p->next;
        if (r->data.name == name)
        {
            p->next = r->next;
            delete r;
            cout << "任务删除成功" << endl;
            found = true;
        }
        else p = p->next;
    }
    if (! found)
    {
        cout << "未找到该任务" << endl;
        return ERROR;
    }
    return OK;
}

Status search_task(LinkList &L, string key, int way)
{
    bool found = false;
    LinkList p = L->next;
    if (way == 1)
    {
        while(p)
        {
            if (p->data.name == key)
            {
                cout << "任务名称：" << p->data.name << endl;
                cout << "任务描述：" << p->data.desc << endl;
                cout << "任务优先级：" << p->data.priority << endl;
                cout << "任务截止日期：" << p->data.deadline << endl;
                found = true;
            }
            p = p->next;
        }
        if (! found)
        {
            cout << "未找到该任务" << endl;
            return ERROR;
        }
    }
    else if (way == 2)
    {
        if (! valid_date_format(key))
        {
            cout << "日期格式错误，请按 YYYY-MM-DD 输入！" << endl;
            return ERROR;
        }
        while(p)
        {
            if (p->data.deadline == key)
            {
                cout << "任务名称：" << p->data.name << endl;
                cout << "任务描述：" << p->data.desc << endl;
                cout << "任务优先级：" << p->data.priority << endl;
                cout << "任务截止日期：" << p->data.deadline << endl;
                found = true;
            }
            p = p->next;
        }
        if (! found)
        {
            cout << "未找到该任务" << endl;
            return ERROR;
        }
    }
    else
    {
        cout << "搜索方式错误,请输入1或2" << endl;
    }
    return OK;
}


Status modify_task(LinkList &L, string name) 
{
    LinkList p = L->next;
    while (p)
    {
        if (p->data.name == name)
        {
            cout << "输入新名称: "; 
            cin >> p->data.name;
            cout << "输入新描述: "; 
            
            while (true) {
                cout << "输入新优先级(1-5): ";
                cin >> p->data.priority;

                if (!cin) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "输入无效，请输入1-5的整数！" << endl;
                    continue;
                }

                if (p->data.priority < 1 || p->data.priority > 5) {
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
            p->data.deadline = deadline;
            cout << "修改任务成功" << endl;
            return OK;
        }
        else p = p->next;
    }
    cout << "未找到该任务" << endl;
    return ERROR;
    
}

Status view_task(LinkList &L)
{
    if (L->next == nullptr)
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
    if (way == 1)
    {
        LinkList sorted = L->next;
        LinkList r = sorted->next;
        sorted->next = nullptr;
        
        while(r)
        {
            LinkList t = r->next;
            LinkList p = L;
            while (p->next && r->data.priority <= p->next->data.priority)
            {
                p = p->next;
            }
            r->next = p->next;
            p->next = r;
            r = t;
        }
        
        LinkList p = L->next;
        int i = 1;
        while (p)
        {
            cout << i << "." << p->data.name << "(优先级" << p->data.priority << ")" << endl;
            p = p->next;
            i++;
        }
    }
    else if (way == 2)
    {
        LinkList sorted = L->next;
        LinkList r = sorted->next;
        sorted->next = nullptr;
        
        while(r)
        {
            LinkList t = r->next;
            LinkList p = L;
            while (p->next && r->data.deadline >= p->next->data.deadline)
            {
                p = p->next;
            }
            r->next = p->next;
            p->next = r;
            r = t;
        }

        LinkList p = L->next;
        int i = 1;
        while (p)
        {
            cout << i << "." << p->data.name << "(截止日期" << p->data.deadline << ")" << endl;
            p = p->next;
            i++;
        }
    }
    else
    {
        cout << "查看方式错误,请输入1或2" << endl;
        return ERROR;
    }
    return OK;
}

Status save_tasks(LinkList &L, const string &filename) 
{
    ofstream fout(filename);
    LinkList p = L->next;
    while (p) 
    {
        fout << p->data.name << " "
             << p->data.desc << " "
             << p->data.priority << " "
             << p->data.deadline << endl;
        p = p->next;
    }
    fout.close();
    cout << "任务已保存到" << filename << endl;
    return OK;
}

Status load_tasks(LinkList &L, const string &filename) 
{
    ifstream fin(filename);
    if (!fin) 
    {
        cout << "未找到文件: " << filename << endl;
        return ERROR;
    }
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
    LinkList L;
    InitList_Link(L);
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
        case 1:
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
                cout << "输入截止日期 (YYYY-MM-DD): ";
                cin >> deadline;
                if (valid_date_format(deadline)) break;
                cout << "日期格式错误，请按 YYYY-MM-DD 输入！" << endl;
            }
            t.deadline = deadline;
            
            add_task(L, t);
            break;
        }
        case 2:
        {
            string name;
            cout << "输入要删除的任务名称: ";
            cin >> name;
            delete_task(L, name);
            break;
        }
        case 3:
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
        case 4:
        {
            string name;
            cout << "请输入要修改的任务名称" << endl;
            cin >> name;
            modify_task(L, name);
            break;
        }
        case 5:
        {
            view_task(L);
            break;
        }
        case 6:
        {
            string filename;
            cout << "请输入文件名" << endl;
            cin >> filename;
            save_tasks(L, filename);
            break;
        }
        case 7:
        {
            string filename;
            cout << "请输入文件名" << endl;
            cin >> filename;
            load_tasks(L, filename);
            break;
        }
        case 0:
        {
            cout << "退出程序，再见！" << endl;
            return 0;
        }
        default:
        cout << "请输入0-7的整数" << endl;
    }
    }
}