// StudentManagerSystem.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
// 打印有问题
//
#include "StudentManagerSystem.h"

int main()
{
	while (1)
	{
        Welcome();
        // 从键盘接收一个字符
        char ch = getch();
        switch (ch)
        {
        case '1':// 录入学生信息
            InputStudent();
            break;
        case '2':// 打印学生信息
            PrintStudent();
            break;
        case '3':// 保存学生信息
            SaveStudent();
            break;
        case '4':// 读取学生信息
            ReadStudent();
            break;
        case '5':// 统计所有学生人数
            printf("学生总人数为:%d\n", CountStudent());
            system("pause");// 暂停
            system("cls");// 清屏
            break;
        case '6':// 查找学生信息
        {
            Node* p = FindStudent();
            if (p != NULL)
                printf("学号:%d\t姓名:%s\t年龄:%d\t成绩:%d\n", p->stu.stuNum, p->stu.name, p->stu.age, p->stu.score);
            else
                printf("没有找到该学员.\n");

            system("pause");// 暂停
            system("cls");// 清屏
            break;
        }
        case '7':// 修改学生信息
            ModifyStudent();
            break;
        case '8':// 删除学生信息
            DeleteStudent();
            break;
        case '0':// 退出系统
            printf("Bye!!!\n");
            return 0;
            break;
        }
	}
}

// 欢迎
void Welcome()
{
    printf("*************************************************\n");
    printf("*\t欢迎使用高效学生成绩管理系统V1.0\t*\n");
    printf("*************************************************\n");
    printf("*\t\t请选择功能性列表\t\t*\n");
    printf("*************************************************\n");
    printf("*\t\t1.录入学生信息\t\t\t*\n");
    printf("*\t\t2.打印学生信息\t\t\t*\n");
    printf("*\t\t3.保存学生信息\t\t\t*\n");
    printf("*\t\t4.读取学生信息\t\t\t*\n");
    printf("*\t\t5.统计所有学生人数\t\t*\n");
    printf("*\t\t6.查找学生信息\t\t\t*\n");
    printf("*\t\t7.修改学生信息\t\t\t*\n");
    printf("*\t\t8.删除学生信息\t\t\t*\n");
    printf("*\t\t0.退出系统\t\t\t*\n");
    printf("*************************************************\n");
}

// 录入学生信息
void InputStudent()
{
    Node* pNewNode = (Node*)malloc(sizeof(Node));
    pNewNode->pNext = NULL;
    
    // 头插法
    if (g_pHead == NULL)
    {
        g_pHead = pNewNode;
    }
    else
    {
        pNewNode -> pNext = g_pHead;
        g_pHead = pNewNode;
    }
    printf("请输入学生姓名：\n");
    scanf("%s", pNewNode->stu.name);
    printf("请输入学生年龄：\n");
    scanf("%d", &pNewNode->stu.age);
    printf("请输入学生学号：\n");
    scanf("%d", &pNewNode->stu.stuNum);
    printf("请输入学生分数：\n");
    scanf("%d", &pNewNode->stu.score);
    printf("学生信息录入成功.\n");
    system("pause");// 暂停
    system("cls");// 清屏
}

// 打印学生信息
void PrintStudent()
{
    system("cls");// 清屏
    printf("*************************************************\n");
    printf("*\t欢迎使用高效学生成绩管理系统V1.0\t*\n");
    printf("*************************************************\n");
    printf("*\t学号\t姓名\t年龄\t成绩\t\t*\n");
    printf("*************************************************\n");
    // 遍历链表
    Node* p = g_pHead;
    while (p != NULL)
    {
        printf("*\t%d\t%s\t%d\t%d\t\t*\n",
            p->stu.stuNum,
            p->stu.name,
            p->stu.age,
            p->stu.score);
        p = p->pNext;
    }
    printf("*************************************************\n");
    system("pause");// 暂停
    system("cls");// 清屏
}

// 保存学生信息
void SaveStudent()
{
    // 打开文件
    FILE* fp = fopen("C:\\stuinfo.txt", "w");

    if (fp == NULL)
    {
        printf("打开文件失败.\n");
        return;
    }

    Node* p = g_pHead;
    while (p != NULL)
    {
        fwrite(&p->stu, 1, sizeof(Student), fp);
        p = p->pNext;
    }

    // 关闭文件
    fclose(fp);
    printf("\n数据保存成功\n");
    system("pause");// 暂停
    system("cls");// 清屏
}


// 读取学生信息
void ReadStudent()
{
    // 打开文件
    FILE* fp = fopen("C:\\stuinfo.txt", "r");

    if (fp == NULL)
    {
        printf("打开文件失败.\n");
        return;
    }

    // 读文件
    Student stu;
    while (fread(&stu,1,sizeof(Student),fp))
    {
        // 创建一个新的节点
        Node* pNewNode = (Node*)malloc(sizeof(Node));
        pNewNode->pNext = NULL;

        memcpy(pNewNode, &stu, sizeof(Student));

        // 头插法
        if (g_pHead == NULL)
        {
            g_pHead = pNewNode;
        }
        else
        {
            pNewNode->pNext = g_pHead;
            g_pHead = pNewNode;
        }
    }
    // 关闭文件
    fclose(fp);
    printf("\n数据读取成功\n");
    system("pause");// 暂停
    system("cls");// 清屏
}

// 统计所有学生人数
int CountStudent()
{
    int nCount = 0;// 学生总数
    // 遍历
    Node* p = g_pHead;
    while (p != NULL)
    {
        nCount++;
        p = p->pNext;
    }
    return nCount;
}

// 查找学生信息
Node* FindStudent()
{
    int nStuNum;
    char szName[20];
    printf("请输入要查找的学生的学号\n");
    scanf("%d", &nStuNum);
    printf("请输入要查找的学生的姓名\n");
    scanf("%s", szName);

    Node* p = g_pHead;
    while (p != NULL)
    {
        if (p->stu.stuNum == nStuNum || 0 == strcmp(p->stu.name, szName))
        {
            return p;
        }
        p = p->pNext;
    }
    return NULL;
}

// 修改学生信息
void ModifyStudent()
{
    int nStuNum;
    printf("请输入需要修改的学生信息的学号\n");
    scanf("%d", &nStuNum);

    Node* p = g_pHead;
    while (p != NULL)
    {
        if (p->stu.stuNum == nStuNum)
        {
            printf("请输入要修改的学生姓名 年龄 成绩:\n");
            scanf("%s %d %d", p->stu.name, &p->stu.age, &p->stu.score);
            printf("修改成功.\n");
            break;
        }
        p = p->pNext;
    }
    if (p == NULL)
        printf("没有找到该学生信息.\n");

    system("pause");// 暂停
    system("cls");// 清屏
}
// 删除学生信息
void DeleteStudent()
{
    int nStuNum;
    printf("请输入要删除的学生的学号:\n");
    scanf("%d", &nStuNum);
    
    Node* p1, * p2;

    // 判断是不是头节点
    if (g_pHead->stu.stuNum == nStuNum)
    {
        p1 = g_pHead;
        g_pHead = g_pHead->pNext;
        free(p1);

        system("pause");// 暂停
        system("cls");// 清屏
        return;
    }
    Node* p = g_pHead;
    // 不是头节点
    while (p->pNext != NULL)
    {
        if (p->pNext->stu.stuNum == nStuNum)
        {
            p2 = p->pNext;
            p->pNext = p->pNext->pNext;
            free(p2);

            system("pause");// 暂停
            system("cls");// 清屏
            return;
        }
        p = p->pNext;

        if (p->pNext == NULL)
        {
            break;
        }
    }
    if (p->pNext == NULL)
        printf("没有找到该学生");
    system("pause");// 暂停
    system("cls");// 清屏
}