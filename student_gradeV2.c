
#include <stdio.h>

#define N 5

int grade[N][4] = {0};
int used[N] = {0};

// 统一处理整数输入
int read_num(const char *prompt, int *value)
{
    char line[128], extra;
    while (1)
    {
        printf("%s", prompt);
        if (!fgets(line, sizeof(line), stdin)) return 0;
        if (sscanf(line, " %d %c", value, &extra) == 1) return 1;
        printf("请输入有效整数！\n");
    }
}

// 检查成绩范围
int read_score(const char *prompt, int *score)
{
    while (read_num(prompt, score))
    {
        if (*score >= 0 && *score <= 100) return 1;
        printf("成绩必须在0到100之间！\n");
    }
    return 0;
}

// 选择学生
int select_student(void)
{
    int y;
    if (!read_num("请输入第几个学生（1-5）：", &y)) return -1;
    if (y < 1 || y > N)
    {
        printf("学生编号必须在1到5之间！\n");
        return -1;
    }
    return y - 1;
}

// 录入和查询共用的范围选择
int select_range(const char *action, int *start, int *end)
{
    int mode, i;
    printf("%s全部学生还是一个学生（1=全部，2=一个）：", action);
    if (!read_num("", &mode)) return 0;

    if (mode == 1)
    {
        *start = 0;
        *end = N;
        return 1;
    }

    if (mode == 2)
    {
        i = select_student();
        if (i < 0) return 0;
        *start = i;
        *end = i + 1;
        return 1;
    }

    printf("只能选择1或2！\n");
    return 0;
}

// 录入一个学生的信息
void input_one(int i)
{
    const char *prompts[4] = {
        "请输入学生学号：", "请输入英语成绩：",
        "请输入数学成绩：", "请输入语文成绩："
    };

    int values[4], j, ok;
    printf("\n正在录入第%d个学生\n", i + 1);

    for (j = 0; j < 4; j++)
    {
        if (j == 0) ok = read_num(prompts[j], &values[j]);
        else ok = read_score(prompts[j], &values[j]);
        if (!ok) return;
    }

    for (j = 0; j < 4; j++) grade[i][j] = values[j];
    used[i] = 1;
    printf("第%d个学生录入成功！\n", i + 1);
}

// 合并录入与查询
void input_or_query(int action)
{
    int start, end, i;

    if (!select_range(action == 1 ? "录入" : "查询",
                      &start, &end)) return;

    for (i = start; i < end; i++)
    {
        if (action == 1) input_one(i);
        else if (!used[i])
            printf("第%d个学生：暂无记录\n", i + 1);
        else
            printf("第%d个学生：学号%d  英语%d  数学%d  语文%d\n",
                   i + 1, grade[i][0], grade[i][1],
                   grade[i][2], grade[i][3]);
    }
}

// 合并修改与删除
void modify_or_delete(int action)
{
    int i = select_student(), j, subject, score;
    if (i < 0) return;

    if (!used[i])
    {
        printf("该学生尚未录入！\n");
        return;
    }

    if (action == 3)
    {
        for (j = 0; j < 4; j++) grade[i][j] = 0;
        used[i] = 0;
        printf("第%d个学生的信息已删除！\n", i + 1);
        return;
    }

    if (!read_num("修改科目（1=英语，2=数学，3=语文）：",
                  &subject)) return;

    if (subject < 1 || subject > 3)
    {
        printf("科目选择错误！\n");
        return;
    }

    if (!read_score("请输入新成绩：", &score)) return;
    grade[i][subject] = score;
    printf("成绩修改成功！\n");
}

// 主函数
int main(void)
{
    int choice;

    while (1)
    {
        printf("\n==========学生成绩管理系统==========\n"
               "1.录入学生成绩\n2.修改学生成绩\n"
               "3.删除学生成绩\n4.查询学生成绩\n"
               "5.退出系统\n====================================\n");

        if (!read_num("请输入选择的功能（1-5）：", &choice))
            break;

        switch (choice)
        {
            case 1: case 4: input_or_query(choice); break;
            case 2: case 3: modify_or_delete(choice); break;
            case 5: printf("系统已退出！\n"); return 0;
            default: printf("请输入1到5之间的数字！\n");
        }
    }

    return 0;
}
