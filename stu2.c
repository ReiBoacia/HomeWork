
#include <stdio.h>

// 安全读取整数，避免输入字母造成程序异常
int inputInt(const char *prompt, int *value)
{
    char line[128];
    char extra;
    int number;

    while (1)
    {
        printf("%s", prompt);

        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            return 0;
        }

        if (sscanf(line, " %d %c", &number, &extra) == 1)
        {
            *value = number;
            return 1;
        }

        printf("输入错误，请输入整数！\n");
    }
}

int main(void)
{
    int a;
    int score = 0;
    int newScore;

    // 主菜单循环
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

        // 读取用户选择
        if (!inputInt("请输入你的选择（1-5）：", &a))
        {
            return 0;
        }

        printf("选择功能是：%d\n", a);

        // switch 多分支选择
        switch (a)
        {
            // 功能1：录入学生成绩
            case 1:
            {
                if (!inputInt("输入学生的成绩：", &newScore))
                {
                    return 0;
                }

                while (newScore < 0 || newScore > 100)
                {
                    if (!inputInt("请输入学生的成绩（0-100）：", &newScore))
                    {
                        return 0;
                    }
                }

                score = newScore;

                printf("学生的成绩是：%d\n", score);
                break;
            }

            // 功能2：修改学生成绩
            case 2:
            {
                if (!inputInt("修改学生的成绩为：", &newScore))
                {
                    return 0;
                }

                while (newScore < 0 || newScore > 100)
                {
                    if (!inputInt("请输入学生的成绩（0-100）：", &newScore))
                    {
                        return 0;
                    }
                }

                score = newScore;

                printf("学生修改后的成绩是：%d\n", score);
                break;
            }

            // 功能3：删除学生成绩
            case 3:
            {
                printf("需要删除学生的成绩是：%d\n", score);

                score = 0;

                printf("现在的学生成绩是：%d\n", score);
                break;
            }

            // 功能4：查询学生成绩
            case 4:
            {
                printf("查询学生的成绩是：%d\n", score);
                break;
            }

            // 功能5：退出系统
            case 5:
            {
                printf("已退出学生成绩管理系统！\n");
                return 0;
            }

            // 处理无效选项
            default:
            {
                printf("请输入1到5之间的数字！\n");
                break;
            }
        }
    }
}
