#include "parser.h"

void whitespaces(int *iter, char* str)
{
    while(str[*iter] == ' ') {
        (*iter)++;
        continue;
    }
}

int parse_node(struct node *n, int *iter, char *str) {
    n->matcher = EQUAL;

    if(str[(*iter)++] == '<') {
        if(str[*iter] == '='){
            // lesser or equal
            n->matcher = LESSER_OR_EQUAL;
            (*iter)++;
        } else {
            // lesser
            n->matcher = LESSER;
        }
    } else if(str[*iter++] == '>') {
        if(str[*iter] == '='){
            // greater or equal
            n->matcher = GREATER_OR_EQUAL;
            (*iter)++;
        } else {
            // greater
            n->matcher = GREATER;
        }
    }

    whitespaces(iter, str);

    if(str[*iter] >= '0' && str[*iter] <= '9') {
        //integer
        n->type = INTEGER;
        n->value = 0;
        while(str[*iter] >= '0' && str[*iter] <= '9') {
            n->value = n->value * 10;
            n->value += str[*iter] - '0';
            (*iter)++;
        }
    } else if(str[*iter] == '\"') {
        //string
        int i = -1;
        n->type = STRING;
        do {
            (*iter)++;
            i++;
            if(str[*iter] == '\0') {
                return 0;
            }
        } while (str[*iter] != '\"')

        n->value = (char*)malloc(i);
        strncpy(n->value, str + iter - i, i);
    } else if(str[*iter == '*']) {
        //any integer
        n->matcher = ANY_INTEGER;
        n->type = INTEGER;
    } else {
        return 0;
    }

    return 1;
}

struct parse_result parse(char* str) {
    int iter = 0;
    struct parse_result res;
    char word[7];

    whitespaces(&iter, str);

    //input / output / read
    switch(str[iter]) {
        case 'i':
            strcpy(word, "input");
            res.operation = INPUT;
            break;
        case 'o':
            strcpy(word, "output");
            res.operation = OUTPUT;
            break;
        case 'r':
            strcpy(word, "read");
            res.operation = READ;
            break;
    }

    if(strncmp(str, word, strlen(word)) != 0) {
        res.error = iter + 1;
        return res;
    }

    iter+= strlen(word);

    whitespaces(&iter, str);

    // left bracket (
    if(str[iter] != '(') {
        res.error = iter + 1;
        return res;
    }

    whitespaces(&iter, str);
    //tuples
    res.tuple = make_tuple();
    struct node n;
    int i = 0;

    while (i < 5 && str[iter] != ')') {
        // tuple
        if(parse_node(&n, &iter, str) != 0) {
            res.error = iter + 1;
            return res;
        }

        tuple_append(&res.tuple, node);
        whitespaces(&iter, str);
        //comma
        if(str[iter] != ',') {
            break;
        }
    }
    whitespaces(&iter, str);

    // right bracket )
    if(str[iter] != ')') {
        res.error = iter + 1;
        return res;
    }
    whitespaces(&iter, str);

    // number (timeout)
    res.timeout_ms = 0;
    if(str[iter] >= '0' && str[*iter] <= '9') {
        while(str[iter] >= '0' && str[iter] <= '9') {
            res.timeout_ms *= 10;
            res.timeout_ms += str[*iter] - '0';
            (*iter)++;
        }
    } else {
        res.error = iter + 1;
        return res;
    }

    whitespaces(&iter, str);
    if(str[iter] == '\0') {
        res.error = 0;
    } else {
        res.error = 1;
    }
    return res;
}
