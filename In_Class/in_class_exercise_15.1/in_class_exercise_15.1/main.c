//
//  main.c
//  in_class_exercise_15.1
//
//  Created by Noah Turnquist on 4/16/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMNODES 5
#define NAMELEN 20

typedef struct {
    char name[NAMELEN + 1];
} DATA;

typedef struct {
    DATA data;
    struct NODE* link;
} NODE;

NODE* CreateNodes(int numNodes);
int EmptyList(NODE* pHead, int numNodes);

int main(int argc, const char * argv[]) {
    NODE* pHead;
    
    pHead = CreateNodes(NUMNODES);
    
    pHead = NULL;
    
    return 0;
}

NODE* CreateNodes(int numNodes) {
    NODE* pTempNode = (NODE*) malloc(sizeof(NODE));
    strncpy(pTempNode->data.name, "Default Name", NAMELEN);
    pTempNode->link = NULL;
    for (int i = 0; i < numNodes; i++) {
        NODE* pNode = (NODE*) malloc(sizeof(NODE));
        strncpy(pNode->data.name, "Default Name", NAMELEN);
        pNode->link = pTempNode;
        pTempNode = pNode;
    }
    return pTempNode;
}

int EmptyList(NODE* pHead) {
    NODE* pWalker = pHead;
    while (pHead) {
        
    }
}
