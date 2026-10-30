#include <stdio.h>

#define STUDENTS 5

int grade[STUDENTS][4] = {0};
int has_record[STUDENTS] = {0};

// 读取整数，避免非数字输入导致程序卡住
int read_int(const char *message, int *value)
{
    char line[128];
    char extra;

    while (1)
    {
        printf("%s", message);
        if (fgets(line, sizeof(line), stdin) == NULL)
            return 0;

        if (sscanf(line, " %d %c", value, &extra) == 1)
            return 1;

        printf("请输入有效的整数！\n");
    }
}

int read_score(const char *message, int *value)
{
    while (1)
    {
        if (!read_int(message, value))
            return 0;
        if (*value >= 0 && *value <= 100)
            return 1;
        printf("成绩必须在0到100之间！\n");
    }
}

void menu(void)
{
    printf("\n==========学生成绩管理系统==========\n");
    printf("1.录入学生成绩\n");
    printf("2.修改学生成绩\n");
    printf("3.删除学生成绩\n");
    printf("4.查询学生成绩\n");
    printf("5.退出系统\n");
    printf("====================================\n");
}

void input_one(int i)
{
    int num, eng, math, chi;

    printf("\n正在录入第%d个学生\n", i + 1);
    if (!read_int("请输入学生学号：", &num)) return;
    if (!read_score("请输入英语成绩：", &eng)) return;
    if (!read_score("请输入数学成绩：", &math)) return;
    if (!read_score("请输入语文成绩：", &chi)) return;

    grade[i][0] = num;
    grade[i][1] = eng;
    grade[i][2] = math;
    grade[i][3] = chi;
    has_record[i] = 1;
    printf("第%d个学生录入成功！\n", i + 1);
}

void input_score(void)
{
    int x, y, i;
    if (!read_int("录入全部学生还是一个学生（1=全部，2=一个）：", &x)) return;

    if (x == 1)
    {
        for (i = 0; i < STUDENTS; i++)
            input_one(i);
    }
    else if (x == 2)
    {
        if (!read_int("请输入第几个学生（1-5）：", &y)) return;
        if (y < 1 || y > STUDENTS)
        {
            printf("学生编号必须在1到5之间！\n");
            return;
        }
        input_one(y - 1);
    }
    else
        printf("录入模式只能选择1或2！\n");
}

void modify_score(void)
{
    int y, subject, new_score;
    if (!read_int("请输入第几个学生（1-5）：", &y)) return;
    if (y < 1 || y > STUDENTS || !has_record[y - 1])
    {
        printf("该学生编号无效或尚未录入！\n");
        return;
    }

    if (!read_int("修改科目（1=英语，2=数学，3=语文）：", &subject)) return;
    if (subject < 1 || subject > 3)
    {
        printf("科目选择错误！\n");
        return;
    }
    if (!read_score("请输入新成绩：", &new_score)) return;

    grade[y - 1][subject] = new_score;
    printf("成绩修改成功！\n");
}

void delete_score(void)
{
    int y, j;
    if (!read_int("请输入要删除的第几个学生（1-5）：", &y)) return;
    if (y < 1 || y > STUDENTS || !has_record[y - 1])
    {
        printf("该学生编号无效或尚未录入！\n");
        return;
    }

    for (j = 0; j < 4; j++)
        grade[y - 1][j] = 0;
    has_record[y - 1] = 0;
    printf("第%d个学生的信息已删除！\n", y);
}

void print_student(int i)
{
    if (!has_record[i])
    {
        printf("第%d个学生：暂无记录\n", i + 1);
        return;
    }
    printf("第%d个学生：学号%d  英语%d  数学%d  语文%d\n",
           i + 1, grade[i][0], grade[i][1], grade[i][2], grade[i][3]);
}

void query_score(void)
{
    int x, y, i;
    if (!read_int("查询全部学生还是一个学生（1=全部，2=一个）：", &x)) return;

    if (x == 1)
    {
        for (i = 0; i < STUDENTS; i++)
            print_student(i);
    }
    else if (x == 2)
    {
        if (!read_int("请输入第几个学生（1-5）：", &y)) return;
        if (y < 1 || y > STUDENTS)
            printf("学生编号必须在1到5之间！\n");
        else
            print_student(y - 1);
    }
    else
        printf("查询模式只能选择1或2！\n");
}

int main(void)
{
    int choice;
    while (1)
    {
        menu();
        if (!read_int("请输入选择的功能（1-5）：", &choice))
            break;
        switch (choice)
        {
            case 1: input_score();  break;
            case 2: modify_score(); break;
            case 3: delete_score(); break;
            case 4: query_score();  break;
            case 5: printf("系统已退出！\n"); return 0;
            default: printf("请输入1到5之间的数字！\n"); break;
        }
    }
    return 0;
}
