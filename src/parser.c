#include "parser.h"

void whitespaces(int *iter, char* str) {
    while(str[*iter] == ' ') {
        (*iter)++;
    }
}

void print_error(int iter, char* str) {
    printf("error: %s\n      ", str);
    for(int i = 0; i <= iter; i++)
        printf(" ");
    printf("^\n");
}

int parse_node(struct node *n, int *iter, char *str) {
    n->matcher = EQUAL;

    if(str[*iter] == '<') {
        (*iter)++;
        if(str[*iter] == '='){
            // lesser or equal
            n->matcher = LESSER_OR_EQUAL;
            (*iter)++;
        } else {
            // lesser
            n->matcher = LESSER;
        }
    } else if(str[*iter] == '>') {
        (*iter)++;
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
        n->int_value = 0;
        while(str[*iter] >= '0' && str[*iter] <= '9') {
            n->int_value *= 10;
            n->int_value += str[*iter] - '0';
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
                return 1;
            }
        } while (str[*iter] != '\"');

        n->str_value = (char*)malloc(i);
        strncpy(n->str_value, str + *iter - i, i);
        (*iter)++;
    } else if(str[*iter] == '*') {
        //any integer
        (*iter)++;
        n->matcher = ANY;
        n->type = INTEGER;
    } else {
        return 1;
    }

    return 0;
}

int parse(struct parse_result *res, char* str) {
    int iter = 0;
    char word[8];

    whitespaces(&iter, str);

    //input / output / read
    switch(str[iter]) {
        case 'i':
            strcpy(word, "input");
            res->operation = INPUT;
            break;
        case 'o':
            strcpy(word, "output");
            res->operation = OUTPUT;
            break;
        case 'r':
            strcpy(word, "read");
            res->operation = READ;
            break;
    }

    if(strncmp(str + iter, word, strlen(word)) != 0) {
        print_error(iter, str);
        return iter;
    }

    iter+= strlen(word);
    whitespaces(&iter, str);
    // left bracket (
    if(str[iter] != '(') {
        print_error(iter, str);
        return iter;
    }
    iter++;

    //tuples
    res->tuple = make_tuple();
    struct node n;
    int i = 0;
    while (str[iter] != ')') {
        whitespaces(&iter, str);

        // node
        if(parse_node(&n, &iter, str) != 0) {
            print_error(iter, str);
            return iter;
        }

        tuple_append(&res->tuple, n);
        whitespaces(&iter, str);
        //comma
        if(i == 4 || str[iter] != ',') {
            break;
        }
        i++;
        iter++;
    }

    // right bracket )
    if(str[iter] != ')') {
        print_error(iter, str);
        return iter;
    }
    iter++;

    whitespaces(&iter, str);

    // number (timeout)
    res->timeout_ms = 0;
    if(str[iter] >= '0' && str[iter] <= '9') {
        while(str[iter] >= '0' && str[iter] <= '9') {
            res->timeout_ms *= 10;
            res->timeout_ms += str[iter] - '0';
            iter++;
        }
    } else {
        print_error(iter, str);
        return iter + 1;
    }

    whitespaces(&iter, str);

    if(str[iter] == '\0') {
        return 0;
    } else {
        print_error(iter, str);
        return iter + 1;
    }
}
