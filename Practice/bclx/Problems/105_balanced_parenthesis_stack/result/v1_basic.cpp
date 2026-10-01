// 105 — Balanced Parenthesis Problem | v1: stack check
// Given a string of parentheses, check whether it is balanced (the stack
// version — 68 generates all balanced combinations).
// Run: make run PROB=105_balanced_parenthesis_stack SRC=result/v1_basic.cpp NP=4 ARGS="(()())"
// (also try "(()" and "())(")

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stack>
#include <bclx/bclx.hpp>

struct str_msg {
    char s[80];
    uint64_t len;
};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <parenthesis string>\n", argv[0]);
        return 1;
    }

    BCL::init();

    str_msg m{};
    m.len = 0;
    if (BCL::rank() == 0) {
        strncpy(m.s, argv[1], sizeof(m.s) - 1);
        m.len = strlen(m.s);
    }
    m = BCL::broadcast(m, 0);

    if (BCL::rank() == 0) {
        std::stack<char> st;
        bool balanced = true;
        for (uint64_t i = 0; i < m.len && balanced; ++i) {
            if (m.s[i] == '(') {
                st.push('(');
            } else if (m.s[i] == ')') {
                if (st.empty()) balanced = false;
                else st.pop();
            } else {
                balanced = false;   // only parentheses are allowed here
            }
        }
        balanced = balanced && st.empty();

        printf("\"%s\" is %sbalanced (stack check)\n", m.s, balanced ? "" : "NOT ");
    }

    BCL::finalize();
    return 0;
}
