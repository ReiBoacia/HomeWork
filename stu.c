#include <stdio.h>

int main(void)
{
    int a;
    int score = 0;
    int newScore;

    while (1)
    {
        printf("\n");
        printf("==========学生成绩管理系统==========\n");
        printf("1.录入学生信息\n");
        printf("2.修改学生信息\n");
        printf("3.删除学生信息\n");
        printf("4.查询学生信息\n");
        printf("5.退出\n");
        printf("====================================\n");
        printf("请输入你的选择（1-5）：");

        scanf("%d", &a);

        printf("选择功能是：%d\n", a);

        // 功能1：录入学生成绩
        if (a == 1)
        {
            printf("输入学生的成绩：");
            scanf("%d", &newScore);

            while (newScore < 0 || newScore > 100)
            {
                printf("请输入学生的成绩（0-100）：");
                scanf("%d", &newScore);
            }

            score = newScore;

            printf("学生的成绩是：%d\n", score);
        }

        // 功能2：修改学生成绩
        else if (a == 2)
        {
            printf("修改学生的成绩为：");
            scanf("%d", &newScore);

            while (newScore < 0 || newScore > 100)
            {
                printf("请输入学生的成绩（0-100）：");
                scanf("%d", &newScore);
            }

            score = newScore;

            printf("学生修改后的成绩是：%d\n", score);
        }

        // 功能3：删除学生成绩
        else if (a == 3)
        {
            printf("需要删除学生的成绩是：%d\n", score);

            score = 0;

            printf("现在的学生成绩是：%d\n", score);
        }

        // 功能4：查询学生成绩
        else if (a == 4)
        {
            printf("查询学生的成绩是：%d\n", score);
        }

        // 功能5：退出系统
        else if (a == 5)
        {
            printf("已退出学生成绩管理系统！\n");
            break;
        }

        // 输入错误
        else
        {
            printf("请输入1到5之间的数字！\n");
        }
    }

    return 0;
}