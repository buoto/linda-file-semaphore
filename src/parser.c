#include "parser.h"

void whitespaces(int *iter, const char* str) {
    while(str[*iter] == ' ') {
        (*iter)++;
    }
}

int parse_node(struct node *n, int *iter, const char *str) {
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
        n->str_value[i] = '\0';
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

int parse_tuple(struct tuple *t, const char *str, int *iter) {

    whitespaces(iter, str);
    if(str[*iter] != '(') {
        return *iter;
    }
    (*iter)++;

    //tuples
    struct node n;
    int i = 0;
    while (str[*iter] != ')') {
        whitespaces(iter, str);

        // node
        if(parse_node(&n, iter, str) != 0) {
            return *iter;
        }

        tuple_append(t, n);
        whitespaces(iter, str);
        //comma
        if(i == 4 || str[*iter] != ',') {
            break;
        }
        i++;
        (*iter)++;
    }

    // right bracket )
    if(str[*iter] != ')') {
        return *iter;
    }
    (*iter)++;
    return 0;
}

int parse(struct parse_result *res, char *str) {
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
        case 'e':
            strcpy(word, "exit");
            res->operation = EXIT;
            break;
        case 'c':
            strcpy(word, "cleanup");
            res->operation = CLEANUP;
            break;
        default:
            return iter + 1;
    }

    if(strncmp(str + iter, word, strlen(word)) != 0) {
        return iter + 1;
    }

    iter+= strlen(word);
    res->tuple = make_tuple();

    if(res->operation == EXIT || res->operation == CLEANUP) {
        return 0;
    }

    if(parse_tuple(&res->tuple, str, &iter) != 0) {
        return iter + 1;
    };

    if(res->operation != OUTPUT) {
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
            return iter + 1;
        }
    }
    whitespaces(&iter, str);

    if(str[iter] == '\0') {
        return 0;
    } else {
        return iter + 1;
    }
}

int deserialize_tuple(struct tuple *t, const char *str) {
    int iter = 0;

    if(parse_tuple(t, str, &iter) != 0) {
        return iter + 1;
    };

    if(str[iter] == '\n' || str[iter] == '\0') {
        return 0;
    } else {
        return iter + 1;
    }
}
