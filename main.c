#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_LINE 100
#define MAX_VISITED 100

// Arkadaþ listesi düðümü
typedef struct FriendNode {
    int userId;
    struct FriendNode* next;
} FriendNode;

// Kullanýcý yapýsý
typedef struct User {
    int id;
    FriendNode* friends;
} User;

// Sosyal að grafý
typedef struct Graph {
    User* users[MAX_USERS];
    int userCount;
} Graph;

// Kýrmýzý-siyah aðaç düðümü tanýmý
typedef enum { RED, BLACK } Color;

typedef struct RBNode {
    int id;
    Color color;
    struct RBNode *left, *right, *parent;
} RBNode;

// Kullanýcý oluþtur
User* createUser(int id) {
    User* user = (User*)malloc(sizeof(User));
    user->id = id;
    user->friends = NULL;
    return user;
}

void initGraph(Graph* graph) {
	int i;
    for (i = 0; i < MAX_USERS; i++) {
        graph->users[i] = NULL;
    }
    graph->userCount = 0;
}

User* getUser(Graph* graph, int id) {
	int i;
    for (i = 0; i < graph->userCount; i++) {
        if (graph->users[i]->id == id)
            return graph->users[i];
    }
    return NULL;
}

void addUser(Graph* graph, int id) {
    if (getUser(graph, id) != NULL) return;
    User* newUser = createUser(id);
    graph->users[graph->userCount++] = newUser;
}

void addFriendship(Graph* graph, int id1, int id2) {
    addUser(graph, id1);
    addUser(graph, id2);
    User* user1 = getUser(graph, id1);
    User* user2 = getUser(graph, id2);
    FriendNode* f1 = (FriendNode*)malloc(sizeof(FriendNode));
    f1->userId = id2;
    f1->next = user1->friends;
    user1->friends = f1;
    FriendNode* f2 = (FriendNode*)malloc(sizeof(FriendNode));
    f2->userId = id1;
    f2->next = user2->friends;
    user2->friends = f2;
}

void loadFromFile(Graph* graph, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Dosya açýlamadý");
        return;
    }
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), file)) {
        int id1, id2;
        if (sscanf(line, "USER %d", &id1) == 1) {
            addUser(graph, id1);
        } else if (sscanf(line, "FRIEND %d %d", &id1, &id2) == 2) {
            addFriendship(graph, id1, id2);
        }
    }
    fclose(file);
}

// DFS ile belirli mesafedeki arkadaþlarý bulma
void dfs(Graph* graph, int userId, int depth, int currentDepth, int* visited) {
    if (currentDepth > depth) return;
    visited[userId] = 1;
    if (currentDepth == depth) {
        printf("%d ", userId);
        return;
    }
    User* user = getUser(graph, userId);
    FriendNode* f = user->friends;
    while (f) {
        if (!visited[f->userId]) {
            dfs(graph, f->userId, depth, currentDepth + 1, visited);
        }
        f = f->next;
    }
}

// Ortak arkadaþ analizi
void commonFriends(Graph* graph, int id1, int id2) {
    User* u1 = getUser(graph, id1);
    User* u2 = getUser(graph, id2);
    FriendNode* f1 = u1->friends;
    printf("Ortak arkadaslar: ");
    while (f1) {
        FriendNode* f2 = u2->friends;
        while (f2) {
            if (f1->userId == f2->userId) {
                printf("%d ", f1->userId);
            }
            f2 = f2->next;
        }
        f1 = f1->next;
    }
    printf("\n");
}

// Topluluk tespiti (connected component sayýsý)
void dfsCommunity(Graph* graph, int userId, int* visited) {
    visited[userId] = 1;
    User* user = getUser(graph, userId);
    FriendNode* f = user->friends;
    while (f) {
        if (!visited[f->userId]) {
            dfsCommunity(graph, f->userId, visited);
        }
        f = f->next;
    }
}

int detectCommunities(Graph* graph) {
    int visited[MAX_USERS] = {0};
    int count = 0;
    int i;
    for (i = 0; i < graph->userCount; i++) {
        int uid = graph->users[i]->id;
        if (!visited[uid]) {
            dfsCommunity(graph, uid, visited);
            count++;
        }
    }
    return count;
}

// Etki alaný hesaplama (kaç kullanýcýya ulaþabilir)
int influence(Graph* graph, int userId) {
    int visited[MAX_USERS] = {0};
    int count = 0;
    void dfsInfluence(int id) {
        if (visited[id]) return;
        visited[id] = 1;
        count++;
        User* user = getUser(graph, id);
        FriendNode* f = user->friends;
        while (f) {
            dfsInfluence(f->userId);
            f = f->next;
        }
    }
    dfsInfluence(userId);
    return count - 1;
}

// Kýrmýzý-siyah aðaç (sadece ekleme ve arama için basit yapý, dengeleme yok)
RBNode* createRBNode(int id) {
    RBNode* node = (RBNode*)malloc(sizeof(RBNode));
    node->id = id;
    node->color = RED;
    node->left = node->right = node->parent = NULL;
    return node;
}

RBNode* insertRB(RBNode* root, RBNode* node) {
    if (root == NULL) return node;
    if (node->id < root->id) {
        root->left = insertRB(root->left, node);
    } else if (node->id > root->id) {
        root->right = insertRB(root->right, node);
    }
    return root;
}

RBNode* searchRB(RBNode* root, int id) {
    if (!root || root->id == id) return root;
    if (id < root->id) return searchRB(root->left, id);
    return searchRB(root->right, id);
}

void printGraph(Graph* graph) {
	int i;
    for (i = 0; i < graph->userCount; i++) {
        User* user = graph->users[i];
        printf("Kullanici %d arkadaslari: ", user->id);
        FriendNode* friend = user->friends;
        while (friend) {
            printf("%d ", friend->userId);
            friend = friend->next;
        }
        printf("\n");
    }
}

int main() {
    Graph graph;
    initGraph(&graph);
    loadFromFile(&graph, "veriseti.txt");
    printGraph(&graph);

    printf("\n\n101 ve 104 ortak arkadaslari:\n");
    commonFriends(&graph, 101, 104);

    int toplulukSayisi = detectCommunities(&graph);
    printf("\nToplam topluluk sayisi: %d\n", toplulukSayisi);

    int etkialani = influence(&graph, 101);
    printf("\nKullanici 101'in etki alani: %d kisi\n", etkialani);

    RBNode* root = NULL;
    int i;
    for (i = 0; i < graph.userCount; i++) {
        int id = graph.users[i]->id;
        root = insertRB(root, createRBNode(id));
    }
    int aranan = 104;
    RBNode* bulunan = searchRB(root, aranan);
    if (bulunan) printf("\nKullanici %d RB agacta bulundu.\n", aranan);
    else printf("\nKullanici %d RB agacta bulunamadi.\n", aranan);

    return 0;
}

