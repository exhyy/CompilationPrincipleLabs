#ifndef LAB02_H_
#define LAB02_H_

#include <cstdio>
#include <string>

#define EOI 0
#define EQ 1
#define LP 2
#define RP 3
#define ADD 4
#define SUB 5
#define MUL 6
#define DIV 7
#define ID 8

class Lab02
{
private:
    std::string lookahead = "";
    FILE *fp = nullptr;
    bool pass;

public:
    Lab02(std::string filename);
    void S();
    void E();
    void E_prime();
    void T();
    void T_prime();
    void F();
    void V();
    void advance();
    bool match(int word_id);
    void run();
};

void S(FILE *fp);
void E(FILE *fp);
void E_prime(FILE *fp);
void T(FILE *fp);
void T_prime(FILE *fp);
void F(FILE *fp);
void V(FILE *fp);
void advance(FILE *fp);
bool match(int word_id);

#endif