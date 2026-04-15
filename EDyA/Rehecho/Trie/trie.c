#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct _TRIENODE{
    struct _TRIENODE* letras[26];
    int isEndOfWord;
}TrieNode;

typedef TrieNode* Trie;

Trie createTrie(){
    Trie trie = malloc(sizeof(TrieNode));
    for(int i = 0; i < 26; i++){
        trie->letras[i] = NULL;
        trie->isEndOfWord = 0;
    }
    return trie;
}

Trie insertTrie(Trie trie, char* s){
    if(trie == NULL){
        trie = createTrie();
    }
    if(*s == '\0'){
        trie->isEndOfWord = 1;
        return trie;
    }
    int idx = tolower(*s) % 97;
    if(trie->letras[idx] == NULL) trie->letras[idx] = createTrie();
    trie->letras[idx] = insertTrie(trie->letras[idx], s+1);

    return trie;
}

int searchTrie(Trie trie, char* s){
    if(trie == NULL) return 0;
    if(*s == '\0') {
        return trie->isEndOfWord;
    }
    
    int idx = tolower(*s) - 'a';
    return searchTrie(trie->letras[idx], s+1);
}


void printTrie(Trie trie, char* buffer, int len){
    if(trie == NULL) return;
    if(trie->isEndOfWord == 1){
        buffer[len] = '\0';
        printf("%s\n", buffer);
    }
    for(int i = 0; i<26; i++){
        if(trie->letras[i] != NULL){
            buffer[len] = i+97;
            printTrie(trie->letras[i], buffer, len+1);
        }
    }
}

int hasChildren(Trie nodo){
    int no = 0;
    for(int i = 0; i<26; i++){
        if(nodo->letras[i] != NULL) no = 1;
    }
    return no;
}

void destroyTrie(Trie trie){
    if(trie == NULL) return;
    for(int i = 0; i<26; i++)
        destroyTrie(trie->letras[i]);
    
    free(trie);
}

Trie deleteTrie(Trie trie, char* s){
    if(trie == NULL) return trie;

    if(*s == '\0'){
        if(hasChildren(trie)){
            trie->isEndOfWord = 0;
            return trie;
        } 
        else{
            free(trie);
            return NULL;
        }
    }
    int idx = *s - 'a';
    trie->letras[idx] = deleteTrie(trie->letras[idx], s+1);
    
    if(trie->isEndOfWord == 0 && !hasChildren(trie)){
        free(trie); return NULL;
    }

    return trie;
}

int main(){
    Trie trie = createTrie();
    char buffer[100];

    char* s = "casasa", *s1 = "cas", *s2 = "casasas", *s3 = "casa";
    char* op = "casca";
    char* op2 = "sol";
    insertTrie(trie, s);
    insertTrie(trie, s1);
    insertTrie(trie, s2);
    insertTrie(trie, s3);
    insertTrie(trie, op);
    insertTrie(trie, op2);
    printTrie(trie, buffer, 0);
    puts("");
    //printf("%d", searchTrie(trie, "cas"));
    
    deleteTrie(trie, "sol");
    printTrie(trie, buffer, 0);

    destroyTrie(trie);
    return 0;
}