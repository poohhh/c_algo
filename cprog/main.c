//
//  main.c
//  cprog
//
//  Created by Nayomi on 9/11/21.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define MAX_STACK_SIZE 100
#define INT_MAX 987654321
#define SWAP(x, y, t) ((t) = (x), (x) = (y), (y) = (t))

// 최대 힙-----------------------------------------------------------------------
#define MAX_ELEMENTS 200
#define HEAP_FULL(n) (n == MAX_ELEMENTS - 1)
#define HEAP_EMPTY(n) (n == 0)
int heap[MAX_ELEMENTS]; // 노드의 데이터가 정수라고 가정
int n = 0; // 힙에 저장된 노드의 수

void insert_max_heap(int, int*);
int delete_max_heap(int *);
//-----------------------------------------------------------------------

struct node {
    int data;
    struct node *link;
};

struct dnode {
    struct dnode *llink;
    int data;
    struct dnode *rlink;
};

struct tree {
    char data;
    struct tree *left_child;
    struct tree *right_child;
};

struct node *front, *rear;

int isQueueEmpty(void);
void addQ(int);
int deleteQ(void);
struct node* invert(struct node*);
int length(struct node*);
void insert_front(struct node**, struct node*);

// 이중 연결리스트
void dinsert(struct dnode*, struct dnode*);
void ddelete(struct dnode*);

// 이진 트리
void inorder(struct tree*);
void pre_order(struct tree*);
void post_order(struct tree*);
struct tree* copy(struct tree*);
int eqaul(struct tree*, struct tree*);
int get_node_count(struct tree*);
int get_leaf_count(struct tree*);

// 이진 검색 트리
struct tree* search(struct tree*, int);
void insert_binary_tree(struct tree **, int);
struct tree* modified_search(struct tree*, int);

// dfs
short int visited[MAX_STACK_SIZE];
void dfs(int);
struct node* graph[];

// bfs
void bfs(int);

// 최단 경로
int choose(int [],int, short int[]);
void dijkstra(int, int[][MAX_STACK_SIZE], int[], int, short int[]);
void floy_warchall(int [][MAX_STACK_SIZE], int[][MAX_STACK_SIZE], int);

// 정렬 알고리즘
void selection_sort(int [], int);
void insertion_sort(int [], int);
void quick_sort(int [], int, int);
void adjust(int [], int, int);
void heap_sort(int [], int);


int main(void) {
    front = NULL;
    addQ(2);
    addQ(6);
    addQ(9);
    printf("%d\n", front -> link -> link -> data);
//    struct node* temp = invert(front);
//    printf("%d\n", temp -> link -> data);
    
    struct node *ptr = front;
    int sum = ptr -> data;
    for (ptr = front -> link; ptr != front; ptr = ptr -> link)
        sum += ptr -> data;
    printf("%d\n", sum);
    printf("length: %d\n", length(front));
    
    // 최대힙
    insert_max_heap(15, &n);
    printf("%d\n", heap[1]);
    insert_max_heap(10, &n);
    printf("%d\n", heap[1]);
    insert_max_heap(14, &n);
    printf("%d\n", heap[1]);
    insert_max_heap(2, &n);
    printf("%d\n", heap[1]);
    insert_max_heap(20, &n);
    printf("%d\n", heap[1]);
    
    int list[10] = {26, 5, 37, 1, 61, 11, 59, 15, 48, 19};
//    selection_sort(list, 10);
    quick_sort(list, 0, 9);
    printf("\n");
    for (int i = 0; i < 10; i++)
        printf("%d ", list[i]);
    printf("\n");
    return 0;
}

// 정렬 알고리즘-------------------------------------------------------------
// 선택정렬
void selection_sort(int list[], int n) {
    int i, j, min, temp;
    for (i = 0; i < n; i++) {
        min = i;
        for (j = i+1; j < n; j++)
            if (list[j] < min)
                min = j;
        SWAP(list[i], list[min], temp);
    }
}

// 삽입정렬
void insertion_sort(int list[], int n) {
    int i, j, temp;
    for (i = 1; i < n; i++) {
        for (j = i; j >= 0; j--) {
            if (list[j] < list[j-1])
                SWAP(list[j], list[j-1], temp);
            else
                break;
        }
    }
}

// 퀵 정렬
void quick_sort(int list[], int start, int end) {
    // list[start], ..., list[end]까지 오름차순으로 정렬.
    // list[start]를 초기 기준 값 (pivot key)로 선정
    if (start >= end)
        return;
    int pivot, left, right, temp;
    pivot = start; left = start + 1; right = end;
    while (left < right) {
        while (left <= end && list[left] <= list[pivot])
            left++;
        while (right > start && list[right] >= list[pivot])
            right--;
        if (left > right)
            SWAP(right, pivot, temp);
        else
            SWAP(left, right, temp);
    }
    quick_sort(list, start, right-1);
    quick_sort(list, right+1, end);
}

// 힙에서 삭제하는 알고리즘과 동일..
void adjust(int list[], int root, int n) {
    // list[root]의 위치를 조정하여 최대 힙을 재구성
    int rootkey = list[root], child = 2 * root; // left child
    
    while (child <= n) {
        if ((child < n) && (list[child] < list[child+1]))
            child++;
        if (rootkey > list[child]) break;
        else {
            list[child / 2] = list[child];
            child *= 2;
        }
    }
    list[child / 2] = rootkey;
}

// 힙 정렬
void heap_sort(int list[], int n) {
    int i;
    int temp;
    
    for (i = n / 2; i > 0; i--)     // 초기 최대 힙 구성
        adjust(list, i, n);         // n/2은 child를 갖는 첫번째 노드
    for (i = n-1; i > 0; i--) {
        SWAP(list[1], list[i+1], temp); // root를 맨 뒤의 원소와 교환
        adjust(list, 1, i);     // 원소를 하나 적게 가진 heap을 재구성
    }
}


// 최단경로----------------------------------------------------------------
int choose(int distance[],int n, short int found[]) {
    // 아직 S에 포함되지 않은 vertex 중에서 최소 거리를 갖는 vertex를 return
    int i, min, minpos;
    min = INT_MAX;
    minpos = -1;
    for (i = 0; i < n; i++)
        if (distance[i] < min && found[i] == FALSE) {
            min = distance[i];
            minpos = i;
        }
    return minpos;
}

void dijkstra(int v, int cost[][MAX_STACK_SIZE], int distance[], int n, short int found[]) {
    // Dijkstra 알고리즘 구현
    int i, u, w;
    for (i = 0; i < n; i++){
        found[i] = FALSE;
        distance[i] = cost[v][i];
    }
    found[v] = TRUE;
    distance[v] = 0;
    for (i = 0; i < n-2; i++){
        u = choose(distance, n, found);
        found[u] = TRUE;
        for (w = 0; w < n; w++)
            if (found[w] == FALSE)
                if (distance[u] + cost[u][w] < distance[w])
                    distance[w] = distance[u] + cost[u][w];
    }
}

void floy_warchall(int cost[][MAX_STACK_SIZE], int distance[][MAX_STACK_SIZE], int n) {
    // 각 정점에서 모든 정점까지의 최단 경로 계산
    // cost[][]: 인접 행렬, distance[][]: 경로 거리 저장
    int i, j, k;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            distance[i][j] = cost[i][j];
    for (k = 0; k < n; k++)
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                if (distance[i][k] + distance[k][j] < distance[i][j])
                    distance[i][j] = distance[i][k] + distance[k][j];
}

// dfs-------------------------------------------------------------------
void dfs(int v) {
    struct node *w;
    visited[v] = TRUE;
    printf("%5d", v);
    for (w = graph[v]; w; w = w ->link)
        if (!visited[w->data]) dfs(w->data);
}

// bfs-------------------------------------------------------------------
void bfs(int v) {
    struct node *w;
    printf("%5d", v);
    addQ(v);
    while (front) {
        v = deleteQ();
        for (w = graph[v]; w; w = w -> link)
            if (!visited[w->data]) {
                printf("%5d", w->data);
//                addQ(&front);
//                addQ(&rear);
                addQ(w->data);
                visited[w->data] = TRUE;
            }
    }
}

// 이진 검색 트리------------------------------------------------------------
struct tree *search(struct tree *root, int key) {
    // key를 포함하고 있는 노드의 포인터를 return
    // 해당되는 노드가 없을 경우, return NULL
    // Recursive version
    if (!root) return NULL;
    if (key == root -> data) return root;
    if (key < root -> data)
        return search(root -> left_child, key);
    return search(root -> right_child, key);
    
    /* Iterative version
    while (root != NULL) {
        if (key == root -> data) return root;
        if (key < root -> data)
            root = root -> left_child;
        else
            root = root -> right_child;
    }
    return NULL;
     */
}

struct tree* modified_search(struct tree *root, int key) {
    if (!root) return NULL;
    if (key == root -> data) return NULL;
    if ((root -> left_child) == NULL && (root -> right_child) == NULL) return root;
    if (key < root -> data)
        return modified_search(root -> left_child, key);
    return modified_search(root -> right_child, key);
}

void insert_binary_tree(struct tree **root, int key) {
    // root: 트리의 루트 노드, key: 추가할 키 값.
    struct tree *ptr, *parent = modified_search(*root, key);
    if (parent || !(*root)) { // key가 tree에 존재하지 않음.
        ptr = (struct tree*) malloc(sizeof(struct tree));
        ptr->data = key;    // key를 데이터 값으로 하는 노드 생성
        ptr->left_child = ptr -> right_child = NULL;
        
        if (*root)         // parent의 child로 삽입
            if (key < parent -> data)
                parent->left_child = ptr;
            else
                parent->right_child = ptr;
        else *root = ptr;   // 트리가 empty일 경우, root로 등록
    }
}

//-----------------------------------------------------------------------
// 최대 힙 노드 추가 알고리즘
void insert_max_heap(int item, int *n) {
    // 노드 수가 *n인 최대 힙에 item 값을 추가
    int i;
    if (HEAP_FULL(*n)) {
        fprintf(stderr, "The heap is full.\n");
        exit(1);
    }
    i = ++(*n);
    while ((i != 1) && (item > heap[i/2])) {
        heap[i] = heap[i/2];
        i /= 2;
    }
    heap[i] = item;
}

int delete_max_heap(int *n) {
    int parent, child;
    int item, temp;
    
    if (HEAP_EMPTY(*n)) { fprintf(stderr, "The heap is empty.\n"); exit(1); }
    item = heap[1];
    temp = heap[(*n)--]; // 제일 마지막 원소를 비교 대상으로...
    parent = 1; child = 2; // 루트 노드부터 시작
    while (child <= (*n)) {
        if ((child < *n) && (heap[child] < heap[child+1]))
            child++;    // 두 개의 자식 노드 중에 큰 쪽과 비교
        if (temp >= heap[child]) break; // 더 이상 내려갈 필요 없음
        heap[parent] = heap[child]; // child의 데이터를 위로 이동
        parent = child;     // 아래로 내려가자
        child *= 2;
    }
    heap[parent] = temp;
    return item;
}
//-----------------------------------------------------------------------

// 이진트리 중위순회
void inorder(struct tree *ptr) {
    if (ptr) {
        inorder(ptr -> left_child);
        printf("%d", ptr -> data);
        inorder(ptr -> right_child);
    }
}

void pre_order(struct tree *ptr) {
    if (ptr) {
        printf("%d", ptr -> data);
        pre_order(ptr -> left_child);
        pre_order(ptr -> right_child);
    }
}

void post_order(struct tree *ptr) {
    if (ptr) {
        post_order(ptr -> left_child);
        post_order(ptr -> right_child);
        printf("%d", ptr -> data);
    }
}

// 트리 복사 (후위순회 이용)
struct tree* copy(struct tree* original) {
    struct tree *temp;
    if (original) {
        temp = (struct tree*) malloc(sizeof(struct tree));
        temp -> left_child = copy(original -> left_child);
        temp -> right_child = copy(original -> right_child);
        temp -> data = original -> data;
        return temp;
    }
    return NULL;
}

// 트리 동일성 검사 (전위순회 이용)
int eqaul(struct tree *first, struct tree *second) {
    return ((!first && !second) || (first && second &&
            (first->data == second->data) &&
            eqaul(first->left_child, second->left_child)&&
            eqaul(first->right_child, first->right_child)));
}

// 트리의 전체 노드 수 계산
int get_node_count(struct tree *ptr) {
    int count = 0;
    if (ptr != NULL) {
        count = 1 + get_node_count(ptr->left_child) +
        get_node_count(ptr->right_child);
    }
    return count;
}

// 트리의 단말 노드 수 계산
int get_leaf_count(struct tree *ptr) {
    int count = 0;
    if (ptr != NULL) {
        if (ptr -> left_child == NULL && ptr -> right_child == NULL)
            return 1;
        else
            count = get_leaf_count(ptr -> left_child) + get_leaf_count(ptr -> right_child);
    }
    return count;
}

//-----------------------------------------------------------------------
void dinsert(struct dnode *node, struct dnode *newnode) {
    // 새로추가되는 노드의 링크를 먼저 바꾸고..
    newnode -> llink = node;
    newnode -> rlink = node -> rlink;
    // 원래 노드 링크들 바꿔주기
    node -> rlink -> llink = newnode;
    node -> rlink = newnode;
}

void ddelete(struct dnode *deleted) {
    deleted -> llink -> rlink = deleted -> rlink;
    deleted -> rlink -> llink = deleted -> llink;
    free(deleted);
}

void insert_front(struct node **ptr, struct node *node) {
    if (*ptr == NULL) {
        *ptr = node;
        node -> link = node;
    } else {
        node -> link = (*ptr) -> link;
        (*ptr) -> link = node;
    }
}

//-----------------------------------------------------------------------

int length(struct node *ptr) {
    int count = 0;
    struct node *t;
    if (ptr != NULL) {
        t = ptr;
        do {
            count++;
            t = t -> link;
        } while (t != ptr);
    }
    return count;
}

struct node* invert(struct node *lead) {
    struct node *middle, *tail;
    middle = NULL;
    while (lead) {
        tail = middle;
        middle = lead;
        lead = lead -> link;
        middle -> link = tail;
    }
    return middle;
}

int isQueueEmpty(void) {
    if (front == NULL)
        return TRUE;
    return FALSE;
}

void addQ(int item) {
    struct node *temp = (struct node *) malloc(sizeof(struct node));
    if (temp == NULL) {
        fprintf(stderr, "The memory is full..\n");
        exit(1);
    }
    temp -> data = item;
    temp -> link = NULL;
    if (front != NULL)
        rear -> link = temp;
    else
        front = temp;
    
    rear = temp;
    // 원형 연결 리스트
    rear -> link = front;
}

int deleteQ(void) {
    struct node *temp = front;
    int item;
    if (temp == NULL) {
        fprintf(stderr, "The memory is full..\n");
        exit(1);
    }
    item = temp -> data;
    front = temp -> link;
    free(temp);
    return item;
}
