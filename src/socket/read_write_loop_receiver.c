/**
 * Projet1 - Reseaux Informatiques
 * Octobre 2018
 *
 * Auteurs : Brieuc de Voghel   &   Nicolas van de Walle
 * NOMA    : 59101600           &   27901600
 *
 * Contenu repris et complete de l'exercice preparatoire au projet : https://inginious.info.ucl.ac.be/course/LINGI1341/envoyer-et-recevoir-des-donnees
 * Réalisé avec l'aide des sites suivants : https://github.com/Donaschmi/LINGI1341/blob/master/Inginious/Envoyer_et_recevoir_des_donn%C3%A9es/read_write_loop.c
 */


#include "read_write_loop_receiver.h"
#include "../packet/packet.h" // MAX_PAYLOAD_SIZE

uint8_t expectedSeqnum;
int window;

/**
 * Loop reading a socket and printing to stdout,
 * while reading stdin and writing to the socket
 * @sfd: The socket file descriptor. It is both bound and connected.
 * @return: as soon as stdin signals EOF
 */
void read_write_loop_receiver(int sfd, stack_t *receivingStack, int outputFileDescriptor) {
    if(outputFileDescriptor) {
        //TODO REMOVE UNUSED
    }
    int getOut = 0;

    char sfdBuffer[MAX_PAYLOAD_SIZE+16];

    fd_set fdSet;

    struct timeval timeout;
    timeout.tv_sec = 10000;
    timeout.tv_usec = 0;

    //Unuseful but inginious needs to go through the warnings
    int justRead;

    pkt_t *packet = pkt_new();
    pkt_status_code decodeResult;
    perror("BEFORE WHILE");

    while (getOut == 0) {
        // Reset everything for new iteration of the loop
        memset(sfdBuffer, 0, MAX_PAYLOAD_SIZE+16);

        justRead = 0;

        FD_ZERO(&fdSet);
        FD_SET(0, &fdSet);
        FD_SET(sfd, &fdSet);
        perror("BEFORE SELECT");

        select(sfd + 1, &fdSet, NULL, NULL, &timeout);

        if (FD_ISSET(sfd, &fdSet)) {

            justRead = read(sfd, sfdBuffer, MAX_PAYLOAD_SIZE);
            perror("BEFORE DECODE");
            decodeResult = pkt_decode(sfdBuffer, justRead, packet);
            perror("AFTER DECODE");
            if (decodeResult == PKT_OK) {
                // Store packet into the buffer
                perror("BEFORE SEQNUM");
                if (pkt_get_seqnum(packet) - expectedSeqnum >= 0 && pkt_get_seqnum(packet) - expectedSeqnum < window) {
                    perror(pkt_get_payload(packet));
                    stack_enqueue(receivingStack, packet);
                }
            }else {
                // TODO : send NACK
            }


        }

        if (feof(stdin) != 0) {
            getOut = 1;
        }

    }
}