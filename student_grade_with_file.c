#include <stdio.h>

#define STUDENTS 5
#define FILE_NAME "file.txt"

// 每行对应一名学生：学号、英语、数学、语文
int grade[STUDENTS][4] = {0};
int has_record[STUDENTS] = {0};

// 读取一个整数
int read_int(const char *prompt, int *value)
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

// 只接受 0~100 的成绩
int read_score(const char *prompt, int *score)
{
    while (read_int(prompt, score))
    {
        if (*score >= 0 && *score <= 100) return 1;
        printf("成绩必须在0到100之间！\n");
    }
    return 0;
}

// 保存所有记录：每行格式为 存在标记 学号 英语 数学 语文
int save_data(void)
{
    FILE *fp = fopen(FILE_NAME, "w");
    int i;
    if (!fp)
    {
        perror("无法打开 file.txt 进行保存");
        return 0;
    }

    for (i = 0; i < STUDENTS; i++)
    {
        if (fprintf(fp, "%d %d %d %d %d\n", has_record[i],
                    grade[i][0], grade[i][1], grade[i][2], grade[i][3]) < 0)
        {
            perror("写入 file.txt 失败");
            fclose(fp);
            return 0;
        }
    }
    if (fclose(fp) != 0)
    {
        perror("保存 file.txt 失败");
        return 0;
    }
    return 1;
}

// 启动时读取记录；文件不存在时自动创建
void load_data(void)
{
    FILE *fp = fopen(FILE_NAME, "r");
    int data[STUDENTS][5], i, j, ok = 1;

    if (!fp)
    {
        if (save_data()) printf("已创建 %s\n", FILE_NAME);
        return;
    }

    // 已经存在的空文件也视为首次使用
    if (fgetc(fp) == EOF)
    {
        fclose(fp);
        if (save_data()) printf("已初始化空的 %s\n", FILE_NAME);
        return;
    }
    rewind(fp);

    for (i = 0; i < STUDENTS; i++)
    {
        for (j = 0; j < 5; j++)
            if (fscanf(fp, "%d", &data[i][j]) != 1) ok = 0;
        if (!ok) break;
        if (data[i][0] != 0 && data[i][0] != 1) ok = 0;
        if (data[i][0] == 1 && (data[i][1] <= 0 ||
            data[i][2] < 0 || data[i][2] > 100 ||
            data[i][3] < 0 || data[i][3] > 100 ||
            data[i][4] < 0 || data[i][4] > 100)) ok = 0;
        if (!ok) break;
    }
    fclose(fp);
    if (!ok)
    {
        printf("file.txt 格式错误，未加载旧数据，请检查文件。\n");
        return;
    }

    for (i = 0; i < STUDENTS; i++)
    {
        has_record[i] = data[i][0];
        for (j = 0; j < 4; j++)
            grade[i][j] = data[i][j + 1];
    }
    printf("已从 %s 读取学生数据。\n", FILE_NAME);
}

// 检查学号：必须为正整数，而且不能与其他记录重复
int read_id(int current, int *id)
{
    int i;
    while (read_int("请输入学号：", id))
    {
        if (*id > 0)
        {
            for (i = 0; i < STUDENTS; i++)
                if (i != current && has_record[i] && grade[i][0] == *id)
                    break;
            if (i == STUDENTS) return 1;
        }
        printf("学号必须为不重复的正整数！\n");
    }
    return 0;
}

void menu(void)
{
    printf("\n==========学生成绩管理系统==========\n"
           "1.录入学生成绩\n"
           "2.修改学生成绩（含学号）\n"
           "3.删除学生成绩\n"
           "4.查询学生成绩\n"
           "5.退出系统\n"
           "====================================\n");
}

// 录入一名学生，全部四项输入成功后才提交
int input_one(int i)
{
    int values[4], j;
    const char *names[3] = {
        "请输入英语成绩：", "请输入数学成绩：", "请输入语文成绩："
    };
    printf("\n正在录入第%d个学生\n", i + 1);
    if (!read_id(i, &values[0])) return 0;
    for (j = 1; j < 4; j++)
        if (!read_score(names[j - 1], &values[j])) return 0;

    for (j = 0; j < 4; j++) grade[i][j] = values[j];
    has_record[i] = 1;
    if (save_data()) printf("第%d个学生录入成功，已保存！\n", i + 1);
    return 1;
}

void input_score(void)
{
    int mode, y, i;
    if (!read_int("录入全部学生还是一个学生（1=全部，2=一个）：", &mode)) return;
    if (mode == 1)
    {
        for (i = 0; i < STUDENTS; i++)
            if (!input_one(i)) break;
    }
    else if (mode == 2)
    {
        if (!read_int("请输入第几个学生（1-5）：", &y)) return;
        if (y < 1 || y > STUDENTS)
            printf("学生编号必须在1到5之间！\n");
        else
            input_one(y - 1);
    }
    else
        printf("录入模式只能选择1或2！\n");
}

void modify_score(void)
{
    int y, subject, new_value;
    if (!read_int("请输入第几个学生（1-5）：", &y)) return;
    if (y < 1 || y > STUDENTS || !has_record[y - 1])
    {
        printf("该学生编号无效或尚未录入！\n");
        return;
    }

    if (!read_int("修改内容（0=学号，1=英语，2=数学，3=语文）：", &subject)) return;
    if (subject < 0 || subject > 3)
    {
        printf("选择错误！\n");
        return;
    }
    if (subject == 0)
    {
        if (!read_id(y - 1, &new_value)) return;
    }
    else if (!read_score("请输入新成绩：", &new_value)) return;

    grade[y - 1][subject] = new_value;
    if (save_data()) printf("修改成功，已保存！\n");
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
    for (j = 0; j < 4; j++) grade[y - 1][j] = 0;
    has_record[y - 1] = 0;
    if (save_data()) printf("第%d个学生记录已删除，已保存！\n", y);
}

void print_student(int i)
{
    if (!has_record[i])
        printf("第%d个学生：暂无记录\n", i + 1);
    else
        printf("第%d个学生：学号%d  英语%d  数学%d  语文%d\n",
               i + 1, grade[i][0], grade[i][1], grade[i][2], grade[i][3]);
}

void query_score(void)
{
    int mode, y, i;
    if (!read_int("查询全部学生还是一个学生（1=全部，2=一个）：", &mode)) return;
    if (mode == 1)
        for (i = 0; i < STUDENTS; i++) print_student(i);
    else if (mode == 2)
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
    load_data(); // 每次启动先读取 file.txt
    while (1)
    {
        menu();
        if (!read_int("请输入选择的功能（1-5）：", &choice)) break;
        switch (choice)
        {
            case 1: input_score();  break;
            case 2: modify_score(); break;
            case 3: delete_score(); break;
            case 4: query_score();  break;
            case 5: puts("系统已退出！"); return 0;
            default: puts("请输入1到5之间的数字！");
        }
    }
    return 0;
}
