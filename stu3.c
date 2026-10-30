
#include <stdio.h>

#define MAX_STUDENTS 100

// 读取整数，防止输入字母造成异常
int readInt(const char *prompt, int *value)
{
    char line[128];
    char extra;

    while (1)
    {
        printf("%s", prompt);

        if (fgets(line, sizeof(line), stdin) == NULL)
            return 0;

        if (sscanf(line, " %d %c", value, &extra) == 1)
            return 1;

        printf("输入错误，请输入整数！\n");
    }
}

// 检查成绩是否在0到100之间
int readScore(const char *prompt, int *score)
{
    while (1)
    {
        if (!readInt(prompt, score))
            return 0;

        if (*score >= 0 && *score <= 100)
            return 1;

        printf("成绩必须在0到100之间！\n");
    }
}

// 根据学号寻找学生
int findStudent(int grade[][4], int count, int id)
{
    int i;

    for (i = 0; i < count; i++)
    {
        if (grade[i][0] == id)
            return i;
    }

    return -1;
}

int main(void)
{
    // 二维数组：100名学生，每人4项信息
    int grade[MAX_STUDENTS][4] = {0};

    int count = 0;
    int choice, id, index;
    int score, subject;
    int eng, math, chi;
    int i, j;

    // 主菜单循环
    while (1)
    {
        printf("\n");
        printf("========== 学生成绩管理系统 ==========\n");
        printf("1. 录入学生成绩\n");
        printf("2. 修改学生成绩\n");
        printf("3. 删除学生成绩\n");
        printf("4. 查询学生成绩\n");
        printf("5. 退出系统\n");
        printf("======================================\n");

        if (!readInt("请选择功能（1-5）：", &choice))
            return 0;

        // 根据用户选择执行对应功能
        switch (choice)
        {
            // 功能1：录入学生成绩
            case 1:
            {
                if (count >= MAX_STUDENTS)
                {
                    printf("学生人数已达上限！\n");
                    break;
                }

                if (!readInt("请输入学生学号（正整数）：", &id))
                    return 0;

                if (id <= 0)
                {
                    printf("学号必须是正整数！\n");
                    break;
                }

                if (findStudent(grade, count, id) != -1)
                {
                    printf("该学号已经存在，不能重复录入！\n");
                    break;
                }

                if (!readScore("请输入英语成绩（0-100）：", &eng))
                    return 0;

                if (!readScore("请输入数学成绩（0-100）：", &math))
                    return 0;

                if (!readScore("请输入语文成绩（0-100）：", &chi))
                    return 0;

                // 将学生信息存入数组
                grade[count][0] = id;
                grade[count][1] = eng;
                grade[count][2] = math;
                grade[count][3] = chi;

                count++;

                printf("录入成功！当前共有%d名学生。\n", count);
                break;
            }

            // 功能2：修改学生成绩
            case 2:
            {
                if (!readInt("请输入要修改的学生学号：", &id))
                    return 0;

                index = findStudent(grade, count, id);

                if (index == -1)
                {
                    printf("未找到该学号！\n");
                    break;
                }

                printf("1. 英语\n");
                printf("2. 数学\n");
                printf("3. 语文\n");

                if (!readInt("请选择要修改的科目（1-3）：", &subject))
                    return 0;

                if (subject < 1 || subject > 3)
                {
                    printf("科目选择无效！\n");
                    break;
                }

                if (!readScore("请输入新的成绩（0-100）：", &score))
                    return 0;

                grade[index][subject] = score;

                printf("修改成功！\n");
                break;
            }

            // 功能3：删除学生成绩
            case 3:
            {
                if (!readInt("请输入要删除的学生学号：", &id))
                    return 0;

                index = findStudent(grade, count, id);

                if (index == -1)
                {
                    printf("未找到该学号！\n");
                    break;
                }

                // 后面的学生依次向前移动
                for (i = index; i < count - 1; i++)
                {
                    for (j = 0; j < 4; j++)
                    {
                        grade[i][j] = grade[i + 1][j];
                    }
                }

                count--;

                printf("删除成功！当前共有%d名学生。\n", count);
                break;
            }

            // 功能4：查询学生成绩
            case 4:
            {
                if (!readInt("请输入要查询的学生学号：", &id))
                    return 0;

                index = findStudent(grade, count, id);

                if (index == -1)
                {
                    printf("未找到该学号！\n");
                    break;
                }

                printf("\n========== 学生成绩 ==========\n");
                printf("学号：%d\n", grade[index][0]);
                printf("英语：%d\n", grade[index][1]);
                printf("数学：%d\n", grade[index][2]);
                printf("语文：%d\n", grade[index][3]);
                printf("==============================\n");

                break;
            }

            // 功能5：退出系统
            case 5:
            {
                printf("已退出学生成绩管理系统！\n");
                return 0;
            }

            // 错误输入
            default:
            {
                printf("请输入1到5之间的数字！\n");
                break;
            }
        }
    }
}
