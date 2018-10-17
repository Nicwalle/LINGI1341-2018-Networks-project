/**
 * Projet1 - Reseaux Informatiques
 * Octobre 2018
 *
 * Auteurs : Brieuc de Voghel   &   Nicolas van de Walle
 * NOMA    : 59101600           &   27901600
 */

#ifndef CODE_STACK_H
#define CODE_STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stddef.h> //size_t
#include <stdint.h> // uintx_t
#include <string.h> // memcpy

#include "../packet/packet.h"

typedef struct node {
    struct node *next;
    struct node *prev;
    struct pkt *pkt;
    uint8_t seqnum;
} node_t;

typedef struct stack {
    struct node *first;
    struct node *last;
    struct node *toSend;
    size_t size;
} stack_t;

/**
 * initialises stack
 * @param stack : pointer to NULL
 * @return : 0 if succeeds, 1 otherwise (stack points then to NULL)
 */
int stack_init(stack_t *stack);

/**
 * adds new node at the end of [stack] with [pkt] inside
 * @param node
 * @param pkt
 * @return : 0 if succeeds, 1 otherwise (stack not modified)
 */
int stack_enqueue(stack_t *stack, pkt_t *pkt);

/**
 * removes node with seqnum [seqnum] from [stack]
 * @param stack
 * @param seqnum
 * @return : ptr to [pkt] inside removed node, NULL if failed
 */
pkt_t *stack_remove(stack_t *stack, uint8_t seqnum);

/**
 * indicates number of nodes in [stack]
 * @param stack
 * @return : number of nodes stocked in [stack]
 */
size_t stack_size(stack_t *stack);

/**
 * frees the whole stack and its content
 * @param stack
 * @return
 */
void stack_free(stack_t *stack);

/**
 * frees the node and its content
 * @param stack
 * @return
 */
void node_free(node_t *node);

#endif //CODE_STACK_H