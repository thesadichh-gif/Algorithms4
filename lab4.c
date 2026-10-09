#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>

struct node {
    int data;               
    struct node* left;     
    struct node* right;    
};

struct node* root = NULL;  

struct node* create_node(int data) {
    struct node* p = (struct node*)malloc(sizeof(struct node));
    if (p == NULL) {
        printf("[Ошибка] Не удалось выделить память!\n");
        exit(1);
    }
    p->data = data;
    p->left = NULL;
    p->right = NULL;
    return p;
}

struct node* insert_node(struct node* r, int data) {
    if (r == NULL) return create_node(data);
    if (data < r->data) r->left = insert_node(r->left, data);
    else                r->right = insert_node(r->right, data);
    return r;
}

struct node* CreateTree(struct node* treeRoot, struct node* r, int data) {
    if (treeRoot == NULL) return create_node(data);  

    if (data < r->data) {
        if (r->left == NULL) r->left = create_node(data);
        else                 CreateTree(treeRoot, r->left, data);
    }
    else {
        if (r->right == NULL) r->right = create_node(data);
        else                  CreateTree(treeRoot, r->right, data);
    }
    return treeRoot;
}

struct node* search_tree(struct node* r, int key, int* comparisons) {
    if (r == NULL) return NULL;       

    (*comparisons)++;               

    if (key == r->data) return r;     
    if (key < r->data) return search_tree(r->left, key, comparisons);
    else                return search_tree(r->right, key, comparisons);
}

int count_occurrences(struct node* r, int key) {
    if (r == NULL) return 0;

    int count = 0;
    if (r->data == key) count = 1;

    if (key < r->data) {
        count += count_occurrences(r->left, key);
    }
    else {
        count += count_occurrences(r->right, key);
    }
    return count;
}

struct node* insert_unique(struct node* r, int data, int* added) {
    if (r == NULL) {
        *added = 1;
        return create_node(data);
    }
    if (data == r->data) {
        *added = 0;
        printf("[Защита] Элемент %d уже есть в дереве. Дубликат отклонён!\n", data);
        return r;
    }
    if (data < r->data) r->left = insert_unique(r->left, data, added);
    else                r->right = insert_unique(r->right, data, added);
    return r;
}

void print_tree(struct node* r, int level) {
    if (r == NULL) return;
    print_tree(r->right, level + 1);
    for (int i = 0; i < level; i++) printf("    ");
    printf("%d\n", r->data);
    print_tree(r->left, level + 1);
}

void inorder_traversal(struct node* r) {
    if (r == NULL) return;
    inorder_traversal(r->left);
    printf("%d ", r->data);
    inorder_traversal(r->right);
}

void preorder_traversal(struct node* r) {
    if (r == NULL) return;
    printf("%d ", r->data);
    preorder_traversal(r->left);
    preorder_traversal(r->right);
}

void postorder_traversal(struct node* r) {
    if (r == NULL) return;
    postorder_traversal(r->left);
    postorder_traversal(r->right);
    printf("%d ", r->data);
}

void free_tree(struct node* r) {
    if (r == NULL) return;
    free_tree(r->left);
    free_tree(r->right);
    free(r);
}

int count_nodes(struct node* r) {
    if (r == NULL) return 0;
    return 1 + count_nodes(r->left) + count_nodes(r->right);
}

int tree_height(struct node* r) {
    if (r == NULL) return 0;
    int h_left = tree_height(r->left);
    int h_right = tree_height(r->right);
    return 1 + (h_left > h_right ? h_left : h_right);
}

void print_complexity_info() {
    int n = count_nodes(root);
    int h = tree_height(root);

    printf("\n--- АНАЛИТИЧЕСКАЯ ОЦЕНКА СЛОЖНОСТИ ПОИСКА В BST ---\n");
    printf("1. Лучший случай:  O(1)       (элемент в корне)\n");
    printf("2. Средний случай: O(log2 N)  (сбалансированное дерево)\n");
    printf("   При каждом шаге отсекается примерно половина поддерева.\n");
    printf("3. Худший случай:  O(N)       (вырожденное дерево / список)\n");
    printf("   Возникает, например, при вставке уже отсортированной\n");
    printf("   последовательности — дерево превращается в «лозу».\n");
    printf("---------------------------------------------------\n");
    printf("Текущее состояние дерева:\n");
    printf("  Количество узлов (N): %d\n", n);
    printf("  Фактическая высота (макс. шагов): %d\n", h);
    if (n > 1) {
        printf("  Идеальная высота сбалансированного: ~%.1f шагов\n",
            log2((double)n));
    }
    printf("---------------------------------------------------\n");
}

void run_demo() {
    printf("\n--- АВТОМАТИЧЕСКОЕ ТЕСТИРОВАНИЕ ВСЕХ ЗАДАНИЙ ---\n");

    free_tree(root);
    root = NULL;

    printf("\n[1] Построение дерева через CreateTree (исправленная функция)...\n");
    int data[] = { 50, 30, 70, 20, 40, 60, 80, 40, 30 };
    int n = sizeof(data) / sizeof(data[0]);
    printf("Исходная последовательность: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", data[i]);
        root = CreateTree(root, root, data[i]);
    }
    printf("\n\nДерево (повёрнуто на 90°):\n");
    print_tree(root, 0);

    printf("\n[2 - Задание 1] Поиск элементов:\n");
    int comps = 0;
    struct node* res = NULL;

    comps = 0; res = search_tree(root, 40, &comps);
    printf("  Ключ 40: %s (сравнений: %d)\n",
        res ? "НАЙДЕН" : "НЕ НАЙДЕН", comps);

    comps = 0; res = search_tree(root, 99, &comps);
    printf("  Ключ 99: %s (сравнений: %d)\n",
        res ? "НАЙДЕН" : "НЕ НАЙДЕН", comps);

    printf("\n[3 - Задание 2] Подсчёт вхождений:\n");
    printf("  Вхождений 40: %d\n", count_occurrences(root, 40));
    printf("  Вхождений 30: %d\n", count_occurrences(root, 30));
    printf("  Вхождений 50: %d\n", count_occurrences(root, 50));
    printf("  Вхождений 99: %d\n", count_occurrences(root, 99));

    printf("\n[4 - Задание 3*] Добавление с фильтром дубликатов:\n");
    int added = 0;
    printf("  Пробуем добавить 25 (уникальное):\n");
    root = insert_unique(root, 25, &added);
    if (added) printf("  -> 25 добавлено.\n");

    printf("  Пробуем добавить 50 (уже есть):\n");
    root = insert_unique(root, 50, &added);
    if (!added) printf("  -> 50 отклонено как дубликат.\n");

    printf("  Пробуем добавить 70 (уже есть):\n");
    root = insert_unique(root, 70, &added);
    if (!added) printf("  -> 70 отклонено как дубликат.\n");

    printf("\n[5] Обходы дерева:\n");
    printf("  In-Order  (по возрастанию): "); inorder_traversal(root);  printf("\n");
    printf("  Pre-Order (корень первым):  "); preorder_traversal(root); printf("\n");
    printf("  Post-Order(корень последним):"); postorder_traversal(root);printf("\n");

    print_complexity_info();
}

int main() {
    setlocale(LC_ALL, "Russian");
    int choice = -1, val = 0, comps = 0, added = 0;
    struct node* found = NULL;

    while (choice != 0) {
        printf("\n=====================================\n");
        printf("  ЛАБОРАТОРНАЯ РАБОТА № 4: BST\n");
        printf("=====================================\n");
        printf("1. Запустить демо-тест (все задания)\n");
        printf("2. Добавить узлы вручную (-1 для остановки)\n");
        printf("3. Добавить уникальный узел (Задание 3*)\n");
        printf("4. Найти узел (Задание 1)\n");
        printf("5. Подсчёт вхождений (Задание 2)\n");
        printf("6. Вывести дерево (повёрнуто на 90°)\n");
        printf("7. Вывести по возрастанию (In-Order)\n");
        printf("8. Оценка сложности (Задание 4*)\n");
        printf("9. Очистить дерево\n");
        printf("0. Выход\n");
        printf("-------------------------------------\n");
        printf("Выбор: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Некорректный ввод! Введите число 0..9.\n");
            continue;
        }

        switch (choice) {
        case 1:
            run_demo();
            break;

        case 2:
            printf("Вводите целые числа (-1 для завершения):\n");
            while (1) {
                printf("Число: ");
                if (scanf("%d", &val) != 1) {
                    while (getchar() != '\n');
                    printf("Ошибка ввода! Повторите.\n");
                    continue;
                }
                if (val == -1) break;
                root = insert_node(root, val);
            }
            printf("Текущее дерево:\n");
            print_tree(root, 0);
            break;

        case 3:
            printf("Число для добавления: ");
            if (scanf("%d", &val) != 1) {
                while (getchar() != '\n');
                printf("Ошибка ввода!\n");
                break;
            }
            added = 0;
            root = insert_unique(root, val, &added);
            if (added) printf("Элемент %d успешно добавлен.\n", val);
            break;

        case 4:
            if (root == NULL) { printf("Дерево пусто!\n"); break; }
            printf("Число для поиска: ");
            if (scanf("%d", &val) != 1) {
                while (getchar() != '\n');
                printf("Ошибка ввода!\n");
                break;
            }
            comps = 0;
            found = search_tree(root, val, &comps);
            if (found) printf("НАЙДЕН за %d сравнений.\n", comps);
            else       printf("НЕ НАЙДЕН (сравнений: %d).\n", comps);
            break;

        case 5:
            if (root == NULL) { printf("Дерево пусто!\n"); break; }
            printf("Число для подсчёта вхождений: ");
            if (scanf("%d", &val) != 1) {
                while (getchar() != '\n');
                printf("Ошибка ввода!\n");
                break;
            }
            printf("Вхождений %d: %d\n", val, count_occurrences(root, val));
            break;

        case 6:
            if (root == NULL) printf("Дерево пусто!\n");
            else { printf("\n"); print_tree(root, 0); printf("\n"); }
            break;

        case 7:
            if (root == NULL) printf("Дерево пусто!\n");
            else { inorder_traversal(root); printf("\n"); }
            break;

        case 8:
            print_complexity_info();
            break;

        case 9:
            free_tree(root);
            root = NULL;
            printf("Память дерева освобождена.\n");
            break;

        case 0:
            printf("Освобождение памяти и выход...\n");
            free_tree(root);
            root = NULL;
            break;

        default:
            printf("Неверный выбор! Введите 0..9.\n");
        }
    }

    return 0;
}