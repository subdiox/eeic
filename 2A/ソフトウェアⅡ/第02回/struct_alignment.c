/*
    int型は4バイト，double型は8バイトと4の倍数になっているが，char型は1バイトなので4バイトにアラインメントされる．
    また，char[100]の型は64バイトにアラインメントされているが，これは100以下で最も大きい2のn乗のバイト数が選ばれていると考えられる．
*/

#include <stdio.h>

struct student
{
    int id;
    char name[100];
    int age;
    double height;
    double weight;
};

struct tagged_student1
{
    int id;
    char name[100];
    int age;
    double height;
    double weight;
    char tag;
};

struct tagged_student2
{
    char tag;
    int id;
    char name[100ｓ];
    int age;
    double height;
    double weight;
};

int main (int argc, char**argv)
{
    struct student s;
    struct tagged_student1 s1;
    struct tagged_student2 s2;

    printf("student: %p, %lu\n", &s, sizeof(s));
    printf("{\n");
    printf("    id: %p, %lu\n", &s.id, sizeof(s.id));
    printf("    name: %p, %lu\n", &s.name, sizeof(s.name));
    printf("    age: %p, %lu\n", &s.age, sizeof(s.age));
    printf("    height: %p, %lu\n", &s.height, sizeof(s.height));
    printf("    weight: %p, %lu\n", &s.weight, sizeof(s.weight));
    printf("}\n");
    printf("\n");
    printf("tagged_student1: %p, %lu\n", &s1, sizeof(s1));
    printf("{\n");
    printf("    id: %p, %lu\n", &s1.id, sizeof(s1.id));
    printf("    name: %p, %lu\n", &s1.name, sizeof(s1.name));
    printf("    age: %p, %lu\n", &s1.age, sizeof(s1.age));
    printf("    height: %p, %lu\n", &s1.height, sizeof(s1.height));
    printf("    weight: %p, %lu\n", &s1.weight, sizeof(s1.weight));
    printf("    tag: %p, %lu\n", &s1.tag, sizeof(s1.tag));
    printf("}\n");
    printf("\n");
    printf("tagged_student2: %p, %lu\n", &s2, sizeof(s2));
    printf("{\n");
    printf("    tag: %p, %lu\n", &s2.tag, sizeof(s2.tag));
    printf("    id: %p, %lu\n", &s2.id, sizeof(s2.id));
    printf("    name: %p, %lu\n", &s2.name, sizeof(s2.name));
    printf("    age: %p, %lu\n", &s2.age, sizeof(s2.age));
    printf("    height: %p, %lu\n", &s2.height, sizeof(s2.height));
    printf("    weight: %p, %lu\n", &s2.weight, sizeof(s2.weight));
    printf("}\n");

    return 0;
}
