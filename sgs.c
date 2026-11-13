#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "file.txt"

typedef struct {
    int id, score[3];                 // 学号、英语、数学、语文
} Student;

Student *a = NULL;
int count = 0, cap = 0, sort_subject;
const char *subject[3] = {"英语", "数学", "语文"};

int read_int(const char *tip, int *x)
{
    char line[128], extra;
    while (1) {
        printf("%s", tip);
        if (!fgets(line, sizeof(line), stdin)) return 0;
        if (sscanf(line, " %d %c", x, &extra) == 1) return 1;
        puts("请输入整数！");
    }
}

int read_score(const char *tip, int *x)
{
    while (read_int(tip, x)) {
        if (*x >= 0 && *x <= 100) return 1;
        puts("成绩必须在0到100之间！");
    }
    return 0;
}

int find(int id)
{
    for (int i = 0; i < count; i++)
        if (a[i].id == id) return i;
    return -1;
}

int add(Student s)
{
    if (count == cap) {
        int next = cap ? cap * 2 : 8;
        Student *p = realloc(a, (size_t)next * sizeof(Student));
        if (!p) { puts("内存不足！"); return 0; }
        a = p;
        cap = next;
    }
    a[count++] = s;
    return 1;
}

void save(void)
{
    FILE *fp = fopen(FILE_NAME, "w");
    if (!fp) { perror("无法保存文件"); return; }
    for (int i = 0; i < count; i++)
        fprintf(fp, "%d %d %d %d\n", a[i].id,
                a[i].score[0], a[i].score[1], a[i].score[2]);
    fclose(fp);
}

void load(void)
{
    FILE *fp = fopen(FILE_NAME, "r");
    if (!fp) { save(); return; }
    char line[128];
    while (fgets(line, sizeof(line), fp)) {
        int x, y, z, w, v;
        int n = sscanf(line, "%d %d %d %d %d", &x, &y, &z, &w, &v);
        Student s;
        if (n == 5) {                   // 兼容原版：存在标记 学号 英语 数学 语文
            if (x != 1) continue;
            s = (Student){y, {z, w, v}};
        } else if (n == 4)
            s = (Student){x, {y, z, w}};
        else continue;
        if (s.id > 0 && find(s.id) < 0 &&
            s.score[0] >= 0 && s.score[0] <= 100 &&
            s.score[1] >= 0 && s.score[1] <= 100 &&
            s.score[2] >= 0 && s.score[2] <= 100 && !add(s)) break;
    }
    fclose(fp);
    printf("已读取 %d 名学生。\n", count);
}

void input_one(void)
{
    Student s;
    if (!read_int("学号：", &s.id)) return;
    if (s.id <= 0 || find(s.id) >= 0) { puts("学号无效或重复！"); return; }
    for (int k = 0; k < 3; k++) {
        char tip[40];
        snprintf(tip, sizeof(tip), "%s成绩（0-100）：", subject[k]);
        if (!read_score(tip, &s.score[k])) return;
    }
    if (add(s)) { save(); puts("已录入并保存！"); }
}

void input(void)
{
    int mode, n = 1;
    if (!read_int("1.批量录入  2.单人录入：", &mode)) return;
    if (mode == 1) {
        if (!read_int("录入人数：", &n)) return;
        if (n <= 0) return;
    } else if (mode != 2) return;
    for (int i = 0; i < n; i++) input_one();
}

void edit(int action)
{
    int id, i, col, x;
    if (!read_int("请输入学生学号：", &id)) return;
    i = find(id);
    if (i < 0) { puts("未找到该学生！"); return; }
    if (action == 3) {                // 删除：后续记录前移
        memmove(a + i, a + i + 1, (size_t)(count - i - 1) * sizeof(Student));
        count--;
    } else {                         // 修改学号或某科成绩
        if (!read_int("0.学号  1.英语  2.数学  3.语文：", &col)) return;
        if (col < 0 || col > 3) return;
        if (col == 0) {
            if (!read_int("新学号：", &x)) return;
            if (x <= 0 || (find(x) >= 0 && x != id)) {
                puts("学号无效或重复！"); return;
            }
            a[i].id = x;
        } else {
            if (!read_score("新成绩：", &x)) return;
            a[i].score[col - 1] = x;
        }
    }
    save();
    puts(action == 3 ? "已删除并保存！" : "已修改并保存！");
}

int compare(const void *p, const void *q)
{
    const Student *x = p, *y = q;
    if (x->score[sort_subject] != y->score[sort_subject])
        return x->score[sort_subject] < y->score[sort_subject] ? 1 : -1;
    return (x->id > y->id) - (x->id < y->id);
}

void query(void)
{
    int mode, id;
    if (!count) { puts("暂无学生记录！"); return; }
    if (!read_int("1.全部科目排名  2.查询单人：", &mode)) return;
    if (mode == 2) {
        if (!read_int("学号：", &id)) return;
        int i = find(id);
        if (i < 0) { puts("未找到该学生！"); return; }
        printf("学号：%d\n", a[i].id);
        for (int k = 0; k < 3; k++) {
            int rank = 1;
            for (int j = 0; j < count; j++)
                if (a[j].score[k] > a[i].score[k]) rank++;
            printf("%s：%d分（第%d名）\n", subject[k], a[i].score[k], rank);
        }
    } else if (mode == 1) {
        Student *order = malloc((size_t)count * sizeof(Student));
        if (!order) return;
        for (int k = 0; k < 3; k++) {
            memcpy(order, a, (size_t)count * sizeof(Student));
            sort_subject = k;
            qsort(order, count, sizeof(Student), compare);
            printf("\n===== %s成绩排名（从高到低）=====\n", subject[k]);
            puts("名次\t学号\t成绩");
            int rank = 0;
            for (int i = 0; i < count; i++) {
                if (!i || order[i].score[k] != order[i - 1].score[k]) rank = i + 1;
                printf("%d\t%d\t%d\n", rank, order[i].id, order[i].score[k]);
            }
        }
        free(order);
    } else puts("请选择1或2！");
}

int main(void)
{
    int choice;
    load();
    while (1) {
        puts("\n==========学生成绩管理系统=========="
             "\n1.录入学生成绩\n2.修改学生信息（含学号）"
             "\n3.删除学生信息\n4.查询成绩及排名"
             "\n5.退出系统\n====================================");
        if (!read_int("请选择功能（1-5）：", &choice)) break;
        switch (choice) {
            case 1: input(); break;
            case 2: case 3: edit(choice); break;
            case 4: query(); break;
            case 5: puts("系统已退出！"); free(a); return 0;
            default: puts("请输入1到5之间的数字！");
        }
    }
    free(a);
    return 0;
}
