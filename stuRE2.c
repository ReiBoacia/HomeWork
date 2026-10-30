
#include <stdio.h>

int main(void)
{
    int grade[5][4] = {0};
    int choice, num;
    int eng, math, chi;
    int i, j, index;

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
        printf("请选择功能（1-5）：");

        if (scanf("%d", &choice) != 1)
        {
            printf("输入错误！\n");
            return 1;
        }

        switch (choice)
        {
            case 1:
                printf("录入学生成绩\n");

                index = -1;

                for (i = 0; i < 5; i++)
                {
                    if (grade[i][0] == 0)
                    {
                        index = i;
                        break;
                    }
                }

                if (index == -1)
                {
                    printf("学生信息已满！\n");
                    break;
                }

                printf("请输入学生的学号：");
                scanf("%d", &num);

                if (num <= 0)
                {
                    printf("学号必须是正整数！\n");
                    break;
                }

                for (i = 0; i < 5; i++)
                {
                    if (grade[i][0] == num)
                    {
                        break;
                    }
                }

                if (i < 5)
                {
                    printf("该学号已经存在！\n");
                    break;
                }

                printf("请输入学生的英语成绩：");
                scanf("%d", &eng);

                printf("请输入学生的数学成绩：");
                scanf("%d", &math);

                printf("请输入学生的语文成绩：");
                scanf("%d", &chi);

                grade[index][0] = num;
                grade[index][1] = eng;
                grade[index][2] = math;
                grade[index][3] = chi;

                printf("学生信息录入成功！\n");
                break;

            case 2:
                printf("修改学生成绩\n");
                printf("请输入需要修改的学生学号：");
                scanf("%d", &num);

                index = -1;

                for (i = 0; i < 5; i++)
                {
                    if (grade[i][0] == num && num > 0)
                    {
                        index = i;
                        break;
                    }
                }

                if (index == -1)
                {
                    printf("未找到该学生！\n");
                    break;
                }

                printf("请输入新的英语成绩：");
                scanf("%d", &grade[index][1]);

                printf("请输入新的数学成绩：");
                scanf("%d", &grade[index][2]);

                printf("请输入新的语文成绩：");
                scanf("%d", &grade[index][3]);

                printf("学生成绩修改成功！\n");
                break;

            case 3:
                printf("删除学生信息\n");
                printf("请输入需要删除的学生学号：");
                scanf("%d", &num);

                index = -1;

                for (i = 0; i < 5; i++)
                {
                    if (grade[i][0] == num && num > 0)
                    {
                        index = i;
                        break;
                    }
                }

                if (index == -1)
                {
                    printf("未找到该学生！\n");
                    break;
                }

                for (j = 0; j < 4; j++)
                {
                    grade[index][j] = 0;
                }

                printf("学生信息删除成功！\n");
                break;

            case 4:
                printf("查询学生信息\n");
                printf("请输入需要查询的学生学号：");
                scanf("%d", &num);

                index = -1;

                for (i = 0; i < 5; i++)
                {
                    if (grade[i][0] == num && num > 0)
                    {
                        index = i;
                        break;
                    }
                }

                if (index == -1)
                {
                    printf("未找到该学生！\n");
                    break;
                }

                printf("学号：%d\n", grade[index][0]);
                printf("英语：%d\n", grade[index][1]);
                printf("数学：%d\n", grade[index][2]);
                printf("语文：%d\n", grade[index][3]);
                break;

            case 5:
                printf("系统已退出，再见！\n");
                return 0;

            default:
                printf("请重新输入1~5\n");
                break;
        }
    }

    return 0;
}
