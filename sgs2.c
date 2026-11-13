#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILE_NAME "file.txt"
typedef struct {
    int id;
    char name[128];                 // UTF-8姓名
    int chinese, math, english, total;
} Student;
Student *students = NULL;
int count = 0, capacity = 0, sort_by = 0;
int ask(const char *tip, int *value)
{
    char line[128];
    printf("%s", tip);
    return fgets(line, sizeof(line), stdin) && sscanf(line, "%d", value) == 1;
}
int read_score(const char *tip, int *score)
{
    do {
        if (!ask(tip, score)) return 0;
        if (*score >= 0 && *score <= 100) return 1;
        puts("成绩必须在0到100之间！");
    } while (1);
}
int find(int id)
{
    for (int i = 0; i < count; i++)
        if (students[i].id == id) return i;
    return -1;
}
int append(Student s)
{
    if (count == capacity) {
        int next = capacity ? capacity * 2 : 8;
        Student *p = realloc(students, (size_t)next * sizeof(Student));
        if (!p) { puts("内存不足！"); return 0; }
        students = p;
        capacity = next;
    }
    students[count++] = s;
    return 1;
}
int save(void)
{
    FILE *f = fopen(FILE_NAME, "w");
    if (!f) { perror("文件保存失败"); return 0; }
    for (int i = 0; i < count; i++) {
        Student s = students[i];
        fprintf(f, "%d\t%s\t%d\t%d\t%d\t%d\n", s.id, s.name,
                s.chinese, s.math, s.english, s.total);
    }
    return fclose(f) == 0;
}
void load(void)
{
    FILE *f = fopen(FILE_NAME, "r");
    if (!f) { save(); return; }
    char line[512];
    while (fgets(line, sizeof(line), f)) {
        Student s = {0};
        int flag, id, eng, math, chi;
        if (sscanf(line, "%d\t%127[^\t]\t%d\t%d\t%d\t%d",
                   &s.id, s.name, &s.chinese, &s.math, &s.english, &s.total) != 6) {
            strcpy(s.name, "未命名");
            if (sscanf(line, "%d %d %d %d %d", &flag, &id, &eng, &math, &chi) == 5) {
                if (flag != 1) continue;
                s.id = id;
            } else if (sscanf(line, "%d %d %d %d", &id, &eng, &math, &chi) == 4) {
                s.id = id;
            } else continue;
            s.chinese = chi;
            s.math = math;
            s.english = eng;
        }
        s.total = s.chinese + s.math + s.english;
        if (s.id > 0 && find(s.id) < 0) append(s);
    }
    fclose(f);
    printf("已读取%d名学生。\n", count);
}
void print_one(Student s)
{
    printf("学号：%d  姓名：%s  语文：%d  数学：%d  英语：%d  总分：%d\n",
           s.id, s.name, s.chinese, s.math, s.english, s.total);
}
void input(void)
{
    int n, mode;
    if (!ask("1.批量录入  2.单人录入：", &mode)) return;
    if (mode != 1 && mode != 2) return;
    n = 1;
    if (mode == 1 && (!ask("录入人数：", &n) || n < 1)) return;
    for (int i = 0; i < n; i++) {
        Student s = {0};
        if (!ask("学号：", &s.id)) return;
        if (s.id <= 0 || find(s.id) >= 0) { puts("学号无效或重复！"); continue; }
        printf("姓名：");
        if (!fgets(s.name, sizeof(s.name), stdin)) return;
        s.name[strcspn(s.name, "\r\n")] = 0;
        if (!s.name[0]) { puts("姓名不能为空！"); continue; }
        if (!read_score("语文：", &s.chinese) ||
            !read_score("数学：", &s.math) ||
            !read_score("英语：", &s.english)) return;
        s.total = s.chinese + s.math + s.english;
        if (!append(s)) return;
        save();
        puts("录入成功！");
    }
}
void edit_or_delete(int action)
{
    int id, i, op, x;
    if (!ask("请输入学号：", &id)) return;
    i = find(id);
    if (i < 0) { puts("未找到该学生！"); return; }
    if (action == 3) {
        if (i < count - 1)
            memmove(students + i, students + i + 1,
                    (size_t)(count - i - 1) * sizeof(Student));
        count--;
    } else {
        if (!ask("修改：1.学号 2.姓名 3.语文 4.数学 5.英语：", &op)) return;
        switch (op) {
            case 1:
                if (!ask("新学号：", &x)) return;
                if (x <= 0 || (find(x) >= 0 && x != id)) { puts("学号无效或重复！"); return; }
                students[i].id = x;
                break;
            case 2: {
                char name[128];
                printf("新姓名：");
                if (!fgets(name, sizeof(name), stdin)) return;
                name[strcspn(name, "\r\n")] = 0;
                if (!name[0]) { puts("姓名不能为空！"); return; }
                strcpy(students[i].name, name);
                break;
            }
            case 3: case 4: case 5:
                if (!read_score("新成绩：", &x)) return;
                if (op == 3) students[i].chinese = x;
                if (op == 4) students[i].math = x;
                if (op == 5) students[i].english = x;
                break;
            default: return;
        }
        students[i].total = students[i].chinese + students[i].math + students[i].english;
    }
    if (save()) puts("操作成功，已保存！");
}
void query(void)
{
    int mode, id;
    if (!count) { puts("暂无记录！"); return; }
    if (!ask("1.查询全部  2.按学号查询：", &mode)) return;
    if (mode == 1)
        for (int i = 0; i < count; i++) print_one(students[i]);
    else if (mode == 2) {
        if (!ask("学号：", &id)) return;
        int i = find(id);
        if (i >= 0) print_one(students[i]);
        else puts("未找到该学生！");
    }
}
int score_of(const Student *s)
{
    switch (sort_by) {
        case 1: return s->chinese;
        case 2: return s->math;
        case 3: return s->english;
        default: return s->total;
    }
}
int compare(const void *a, const void *b)
{
    const Student *x = a, *y = b;
    int sx = score_of(x), sy = score_of(y);
    if (sx != sy) return sx < sy ? 1 : -1;
    return (x->id > y->id) - (x->id < y->id);
}
void sort_students(void)
{
    if (!count) { puts("暂无记录！"); return; }
    if (!ask("排序：1.语文 2.数学 3.英语 4.总分：", &sort_by)) return;
    if (sort_by < 1 || sort_by > 4) return;
    Student *copy = malloc((size_t)count * sizeof(Student));
    if (!copy) return;
    memcpy(copy, students, (size_t)count * sizeof(Student));
    qsort(copy, count, sizeof(Student), compare);
    for (int i = 0, rank = 0; i < count; i++) {
        if (!i || score_of(&copy[i]) != score_of(&copy[i - 1])) rank = i + 1;
        printf("第%d名  ", rank);
        print_one(copy[i]);
    }
    free(copy);
}
int main(void)
{
    int choice;
    load();
    while (1) {
        puts("\n=====学生成绩管理系统=====\n"
             "1.录入  2.修改  3.删除\n"
             "4.查询  5.排序  6.退出\n========================");
        if (!ask("请选择：", &choice)) break;
        switch (choice) {
            case 1: input(); break;
            case 2: case 3: edit_or_delete(choice); break;
            case 4: query(); break;
            case 5: sort_students(); break;
            case 6: puts("已退出！"); free(students); return 0;
            default: puts("请输入1到6！");
        }
    }
    free(students);
    return 0;
}
